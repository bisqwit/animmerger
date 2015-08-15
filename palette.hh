#ifndef bqtTileTrackerPaletteHH
#define bqtTileTrackerPaletteHH

#include <string>
#include <cmath>

#include "types.hh"

#include "kdtree.hh"

#define DefinePaletteMethods(callback) \
    callback(m,MedianCut) \
    callback(d,Diversity) \
    callback(b,BlendDiversity) \
    callback(g,Merging) \
    callback(o,Octree) \
    callback(q,NeuQuant)

#define MakeEnum(o,name) quant_##name,
enum PaletteMethod { DefinePaletteMethods(MakeEnum) };
#undef MakeEnum

struct PaletteMethodItem
{
    PaletteMethod       method;
    unsigned            size;
    // If this vector is nonempty, set palette to this selection
    std::vector<uint32> entries;
};
extern std::vector<PaletteMethodItem> PaletteReductionMethod;

extern enum ColorCompareMethod
{
    Compare_RGB,
    Compare_CIE76_DeltaE,
    Compare_CIE94_DeltaE,
    Compare_CMC_lc,
    Compare_BFD_lc,
    Compare_CIEDE2000_DeltaE,
    Compare_fparser
} ColorComparing;

extern double   DitherGamma;

inline double GammaCorrect(double x)
{
    //return std::pow(x, DitherGamma);
    if(DitherGamma == 1.0) return x;
    if(DitherGamma == 2.0) return x*x;
    if(DitherGamma == 1.5) return x * std::sqrt(x);
    return std::exp(std::log(x) * DitherGamma);
}
inline double GammaUncorrect(double x)
{
    //return std::pow(x, 1.0 / DitherGamma);
    if(DitherGamma == 1.0) return x;
    if(DitherGamma == 2.0) return std::sqrt(x);
    return std::exp(std::log(x) / DitherGamma);
}

const double* GetIlluminant();

struct GammaColorVec
{
    /* Maximally utilize for-loops to provide the
     * compiler chances for automatic vectorization.
     */
    union
    {
        float vec[4];
        struct { float r,g,b,a; };
    };

    GammaColorVec() {}
    GammaColorVec(float v) : vec{v,v,v,v} { }

    inline GammaColorVec operator- (const GammaColorVec& b) const
    {
        GammaColorVec res(*this);
        for(unsigned n=0; n<4; ++n) res.vec[n] -= b.vec[n];
        return res;
    }
    inline GammaColorVec operator+ (const GammaColorVec& b) const
    {
        GammaColorVec res(*this);
        for(unsigned n=0; n<4; ++n) res.vec[n] += b.vec[n];
        return res;
    }
    inline GammaColorVec& operator+= (const GammaColorVec& b)
    {
        for(unsigned n=0; n<4; ++n) vec[n] += b.vec[n];
        return *this;
    }
    inline GammaColorVec& operator-= (const GammaColorVec& b)
    {
        for(unsigned n=0; n<4; ++n) vec[n] -= b.vec[n];
        return *this;
    }
    inline GammaColorVec operator* (float v) const
    {
        GammaColorVec res(*this);
        for(unsigned n=0; n<4; ++n) res.vec[n] *= v;
        return res;
    }
    void ClampTo0and1()
    {
        for(unsigned n=0; n<4; ++n)
            /**/ if(vec[n] < 0.0f) vec[n] = 0.0f;
            else if(vec[n] > 1.0f) vec[n] = 1.0f;
    }
    inline uint32 GetGammaUncorrectedRGB() const
    {
        int rgb[3], aa = /*GammaUncorrect*/(a)*127.0f + 0.5f;
        for(unsigned n=0; n<3; ++n)
            rgb[n] = GammaUncorrect( vec[n] ) * 255.0f + 0.5f;
        return (rgb[0]<<16)|(rgb[1]<<8)|(rgb[2])|(aa<<24);
    }
};

struct ColorInfo
{
    // 32-bit ARGB, as it is saved/loaded with a file:
    union
    {
        uint32 rgb;
      #ifdef IS_BIG_ENDIAN
        struct { unsigned char A,R,G,B; };
      #else
        struct { unsigned char B,G,R,A; };
      #endif
    };
    // Luma in 0..255000 range:
    int     luma;
    // ARGB, gamma corrected, all values in 0..1 range:
    GammaColorVec gammac;
    // CIE XYZ value
    struct
    {
        double  X,Y,Z;
    } xyz;
    // CIE L*a*b* value
    struct
    {
        double  L,a,b, C,h;
    } lab;
public:
    // Dummy constructor
    ColorInfo() {}
    // Constructor with 32-bit ARGB value
    ColorInfo(uint32 val)
    {
        rgb = val;
        MakeNeeded<false,false,false,false> ();
    }
    // Constructor with 32-bit ARGB value in separate fields
    ColorInfo(int rr,int gg,int bb, int aa=0)
    {
        rgb = ((rr<<16)|(gg<<8)|(bb)|(aa<<24));
        MakeNeeded<false,false,false,false> ();
    }
    // Constructor with 32-bit ARGB value + gamma-corrected values
    // Use this constructor if you already have the gamma-corrected
    // values, so they will not need to be recalculated.
    ColorInfo(uint32 val, double rr,double gg,double bb,double aa)
    {
        rgb = val;
        gammac.r = rr;
        gammac.g = gg;
        gammac.b = bb;
        gammac.a = aa;
        MakeNeeded<false,true,false,false> ();
    }
    ColorInfo(uint32 val, const GammaColorVec& gammavec)
    {
        rgb    = val;
        gammac = gammavec;
        MakeNeeded<false,true,false,false> ();
    }
    ColorInfo(const GammaColorVec& gammavec)
    {
        rgb    = gammavec.GetGammaUncorrectedRGB();
        gammac = gammavec;
        MakeNeeded<false,true,false,false> ();
    }
    ColorInfo& operator=(const GammaColorVec& gammavec)
    {
        rgb    = gammavec.GetGammaUncorrectedRGB();
        gammac = gammavec;
        MakeNeeded<false,true,false,false> ();
        return *this;
    }
private:
    // I don't like having to include the function definition in this .hh
    // file, but it is the only choice if I want it to be inlined _and_
    // usable in both palette.cc and dither.cc .
    // Same for the constructors in this class.
    template<bool HaveLuma, bool HaveGamma, bool HaveXYZ, bool HaveLAB>
    inline void MakeNeeded()
    {
        // Luma is always needed (Analyze and most dithers break without it)
        if(!HaveLuma)
        {
            luma = R*299 + G*587 + B*114;
        }

        // Gamma is always needed (all dithers use it, including error diffusion)
        if(!HaveGamma)
        {
            gammac.r = GammaCorrect( R/255.0 );
            gammac.g = GammaCorrect( G/255.0 );
            gammac.b = GammaCorrect( B/255.0 );
            gammac.a = A/127.0;
        }

        // Don't calculate L*a*b* if we will not need it.
        // Only RGB comparison doesn't need it.
        if(!HaveLAB
        && ColorComparing != Compare_RGB)
        {
            if(!HaveXYZ)
            {
                const double* illum = GetIlluminant();
                // double Rn = gammac.r, Gn = gammac.g, Bn = gammac.b;
                double Rn = R/255.0, Gn=G/255.0, Bn=B/255.0;
                xyz.X = (illum[0])*Rn + (illum[3])*Gn + (illum[6])*Bn;
                xyz.Y = (illum[1])*Rn + (illum[4])*Gn + (illum[7])*Bn;
                xyz.Z = (illum[2])*Rn + (illum[5])*Gn + (illum[8])*Bn;
            }
            // Use Profile illuminant - D65 */
            const double* illum = GetIlluminant();
            double Xn = 1.0 / (illum[0]+illum[1]+illum[2]);
            double Yn = 1.0 / (illum[3]+illum[4]+illum[5]);
            double Zn = 1.0 / (illum[6]+illum[7]+illum[8]);
            double x = xyz.X * Xn;
            double y = xyz.Y * Yn;
            double z = xyz.Z * Zn;
            const double threshold1 = (6*6*6.0)/(29*29*29.0);
            const double threshold2 = (29*29.0)/(6*6*3.0);
            double x1 = (x > threshold1) ? pow(x, 1.0/3.0) : (threshold2*x)+(4/29.0);
            double y1 = (y > threshold1) ? pow(y, 1.0/3.0) : (threshold2*y)+(4/29.0);
            double z1 = (z > threshold1) ? pow(z, 1.0/3.0) : (threshold2*z)+(4/29.0);
            lab.L = (29*4)*y1 - (4*4);
            lab.a = (500*(x1-y1) );
            lab.b = (200*(y1-z1) );

            // Of color comparison algorithms that
            // might need L*a*b*, only CIE76 doesn't need C & h
            if(ColorComparing != Compare_CIE76_DeltaE)
            {
                lab.C = std::sqrt(lab.a*lab.a + lab.b+lab.b);
                lab.h = std::atan2(lab.b, lab.a);
            }
        }
    }
};

struct Palette
{
    void SetHardcoded(unsigned n, ...);

    void Analyze(); // Assigns luma to each element.

    size_t Size() const { return Data.size(); }
    size_t NumCombinations() const { return Combinations.size(); }

    uint32 GetColor(unsigned index) const { return Data[index].rgb; }
    int    GetLuma(unsigned index) const { return Data[index].luma; }
    const ColorInfo& GetMeta(unsigned index) const { return Data[index]; }

    uint32 GetCombinationColor(unsigned index) const { return Combinations[index].combination.rgb; }
    int    GetCombinationLuma(unsigned index) const { return Combinations[index].combination.luma; }
    const ColorInfo& GetCombinationMeta(unsigned index) const { return Combinations[index].combination; }

    Palette GetSlice(unsigned offset, unsigned count) const;
    Palette GetTwoColors(unsigned color1, unsigned color2) const;
    Palette GetFourColors(unsigned,unsigned,unsigned,unsigned) const;

    void AddPaletteRGB(uint32 p);
public:
    struct PaletteItem: public ColorInfo
    {
        PaletteItem()         : ColorInfo()  {}
        PaletteItem(uint32 v) : ColorInfo(v) { }
    };
    std::vector<PaletteItem> Data;
    struct Combination
    {
        std::vector<unsigned> indexlist;
        ColorInfo combination;

        Combination(const std::vector<unsigned>& i, uint32 v)
            : indexlist(i), combination(v) { }
        Combination(const std::vector<unsigned>& i, uint32 v,
                    double r,double g,double b,double a)
            : indexlist(i), combination(v, r,g,b,a) { }
        Combination(const std::vector<unsigned>& i, uint32 v,
                    const GammaColorVec& gammac)
            : indexlist(i), combination(v, gammac) { }
    };
    std::vector<Combination> Combinations;

    KDTree<unsigned,4> CombinationTree;

    std::pair<unsigned,double>
        FindClosestCombinationIndex(const ColorInfo& meta) const;
};

typedef std::vector<unsigned short> MixingPlan;

MixingPlan FindBestMixingPlan(const ColorInfo& input,  const Palette& Palette);

double ColorCompare(const ColorInfo&, const ColorInfo& );
void SetColorCompareFormula(const std::string& expr);

#endif
