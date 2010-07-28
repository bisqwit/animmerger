#include "canvas.hh"

unsigned CurrentTimer = 0;       // For animated
unsigned SequenceBegin = 0;      // For animated
struct ScrollingPosition         // For animated
{
    unsigned org_x, org_y;
};
std::vector<ScrollingPosition> scrolls; // For animated



const std::vector<uint32>
TILE_Tracker::LoadScreen(int ox,int oy, unsigned sx,unsigned sy)
{
    // Create the result vector filled with default pixel value
    std::vector<uint32> result(sy*sx, DefaultPixel);

    const int xbegin = ox;
    const int xend   = ox+sx-1;
    
    const int xscreen_begin = xbegin/256;
    const int xscreen_end   = xend  /256;
    
    const int ybegin = oy;
    const int yend   = oy+sy-1;
    
    const int yscreen_begin = ybegin/256;
    const int yscreen_end   = yend  /256;
    
/*
    fprintf(stderr, "Loading screens x(%d..%d)y(%d..%d)\n",
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
            fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
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
    fprintf(stderr, "Writing screens x(%d..%d)y(%d..%d)\n",
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
            if(cube.mostused.empty()) cube.mostused.resize(256*256);
            cube.changed = true;
            
/*
            fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                this_cube_xstart,this_cube_xend,
                this_cube_ystart,this_cube_yend);
*/
            /* Write this particular cube */
            for(unsigned yp=this_cube_ystart, y=0; yp<=this_cube_yend; ++y, ++yp)
                for(unsigned xp=this_cube_xstart, x=0; xp<=this_cube_xend; ++x, ++xp)
                {
                    uint32 pix = input[targetpos + x + y*sx];
                    cube.pixels  [xp + 256*yp].set(pix);
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
        fprintf(stderr, "Saving(%d,%d)\n", first,CurrentTimer);
    else
        fprintf(stderr, "Saving(%d)\n", first);
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
                fprintf(stderr, "Saving frame %u/%u @ %u\n",
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
    
    fprintf(stderr, " (%d,%d)-(%d,%d)\n", 0,0, xma-xmi, yma-ymi);
    
    char Filename[512];
    sprintf(Filename, "tile-%04u.png", count++);
    
    std::vector<uint32> screen = LoadScreen(xmi,ymi, wid,hei);
    
    if(UncertainPixel::is_changelog())
    {
        if(veq(screen, LastScreen) && !LastFilename.empty())
        {
            fprintf(stderr, "->link (%u,%u)\n", screen.size(), LastScreen.size());
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
                (pix32 >> 24) & 0xFF,
                (pix32 >> 16) & 0xFF,
                (pix32 >> 8) & 0xFF
            )*/;
            gdImageSetPixel(im, x,y, pix);
        }
    
    FILE* fp = fopen(Filename, "wb");
    gdImagePngEx(im, fp, 1);
    fclose(fp);
    gdImageDestroy(im);
}

namespace
{
    static const int RGB2YUV_SHIFT = 15; /* highest value where [RGB][YUV] fit in signed short */

    static const int RY = 8414;  //  ((int)(( 65.738/256.0)*(1<<RGB2YUV_SHIFT)+0.5));
    static const int RV = 14392; //  ((int)((112.439/256.0)*(1<<RGB2YUV_SHIFT)+0.5));
    static const int RU = -4856; //  ((int)((-37.945/256.0)*(1<<RGB2YUV_SHIFT)+0.5));

    static const int GY = 16519; //  ((int)((129.057/256.0)*(1<<RGB2YUV_SHIFT)+0.5));
    static const int GV = -12051;//  ((int)((-94.154/256.0)*(1<<RGB2YUV_SHIFT)+0.5));
    static const int GU = -9534; //  ((int)((-74.494/256.0)*(1<<RGB2YUV_SHIFT)+0.5));

    static const int BY = 3208;  //  ((int)(( 25.064/256.0)*(1<<RGB2YUV_SHIFT)+0.5));
    static const int BV = -2339; //  ((int)((-18.285/256.0)*(1<<RGB2YUV_SHIFT)+0.5));
    static const int BU = 14392; //  ((int)((112.439/256.0)*(1<<RGB2YUV_SHIFT)+0.5));

    static const int Y_ADD = 16;
    static const int U_ADD = 128;
    static const int V_ADD = 128;

    struct IntCoordinate
    {
        int x, y;
        
        bool operator<< (const IntCoordinate& b) const
        {
            if(x != b.x) return x < b.x;
            return y < b.y;
        }
        bool operator== (const IntCoordinate& b) const
        {
            return x == b.x && y == b.y;
        }
    };

    typedef std::pair<uint64,uint64> SpotType;
    struct InterestingSpot
    {
        IntCoordinate   point;
        SpotType        data;
    };
    
    struct AlignResult
    {
        int  x_offs;
        int  y_offs;
        bool suspect_change;
    };

    void FindInterestingSpots(
        std::vector<InterestingSpot>& output,
        const uint32* input,
        int xoffs, int yoffs,
        unsigned sx, unsigned sy,
        bool force_all_pixels)
    {
        std::vector<unsigned char> Y(sx*sy);
        for(unsigned p=0, y=0; y<sy; ++y)
            for(unsigned x=0; x<sx; ++x, ++p)
                Y[p] = ((((input[p] >> 16)      ) * RY
                      +  ((input[p] >> 8) & 0xFF) * GY
                      +  ((input[p]     ) & 0xFF) * BY
                        ) >> RGB2YUV_SHIFT) + Y_ADD;

        std::vector<SpotType> spots;
        if(!force_all_pixels)
            spots.resize(sx*sy);
        for(unsigned p=0, y=0; y+4<=sy; ++y, p+=3)
            for(unsigned x=0; x+4<=sx; ++x, ++p)
            {
                uint32 pix[4] = { *(uint32*)&Y[p],
                                  *(uint32*)&Y[p+sy],
                                  *(uint32*)&Y[p+sy*2],
                                  *(uint32*)&Y[p+sy*3] };
                SpotType data = { pix[0] | (uint64(pix[1]) << 32),
                                  pix[2] | (uint64(pix[3]) << 32) };

                if(force_all_pixels)
                {
                    InterestingSpot spot = { { xoffs+x, yoffs+y }, data };
                    output.push_back(spot);
                }
                else
                {
                    spots[p] = data;
                }
            }
    }
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
     
    std::vector<InterestingSpot> input_spots;
    std::vector<InterestingSpot> reference_spots;
    FindInterestingSpots(input_spots, input, 0,0, sx,sy, true);
    
    static std::map<IntCoordinate, std::vector<InterestingSpot> > cache;

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
            const vectype& cube       = xi->second;
            
            IntCoordinate cache_key = {x_screen_offset,y_screen_offset};
            
            if(cube.changed)
            {
                uint32 result[256*256];
                for(unsigned p=0; p<256; ++p)
                    result[p] = cube.mostused[p];
                
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
                const std::vector<InterestingSpot>& found = cache[cache_key];
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
}