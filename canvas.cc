#include <gd.h>
#include <cstdio>
#include <cmath>

#include "openmp.hh"
#include "canvas.hh"
#include "align.hh"
#include "palette.hh"

int SaveGif = -1;

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

    if(animated)
    {
        unsigned SavedTimer = CurrentTimer;

        if((1ul << method) & LoopingPixelMethodsMask)
        {
            if(SavedTimer >= LoopingLogLength)
                SavedTimer = LoopingLogLength;
        }

        if(!PaletteReductionMethod.empty())
            CreatePalette( (PixelMethod) method, SavedTimer );

        #pragma omp parallel for schedule(dynamic) ordered
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
        /* This dummy OMP loop is required, because SaveFrame()
         * contains an ordered statement, which must never exist
         * without a surrounding "for ordered" statement context.
         */
      #pragma omp parallel for ordered schedule(static,1)
        for(unsigned dummy=0; dummy<1; ++dummy)
            SaveFrame( (PixelMethod)method, 0, SequenceBegin);
    }
}

void TILE_Tracker::CreatePalette(PixelMethod method, unsigned nframes)
{
/* // monochrome
    Palette[0] = 0x000000;
    Palette[1] = 0xFFFFFF;
    PaletteSize = 2; return;
*/
/* // NES palette
    // selection 1: reds
    Palette[0] = 0x000000; // black
    Palette[1] = 0xF83800; // red
    Palette[2] = 0x7D7D7D; // gray2
    Palette[3] = 0xFCE0A8; // desaturated yellow
    // selection 2: blues
    Palette[4] = 0x000000; // black
    Palette[5] = 0x0E78F6; // blue
    Palette[6] = 0x503000; // brown
    Palette[7] = 0x7D7D7D; // lightgray
    // selection 3: greens
    Palette[8] = 0x000000; // black
    Palette[9] = 0x005800; // dark green
    Palette[10] = 0x00A844; // green-turquoise
    Palette[11] = 0xB8F8B8; // light green
    // selection 4: grayscale
    Palette[12] = 0x000000; // black
    Palette[13] = 0x343434; // gray
    Palette[14] = 0x7D7D7D; // gray2
    Palette[15] = 0xD8D8D8; // gray4
    PaletteSize = 16; return;
*/
/* // composite cga (to be rendered on 640x200 monochrome mode)
    Palette[ 0] = 0x000000; // 0000
    Palette[ 1] = 0x007A00; // 0001
    Palette[ 2] = 0x1B33DF; // 0010
    Palette[ 3] = 0x01ADDE; // 0011
    Palette[ 4] = 0x990580; // 0100
    Palette[ 5] = 0x7F7F7F; // 0101, 1010
    Palette[ 6] = 0xB538FF; // 0110
    Palette[ 7] = 0x9AB2FF; // 0111
    Palette[ 8] = 0x644C00; // 1000
    Palette[ 9] = 0x49C600; // 1001
    Palette[10] = 0x65F97E; // 1011
    Palette[11] = 0xFD5120; // 1100
    Palette[12] = 0xE3CB1F; // 1101
    Palette[13] = 0xFF84FF; // 1110
    Palette[14] = 0xFFFFFF; // 1111
    PaletteSize = 15; return;
*/
/* // ega standard
    Palette[ 0] = 0x000000;
    Palette[ 1] = 0x3333AA;
    Palette[ 2] = 0x11AA11;
    Palette[ 3] = 0x11AAAA;
    Palette[ 4] = 0xAA1111;
    Palette[ 5] = 0xAA11AA;
    Palette[ 6] = 0xAA5511;
    Palette[ 7] = 0xAAAAAA;
    Palette[ 8] = 0x555555;
    Palette[ 9] = 0x7777FF;
    Palette[10] = 0x55FF55;
    Palette[11] = 0x55FFFF;
    Palette[12] = 0xFF5555;
    Palette[13] = 0xFF55FF;
    Palette[14] = 0xFFFF55;
    Palette[15] = 0xFFFFFF;
    PaletteSize = 16; return;
*/
/* // dragon
    Palette[ 0] = 0x191819;
    Palette[ 1] = 0x202610;
    Palette[ 2] = 0x1C3209;
    Palette[ 3] = 0x307010;
    Palette[ 4] = 0x3E34D0; //
    Palette[ 5] = 0x5858F0; //
    Palette[ 6] = 0x7878F9; //
    Palette[ 7] = 0x9898F5; //
    Palette[ 8] = 0x2C2C2C;
    Palette[ 9] = 0x5A586A;
    Palette[10] = 0x5B5E6D;
    Palette[11] = 0xB8B8FF; //
    Palette[12] = 0x736962;
    Palette[13] = 0x857C78;
    Palette[14] = 0xA8A3AA;
    Palette[15] = 0xC4BCCB;
    SortPalette(Palette, 16);
    PaletteSize = 16; return;
*/
/* // psx scene
    static unsigned p[16] = {
 0x080000,0xFCFAE2,0x2B347C,0x234309,
 0xFCE76E,0xD5C4B3,0x432817,0x9C6B20,
 0xA9220F,0xD0CA40,0x6A94AB,0x201A0B,
 0x492910,0x2B7409,0xE8A077,0x5D4F1E };
    for(unsigned a=0; a<16; ++a) Palette[a] = p[a];
    SortPalette(Palette, 16);
    PaletteSize = 16; return;
*/
/* // dither demo pal
    static unsigned p[18] = {
0x000000,0x0000FF,
0x008000,0x0080FF,
0x00FF00,0x00FFFF,
0x800000,0x8000FF,
0x808000,0x8080FF,
0x80FF00,0x80FFFF,
0xFF0000,0xFF00FF,
0xFF8000,0xFF80FF,
0xFFFF00,0xFFFFFF };
    for(unsigned a=0; a<18; ++a) Palette[a] = p[a];
    SortPalette(Palette, 18);
    PaletteSize = 18; return;
*/
/* // ega, custom
    Palette[ 0] = 0x000000;
    Palette[ 1] = 0xAAAA55;
    Palette[ 2] = 0xAAAAFF;
    Palette[ 3] = 0x005500;
    Palette[ 4] = 0x000055;
    Palette[ 5] = 0x0000FF;
    Palette[ 6] = 0xAA55AA;
    Palette[ 7] = 0xFF0000;
    Palette[ 8] = 0x555555;
    Palette[ 9] = 0x005555;
    Palette[10] = 0xAAAAAA;
    Palette[11] = 0x5500AA;
    Palette[12] = 0xFFFFAA;
    Palette[13] = 0x550055;
    Palette[14] = 0x555500;
    Palette[15] = 0xFFFFFF;
    PaletteSize = 16; return;
*/
/* // cga 1 lo
    Palette[ 0] = 0x000000;
    Palette[ 1] = 0x11AAAA;
    Palette[ 2] = 0xAA11AA;
    Palette[ 3] = 0xAAAAAA;
    PaletteSize = 4; return;
*/
/* // cga 1 hi
    Palette[ 0] = 0x000000;
    Palette[ 1] = 0x55FFFF;
    Palette[ 2] = 0xFF55FF;
    Palette[ 3] = 0xFFFFFF;
    PaletteSize = 4; return;
*/
/* // cga 0 lo
    Palette[ 0] = 0x000000;
    Palette[ 1] = 0x11AA11;
    Palette[ 2] = 0xAA1111;
    Palette[ 3] = 0xAA5511;
    PaletteSize = 4; return;
*/
/* // cga 0 hi
    Palette[ 0] = 0x000000;
    Palette[ 1] = 0x55FF55;
    Palette[ 2] = 0xFF5555;
    Palette[ 3] = 0xFFFF55;
    PaletteSize = 4; return;
*/
/* // cga 2 tweak
    Palette[ 0] = 0x0000AA;
    Palette[ 1] = 0x00AAAA;
    Palette[ 2] = 0xAA0000;
    Palette[ 3] = 0xAAAAAA;
    PaletteSize = 4; return;
*/
/* // cga 0 tweak
    Palette[ 0] = 0x0000AA;
    Palette[ 1] = 0x11AA11;
    Palette[ 2] = 0xAA1111;
    Palette[ 3] = 0xAA5511;
    PaletteSize = 4; return;
*/
    HistogramType Histogram;

    const int ymi = ymin, yma = ymax;
    const int xmi = xmin, xma = xmax;
    const unsigned wid = xma-xmi;
    const unsigned hei = yma-ymi;
    fprintf(stderr, "Counting colors...\n");
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
                ++Histogram[prev_frame[a]];
                ++Histogram[frame[a]];
            }
        }
        for(; a < frame.size(); ++a)
        {
            ++Histogram[frame[a]];
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
                ++Histogram[ result[a] ];
        }
      #endif
    }
    // Reduce the histogram into a usable palette
    fprintf(stderr, "\n%u colors detected\n",(unsigned) Histogram.size());
    ReduceHistogram(Histogram);

    PaletteSize = MakePalette(Palette, Histogram, 256);
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

    if(SaveGif == 1)
        std::sprintf(Filename, "%s-%04u.gif", nametemplate, img_counter);
    else
        std::sprintf(Filename, "%s-%04u.png", nametemplate, img_counter);

    std::fprintf(stderr, "%s: (%d,%d)-(%d,%d)\n", Filename, 0,0, xma-xmi, yma-ymi);
    std::fflush(stderr);

    bool was_identical = false;

#if 1 // disable for temporal dither
  #pragma omp ordered
  {
    if(animated)
    {
        if(veq(screen, LastScreen) && !LastFilename.empty())
        {
            std::fprintf(stderr, "->link (%u,%u)\n",
                (unsigned)screen.size(),
                (unsigned)LastScreen.size());
            std::string cmd = "ln "+LastFilename+" "+Filename;
            system(cmd.c_str());
            was_identical = true;
        }
        LastScreen   = screen;
        LastFilename = Filename;
    }
  }
#endif

    #pragma omp flush(was_identical)
    if(was_identical) return;

    if(SaveGif == 1 || !PaletteReductionMethod.empty())
    {
        bool palette_failed = false;

        typedef std::map<uint32, PalettePair, std::less<uint32>, FSBAllocator<int> >
            pixel_cache_t;
        pixel_cache_t pixel_cache;

        /* First try to create a paletted image */
        gdImagePtr im = gdImageCreate(wid,hei);
    #if 0 /* CGA16 mode */
        gdImagePtr im2 = gdImageCreateTrueColor(wid*4, hei);
    #endif
    #if 0 /* NES mode */
        gdImagePtr im2 = im, imtab[4] = {
            gdImageCreate(wid,hei),
            gdImageCreate(wid,hei),
            gdImageCreate(wid,hei),
            gdImageCreate(wid,hei) };
    #endif
    reloop:;
    #if 0 /* NES mode */
        for(unsigned mode=0; mode<5 && (im=mode?imtab[mode-1]:im2); ++mode)
        {
    #endif
        gdImageAlphaBlending(im, false);
        gdImageSaveAlpha(im, true);
        if(!palette_failed)
        {
            if(!PaletteReductionMethod.empty())
            {
                for(unsigned a=0; a<PaletteSize; ++a)
                {
                    unsigned pix = Palette[a];
                    gdImageColorAllocate(im, (pix>>16)&0xFF, (pix>>8)&0xFF, pix&0xFF);
                }
                gdImageColorAllocateAlpha(im, 0,0,0, 127); //0xFF000000u;
            }
        }
    #if 0 /* NES mode */
        }
        for(unsigned mode=0; mode<4 && (im=imtab[mode]); ++mode)
    #endif
        for(unsigned p=0, y=0; y<hei; ++y)
        {
            for(unsigned x=0; x<wid; ++x)
            {
                uint32 pix = screen[p++];
                if(pix == DefaultPixel)
                    pix = 0x7F000000u;
                if(!palette_failed)
                {
                    if(false) // test color palette
                    {
                    /*r = 256*y/hei; // test color scale dithering
                      g = 256*x/wid;
                      b = 0;*/
                        if(y < 32)
                            pix = Palette[x * PaletteSize / wid];
                    }

                    int r = (pix >> 16)&0xFF;
                    int g = (pix >>  8)&0xFF;
                    int b = (pix      )&0xFF;
                    int a = (pix >> 24); if(a&0x80) a>>=1;

                    int color = gdImageColorExactAlpha(im, r,g,b,a);
                    #if 0 /* NES mode */
                    color = -1;
                    #endif
                    if(color == -1)
                    {
                        if(!PaletteReductionMethod.empty())
                        {
                        #if 0 /* NES mode */
                            pix = (pix & 0xFFFFFF) | (mode << 24);
                        #endif
                            // Find two closest entries from palette and use o8x8 dithering
                            PalettePair output;
                            pixel_cache_t::iterator i = pixel_cache.lower_bound(pix);
                            if(i == pixel_cache.end() || i->first != pix)
                            {
                                uint32* pal = Palette;
                                unsigned palsize = PaletteSize;
                            #if 0 /* NES mode */
                                pal = Palette + mode*4; palsize = 4;
                            #endif
                                output = FindBestPalettePair(
                                    r,g,b,
                                    pal, palsize);
                                pixel_cache.insert(i, std::make_pair(pix, output));
                            }
                            else
                                output = i->second;

                            #define d(x) x/64.0
                            static const float pattern[8*8]
    = { d(1 ), d(49), d(13), d(61), d( 4), d(52), d(16), d(64),
        d(33), d(17), d(45), d(29), d(36), d(20), d(48), d(32),
        d(9 ), d(57), d( 5), d(53), d(12), d(60), d( 8), d(56),
        d(41), d(25), d(37), d(21), d(44), d(28), d(40), d(24),
        d(3 ), d(51), d(15), d(63), d( 2), d(50), d(14), d(62),
        d(35), d(19), d(47), d(31), d(34), d(18), d(46), d(30),
        d(11), d(59), d( 7), d(55), d(10), d(58), d( 6), d(54),
        d(43), d(27), d(39), d(23), d(42), d(26), d(38), d(22) };
                            #undef d
                            #if 0 // temporal dither
                            if((frameno^x^y)&1) output.result += 1/128.0;
                            #endif
                            float position = output.result + pattern[(y&7)*8+(x&7)];
                            color = (position > 1.0f) ? output.entry2 : output.entry1;
                            #if 0 /* NES mode */
                            color += mode*4;
                            pix &= 0xFFFFFF;
                            #endif
                        }
                        else
                        {
                            color = gdImageColorAllocateAlpha(im, r,g,b,a);
                        }
                    }
                    if(color == -1)
                    {
                        /* If palette is exhausted, try without palette */
                        palette_failed = true;
                        gdImageDestroy(im);
                        im = gdImageCreateTrueColor(wid, hei);
                        goto reloop;
                    }
                    if(pix & 0xFF000000u) gdImageColorTransparent(im, color);
                #if 0 /* CGA16 mode */
                    if(color >= 10) color += 1; // pattern 1010 was skipped over
                #endif
                    gdImageSetPixel(im, x,y, color);
                }
                else
                    gdImageSetPixel(im, x,y, pix);
            }
            #if 0 /* CGA16 Mode */
            static unsigned temp[643] = { 0 };
            static unsigned cga16_palette[16*5]; static bool cga16_init = false;
            if(!cga16_init)
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

                        double pixelI = I * 1.0 / 3.0; //I* tvSaturnation / 3.0
                        double pixelQ = Q * 1.0 / 3.0; //Q* tvSaturnation / 3.0
                        I = pixelI*coshue + pixelQ*sinhue;
                        Q = pixelQ*coshue - pixelI*sinhue;

                        double R = Y + 0.956*I + 0.621*Q; if (R < 0.0) R = 0.0; if (R > 1.0) R = 1.0;
                        double G = Y - 0.272*I - 0.647*Q; if (G < 0.0) G = 0.0; if (G > 1.0) G = 1.0;
                        double B = Y - 1.105*I + 1.702*Q; if (B < 0.0) B = 0.0; if (B > 1.0) B = 1.0;
                        unsigned char rr = R*0xFF, gg = G*0xFF, bb = B*0xFF;
                        cga16_palette[(j<<4)|i] = (rr << 16) | (gg << 8) | bb;
                    }
                cga16_init = true;
            }
            for(unsigned x=0; x<640; ++x)
                temp[x+2] = (( gdImageGetPixel(im,x>>2,y) >> (3-(x&3)) ) & 1) << 4;
            for(unsigned i=0, x=0; x<160; ++x)
            {
                unsigned v = gdImageGetPixel(im,x,y);
                for(unsigned c=0; c<4; ++c)
                {
                    unsigned p = v | (temp[i] + temp[i+1] + temp[i+2] + temp[i+3]);
                    gdImageSetPixel(im2, i++, y,  cga16_palette[p]);
                }
            }
            #endif
        }
        #if 0 /* CGA16 Mode */
        gdImageDestroy(im);
        im = im2;
        palette_failed = true;
        #endif
        #if 0 /* NES mode */
        /* For each 16x16 pixel region in the target image,
         * choose one of the corresponding regions in the four
         * sample images that best represents the input data */
        for(unsigned by=0; by<hei; by += 16)
        {
            unsigned ey = by+16; if(ey > hei) ey = hei;
            for(unsigned bx=0; bx<wid; bx += 16)
            {
                unsigned ex = bx+16; if(ex > wid) ex = wid;

                double bestdiff = 1e99;
                unsigned bestmode = 0;
                for(unsigned mode=0; mode<4; ++mode)
                {
                    im = imtab[mode];
                    double diff = 0;
                    const double down = 1.0/(4*4);
                    for(unsigned y=by; y+4<=ey; y+=1)
                        for(unsigned x=bx; x+4<=ex; x+=1)
                        {
                            int r1=0,g1=0,b1=0, r2=0,g2=0,b2=0;
                            for(unsigned cy=0; cy<4; ++cy)
                            for(unsigned cx=0; cx<4; ++cx)
                            {
                                uint32 pix1 = screen[(y+cy)*wid+(x+cx)];
                                uint32 pix2  = Palette[gdImageGetPixel(im,x+cx,y+cy)];
                                r1 += (pix1 >> 16) & 0xFF; g1 += (pix1 >> 8) & 0xFF; b1 += (pix1) & 0xFF;
                                r2 += (pix2 >> 16) & 0xFF; g2 += (pix2 >> 8) & 0xFF; b2 += (pix2) & 0xFF;
                            }
                            double luma1 = (r1*299 + g1*587 + b1*114)*(1e-3*down);
                            double luma2 = (r2*299 + g2*587 + b2*114)*(1e-3*down);
                            double lumadiff = (luma1-luma2);
                            double avgluma = (luma1+luma2);
                            double rdiff = (r1-r2)*down, gdiff = (g1-g2)*down, bdiff = (b1-b2)*down;
                            diff += (rdiff*rdiff + gdiff*gdiff + bdiff*bdiff);
                        }
                    if(diff < bestdiff)
                        { bestdiff = diff; bestmode = mode; }
                }
                gdImageCopy(im2, imtab[bestmode], bx,by, bx,by, ex-bx, ey-by);
            }
        }
        for(unsigned a=0; a<4; ++a) gdImageDestroy(imtab[a]);
        im = im2;
        #endif

        FILE* fp = std::fopen(Filename, "wb");
        if(palette_failed)
        {
            gdImageTrueColorToPalette(im, false, 256);
            gdImageColorTransparent(im, gdImageColorExactAlpha(im, 0,0,0, 127));
        }
        if(SaveGif == 1)
            gdImageGif(im, fp);
        else
            gdImagePngEx(im, fp, 1);
        std::fclose(fp);
        gdImageDestroy(im);
    }
    else
    {
        gdImagePtr im = gdImageCreateTrueColor(wid,hei);
        gdImageAlphaBlending(im, false);
        gdImageSaveAlpha(im, true);

        for(unsigned p=0, y=0; y<hei; ++y)
            for(unsigned x=0; x<wid; ++x)
            {
                uint32 pix = screen[p++];
                if(pix == DefaultPixel)
                    pix = 0x7F000000u;
                gdImageSetPixel(im, x,y, pix);
            }
        FILE* fp = std::fopen(Filename, "wb");
        gdImagePngEx(im, fp, 1);
        std::fclose(fp);
        gdImageDestroy(im);
    }
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
