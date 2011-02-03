#include <gd.h>
#include <cstdio>
#include <cmath>

#include "openmp.hh"
#include "canvas.hh"
#include "align.hh"
#include "palette.hh"

#include "fparser.hh"

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef NESmode
# define NESmode   0
#endif
#ifndef CGA16mode
# define CGA16mode 0
#endif

int SaveGif = -1;

static class CanvasFunctionParser: public FunctionParser
{
public:
    CanvasFunctionParser()
    {
        AddConstant("pi",  M_PI);
        AddConstant("e",   M_E);
    }
} parser_r,parser_g,parser_b;
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
    int r_error = parser_r.Parse(transform_r.c_str(), "r,g,b,frameno,x,y");
    int g_error = parser_g.Parse(transform_g.c_str(), "r,g,b,frameno,x,y");
    int b_error = parser_b.Parse(transform_b.c_str(), "r,g,b,frameno,x,y");
    if(r_error >= 0 || g_error >= 0 || b_error >= 0)
    {
        if(r_error >= 0)
            fprintf(stderr, "Parse error (%s) in red color formula:\n%s\n%*s\n",
                parser_r.ErrorMsg(), transform_r.c_str(), r_error+1, "^");
        if(g_error >= 0)
            fprintf(stderr, "Parse error (%s) in green color formula:\n%s\n%*s\n",
                parser_g.ErrorMsg(), transform_g.c_str(), g_error+1, "^");
        if(b_error >= 0)
            fprintf(stderr, "Parse error (%s) in blue color formula:\n%s\n%*s\n",
                parser_b.ErrorMsg(), transform_b.c_str(), b_error+1, "^");
        return;
    }
    parser_r.Optimize();
    parser_g.Optimize();
    parser_b.Optimize();
    TransformationDependsOnX       = false;
    TransformationDependsOnY       = false;
    TransformationDependsOnFrameNo = false;
    TransformationGsameAsR = transform_g == transform_r;
    TransformationBsameAsR = transform_b == transform_r;
    TransformationBsameAsG = transform_b == transform_g;
    UsingTransformations = transform_r != "r"
                        || transform_g != "g"
                        || transform_b != "b";
    if(UsingTransformations)
    {
        if(FunctionParser().Parse(transform_r.c_str(), "r,g,b,frameno,y") >= 0
        || FunctionParser().Parse(transform_g.c_str(), "r,g,b,frameno,y") >= 0
        || FunctionParser().Parse(transform_b.c_str(), "r,g,b,frameno,y") >= 0)
            TransformationDependsOnX = true;
        if(FunctionParser().Parse(transform_r.c_str(), "r,g,b,frameno,x") >= 0
        || FunctionParser().Parse(transform_g.c_str(), "r,g,b,frameno,x") >= 0
        || FunctionParser().Parse(transform_b.c_str(), "r,g,b,frameno,x") >= 0)
            TransformationDependsOnY = true;
        if(FunctionParser().Parse(transform_r.c_str(), "r,g,b,x,y") >= 0
        || FunctionParser().Parse(transform_g.c_str(), "r,g,b,x,y") >= 0
        || FunctionParser().Parse(transform_b.c_str(), "r,g,b,x,y") >= 0)
            TransformationDependsOnFrameNo = true;
    }
}
void TransformColor(int&r, int&g, int&b, unsigned frameno,unsigned x,unsigned y)
{
    double vars[6] = {
        double(r), double(g), double(b),
        double(frameno), double(x), double(y)
    };
    r = parser_r.Eval(vars); if(r<0) r=0; else if(r>255) r=255;
    if(TransformationGsameAsR)
        g = r;
    else
    {
        g = parser_g.Eval(vars); if(g<0) g=0; else if(g>255) g=255;
        if(TransformationBsameAsR) { b = r; return; }
    }
    if(TransformationBsameAsG) { b = g; return; }
    b = parser_b.Eval(vars); if(b<0) b=0; else if(b>255) b=255;
}
void TransformColor(uint32& pix, unsigned frameno,unsigned x,unsigned y)
{
    int r = (pix >> 16) & 0xFF, g = (pix >> 8) & 0xFF, b = (pix & 0xFF);
    TransformColor(r,g,b, frameno,x,y);
    pix = (pix & 0xFF000000u) + (r<<16) + (g<<8) + b;
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

    omp_set_nested(1);

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

    // Create a historgram of the used colors, unless it's going
    // to be replaced immediately thereafter.
    if(PaletteReductionMethod.empty()
    || PaletteReductionMethod.front().filename.empty())
    {
        const int ymi = ymin, yma = ymax;
        const int xmi = xmin, xma = xmax;
        const unsigned wid = xma-xmi;
        const unsigned hei = yma-ymi;
        fprintf(stderr, "Counting colors... (%u frames)\n", nframes);
        VecType<uint32> prev_frame;
        for(unsigned frameno=0; frameno<nframes; frameno+=1)
        {
            fprintf(stderr, "\rFrame %u/%u...", frameno+1, nframes); fflush(stderr);
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
        fprintf(stderr, "\n%u colors detected\n",(unsigned) Histogram.size());
    }
    return Histogram;
}

void TILE_Tracker::CreatePalette(PixelMethod method, unsigned nframes)
{
    #if NESmode
    // NES palette
    CurrentPalette.SetHardcoded(16,
        // selection 1: reds
        0x000000, // black
        0xF83800, // red
        0x7D7D7D, // gray2
        0xFCE0A8, // desaturated yellow
        // selection 2: blues
        0x000000, // black
        0x0E78F6, // blue
        0x503000, // brown
        0x7D7D7D, // lightgray
        // selection 3: greens
        0x000000, // black
        0x005800, // dark green
        0x00A844, // green-turquoise
        0xB8F8B8, // light green
        // selection 4: grayscale
        0x000000, // black
        0x343434, // gray
        0x7D7D7D, // gray2
        0xD8D8D8); // gray4
    return;
    #endif
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

    char Filename[512] = {0}; // explicit init keeps valgrind happy
    const char* nametemplate = "tile";
    if(pixelmethods_result != (1ul << method))
    {
        // Multi-method output
        #define MakePixName(o,f,name) #name,
        static const char* const Templates[NPixelMethods] =
        {
             DefinePixelMethods(MakePixName)
        };
        #undef MakePixName
        nametemplate = Templates[method];
    }

    bool MakeGif  = SaveGif == 1 || (SaveGif == -1 && animated);
    bool Dithered = !PaletteReductionMethod.empty();

    if(MakeGif)
        std::sprintf(Filename, "%s-%04u.gif", nametemplate, img_counter);
    else
        std::sprintf(Filename, "%s-%04u.png", nametemplate, img_counter);

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
          #if NESmode
            if(UsingTransformations)
              im = CreateFrame_Palette_Dither_NES<true,false>(screen, frameno, wid, hei);
            else
              im = CreateFrame_Palette_Dither_NES<false,false>(screen, frameno, wid, hei);
          #elif CGA16mode
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
          #if NESmode
            if(UsingTransformations)
              im = CreateFrame_Palette_Dither_NES<true,true>(screen, frameno, wid, hei);
            else
              im = CreateFrame_Palette_Dither_NES<false,true>(screen, frameno, wid, hei);
          #elif CGA16mode
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

typedef std::map<uint32,uint32, std::less<uint32>, FSBAllocator<int> >
    transform_cache_t;
typedef std::map<unsigned,transform_cache_t, std::less<unsigned>, FSBAllocator<int> >
    transform_caches_t;
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


typedef std::map<uint32, MixingPlan, std::less<uint32>, FSBAllocator<int> >
    dither_cache_t;
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
    transform_caches_t& transform_cache,
    uint32 pix, unsigned wid,unsigned hei,
    unsigned frameno,unsigned x,unsigned y)
{
    unsigned profile = 0, profilemax = 1;
    if(TransformationDependsOnX)       {profile += x*profilemax; profilemax*=wid; }
    if(TransformationDependsOnY)       {profile += y*profilemax; profilemax*=hei; }
    if(TransformationDependsOnFrameNo) {profile += frameno*profilemax; }

    transform_cache_t& cachepos = transform_cache[profile];
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
gdImagePtr TILE_Tracker::CreateFrame_TrueColor(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei)
{
    gdImagePtr im = gdImageCreateTrueColor(wid,hei);
    gdImageAlphaBlending(im, false);
    gdImageSaveAlpha(im, true);

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
            gdImageSetPixel(im, x,y, pix);
        }
    }
    return im;
}

template<bool TransformColors>
gdImagePtr TILE_Tracker::CreateFrame_Palette_Auto(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei)
{
    gdImagePtr im = gdImageCreateTrueColor(wid,hei);
    gdImageAlphaBlending(im, false);
    gdImageSaveAlpha(im, true);

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
            gdImageSetPixel(im, x,y, pix);
        }
    }
    return im;
}

template<bool TransformColors, bool UseErrorDiffusion>
gdImagePtr TILE_Tracker::CreateFrame_Palette_Dither_With(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei,
    const Palette& pal)
{
    const unsigned max_pattern_value = DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize;

    /* First try to create a paletted image */
    gdImagePtr im = gdImageCreate(wid,hei);

    gdImageAlphaBlending(im, false);
    gdImageSaveAlpha(im, true);
    for(unsigned a=0; a<pal.Size(); ++a)
    {
        unsigned pix = pal.GetColor(a);
        gdImageColorAllocate(im, (pix>>16)&0xFF, (pix>>8)&0xFF, pix&0xFF);
    }
    gdImageColorAllocateAlpha(im, 0,0,0, 127); //0xFF000000u;

    const unsigned ErrorDiffusionMaxHeight = 4;
    std::vector<float> Errors(
        UseErrorDiffusion ? ErrorDiffusionMaxHeight*(wid+8)*3 : 0 );

    const int n = UseErrorDiffusion ? 1 : omp_get_num_procs();

    #pragma omp parallel for schedule(static,2) num_threads(n)
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

            int r = (pix >> 16)&0xFF;
            int g = (pix >>  8)&0xFF;
            int b = (pix      )&0xFF;
            int a = (pix >> 24); if(a&0x80) a>>=1;

            float orig_color[3] = { (float) GammaCorrect(r/255.0),
                                    (float) GammaCorrect(g/255.0),
                                    (float) GammaCorrect(b/255.0) };

            if(UseErrorDiffusion)
            {
                float* pos = &Errors[ ((y%ErrorDiffusionMaxHeight)*(wid+8) + (x+4)) * 3 + 0];
                for(unsigned c=0; c<3; ++c) orig_color[c] += pos[c];
                for(unsigned c=0; c<3; ++c) pos[c] = 0.0f;
                r = int( 255*GammaUncorrect( orig_color[0] ) );
                g = int( 255*GammaUncorrect( orig_color[1] ) );
                b = int( 255*GammaUncorrect( orig_color[2] ) );

                if(r < 0) r = 0; else if(r > 255) r = 255;
                if(g < 0) g = 0; else if(g > 255) g = 255;
                if(b < 0) b = 0; else if(b > 255) b = 255;
                pix = (pix & 0xFF000000) | ((r << 16) + (g << 8) + b);
            }

            // Find two closest entries from palette and use o8x8 dithering
            MixingPlan output;
            dither_cache_t::iterator i = dither_cache.lower_bound(pix);
            if(i == dither_cache.end() || i->first != pix)
            {
                output = FindBestMixingPlan(r,g,b, pal);
                dither_cache.insert(i, std::make_pair(pix, output));
            }
            else
                output = i->second;

            unsigned pattern_value =
                DitheringMatrix
                    [ ((y%DitherMatrixHeight)*DitherMatrixWidth
                     + (x%DitherMatrixWidth)
                       )// % (DitherMatrixHeight*DitherMatrixWidth)
                    ];

            unsigned skew = x-y+x/3-y/3;
            unsigned temp_pos = TemporalMatrix[ (frameno+skew) % TemporalDitherSize ];

            if(TemporalDitherSize > 1)
            {
                if(TemporalDitherMSB)
                    pattern_value = pattern_value + (DitherMatrixWidth*DitherMatrixHeight)*temp_pos;
                else
                    pattern_value = pattern_value * TemporalDitherSize + temp_pos;
            }
            int color = output[ pattern_value * output.size() / max_pattern_value ];
            if(pix & 0xFF000000u) gdImageColorTransparent(im, color);
            gdImageSetPixel(im, x,y, color);

            if(UseErrorDiffusion)
            {
                float flterror[3] = { float(pal.Data[color].r) - orig_color[0],
                                      float(pal.Data[color].g) - orig_color[1],
                                      float(pal.Data[color].b) - orig_color[2] };
                #define put(xo,yo, factor) \
                  for(unsigned c=0; c<3; ++c) \
                    Errors[ (((y+yo)%ErrorDiffusionMaxHeight)*(wid+8) \
                            + (x+xo+4))*3 + c] -= flterror[c]*(factor##f)
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

template<bool TransformColors, bool UseErrorDiffusion>
gdImagePtr TILE_Tracker::CreateFrame_Palette_Dither_NES(
    const VecType<uint32>& screen,
    unsigned frameno, unsigned wid, unsigned hei)
{
    const unsigned max_pattern_value = DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize;

    gdImagePtr imtab[4] =
    {
        CreateFrame_Palette_Dither_With<TransformColors,UseErrorDiffusion>
            (screen, frameno, wid, hei, CurrentPalette.GetSlice( 0,4)),
        CreateFrame_Palette_Dither_With<TransformColors,UseErrorDiffusion>
            (screen, frameno, wid, hei, CurrentPalette.GetSlice( 4,4)),
        CreateFrame_Palette_Dither_With<TransformColors,UseErrorDiffusion>
            (screen, frameno, wid, hei, CurrentPalette.GetSlice( 8,4)),
        CreateFrame_Palette_Dither_With<TransformColors,UseErrorDiffusion>
            (screen, frameno, wid, hei, CurrentPalette.GetSlice(12,4))
    };

    gdImagePtr im2 = gdImageCreate(wid,hei);
    gdImageAlphaBlending(im2, false);
    gdImageSaveAlpha(im2, true);
    for(unsigned a=0; a<CurrentPalette.Size(); ++a)
    {
        unsigned pix = CurrentPalette.GetColor(a);
        gdImageColorAllocate(im2, (pix>>16)&0xFF, (pix>>8)&0xFF, pix&0xFF);
    }
    gdImageColorAllocateAlpha(im2, 0,0,0, 127); //0xFF000000u;

    /* For each 16x16 pixel region in the target image,
     * choose one of the corresponding regions in the four
     * sample images that best represents the input data */
    for(unsigned by=0; by<hei; by += 16)
    {
        unsigned ey = by+16; if(ey > hei) ey = hei;
        for(unsigned bx=0; bx<wid; bx += 16)
        {
            unsigned ex = bx+16; if(ex > wid) ex = wid;

            double bestdiff = -1;
            unsigned bestmode = 0;
            for(unsigned mode=0; mode<4; ++mode)
            {
                gdImagePtr im = imtab[mode];
                double diff = 0;
                const unsigned down = 4*4;
                for(unsigned y=by; y+4<=ey; y+=1)
                    for(unsigned x=bx; x+4<=ex; x+=1)
                    {
                        int r1=0,g1=0,b1=0, r2=0,g2=0,b2=0;
                        for(unsigned cy=0; cy<4; ++cy)
                        for(unsigned cx=0; cx<4; ++cx)
                        {
                            uint32 pix1 = screen[(y+cy)*wid+(x+cx)];
                            unsigned i = gdImageGetPixel(im,x+cx,y+cy) + mode*4;
                            uint32 pix2  = CurrentPalette.GetColor(i);
                            r1 += (pix1 >> 16) & 0xFF; g1 += (pix1 >> 8) & 0xFF; b1 += (pix1) & 0xFF;
                            r2 += (pix2 >> 16) & 0xFF; g2 += (pix2 >> 8) & 0xFF; b2 += (pix2) & 0xFF;
                        }
                        diff += ColorCompare(
                            r1/down,g1/down,b1/down,
                            LabAndLuma( r1/down,g1/down,b1/down ),
                            r2/down,g2/down,b2/down,
                            LabAndLuma( r2/down,g2/down,b2/down ) );
                    }
                if(diff < bestdiff || bestdiff < 0)
                    { bestdiff = diff; bestmode = mode; }
            }
            gdImageCopy(im2, imtab[bestmode], bx,by, bx,by, ex-bx, ey-by);
        }
    }
    for(unsigned a=0; a<4; ++a) gdImageDestroy(imtab[a]);

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
    if(prev_frame.size() == sx*sy)
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
