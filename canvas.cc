#include "canvas.hh"
#include "align.hh"
#include <gd.h>
#include <cstdio>

unsigned CurrentTimer = 0;       // For animated
unsigned SequenceBegin = 0;      // For animated

namespace
{
    struct ScrollingPosition            // For animated
    {
        unsigned org_x, org_y;
    };
    VecType<ScrollingPosition> scrolls; // For animated
}

static inline bool veq
    (const VecType<uint32>& a,
     const VecType<uint32>& b) // For ChangeLog
{
    if(a.size() != b.size()) return false;
    return std::memcmp(&a[0], &b[0], a.size()*sizeof(a[0])) == 0;
}

const VecType<uint32>
TILE_Tracker::LoadScreen(int ox,int oy, unsigned sx,unsigned sy)
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
        xmaptype& xmap = screens[yscreen];

        unsigned this_cube_yend = yscreen==yscreen_end ? ((oy+sy-1)&255) : 255;

        unsigned this_cube_xstart = ox&255;
        for(int xscreen=xscreen_begin; xscreen<=xscreen_end; ++xscreen)
        {
            unsigned this_cube_xend = xscreen==xscreen_end ? ((ox+sx-1)&255) : 255;
/*
            std::fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                this_cube_xstart,this_cube_xend,
                this_cube_ystart,this_cube_yend);
*/
            const vectype& cube = xmap[xscreen].pixels;
            /* If this screen is not yet initialized, we'll skip over
             * it, since there's no real reason to initialize it at
             * this point. */
            if(!cube.empty())
            {
                /* Load this particular cube */
                for(unsigned yp=this_cube_ystart, y=0; yp<=this_cube_yend; ++y, ++yp)
                {
                    unsigned srcp  = 256*yp + this_cube_xstart - this_cube_xstart;
                    unsigned destp =   sx*y + targetpos        - this_cube_xstart;
                    for(unsigned xp=this_cube_xstart; xp<=this_cube_xend; ++xp)
                        result[destp + xp] = cube[srcp + xp];
                }
            }

            unsigned this_cube_xsize = (this_cube_xend-this_cube_xstart)+1;

            targetpos+= this_cube_xsize;

            this_cube_xstart=0;
        }

        unsigned this_cube_ysize = (this_cube_yend-this_cube_ystart)+1;

        targetpos += sx * (this_cube_ysize-1);

        this_cube_ystart=0;
    }

    return result;
}

void
TILE_Tracker::PutScreen
    (const uint32*const input, int ox,int oy, unsigned sx,unsigned sy)
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

        unsigned this_cube_xstart = ox&255;
        for(int xscreen=xscreen_begin; xscreen<=xscreen_end; ++xscreen)
        {
            unsigned this_cube_xend = xscreen==xscreen_end ? ((ox+sx-1)&255) : 255;

            cubetype& cube = xmap[xscreen];

            /* If this screen is not yet initialized, we'll initialize it */
            if(cube.pixels.empty()) cube.pixels.resize(256*256);
            if(method != pm_MostUsedPixel
            && method != pm_ChangeLogPixel
            && method != pm_LoopingLogPixel)
                if(cube.mostused.empty()) cube.mostused.resize(256*256);
            cube.changed = true;

/*
            std::fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                this_cube_xstart,this_cube_xend,
                this_cube_ystart,this_cube_yend);
*/
            /* Write this particular cube */
            for(unsigned yp=this_cube_ystart, y=0; yp<=this_cube_yend; ++y, ++yp)
                for(unsigned xp=this_cube_xstart, x=0; xp<=this_cube_xend; ++x, ++xp)
                {
                    uint32 pix = input[targetpos + x + y*sx];
                    if(pix & 0xFF000000u) continue; // Do not plot transparent pixels
                    cube.pixels  [xp + 256*yp].set(pix);
                    if(!cube.mostused.empty())
                        cube.mostused[xp + 256*yp].set(pix);
                }

            unsigned this_cube_xsize = (this_cube_xend-this_cube_xstart)+1;

            targetpos+= this_cube_xsize;

            this_cube_xstart=0;
        }

        unsigned this_cube_ysize = (this_cube_yend-this_cube_ystart)+1;

        targetpos += sx * (this_cube_ysize-1);

        this_cube_ystart=0;
    }
}

void TILE_Tracker::Save()
{
    if(UncertainPixel::is_animated())
        std::fprintf(stderr, "Saving(%d,%d)\n", first,CurrentTimer);
    else
        std::fprintf(stderr, "Saving(%d)\n", first);
    if(first) return;

    if(UncertainPixel::is_animated())
    {
        static bool Saving = false;
        if(!Saving)
        {
            Saving = true;
            unsigned SavedTimer = CurrentTimer;

            if(UncertainPixel::is_loopinglog())
            {
                const unsigned LoopLength = UncertainPixel::GetLoopLength();
                if(SavedTimer >= LoopLength) SavedTimer = LoopLength;
            }

            for(CurrentTimer=0; CurrentTimer<SavedTimer; CurrentTimer += 1)
            {
                count = CurrentTimer + SequenceBegin;
                std::fprintf(stderr, "Saving frame %u/%u @ %u\n",
                    count-SequenceBegin,SavedTimer, SequenceBegin);
                Save();
            }
            CurrentTimer  = SavedTimer;
            scrolls.clear();
            Saving = false;
        }
        else
        {
            printf("/*%u*/ %d,%d, %d,%d,\n",
                count,
                scrolls[CurrentTimer].org_x - get_min_x(),
                scrolls[CurrentTimer].org_y - get_min_y(),
                0,0
                );
            fflush(stdout);
        }
    }

    int ymi = get_min_y(), yma = get_max_y();
    int xmi = get_min_x(), xma = get_max_x();

    unsigned wid = xma-xmi+1;
    unsigned hei = yma-ymi+1;

    if(wid <= 1 || hei <= 1) return;

    std::fprintf(stderr, " (%d,%d)-(%d,%d)\n", 0,0, xma-xmi, yma-ymi);

    char Filename[512] = {0}; // explicit init keeps valgrind happy
    std::sprintf(Filename, "tile-%04u.png", count++);

    VecType<uint32> screen = LoadScreen(xmi,ymi, wid,hei);

    if(UncertainPixel::is_changelog())
    {
        if(veq(screen, LastScreen) && !LastFilename.empty())
        {
            std::fprintf(stderr, "->link (%u,%u)\n",
                (unsigned)screen.size(),
                (unsigned)LastScreen.size());
            std::string cmd = "ln "+LastFilename+" "+Filename;
            system(cmd.c_str());
            LastScreen   = screen;
            LastFilename = Filename;
            return;
        }
        LastScreen   = screen;
        LastFilename = Filename;
    }

    gdImagePtr im = gdImageCreateTrueColor(wid,hei);

    for(unsigned p=0, y=0; y<hei; ++y)
        for(unsigned x=0; x<wid; ++x)
        {
            unsigned pix32 = screen[p++];
            unsigned pix = pix32/*gdTrueColor
            (
                (pix32 >> 16) & 0xFF,
                (pix32 >> 8) & 0xFF,
                (pix32     ) & 0xFF
            )*/;
            gdImageSetPixel(im, x,y, pix);
        }

    FILE* fp = std::fopen(Filename, "wb");
    gdImagePngEx(im, fp, 1);
    std::fclose(fp);
    gdImageDestroy(im);
}

namespace
{
    /* Cache InterestingSpot lists for each cube */
    /* NOTE: GLOBAL */
    std::map
        <IntCoordinate, VecType<InterestingSpot>,
         std::less<IntCoordinate>,
         FSBAllocator<int> > cache;
}

void
TILE_Tracker::FitScreenAutomatic(const uint32*const input, unsigned sx,unsigned sy)
{
    /* Find spots of interest within the reference image,
     * and within the input image.
     *
     * Select that offset which results in greatest overlap
     * between those two sets of spots.
     */

    VecType<InterestingSpot> input_spots;
    VecType<InterestingSpot> reference_spots;
    FindInterestingSpots(input_spots, input, 0,0, sx,sy, true);

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
                switch(method)
                {
                    case pm_MostUsedPixel:
                        for(unsigned p=0; p<256*256; ++p)
                            result[p] = cube.pixels[p];
                        break;
                    case pm_ChangeLogPixel:
                        for(unsigned p=0; p<256*256; ++p)
                        {
                            void* ptr = cube.pixels[p].GetPtr();
                            result[p] = ((ChangeLogPixel*)ptr) ->GetMostUsed();
                        }
                        break;
                    case pm_LoopingLogPixel:
                        for(unsigned p=0; p<256*256; ++p)
                        {
                            void* ptr = cube.pixels[p].GetPtr();
                            result[p] = ((LoopingLogPixel*)ptr) ->GetMostUsed();
                        }
                        break;
                    default:
                        for(unsigned p=0; p<256*256; ++p)
                            result[p] = cube.mostused[p];
                }

                size_t prev_size = reference_spots.size();
                FindInterestingSpots(reference_spots, result,
                    x_screen_offset,y_screen_offset,
                    256,256,
                    false);

                cache[cache_key].assign(
                    reference_spots.begin() + prev_size,
                    reference_spots.end() );
            }
            else
            {
                const VecType<InterestingSpot>& found = cache[cache_key];
                reference_spots.insert(
                    reference_spots.end(),
                    found.begin(),
                    found.end());
            }
        }
    }

    AlignResult align = Align(
        input_spots,
        reference_spots,
        org_x, org_y);

    FitScreen(input,
        sx,sy,
        align.offs_x,
        align.offs_y,
        align.suspect_reset);
}

void TILE_Tracker::FitScreen
    (const uint32* buf,
     unsigned max_x,
     unsigned max_y,
     int offs_x, int offs_y, bool suspect_reset,
     int extra_offs_x,
     int extra_offs_y
    )
{
    if(! UncertainPixel::is_animated())
    {
/*
    static unsigned framecounter=0;
    if(++framecounter == 600) { Save(); framecounter=0; }
*/
    }

    //if(offs_x != 0 || offs_y != 0)
    {
        std::fprintf(stderr, " Motion(%d,%d), Origo(%d,%d)\n", offs_x,offs_y, org_x,org_y);
    }

    org_x += offs_x; org_y += offs_y;

    int this_org_x = org_x + extra_offs_x;
    int this_org_y = org_y + extra_offs_y;

    if(suspect_reset)
    {
#if 0
        goto AlwaysReset;
#endif
        VecType<uint32> oldbuf = LoadScreen(this_org_x,this_org_y, max_x,max_y);
        unsigned diff = 0;
        for(unsigned a=0; a<oldbuf.size(); ++a)
        {
            unsigned oldpix = oldbuf[a];
            unsigned pix   = buf[a];
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
            SaveAndReset();
#endif
#endif
        }
    }

    if(first || this_org_x < xmin) xmin = this_org_x;
    if(first || this_org_y < ymin) ymin = this_org_y;
    int xtmp = this_org_x+max_x; if(first || xtmp > xmax) xmax=xtmp;
    int ytmp = this_org_y+max_y; if(first || ytmp > ymax) ymax=ytmp;
    first=false;

#if 0
    /* If the image geometry would exceed some bounds */
    if(xmax-xmin > 800 || ymax-ymin > 800)
    {
        SaveAndReset();
        first=true;
    }
#endif

    PutScreen(buf, this_org_x,this_org_y, max_x,max_y);
}

void TILE_Tracker::NextFrame()
{
    if(UncertainPixel::is_animated())
    {
        ScrollingPosition s;
        s.org_x = org_x;
        s.org_y = org_y;
        scrolls.push_back(s);

        ++CurrentTimer;
    }
}
