#include <cstdio>
#include <cmath>
#include <iostream>

#include "canvas.hh"
#include "openmp.hh"
#include "align.hh"
#include "palette.hh"
#include "dither.hh"
#include "quantize.hh"
#include "fparser.hh"

#include <gd.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef CGA16mode
# define CGA16mode 0
#endif

int pad_top=0, pad_bottom=0, pad_left=0, pad_right=0;

int SaveGif = -1;
bool UseDitherCache = true;
std::string OutputNameTemplate = "%2$s-%1$04u.%3$s";

#ifdef __MINGW32__
/* A version of snprintf() that adds support for positional parameters */
static int Msnprintf(char* target, std::size_t limit, const char* format, ...)
{
    std::vector<char>     types;
    std::vector<uint64_t> parameters;

    auto Set = [&](unsigned n, char type)
    {
        if(!n--) return;
        if(types.size() <= n) types.resize(n+1);
        types[n] = type;
    };

    int auto_number = 0;

    va_list ap;
    va_start(ap, format);
    // First scan the format to get the list of all parameters
    for(const char* fmt = format; *fmt; ++fmt)
    {
        if(*fmt != '%') continue;
        if(fmt[1] == '%') { ++fmt; continue; }
        int n, argno = -1;
    another_number:
        n=0;
        ++fmt;
        while(*fmt && *fmt >= '0' && *fmt <= '9') n = n*10 + (*fmt++ - '0');
        if(*fmt == '.') goto another_number;
        if(*fmt == '$') { argno = n; goto another_number; }
        int i = 'd', d = 'f';
        if(argno < 0) argno = ++auto_number;
    more_fmt:
        switch(*fmt)
        {
            case 'l': if(i == 'd') i = 'l'; else {
            case 'L': i = 'L'; d = 'F';          } goto more_fmt;
            case 'z': i = 'z'; goto more_fmt;
            case 'h': goto more_fmt;
            case 'p':
            case 'n':
            case 's':           Set(argno, 's'); break;
            case 'o': case 'u': case 'c':
            case 'x': case 'X':
            case 'd': case 'i': Set(argno, i); break;
            case 'e': case 'E':
            case 'f': case 'F':
            case 'g': case 'G': Set(argno, d); break;
        }
        if(!*fmt) break;
    }
    // Then load all the parameters
    for(auto c: types)
        switch(c)
        {
            case 'd': parameters.push_back(va_arg(ap, unsigned int)); break;
            case 'l': parameters.push_back(va_arg(ap, unsigned long)); break;
            case 'L': parameters.push_back(va_arg(ap, unsigned long long)); break;
            case 'z': parameters.push_back(va_arg(ap, size_t)); break;
            case 's': parameters.push_back((uint64_t)va_arg(ap, void*)); break;
            case 'f':
            {
                double d = va_arg(ap, double);
                parameters.push_back(*(uint64_t*)&d);
                break;
            }
            case 'F':
            {
                double d = va_arg(ap, long double);
                parameters.push_back(*(uint64_t*)&d);
                break;
            }
        }
    va_end(ap);

    auto_number = 0;
    std::string output;
    // Rescan the format string, taking one token at a time.
    for(const char* fmt = format; *fmt; )
    {
        if(*fmt != '%' || !fmt[1]) { output += *fmt++; continue; }
        if(fmt[1] == '%') { output += *fmt; fmt += 2; continue; }
        std::string fmt_string;
        const char* fmt_start = fmt;
        int n, argno = -1;
    another_number2:
        fmt_string.append(fmt_start, ++fmt); fmt_start = fmt;
    another_number2b:
        n=0;
        while(*fmt && *fmt >= '0' && *fmt <= '9') n = n*10 + (*fmt++ - '0');
        if(*fmt == '.') { goto another_number2; }
        if(*fmt == '$') { fmt_start = ++fmt; argno = n; goto another_number2b; }
        int i = 'd', d = 'f';
        if(argno < 0) argno = ++auto_number;
        char typecode = 0;
        bool uns = false;
    more_fmt2:
        switch(*fmt++)
        {
            case 'l': if(i == 'd') i = 'l'; else {
            case 'L': i = 'L'; d = 'F';          } goto more_fmt2;
            case 'z': i = 'z'; goto more_fmt2;
            case 'h': goto more_fmt2;
            case 'p':
            case 'n':
            case 's':           typecode = ('s'); break;
            case 'o': case 'u': case 'c':
            case 'x': case 'X':
            case 'd': case 'i': typecode = (i); break;
            case 'e': case 'E':
            case 'f': case 'F':
            case 'g': case 'G': typecode = (d); break;
        }
        fmt_string.append(fmt_start, fmt);

        if(argno > 0) --argno;
        uint64_t param = parameters[argno];
        n=0;
        switch(typecode)
        {
            case 'd':
                if(uns)
                    n=snprintf(target, limit, fmt_string.c_str(), (unsigned int)(param));
                else
                    n=snprintf(target, limit, fmt_string.c_str(), (int)(param));
                break;
            case 'l':
                if(uns)
                    n=snprintf(target, limit, fmt_string.c_str(), (unsigned long)(param));
                else
                    n=snprintf(target, limit, fmt_string.c_str(), (long)(param));
                break;
            case 'L':
                if(uns)
                    n=snprintf(target, limit, fmt_string.c_str(), (unsigned long long)(param));
                else
                    n=snprintf(target, limit, fmt_string.c_str(), (long long)(param));
                break;
            case 'z': n=snprintf(target, limit, fmt_string.c_str(), size_t(param)); break;
            case 's': n=snprintf(target, limit, fmt_string.c_str(), (void*)(param)); break;
            case 'f':
            {
                double d = *(double*)param;
                n=snprintf(target, limit, fmt_string.c_str(), d);
                break;
            }
            case 'F':
            {
                long double d = *(double*)param;
                n=snprintf(target, limit, fmt_string.c_str(), d);
                break;
            }
        }
        output.append(target, target+n);
    }
    if(output.size() > limit) output.erase(output.begin()+limit, output.end());
    for(unsigned n=0, m=output.size(); n<m; ++n)
        target[n] = output[n];
    return output.size();
}
#endif

class CanvasFunctionParser: public FunctionParser
{
public:
    static double MakeRGB(const double* vars)
    {
        int r = vars[0], g = vars[1], b = vars[2];
        int l = r*299 + g*587 + b*114;
        if(l <= 0)        return double( 0x000000 );
        if(l >= 1000*255) return double( 0xFFFFFF );
        double ll = l / 255e3, ss = 1.0;
        for(unsigned n=0; n<3; ++n)
        {
            if(vars[n] > 255.0)    ss = std::min(ss, (ll-1.0) / (ll - vars[n]/255.0));
            else if(vars[n] < 0.0) ss = std::min(ss,     ll   / (ll - vars[n]/255.0));
        }
        if(ss != 1.0)
        {
            r = ((r/255.0 - ll) * ss + ll) * 255;
            g = ((g/255.0 - ll) * ss + ll) * 255;
            b = ((b/255.0 - ll) * ss + ll) * 255;
        }
        return double( (r<<16) + (g<<8) + (b) );
    }
    CanvasFunctionParser()
    {
        AddConstant("pi",  M_PI);
        AddConstant("e",   M_E);
        AddFunction("make_rgb", MakeRGB, 3);
    }
};

static CanvasFunctionParser transformation_parser;
std::string transform_common;
std::string transform_r = "r";
std::string transform_g = "g";
std::string transform_b = "b";
bool UsingTransformations = false;
bool TransformationDependsOnX       = false;
bool TransformationDependsOnY       = false;
bool TransformationDependsOnFrameNo = false;
bool TransformationGsameAsR = false;
bool TransformationBsameAsR = false;
bool TransformationBsameAsG = false;

void SetColorTransformations()
{
    // First, for diagnostics, parse each color component separately
    {
        std::string tmp_r = transform_common + transform_r;
        std::string tmp_g = transform_common + transform_g;
        std::string tmp_b = transform_common + transform_b;
        CanvasFunctionParser parser_r;
        CanvasFunctionParser parser_g;
        CanvasFunctionParser parser_b;
        int r_error = parser_r.Parse(tmp_r.c_str(), "r,g,b,frameno,x,y");
        int g_error = parser_g.Parse(tmp_g.c_str(), "r,g,b,frameno,x,y");
        int b_error = parser_b.Parse(tmp_b.c_str(), "r,g,b,frameno,x,y");
        if(r_error >= 0 || g_error >= 0 || b_error >= 0)
        {
            if(r_error >= 0)
                std::fprintf(stderr, "Parse error (%s) in red color formula:\n%s\n%*s\n",
                    parser_r.ErrorMsg(), tmp_r.c_str(), r_error+1, "^");
            if(g_error >= 0)
                std::fprintf(stderr, "Parse error (%s) in green color formula:\n%s\n%*s\n",
                    parser_g.ErrorMsg(), tmp_g.c_str(), g_error+1, "^");
            if(b_error >= 0)
                std::fprintf(stderr, "Parse error (%s) in blue color formula:\n%s\n%*s\n",
                    parser_b.ErrorMsg(), tmp_b.c_str(), b_error+1, "^");
            return;
        }
    }

    TransformationGsameAsR = transform_g == transform_r;
    TransformationBsameAsR = transform_b == transform_r;
    TransformationBsameAsG = transform_b == transform_g;
    // Then produce an optimized parser that produces all components at once
    std::string merged = transform_common;
    std::string r_expr = transform_r;
    std::string g_expr = transform_g;
    std::string b_expr = transform_b;
    if(TransformationGsameAsR || TransformationBsameAsR)
    {
        merged += "animmerger_R:=(" + r_expr + ");";
        r_expr = "animmerger_R";
    }
    if(TransformationGsameAsR)
        g_expr = "animmerger_R";
    else if(TransformationBsameAsG)
    {
        merged += "animmerger_G:=(" + g_expr + ");";
        g_expr = "animmerger_G";
    }
    if(TransformationBsameAsR)
        b_expr = "animmerger_R";
    else if(TransformationBsameAsG)
        b_expr = "animmerger_G";
    merged += "make_rgb(" + r_expr + "," + g_expr + "," + b_expr + ")";

    int error = transformation_parser.Parse(merged.c_str(), "r,g,b,frameno,x,y");
    if(error >= 0)
    {
        if(error >= 0)
            std::fprintf(stderr, "Parse error (%s) in color formula:\n%s\n%*s\n",
                transformation_parser.ErrorMsg(), merged.c_str(), error+1, "^");
        return;
    }

    UsingTransformations = transform_r != "r"
                        || transform_g != "g"
                        || transform_b != "b";

    if(UsingTransformations)
    {
        if(verbose >= 1)
        {
            std::printf("Merged color transformation formula: %s\n", merged.c_str());
            if(verbose >= 3)
            {
                std::printf("Bytecode before optimization:\n");
                std::fflush(stdout);
                transformation_parser.PrintByteCode(std::cout);
                std::cout << std::flush;
            }
        }
        transformation_parser.Optimize();
        transformation_parser.Optimize();
        if(verbose >= 3)
        {
            std::printf("Bytecode after optimization:\n");
            std::fflush(stdout);
            transformation_parser.PrintByteCode(std::cout);
            std::cout << std::flush;
        }
    }

    TransformationDependsOnX       = false;
    TransformationDependsOnY       = false;
    TransformationDependsOnFrameNo = false;
    if(UsingTransformations)
    {
        if(CanvasFunctionParser().Parse(merged.c_str(), "r,g,b,frameno,y") >= 0)
            TransformationDependsOnX = true;
        if(CanvasFunctionParser().Parse(merged.c_str(), "r,g,b,frameno,x") >= 0)
            TransformationDependsOnY = true;
        if(CanvasFunctionParser().Parse(merged.c_str(), "r,g,b,x,y") >= 0)
            TransformationDependsOnFrameNo = true;

        if(verbose >= 2)
        {
            std::printf(" - Found out that it %s on the X coordinate\n",
                TransformationDependsOnX ? "depends":"doesn't depend"
                );
            std::printf(" - Found out that it %s on the Y coordinate\n",
                TransformationDependsOnY ? "depends":"doesn't depend"
                );
            std::printf(" - Found out that it %s on the frame number\n",
                TransformationDependsOnFrameNo ? "depends":"doesn't depend"
                );
        }
    }
}
inline double TransformColor(int r, int g, int b, unsigned frameno,unsigned x,unsigned y)
{
    double vars[6] =
    {
        double(r), double(g), double(b),
        double(frameno), double(x), double(y)
    };
    return transformation_parser.Eval(vars);
}
void TransformColor(uint32& pix, unsigned frameno,unsigned x,unsigned y)
{
    int r = (pix >> 16) & 0xFF, g = (pix >> 8) & 0xFF, b = (pix & 0xFF);
    double pix_dbl = TransformColor(r,g,b, frameno,x,y);
    pix = (pix & 0xFF000000u) | ((unsigned) pix_dbl);
}

static inline bool veq
    (const VecType<uint32>& a,
     const VecType<uint32>& b) // For ChangeLog
{
    if(a.size() != b.size()) return false;
    return std::memcmp(&a[0], &b[0], a.size() * sizeof(uint32)) == 0;
}

const VecType<uint32>
TILE_Tracker::LoadScreen(int ox,int oy, unsigned sx,unsigned sy,
                         unsigned timer,
                         PixelMethod method) const
{
    // Create the result vector filled with default pixel value
    VecType<uint32> result(sy*sx, DefaultPixel);

    const int xbegin = ox;
    const int xend   = ox+sx-1;

    const int xscreen_begin = xbegin/256;
    const int xscreen_end   = xend  /256;

    const int ybegin = oy;
    const int yend   = oy+sy-1;

    const int yscreen_begin = ybegin/256;
    const int yscreen_end   = yend  /256;

/*
    std::fprintf(stderr, "Loading screens x(%d..%d)y(%d..%d)\n",
        xscreen_begin,xscreen_end,
        yscreen_begin,yscreen_end);
*/

    // Load each cube that falls into the requested region

    unsigned targetpos=0;
    unsigned this_cube_ystart = oy&255;
    for(int yscreen=yscreen_begin; yscreen<=yscreen_end; ++yscreen)
    {
        unsigned this_cube_yend = yscreen==yscreen_end ? ((oy+sy-1)&255) : 255;
        unsigned this_cube_ysize = (this_cube_yend-this_cube_ystart)+1;

        ymaptype::const_iterator yi = screens.find(yscreen);
        if(yi != screens.end())
        {
            const xmaptype& xmap = yi->second;

            unsigned this_cube_xstart = ox&255;
            for(int xscreen=xscreen_begin; xscreen<=xscreen_end; ++xscreen)
            {
                unsigned this_cube_xend = xscreen==xscreen_end ? ((ox+sx-1)&255) : 255;
                unsigned this_cube_xsize = (this_cube_xend-this_cube_xstart)+1;
    /*
                std::fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                    this_cube_xstart,this_cube_xend,
                    this_cube_ystart,this_cube_yend);
    */
                xmaptype::const_iterator xi = xmap.find(xscreen);
                if(xi != xmap.end())
                {
                    const cubetype& cube = xi->second;
                    /* If this screen is not yet initialized, we'll skip over
                     * it, since there's no real reason to initialize it at
                     * this point. */

                    cube.pixels->GetLiveSectionInto(
                        method,timer,
                        &result[targetpos], sx,
                        this_cube_xstart,
                        this_cube_ystart,
                        this_cube_xsize,
                        this_cube_ysize);
                }

                targetpos+= this_cube_xsize;

                this_cube_xstart=0;
            }
            targetpos += sx * (this_cube_ysize-1);
        }
        else
            targetpos += sx * this_cube_ysize;

        this_cube_ystart=0;
    }

    return result;
}

const VecType<uint32>
TILE_Tracker::LoadBackground(int ox,int oy, unsigned sx,unsigned sy) const
{
    // Create the result vector filled with default pixel value
    VecType<uint32> result(sy*sx, DefaultPixel);

    const int xbegin = ox;
    const int xend   = ox+sx-1;

    const int xscreen_begin = xbegin/256;
    const int xscreen_end   = xend  /256;

    const int ybegin = oy;
    const int yend   = oy+sy-1;

    const int yscreen_begin = ybegin/256;
    const int yscreen_end   = yend  /256;

/*
    std::fprintf(stderr, "Loading screens x(%d..%d)y(%d..%d)\n",
        xscreen_begin,xscreen_end,
        yscreen_begin,yscreen_end);
*/

    // Load each cube that falls into the requested region

    unsigned targetpos=0;
    unsigned this_cube_ystart = oy&255;
    for(int yscreen=yscreen_begin; yscreen<=yscreen_end; ++yscreen)
    {
        unsigned this_cube_yend = yscreen==yscreen_end ? ((oy+sy-1)&255) : 255;
        unsigned this_cube_ysize = (this_cube_yend-this_cube_ystart)+1;

        ymaptype::const_iterator yi = screens.find(yscreen);
        if(yi != screens.end())
        {
            const xmaptype& xmap = yi->second;

            unsigned this_cube_xstart = ox&255;
            for(int xscreen=xscreen_begin; xscreen<=xscreen_end; ++xscreen)
            {
                unsigned this_cube_xend = xscreen==xscreen_end ? ((ox+sx-1)&255) : 255;
                unsigned this_cube_xsize = (this_cube_xend-this_cube_xstart)+1;
    /*
                std::fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                    this_cube_xstart,this_cube_xend,
                    this_cube_ystart,this_cube_yend);
    */
                xmaptype::const_iterator xi = xmap.find(xscreen);
                if(xi != xmap.end())
                {
                    const cubetype& cube = xi->second;
                    /* If this screen is not yet initialized, we'll skip over
                     * it, since there's no real reason to initialize it at
                     * this point. */

                    cube.pixels->GetStaticSectionInto(
                        &result[targetpos], sx,
                        this_cube_xstart,
                        this_cube_ystart,
                        this_cube_xsize,
                        this_cube_ysize);
                }

                targetpos+= this_cube_xsize;

                this_cube_xstart=0;
            }
            targetpos += sx * (this_cube_ysize-1);
        }
        else
            targetpos += sx * this_cube_ysize;

        this_cube_ystart=0;
    }

    return result;
}

void
TILE_Tracker::PutScreen
    (const uint32*const input, int ox,int oy, unsigned sx,unsigned sy,
     unsigned timer)
{
    /* Nearly the same as LoadScreen. */

    const int xbegin = ox;
    const int xend   = ox+sx-1;

    const int xscreen_begin = xbegin/256;
    const int xscreen_end   = xend  /256;

    const int ybegin = oy;
    const int yend   = oy+sy-1;

    const int yscreen_begin = ybegin/256;
    const int yscreen_end   = yend  /256;

/*
    std::fprintf(stderr, "Writing screens x(%d..%d)y(%d..%d)\n",
        xscreen_begin,xscreen_end,
        yscreen_begin,yscreen_end);
*/
    unsigned targetpos=0;
    unsigned this_cube_ystart = oy&255;
    for(int yscreen=yscreen_begin; yscreen<=yscreen_end; ++yscreen)
    {
        xmaptype& xmap = screens[yscreen];

        unsigned this_cube_yend = yscreen==yscreen_end ? ((oy+sy-1)&255) : 255;
        unsigned this_cube_ysize = (this_cube_yend-this_cube_ystart)+1;

        unsigned this_cube_xstart = ox&255;
        for(int xscreen=xscreen_begin; xscreen<=xscreen_end; ++xscreen)
        {
            unsigned this_cube_xend = xscreen==xscreen_end ? ((ox+sx-1)&255) : 255;
            unsigned this_cube_xsize = (this_cube_xend-this_cube_xstart)+1;

            cubetype& cube = xmap[xscreen];

            /* If this screen is not yet initialized, we'll initialize it */
            if(cube.pixels.empty())
            {
                //cube.pixels.resize(256*256);
                cube.pixels.init();
            }
            cube.changed = true;

/*
            std::fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                this_cube_xstart,this_cube_xend,
                this_cube_ystart,this_cube_yend);
*/
            cube.pixels->PutSectionInto(
                timer,
                &input[targetpos], sx,
                this_cube_xstart,
                this_cube_ystart,
                this_cube_xsize,
                this_cube_ysize);

            targetpos+= this_cube_xsize;

            this_cube_xstart=0;
        }

        targetpos += sx * (this_cube_ysize-1);

        this_cube_ystart=0;
    }
}

bool TILE_Tracker::IsHeavyDithering(bool animated) const
{
    if(!PaletteReductionMethod.empty() && !(SaveGif == -1 && !animated))
    {
        // Will use dithering engine, so check if the dithering
        // will incur a significant load. If it does, do threading
        // per scanline rather than by frame.
        if(DitherColorListSize > 1
        && DitherErrorFactor > 0.0
        && DitherColorListSize *
             (CurrentPalette.Size() + CurrentPalette.NumCombinations())
           > 1000
          )
        {
            return true;
        }
    }
    return false;
}

void TILE_Tracker::Save(unsigned method)
{
    if(CurrentTimer == 0)
        return;

    if(method == ~0u)
    {
        for(unsigned m=0; m<NPixelMethods; ++m)
        {
            if(pixelmethods_result & (1ul << m))
                Save( (PixelMethod) m);
        }
        return;
    }

    const bool animated = (1ul << method) & AnimatedPixelMethodsMask;

    std::fprintf(stderr, "Saving(%d)\n", CurrentTimer);

    if(!PaletteReductionMethod.empty())
    {
        DitheringMatrix = CreateDispersedDitheringMatrix();
        TemporalMatrix = CreateTemporalDitheringMatrix();
    }

#ifdef _OPENMP
    omp_set_nested(1);
#endif

    if(animated)
    {
        unsigned SavedTimer = CurrentTimer;

        if((1ul << method) & LoopingPixelMethodsMask)
        {
            if(SavedTimer >= LoopingLogLength)
                SavedTimer = LoopingLogLength;
        }

        if(!PaletteReductionMethod.empty())
        {
            CreatePalette( (PixelMethod) method, SavedTimer );
        }

        for(unsigned frame=0; frame<SavedTimer; frame+=1)
        //for(unsigned frame=5200; frame<SavedTimer; frame+=1)
        {
            //std::fprintf(stderr, "Saving frame %u/%u @ %u\n",
            //    frame, SavedTimer, SequenceBegin);
            SaveFrame( (PixelMethod)method, frame, SequenceBegin + frame);
        }
        fflush(stdout);
    }
    else
    {
        if(!PaletteReductionMethod.empty())
        {
            CreatePalette( (PixelMethod) method, 1 );
        }

        for(unsigned dummy=0; dummy<1; ++dummy)
            SaveFrame( (PixelMethod)method, 0, SequenceBegin);
    }
}

template<bool TransformColors>
HistogramType TILE_Tracker::CountColors(PixelMethod method, unsigned nframes)
{
    HistogramType Histogram;

    // Create a histogram of the used colors, unless it's going
    // to be replaced immediately thereafter.
    if(PaletteReductionMethod.empty()
    || PaletteReductionMethod.front().entries.empty())
    {
        const int ymi = ymin, yma = ymax;
        const int xmi = xmin, xma = xmax;
        const unsigned wid = xma-xmi;
        const unsigned hei = yma-ymi;
        std::fprintf(stderr, "Counting colors... (%u frames)\n", nframes);
        VecType<uint32> prev_frame;
        for(unsigned frameno=0; frameno<nframes; frameno+=1)
        {
            /*if(frameno == 20)
                frameno = nframes*6/16;
            else if(frameno == nframes*9/16)
                frameno = nframes*999/1000;*/

            std::fprintf(stderr, "\rFrame %u/%u, %u so far...",
                frameno+1, nframes, (unsigned) Histogram.size());
            std::fflush(stderr);
          #if 1
            /* Only count histogram from content that
             * changes between previous and current frame
             */
            VecType<uint32> frame ( LoadScreen(xmi,ymi, wid,hei, frameno, method) );
            unsigned a=0;
            for(; a < prev_frame.size() && a < frame.size(); ++a)
            {
                if(frame[a] != prev_frame[a])
                {
                    uint32 p = prev_frame[a], q = frame[a];
                    if(TransformColors)
                    {
                        TransformColor(p, frameno, a/256, a%256);
                        TransformColor(q, frameno, a/256, a%256);
                    }
                    ++Histogram[p];
                    ++Histogram[q];
                }
            }
            for(; a < frame.size(); ++a)
            {
                uint32 p = frame[a];
                if(TransformColors)
                {
                    TransformColor(p, frameno,a/256, a%256);
                }
                ++Histogram[p];
            }
            prev_frame.swap(frame);
          #else
            for(ymaptype::const_iterator
                yi = screens.begin();
                yi != screens.end();
                ++yi)
            for(xmaptype::const_iterator
                xi = yi->second.begin();
                xi != yi->second.end();
                ++xi)
            {
                const cubetype& cube      = xi->second;
                uint32 result[256*256];
                cube.pixels->GetLiveSectionInto(method, frameno, result,256, 0,0, 256,256);
                for(unsigned a=0; a<256*256; ++a)
                {
                    uint32 p = result[a];
                    if(TransformColors)
                    {
                        TransformColor(p, frameno, a/256, a%256);
                    }
                    ++Histogram[p];
                }
            }
          #endif
        }
        // Reduce the histogram into a usable palette
        std::fprintf(stderr, "\n%u colors detected\n",(unsigned) Histogram.size());
    }
    return Histogram;
}

void TILE_Tracker::CreatePalette(PixelMethod method, unsigned nframes)
{
    //return; // HACK: DON'T CHANGE PALETTE

    #if CGA16mode
    // composite cga (to be rendered on 640x200 monochrome mode)
    CurrentPalette.SetHardcoded(15,
        0x000000, // 0000
        0x007A00, // 0001
        0x1B33DF, // 0010
        0x01ADDE, // 0011
        0x990580, // 0100
        0x7F7F7F, // 0101, 1010
        0xB538FF, // 0110
        0x9AB2FF, // 0111
        0x644C00, // 1000
        0x49C600, // 1001
        0x65F97E, // 1011
        0xFD5120, // 1100
        0xE3CB1F, // 1101
        0xFF84FF, // 1110
        0xFFFFFF); // 1111
    CurrentPalette.Analyze();
    return;
    #endif

    HistogramType Histogram = UsingTransformations
        ? CountColors<true>(method, nframes)
        : CountColors<false>(method, nframes);
    ReduceHistogram(Histogram);

    const bool animated = (1ul << method) & AnimatedPixelMethodsMask;
    unsigned limit = Histogram.size();
    if(SaveGif == 1 || (SaveGif == -1 && animated)) limit = 256;
    CurrentPalette = MakePalette(Histogram, limit);
}

void TILE_Tracker::SaveFrame(PixelMethod method, unsigned frameno, unsigned img_counter)
{
    const bool animated = (1ul << method) & AnimatedPixelMethodsMask;

    const int ymi = ymin, yma = ymax;
    const int xmi = xmin, xma = xmax;

    unsigned wid = xma-xmi;
    unsigned hei = yma-ymi;

    if(wid <= 1 || hei <= 1) return;

    VecType<uint32> screen ( LoadScreen(xmi,ymi, wid,hei, frameno, method) );

    const char* methodnamepiece = "tile";
    if(pixelmethods_result != (1ul << method))
    {
        // Multi-method output
        #define MakePixName(o,f,name) #name,
        static const char* const Templates[NPixelMethods] =
        {
             DefinePixelMethods(MakePixName)
        };
        #undef MakePixName
        methodnamepiece = Templates[method];
    }

    bool MakeGif  = SaveGif == 1 || (SaveGif == -1 && animated);
    bool Dithered = !PaletteReductionMethod.empty();

    char Filename[512] = {0}; // explicit init keeps valgrind happy
#ifdef __MINGW32__
    Msnprintf(Filename, sizeof(Filename),
#else
    std::snprintf(Filename, sizeof(Filename),
#endif
        OutputNameTemplate.c_str(),
        img_counter,
        methodnamepiece,
        MakeGif ? "gif" : "png");
    Filename[sizeof(Filename)-1] = '\0';

    std::fprintf(stderr, "%s: (%d,%d)-(%d,%d)\n", Filename, 0,0, xma-xmi, yma-ymi);
    std::fflush(stderr);

    bool was_identical = false;

    if(TemporalDitherSize == 1 && animated && !UsingTransformations)
    {
        if(veq(screen, LastScreen) && !LastFilename.empty())
        {
            std::fprintf(stderr, "->link (%u,%u)\n",
                (unsigned)screen.size(),
                (unsigned)LastScreen.size());

            std::string cmd = "ln -f "+LastFilename+" "+Filename;
            system(cmd.c_str());

            was_identical = true;
        }
        LastScreen   = screen;
        LastFilename = Filename;
    }
    if(was_identical) return;

    gdImagePtr im;
    if(Dithered)
    {
        if(Diffusion == Diffusion_None)
        {
          #if CGA16mode
            if(UsingTransformations)
              im = CreateFrame_Palette_Dither_CGA16<true,false>(screen, frameno, wid, hei);
            else
              im = CreateFrame_Palette_Dither_CGA16<false,false>(screen, frameno, wid, hei);
          #else
            if(UsingTransformations)
              im = CreateFrame_Palette_Dither<true,false>(screen, frameno, wid, hei);
            else
              im = CreateFrame_Palette_Dither<false,false>(screen, frameno, wid, hei);
          #endif
        }
        else
        {
          #if CGA16mode
            if(UsingTransformations)
              im = CreateFrame_Palette_Dither_CGA16<true,true>(screen, frameno, wid, hei);
            else
              im = CreateFrame_Palette_Dither_CGA16<false,true>(screen, frameno, wid, hei);
          #else
            if(UsingTransformations)
              im = CreateFrame_Palette_Dither<true,true>(screen, frameno, wid, hei);
            else
              im = CreateFrame_Palette_Dither<false,true>(screen, frameno, wid, hei);
          #endif
        }
    }
    else
    {
        if(MakeGif)
          if(UsingTransformations)
            im = CreateFrame_Palette_Auto<true>(screen, frameno, wid, hei);
          else
            im = CreateFrame_Palette_Auto<false>(screen, frameno, wid, hei);
        else
          if(UsingTransformations)
            im = CreateFrame_TrueColor<true>(screen, frameno, wid, hei);
          else
            im = CreateFrame_TrueColor<false>(screen, frameno, wid, hei);
    }

    ImgResult imgdata;
    if(MakeGif && gdImageTrueColor(im))
        gdImageTrueColorToPalette(im, false, 256);

    imgdata.first = MakeGif
        ? gdImageGifPtr(im, &imgdata.second)
        : gdImagePngPtrEx(im, &imgdata.second, 1);
    gdImageDestroy(im);

    if(imgdata.first)
    {
        FILE* fp = fopen(Filename, "wb");
        if(!fp)
            std::perror(Filename);
        else
        {
            std::fwrite(imgdata.first, 1, imgdata.second, fp);
            std::fclose(fp);
        }
        gdFree(imgdata.first);
    }
}

class transform_cache_t: public std::map<uint32,uint32, std::less<uint32>, FSBAllocator<int> >
{
    typedef std::map<uint32,uint32, std::less<uint32>, FSBAllocator<int> > parent;
public:
    using parent::iterator;
    using parent::const_iterator;
};
class transform_caches_t: public std::map<unsigned,transform_cache_t, std::less<unsigned>, FSBAllocator<int> >
{
    typedef std::map<unsigned,transform_cache_t, std::less<unsigned>, FSBAllocator<int> > parent;
public:
    using parent::iterator;
    using parent::const_iterator;
};

static inline transform_caches_t& GetTransformCache()
{
  #ifdef _OPENMP
    static std::vector<transform_caches_t> transform_caches( omp_get_num_procs()*2 );
    transform_caches_t& transform_cache = transform_caches[omp_get_thread_num()];
  #else
    static transform_caches_t transform_cache;
  #endif
    return transform_cache;
}


class dither_cache_t: public std::map<uint32, MixingPlan, std::less<uint32>, FSBAllocator<int> >
{
    typedef std::map<uint32, MixingPlan, std::less<uint32>, FSBAllocator<int> > parent;
public:
    using parent::iterator;
    using parent::const_iterator;
};

static inline dither_cache_t& GetDitherCache()
{
  #ifdef _OPENMP
    static std::vector<dither_cache_t> dither_caches( omp_get_num_procs()*2 );
    dither_cache_t& dither_cache = dither_caches[omp_get_thread_num()];
  #else
    static dither_cache_t dither_cache;
  #endif
    return dither_cache;
}

static inline uint32 DoCachedPixelTransform(
    transform_caches_t& transform_caches,
    uint32 pix, unsigned wid,unsigned hei,
    unsigned frameno,unsigned x,unsigned y)
{
    unsigned profile = 0, profilemax = 1;
    if(TransformationDependsOnX)       {profile += x*profilemax; profilemax*=wid; }
    if(TransformationDependsOnY)       {profile += y*profilemax; profilemax*=hei; }
    if(TransformationDependsOnFrameNo) {profile += frameno*profilemax; }

    transform_cache_t& cachepos = transform_caches[profile];
    transform_cache_t::iterator i = cachepos.lower_bound(pix);
    if(i == cachepos.end() || i->first != pix)
    {
        uint32 outpix = pix;
        TransformColor(outpix, frameno,x,y);
        cachepos.insert(i, std::make_pair(pix, outpix));
        return outpix;
    }
    return i->second;
}

template<bool TransformColors>
inline unsigned TILE_Tracker::GetMixColor
   (dither_cache_t& cache,
    transform_caches_t& transform_caches,
    unsigned wid,unsigned hei, unsigned frameno,
    unsigned x,unsigned y, uint32 pix,
    const Palette& pal)
{
    if(pix == DefaultPixel) pix = 0x7F000000u;
    if(TransformColors) pix = DoCachedPixelTransform(transform_caches, pix,wid,hei, frameno,x,y);
    ColorInfo input(pix);

    // Find two closest entries from palette and use o8x8 dithering
    MixingPlan output;
    if(UseDitherCache)
    {
        dither_cache_t::iterator i = cache.lower_bound(pix);
        if(i == cache.end() || i->first != pix)
        {
            output = FindBestMixingPlan(input, pal);
            cache.insert(i, std::make_pair(pix, output));
        }
        else
            output = i->second;
    }
    else
    {
        output = FindBestMixingPlan(input, pal);
    }

    //return output[std::rand() % output.size()]; //RANDOM DITHERING

    unsigned pattern_value =
        DitheringMatrix
            [ ((y%DitherMatrixHeight)*DitherMatrixWidth
             + (x%DitherMatrixWidth)
               )// % (DitherMatrixHeight*DitherMatrixWidth)
            ];

    const unsigned max_pattern_value = DitherMatrixWidth * DitherMatrixHeight;
    return output[ pattern_value * output.size() / max_pattern_value ];
}


template<bool TransformColors>
gdImagePtr TILE_Tracker::CreateFrame_TrueColor(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei)
{
    gdImagePtr im = gdImageCreateTrueColor(wid+pad_left+pad_right, hei+pad_top+pad_bottom);
    gdImageAlphaBlending(im, 0);
    gdImageSaveAlpha(im,     1);

    #pragma omp parallel for schedule(static)
    for(unsigned y=0; y<hei; ++y)
    {
        transform_caches_t& transform_cache = GetTransformCache();

        for(unsigned p=y*wid, x=0; x<wid; ++x)
        {
            uint32 pix = screen[p+x];
            if(pix == DefaultPixel)
                pix = 0x7F000000u;
            if(TransformColors)
                pix = DoCachedPixelTransform(transform_cache, pix,wid,hei, frameno,x,y);
            gdImageSetPixel(im, x+pad_left,y+pad_top, pix);
        }
    }
    return im;
}

template<bool TransformColors>
gdImagePtr TILE_Tracker::CreateFrame_Palette_Auto(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei)
{
    return CreateFrame_TrueColor<TransformColors> (screen, frameno,wid,hei);
}

template<bool TransformColors, bool UseErrorDiffusion>
gdImagePtr TILE_Tracker::CreateFrame_Palette_Dither_With(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei,
    const Palette& pal)
{
    const unsigned max_pattern_value = DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize;

    /* First try to create a paletted image */
    gdImagePtr im = pal.Size() <= 256
        ? gdImageCreate(         wid+pad_left+pad_right,hei+pad_top+pad_bottom)
        : gdImageCreateTrueColor(wid+pad_left+pad_right,hei+pad_top+pad_bottom);

    gdImageAlphaBlending(im, 0);
    gdImageSaveAlpha(im,     1);
    if(pal.Size() <= 256)
    {
        for(unsigned a=0; a<pal.Size(); ++a)
        {
            unsigned pix = pal.GetColor(a);
            gdImageColorAllocateAlpha(im, (pix>>16)&0xFF, (pix>>8)&0xFF, pix&0xFF, (pix>>24)&0x7F);
        }
        gdImageColorAllocateAlpha(im, 0,0,0, 127); //0xFF000000u;
    }

    const unsigned ErrorDiffusionMaxHeight = 4;
    std::vector<GammaColorVec> Errors(
        UseErrorDiffusion ? ErrorDiffusionMaxHeight*(wid+8) : 0 );

    #pragma omp parallel for schedule(static,2) if(!UseErrorDiffusion)
    for(unsigned y=0; y<hei; ++y)
    {
        transform_caches_t& transform_cache = GetTransformCache();
        dither_cache_t& dither_cache = GetDitherCache();

        for(unsigned p=y*wid, x=0; x<wid; ++x)
        {
            uint32 pix = screen[p+x];
            if(pix == DefaultPixel)
                pix = 0x7F000000u;
            if(TransformColors)
                pix = DoCachedPixelTransform(transform_cache, pix,wid,hei, frameno,x,y);

            //pix &= 0xFFFCFCFCu; // speed hack

            int r = (pix >> 16)&0xFF;
            int g = (pix >>  8)&0xFF;
            int b = (pix      )&0xFF;
            int a = (pix >> 24); if(a&0x80) a>>=1;

            ColorInfo orig_colorinfo(r,g,b,a);
            GammaColorVec& orig_color = orig_colorinfo.gammac;

            if(UseErrorDiffusion)
            {
                GammaColorVec* pos = &Errors[ ((y%ErrorDiffusionMaxHeight)*(wid+8) + (x+4)) + 0];
                orig_color += *pos;
                *pos       = GammaColorVec(0.0f);

                /*GammaColorVec clamped = orig_color;
                clamped.ClampTo0and1();
                pix = clamped.GetGammaUncorrectedRGB();*/
                orig_color.ClampTo0and1();
                pix = orig_color.GetGammaUncorrectedRGB();
            }

            // Find two closest entries from palette and use o8x8 dithering
            MixingPlan output;
            if(UseDitherCache)
            {
                dither_cache_t::iterator i = dither_cache.lower_bound(pix);
                if(i == dither_cache.end() || i->first != pix)
                {
                    ColorInfo input(pix, orig_color);
                    output = FindBestMixingPlan(input, pal);
                    dither_cache.insert(i, std::make_pair(pix, output));
                }
                else
                    output = i->second;
            }
            else
            {
                /*if(x >= 128
                && (orig_colorinfo.B >= orig_colorinfo.G
                 || orig_colorinfo.luma >= 200000)
                  )
                {
                    DitherColorListSize = 64;
                }
                else if(orig_colorinfo.B >= orig_colorinfo.R)
                {
                    DitherColorListSize = 4;
                }
                else
                {
                    DitherColorListSize = 16;
                }*/

                ColorInfo input(pix, orig_color);
                output = FindBestMixingPlan(input, pal);
            }

            unsigned pattern_value =
                DitheringMatrix
                    [ ((y%DitherMatrixHeight)*DitherMatrixWidth
                     + (x%DitherMatrixWidth)
                       )// % (DitherMatrixHeight*DitherMatrixWidth)
                    ];

           //pattern_value = std::rand() % (DitherMatrixWidth * DitherMatrixHeight); //RANDOM DITHERING

        #if 0
            if(output.size() == 2)
            {
                #define IsMarioPair_Reverse(a,b) \
                    (pal.GetColor(output[0]) == (a) \
                  && pal.GetColor(output[1]) == (b))
                if(IsMarioPair_Reverse(0xF80000, 0x806000)
                || IsMarioPair_Reverse(0xF80000, 0xF88000)
                || IsMarioPair_Reverse(0xF88000, 0xF8F800)
                || IsMarioPair_Reverse(0x806000, 0x00F800)
                || IsMarioPair_Reverse(0x806000, 0x008040)
                || IsMarioPair_Reverse(0x0000F8, 0x00F8F8)
                || IsMarioPair_Reverse(0xC04020, 0x0000F8)
                || IsMarioPair_Reverse(0x806000, 0xC04020)
                || IsMarioPair_Reverse(0x000000, 0x806000)
                || IsMarioPair_Reverse(0xF80000, 0xF8C080)
                || IsMarioPair_Reverse(0x808080, 0xC000C0)
                || IsMarioPair_Reverse(0x000000, 0x808080)
                || IsMarioPair_Reverse(0x808080, 0xC0C0C0)
                || IsMarioPair_Reverse(0xF8F8F8, 0xC0C0C0)
                || IsMarioPair_Reverse(0x000000, 0xC000C0))
                {
                    // Swap the items
                    unsigned a = output[0]; output[0] = output[1]; output[1] = a;
                    std::fprintf(stderr, "Swapped mario pair %06X, %06X\n",
                        pal.GetColor(output[0]), pal.GetColor(output[1]) );
                }
                #undef IsMarioPair_Reverse
            }
            /*else
                std::fprintf(stderr, "%u colors...\n", (unsigned) output.size());*/
        #endif

            //unsigned skew = x^y^(x>>1)^(y>>1)^(x<<2)^(y<<2)^(x>>3)^(y>>3);
            unsigned skew = x-y+x/3-y/5;
            unsigned temp_pos = TemporalMatrix[ (frameno+skew) % TemporalDitherSize ];

            if(TemporalDitherSize > 1)
            {
                if(TemporalDitherMSB)
                    pattern_value = pattern_value + (DitherMatrixWidth*DitherMatrixHeight)*temp_pos;
                else
                    pattern_value = pattern_value * TemporalDitherSize + temp_pos;
            }
            if(pattern_value >= max_pattern_value)
                fprintf(stderr, "ERROR: pattern_value=%u, max_pattern_value=%u\n", pattern_value, max_pattern_value);

            int color = output[ pattern_value * output.size() / max_pattern_value ];
            if(pix & 0xFF000000u) gdImageColorTransparent(im, color);
            if(pal.Size() <= 256)
                gdImageSetPixel(im, x+pad_left,y+pad_top, color);
            else
                gdImageSetPixel(im, x+pad_left,y+pad_top, pal.GetColor( color ));

            if(UseErrorDiffusion)
            {
                GammaColorVec flterror = pal.Data[color].gammac - orig_color;
                #define put(xo,yo, factor) \
                    Errors[ (((y+yo)%ErrorDiffusionMaxHeight)*(wid+8) \
                            + (x+xo+4)) ] -= flterror * (factor##f)
                switch(Diffusion)
                {
                    case Diffusion_None: break;
                    case Diffusion_FloydSteinberg:
                        put( 1,0, 7/16.0);
                        put(-1,1, 3/16.0);
                        put( 0,1, 5/16.0);
                        put( 1,1, 1/16.0);
                        break;
                    case Diffusion_JarvisJudiceNinke:
                        put( 1,0, 7/48.0);
                        put( 2,0, 5/48.0);
                        put(-2,1, 3/48.0);
                        put(-1,1, 5/48.0);
                        put( 0,1, 7/48.0);
                        put( 1,1, 5/48.0);
                        put( 2,1, 3/48.0);
                        put(-2,2, 1/48.0);
                        put(-1,2, 3/48.0);
                        put( 0,2, 5/48.0);
                        put( 1,2, 3/48.0);
                        put( 2,2, 1/48.0);
                        break;
                    case Diffusion_Stucki:
                        put( 1,0, 8/42.0);
                        put( 2,0, 4/42.0);
                        put(-2,1, 2/42.0);
                        put(-1,1, 4/42.0);
                        put( 0,1, 8/42.0);
                        put( 1,1, 4/42.0);
                        put( 2,1, 2/42.0);
                        put(-2,2, 1/42.0);
                        put(-1,2, 2/42.0);
                        put( 0,2, 4/42.0);
                        put( 1,2, 2/42.0);
                        put( 2,2, 1/42.0);
                        break;
                    case Diffusion_Burkes:
                        put( 1,0, 8/32.0);
                        put( 2,0, 4/32.0);
                        put(-2,1, 2/32.0);
                        put(-1,1, 4/32.0);
                        put( 0,1, 8/32.0);
                        put( 1,1, 4/32.0);
                        put( 2,1, 2/32.0);
                        break;
                    case Diffusion_Sierra3:
                        put( 1,0, 5/32.0);
                        put( 2,0, 3/32.0);
                        put(-2,1, 2/32.0);
                        put(-1,1, 4/32.0);
                        put( 0,1, 5/32.0);
                        put( 1,1, 4/32.0);
                        put( 2,1, 2/32.0);
                        put(-1,2, 2/32.0);
                        put( 0,2, 3/32.0);
                        put( 1,2, 2/32.0);
                        break;
                    case Diffusion_Sierra2:
                        put( 1,0, 4/16.0);
                        put( 2,0, 3/16.0);
                        put(-2,1, 1/16.0);
                        put(-1,1, 2/16.0);
                        put( 0,1, 3/16.0);
                        put( 1,1, 2/16.0);
                        put( 2,1, 1/16.0);
                        break;
                    case Diffusion_Sierra24A:
                        put( 1,0, 2/4.0);
                        put(-1,1, 1/4.0);
                        put( 0,1, 1/4.0);
                        break;
                    case Diffusion_StevensonArce:
                        put( 2,0, 32/200.0);
                        put(-3,1, 12/200.0);
                        put(-1,1, 26/200.0);
                        put( 1,1, 30/200.0);
                        put( 3,1, 16/200.0);
                        put(-2,2, 12/200.0);
                        put( 0,2, 26/200.0);
                        put( 2,2, 12/200.0);
                        put(-3,3,  5/200.0);
                        put(-1,3, 12/200.0);
                        put( 1,3, 12/200.0);
                        put( 3,3,  5/200.0);
                        break;
                    case Diffusion_Atkinson:
                        put( 1,0,  1/8.0);
                        put( 2,0,  1/8.0);
                        put(-1,1,  1/8.0);
                        put( 0,1,  1/8.0);
                        put( 1,1,  1/8.0);
                        put( 0,2,  1/8.0);
                        break;
                }
            }
        }
    }

    return im;
}

template<bool TransformColors, bool UseErrorDiffusion>
gdImagePtr TILE_Tracker::CreateFrame_Palette_Dither(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei)
{
    return
        CreateFrame_Palette_Dither_With<TransformColors,UseErrorDiffusion>
        (screen, frameno, wid, hei, CurrentPalette);
}


template<bool TransformColors, bool UseErrorDiffusion>
gdImagePtr TILE_Tracker::CreateFrame_Palette_Dither_CGA16(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei)
{
    static const struct cga16_palette_initializer
    {
        unsigned colors[16*5];
        cga16_palette_initializer()
        {
            double hue = (35.0 + 0.0)*0.017453239;
            double sinhue = std::sin(hue), coshue = std::cos(hue);
            for(unsigned i=0; i<16; ++i)
                for(unsigned j=0; j<5; ++j)
                {
                    unsigned colorBit4 = (i&1)>>0;
                    unsigned colorBit3 = (i&2)>>1;
                    unsigned colorBit2 = (i&4)>>2;
                    unsigned colorBit1 = (i&8)>>3;
                    //calculate lookup table   
                    double I = 0, Q = 0, Y;
                    I += (double) colorBit1;
                    Q += (double) colorBit2;
                    I -= (double) colorBit3;
                    Q -= (double) colorBit4;
                    Y  = (double) j / 4.0; //calculated avarage is over 4 bits

                    double pixelI = I * 1.0 / 3.0; //I* tvSaturation / 3.0
                    double pixelQ = Q * 1.0 / 3.0; //Q* tvSaturation / 3.0
                    I = pixelI*coshue + pixelQ*sinhue;
                    Q = pixelQ*coshue - pixelI*sinhue;

                    double R = Y + 0.956*I + 0.621*Q; if (R < 0.0) R = 0.0; if (R > 1.0) R = 1.0;
                    double G = Y - 0.272*I - 0.647*Q; if (G < 0.0) G = 0.0; if (G > 1.0) G = 1.0;
                    double B = Y - 1.105*I + 1.702*Q; if (B < 0.0) B = 0.0; if (B > 1.0) B = 1.0;
                    unsigned char rr = R*0xFF, gg = G*0xFF, bb = B*0xFF;
                    colors[(j<<4)|i] = (rr << 16) | (gg << 8) | bb;
                }
        }
    } cga16_palette;

    gdImagePtr im =
        CreateFrame_Palette_Dither_With<TransformColors,UseErrorDiffusion>
        (screen, frameno, wid, hei, CurrentPalette);

    // FIXME: Padding is not implemented here.
    gdImagePtr im2 = gdImageCreateTrueColor(wid*4, hei);
    std::vector<unsigned char> cga16temp(hei*(wid*4+3), 0);
    #pragma omp parallel for schedule(static,2)
    for(unsigned y=0; y<hei; ++y)
    {
        // Update colors 10..14 to 11..15 because pattern 1010 was skipped over
        for(unsigned x=0; x<wid; ++x)
        {
            unsigned i = gdImageGetPixel(im, x,y);
            if(i >= 10) gdImageSetPixel(im, x,y, i+1);
        }
        unsigned* temp = &cga16temp[y*(wid*4+3)];
        for(unsigned x=0; x<wid*4; ++x)
            temp[x+2] = (( gdImageGetPixel(im,x>>2,y) >> (3-(x&3)) ) & 1) << 4;
        for(unsigned i=0, x=0; x<wid; ++x)
        {
            unsigned v = gdImageGetPixel(im,x,y);
            for(unsigned c=0; c<4; ++c)
            {
                unsigned p = v | (temp[i] + temp[i+1] + temp[i+2] + temp[i+3]);
                gdImageSetPixel(im2, i++, y,  cga16_palette.colors[p]);
            }
        }
    }
    gdImageDestroy(im);
    return im2;
}

AlignResult TILE_Tracker::TryAlignWithHotspots
    (const uint32* input, unsigned sx,unsigned sy) const
{
    /* Find spots of interest within the reference image,
     * and within the input image.
     *
     * Select that offset which results in greatest overlap
     * between those two sets of spots.
     */

    std::vector<InterestingSpot> input_spots;
    std::vector<InterestingSpot> reference_spots;
    FindInterestingSpots(input_spots, input, 0,0, sx,sy, true);

    /* Cache InterestingSpot lists for each cube */
    static std::map
        <IntCoordinate, std::vector<InterestingSpot>,
         std::less<IntCoordinate>,
         FSBAllocator<int> > cache;

    /* For speed reasons, we don't use LoadScreen(), but
     * instead, work on cube-by-cube basis.
     */
    for(ymaptype::const_iterator
        yi = screens.begin();
        yi != screens.end();
        ++yi)
    {
        const int y_screen_offset = yi->first * 256;

        for(xmaptype::const_iterator
            xi = yi->second.begin();
            xi != yi->second.end();
            ++xi)
        {
            const int x_screen_offset = xi->first  * 256;
            const cubetype& cube      = xi->second;

            IntCoordinate cache_key = {x_screen_offset,y_screen_offset};

            if(cube.changed)
            {
                uint32 result[256*256];

                cube.pixels->GetStaticInto(result, 256);

                size_t prev_size = reference_spots.size();
                FindInterestingSpots(reference_spots, result,
                    x_screen_offset,y_screen_offset,
                    256,256,
                    false);

                cache[cache_key].assign(
                    reference_spots.begin() + prev_size,
                    reference_spots.end() );

                cube.changed = false;
            }
            else
            {
                const std::vector<InterestingSpot>& found = cache[cache_key];
                reference_spots.insert(
                    reference_spots.end(),
                    found.begin(),
                    found.end());
            }
        }
    }

    return Align(
        input_spots,
        reference_spots,
        org_x, org_y);
}

AlignResult TILE_Tracker::TryAlignWithBackground
    (const uint32* input, unsigned sx,unsigned sy) const
{
    struct AlignResult align =
        Align(
            &LoadBackground(xmin,ymin, xmax-xmin,ymax-ymin)[0],
            xmax-xmin, ymax-ymin,
            input,
            sx, sy,
            org_x-xmin,
            org_y-ymin
        );

    align.offs_x -= org_x-xmin;
    align.offs_y -= org_y-ymin;
    return align;
}

AlignResult TILE_Tracker::TryAlignWithPrevFrame
    (const uint32* prev_input,
     const uint32* input, unsigned sx,unsigned sy) const
{
    return Align(
        prev_input, sx,sy,
        input,      sx,sy,
        0,0
    );
}

void
TILE_Tracker::FitScreenAutomatic
    (const uint32* input, unsigned sx,unsigned sy)
{
    static VecType<uint32> prev_frame;
    //fprintf(stderr, "sx=%u,sy=%u, prev_frame size=%u\n", sx,sy, prev_frame.size());
    if(prev_frame.size() == sx*sy && !always_align_with_canvas)
    {
        AlignResult align = TryAlignWithPrevFrame(&prev_frame[0], input,sx,sy);
        if(!align.suspect_reset)
        {
            prev_frame.assign(input, input+sx*sy);
            FitScreen(input,sx,sy, align);
            return;
        }
    }
    prev_frame.assign(input, input+sx*sy);

    AlignResult align = TryAlignWithHotspots(input,sx,sy);
    FitScreen(input,sx,sy, align);
}

void TILE_Tracker::FitScreen
    (const uint32* input, unsigned sx, unsigned sy,
     const AlignResult& alignment,
     int extra_offs_x,
     int extra_offs_y
    )
{
    //if(alignment.offs_x != 0 || alignment.offs_y != 0)
    {
        std::fprintf(stderr, "[frame%5u] Motion(%d,%d), Origo(%d,%d)\n",
            CurrentTimer, alignment.offs_x,alignment.offs_y, org_x,org_y);
    }

    org_x += alignment.offs_x; org_y += alignment.offs_y;

    int this_org_x = org_x + extra_offs_x;
    int this_org_y = org_y + extra_offs_y;

    if(alignment.suspect_reset)
    {
#if 0
        goto AlwaysReset;
#endif
        VecType<uint32> oldbuf = LoadScreen(this_org_x,this_org_y, sx,sy, CurrentTimer, bgmethod);
        unsigned diff = 0;
        for(unsigned a=0; a<oldbuf.size(); ++a)
        {
            unsigned oldpix = oldbuf[a];
            unsigned pix   = input[a];
            unsigned r = (pix >> 16) & 0xFF;
            unsigned g = (pix >> 8) & 0xFF;
            unsigned b = (pix    ) & 0xFF;
            unsigned oldr = (oldpix >> 16) & 0xFF;
            unsigned oldg = (oldpix >> 8) & 0xFF;
            unsigned oldb = (oldpix    ) & 0xFF;
            int rdiff = (int)(r-oldr); if(rdiff < 0)rdiff=-rdiff;
            int gdiff = (int)(g-oldg); if(gdiff < 0)gdiff=-gdiff;
            int bdiff = (int)(b-oldb); if(bdiff < 0)bdiff=-bdiff;
            unsigned absdiff = rdiff+gdiff+bdiff;
            diff += absdiff;
        }

        if(diff > oldbuf.size() * 128)
        {
#if 0
            /* Castlevania hack */
            static int org_diff = -180;
            org_y += org_diff;
            org_diff = -org_diff;
#else
#if 1
        //AlwaysReset:
            Save();
            Reset();
#endif
#endif
        }
    }

    const bool first = CurrentTimer == 0;
    if(first || this_org_x < xmin) xmin = this_org_x;
    if(first || this_org_y < ymin) ymin = this_org_y;
    int xtmp = this_org_x+sx; if(first || xtmp > xmax) xmax=xtmp;
    int ytmp = this_org_y+sy; if(first || ytmp > ymax) ymax=ytmp;

#if 0
    /* If the image geometry would exceed some bounds */
    if(xmax-xmin > 800 || ymax-ymin > 800)
    {
        SaveAndReset();
        first=true;
    }
#endif

    PutScreen(input, this_org_x,this_org_y, sx,sy, CurrentTimer);
}

void TILE_Tracker::Reset()
{
    SequenceBegin += CurrentTimer;
    CurrentTimer = 0;

    std::fprintf(stderr, " Resetting\n");
    screens.clear();
    org_x = 0x40000000;
    org_y = 0x40000000;
    xmin=xmax=org_x;
    ymin=ymax=org_y;
}

void TILE_Tracker::NextFrame()
{
    std::printf("/*%5u*/ %d,%d,\n",
        CurrentTimer,
        org_x - xmin,
        org_y - ymin
        );
    std::fflush(stdout);
    ++CurrentTimer;
}
