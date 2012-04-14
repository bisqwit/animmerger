#include <cstdio>
#include <algorithm>
#include <cmath>
#include <stdarg.h>

#include <assert.h>
#include <gd.h>

#include "palette.hh"
#include "fparser.hh"

double   DitherGamma         = 1.0;

static const char colordiff_paramstr[] =
    "R1,G1,B1,A1,luma1,L1,a1,b1,C1,h1,"
    "R2,G2,B2,A2,luma2,L2,a2,b2,C2,h2";

static class FunctionParserWithPaletteCCVariables: public FunctionParser
{
public:
    FunctionParserWithPaletteCCVariables()
    {
        AddFunction("g",   FPGammaCorrect, 1);
        AddFunction("ug",  FPGammaUncorrect, 1);
        AddConstant("pi",  M_PI);
        AddConstant("e",   M_E);
    }
    static double FPGammaCorrect(const double* vars)
    {
        return GammaCorrect(vars[0]);
    }
    static double FPGammaUncorrect(const double* vars)
    {
        return GammaUncorrect(vars[0]);
    }
} colordiff_parser;

ColorCompareMethod ColorComparing = Compare_RGB;

void SetColorCompareFormula(const std::string& expr)
{
    colordiff_parser.AddConstant("gamma", DitherGamma);
    int error = colordiff_parser.Parse(expr.c_str(), colordiff_paramstr);
    if(error >= 0)
    {
        fprintf(stderr, "Parse error (%s) in color difference formula:\n%s\n%*s\n",
            colordiff_parser.ErrorMsg(), expr.c_str(), error+1, "^");
        ColorComparing = Compare_RGB;
    }
    else
        colordiff_parser.Optimize();
}

double ColorCompare(const ColorInfo& meta1,
                    const ColorInfo& meta2)
{
  #define DEG2RAD(xx) (M_PI/180.0 * (xx))
    switch(ColorComparing)
    {
        default:
        case Compare_RGB:
        {
            int r1 = meta1.R, r2 = meta2.R;
            int g1 = meta1.G, g2 = meta2.G;
            int b1 = meta1.B, b2 = meta2.B;
            int a1 = meta1.A, a2 = meta2.A;
            int diffR = r1-r2, diffG = g1-g2, diffB = b1-b2, diffA = a1-a2;
            return diffR*diffR + diffG*diffG + diffB*diffB + diffA*diffA;
        }
        case Compare_CIE76_DeltaE:
        {
            int a1 = meta1.A, a2 = meta2.A;
            double dL = meta1.lab.L - meta2.lab.L;
            double da = meta1.lab.a - meta2.lab.a;
            double db = meta1.lab.b - meta2.lab.b;
            int diffA = a1-a2;
            // CIE normal deltaE squared
            return dL*dL + da*da + db*db + diffA*diffA;
        }
        case Compare_CIE94_DeltaE:
        {
            int a1 = meta1.A, a2 = meta2.A;
            double dL = meta1.lab.L - meta2.lab.L;
            double da = meta1.lab.a - meta2.lab.a;
            double db = meta1.lab.b - meta2.lab.b;
            int diffA = a1-a2;
            /* Although CIE94 and CIEDE2000 are theoretically more developed
             * than the plain CIE distance, in practise both have some kind
             * of issue with violet/purple colors, for which they deem black
             * as the best substitute.
             * (Does it somehow think purple is ultraviolet or something?)
             * (Bisqwit's hue circle + EGA palette test.)
             */

            // CIE94 deltaE color difference squared
            double C12 = std::sqrt(meta1.lab.C * meta2.lab.C); // Symmetric chromance
            double dC = meta2.lab.C - meta1.lab.C;
            double dCsq = dC*dC;
            double dHsq = da*da + db*db - dCsq; // delta hue squared
            if(dHsq < 0.0) dHsq = 0.0;
            double SC = 1.0 + 0.048 * C12; // weighting for delta chromance
            double SH = 1.0 + 0.014 * C12; // weighting for delta hue
            return dL*dL + dCsq / (SC*SC) + dHsq / (SH*SH) + diffA*diffA;
        }
        case Compare_CMC_lc:
        {
            const double l = 0.5, c = 1; // Lightness and chroma parameters
            int a1 = meta1.A, a2 = meta2.A;
            int diffA = a1-a2;
            double dl = meta2.lab.L - meta1.lab.L;
            if(meta1.lab.L <= 0.0 || meta2.lab.L <= 0.0)
                return dl*dl + diffA*diffA;
            double da = meta2.lab.a - meta1.lab.a;
            double db = meta2.lab.b - meta1.lab.b;
            double dC = meta2.lab.C - meta1.lab.C;
            double h1 = meta1.lab.h;
            if(h1 < 0) h1 += 2*M_PI;
            double T = (h1 >= DEG2RAD(164) && h1 <= DEG2RAD(345))
                ? 0.56 + std::fabs(0.2 * std::cos(h1 + DEG2RAD(168)))
                : 0.36 + std::fabs(0.4 * std::cos(h1 + DEG2RAD(35)));
            double C1p4 = meta1.lab.C; C1p4*=C1p4; C1p4*=C1p4; // C1 ^ 4
            double F = std::sqrt(C1p4 / (C1p4 + 1900));
            double SL = meta1.lab.L < 16 ? 0.511 : (0.040975*meta1.lab.L
                                                  /(1+0.01765*meta1.lab.L));
            double SC = 0.0638*meta1.lab.C / (1 + 0.0131*meta1.lab.C) + 0.0638;
            double SH = SC * (F*T + 1 - F);
            double dLSL = dl / (l*SL);
            double dCSC = dC / (c*SC);
            double dHsq = da*da + db*db - dC*dC;
            if(dHsq < 0.0) dHsq = 0.0;
            return dLSL*dLSL + dCSC*dCSC + dHsq / (SH*SH) + diffA*diffA;
        }
        case Compare_BFD_lc:
        {
            const double l = 1.5, c = 1; // Lightness and chroma parameters
            int a1 = meta1.A, a2 = meta2.A;
            int diffA = a1-a2;
            /*double bfd1 = 54.6 * std::log(Y1+1.5) - 9.6;
            double bfd2 = 54.6 * std::log(Y2+1.5) - 9.6;
            double deltaBFD = bfd2 - bfd1;*/
            double deltaBFD_l = (54.6/l) * (std::log((meta2.xyz.Y+1.5) / (meta1.xyz.Y+1.5)));
            double da = meta2.lab.a - meta1.lab.a;
            double db = meta2.lab.b - meta1.lab.b;
            double dC = meta2.lab.C - meta1.lab.C;
            double avgC = (meta2.lab.C + meta1.lab.C) * .5;
            double avgh = (meta2.lab.h + meta1.lab.h) * .5;
            double dc = 0.035 * avgC / (1 + 0.00365*avgC) + 0.521;
            double avgCp4 = avgC; avgCp4*=avgCp4; avgCp4*=avgCp4; // avgC ^ 4
            double g = std::sqrt(avgCp4 / (avgCp4 + 14000));
            double t = 0.627 + 0.055 * std::cos(  avgh - DEG2RAD(254))
                             - 0.040 * std::cos(2*avgh - DEG2RAD(136))
                             + 0.070 * std::cos(3*avgh - DEG2RAD(32))
                             + 0.049 * std::cos(4*avgh + DEG2RAD(114))
                             - 0.015 * std::cos(5*avgh - DEG2RAD(103));
            double dh = dc * (g*(t-1) + 1);
            double RH = -0.260 * std::cos(avgh   - DEG2RAD(308))
                       - 0.379 * std::cos(2*avgh - DEG2RAD(160))
                       - 0.636 * std::cos(3*avgh + DEG2RAD(254))
                       + 0.226 * std::cos(4*avgh + DEG2RAD(140))
                       - 0.194 * std::cos(5*avgh + DEG2RAD(280));
            double avgCp6 = avgC*avgC*avgC; avgCp6*=avgCp6; // avgC ^ 6
            double RC = std::sqrt(avgCp6 / (avgCp6 + 7e7));
            double dHsq = da*da + db*db - dC*dC;
            if(dHsq <= 0.0) dHsq = 0.0;
            double dCcdc = dC/(c*dc);
            return deltaBFD_l*deltaBFD_l + dCcdc*dCcdc + dHsq/(dh*dh)
                    + RH*RC*dC*std::sqrt(dHsq)/(dc*dh) + diffA*diffA;
        }

        case Compare_CIEDE2000_DeltaE:
        {
            /* From the paper "The CIEDE2000 Color-Difference Formula: Implementation Notes, */
            /* Supplementary Test Data, and Mathematical Observations", by */
            /* Gaurav Sharma, Wencheng Wu and Edul N. Dalal, */
            /* Color Res. Appl., vol. 30, no. 1, pp. 21-30, Feb. 2005. */
            /* Return the CIEDE2000 Delta E color difference measure squared, for two Lab values */

            int a1 = meta1.A, a2 = meta2.A;
            int diffA = a1-a2;
            const double v25p7 = 6103515625.0; // 25^7
            const double d180 = DEG2RAD(180), d360 = DEG2RAD(360);
            /* Compute Cromanance and Hue angles */
            double avgC = 0.5 * (meta1.lab.C + meta2.lab.C);
            double avgC2 = avgC; avgC2*=avgC2;
            double avgC7 = avgC2; avgC7*=avgC7; avgC7*=avgC2*avgC; // avgC ^ 7
            double Gt = 0.5 * std::sqrt(avgC7/(avgC7 + v25p7));
            double A1 = (1.5 - Gt) * meta1.lab.a;
            double A2 = (1.5 - Gt) * meta2.lab.a;
            double c1 = std::sqrt(A1 * A1 + meta1.lab.b * meta1.lab.b);
            double c2 = std::sqrt(A2 * A2 + meta2.lab.b * meta2.lab.b);
            double H1,H2;                   // Hue in 0..360 degrees range
            if(c1 < 1e-9) H1 = 0.0; else { H1=std::atan2(meta1.lab.b, A1); if(H1<0.0) H1+=d360; }
            if(c2 < 1e-9) H2 = 0.0; else { H2=std::atan2(meta2.lab.b, A2); if(H2<0.0) H2+=d360; }
            double dH = H2 - H1;
            /**/ if(dH < -d180) dH += d360;
            else if(dH >  d180) dH -= d360; // Hue difference in -180..180 degrees range
            double avgH = H1 + H2;
            if(c1 >= 1e-9 && c2 >= 1e-9)    // There are two hues rather than one; average them
            {
                // Average the closest way, i.e. the average
                // between 30 degrees and 350 degrees is not 190 degrees, but
                // rather, it is 20 degrees, because 350 degrees = -10 degrees.
                if(std::fabs(H1-H2) > d180) avgH += (avgH < d360) ? d360 : -d360;
                avgH *= 0.5;
                // The result is in 0..360 degrees range.
            }                               // Average hue in 0..360 degrees range
            /* Compute delta L, C and H */
            double DH = 2.0 * std::sqrt(c1 * c2) * std::sin(0.5 * dH);
            double avgL = 0.5 * (meta1.lab.L  + meta2.lab.L);
            double avgc = 0.5 * (c1 + c2);
            double T = 1.0
              - 0.17 * std::cos(avgH - DEG2RAD(30.0))
              + 0.24 * std::cos(2.0 * avgH)
              + 0.32 * std::cos(3.0 * avgH + DEG2RAD(6))
              - 0.2 * std::cos(4.0 * avgH - DEG2RAD(63));
            double hh = (avgH - DEG2RAD(275)) / DEG2RAD(25); // range: -11..+3.4
            double avgc2 = avgc; avgc2*=avgc2;
            double avgc7 = avgc2; avgc7*=avgc7; avgc7*=avgc2*avgc; // avgc ^ 7
            double L50sq = (avgL - 50.0); L50sq *= L50sq;
            double SL = 1.0 + (0.015 * L50sq) / std::sqrt(20.0 + L50sq);
            double SC = 1.0 + 0.045 * avgc;
            double SH = 1.0 + 0.015 * avgc * T;
            double RC = -2.0 * std::sqrt(avgc7/(avgc7 + v25p7));
            double RT = std::sin(DEG2RAD(30) * std::exp(-hh*hh)) * RC;
            double dLSL = (meta2.lab.L-meta1.lab.L)/SL, dcSC = (c2-c1) / SC, DHSH = DH/SH;
            return dLSL*dLSL + dcSC*dcSC + DHSH*DHSH - RT*dcSC*DHSH + diffA*diffA;
        }

        case Compare_fparser:
        {
            const double div255 = 1/255.0, div127 = 1/127.0, div255000 = 1/255e3;
            const double params[] =
            {
                meta1.R*div255, meta1.G*div255, meta1.B*div255, meta1.A*div127,
                meta1.luma*div255000,
                meta1.lab.L, meta1.lab.a, meta1.lab.b, meta1.lab.C, meta1.lab.h,
                meta2.R*div255, meta2.G*div255, meta2.B*div255, meta2.A*div127,
                meta2.luma*div255000,
                meta2.lab.L, meta2.lab.a, meta2.lab.b, meta2.lab.C, meta2.lab.h
            };
            return colordiff_parser.Eval(params);
        }
    }
  #undef DEG2RAD
}

std::pair<unsigned,double>
    Palette::FindClosestCombinationIndex(const ColorInfo& test_lab) const
{
    switch(ColorComparing)
    {
        case Compare_RGB:
        {
            KDTree<unsigned,4>::KDPoint q( test_lab.R, test_lab.G, test_lab.B, test_lab.A);
            return CombinationTree.nearest_info(q);
        }
        case Compare_CIE76_DeltaE:
        {
            KDTree<unsigned,4>::KDPoint q( test_lab.lab.L, test_lab.lab.a, test_lab.lab.b, test_lab.A );
            return CombinationTree.nearest_info(q);
        }
        default:
        {
            std::pair<unsigned,double> result(0u, -1.0);
            for(unsigned limit = Combinations.size(), index=0; index<limit; ++index)
            {
                double penalty = ColorCompare(test_lab, GetCombinationMeta(index));
                if(penalty < result.second || result.second == -1.0)
                    { result.first  = index;
                      result.second = penalty; }
            }
            return result;
        }
    }
}

void Palette::SetHardcoded(unsigned n, ...)
{
    va_list ap;
    va_start(ap, n);
    Data.clear();
    Data.reserve(n);
    while(n--)
    {
        uint32 val = va_arg(ap, unsigned);
        AddPaletteRGB(val);
    }
    va_end(ap);
    //Analyze();
}

void Palette::AddPaletteRGB(uint32 val)
{
    Data.push_back( val);
}

const double illuminants[4][3*3] =
  { { // CIE C illuminant
      0.488718, 0.176204, 0.000000,
      0.310680, 0.812985, 0.0102048,
      0.200602, 0.0108109, 0.989795 },
    { // Adobe D65 illuminant
      0.576700, 0.297361, 0.0270328,
      0.185556, 0.627355, 0.0706879,
      0.188212, 0.0752847, 0.99124 },
    { // What is this?
      0.412453, 0.357580, 0.180423,
      0.212671, 0.715160, 0.072169,
      0.019334, 0.119193, 0.950227 } };
const double* GetIlluminant()
{
    switch(ColorComparing)
    {
        case Compare_CIE76_DeltaE:
            return illuminants[2];
        default:
            return illuminants[0];
    }
}

Palette Palette::GetSlice(unsigned begin, unsigned count) const
{
    Palette result;
    result.Data.assign( Data.begin()+begin, Data.begin()+begin+count );
    result.Analyze();
    return result;
}

Palette Palette::GetTwoColors(unsigned color1, unsigned color2) const
{
    Palette result;
    result.Data.push_back(Data[color1]);
    result.Data.push_back(Data[color2]);
    result.Analyze();
    return result;
}
Palette Palette::GetFourColors(unsigned c1,unsigned c2,unsigned c3,unsigned c4) const
{
    Palette result;
    result.Data.push_back(Data[c1]);
    result.Data.push_back(Data[c2]);
    result.Data.push_back(Data[c3]);
    result.Data.push_back(Data[c4]);
    result.Analyze();
    return result;
}
