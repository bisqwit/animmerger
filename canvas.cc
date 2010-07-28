#include "canvas.hh"
#include "alloc/FSBAllocator.hh"

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
        IntCoordinate   where;
        SpotType        data;
    };
    
    struct RelativeCoordinate
    {
        int x, y;
        
        RelativeCoordinate(int xx,int yy) : x(xx), y(yy) { }
        
        bool operator<< (const RelativeCoordinate& b) const
        {
            int l = length(), bl = b.length();
            if(l != bl) return l < bl;
            if(x != b.x) return x < b.x;
            return y < b.y;
        }
        bool operator== (const RelativeCoordinate& b) const
        {
            return x == b.x && y == b.y;
        }
        
        int length() const
        {
            return (x<0 ? -x : x) + (y<0 ? -y : y);
        }
    };

    struct AlignResult
    {
        int  offs_x;
        int  offs_y;
        bool suspect_reset;
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
        if(force_all_pixels)
            return;
        
        const unsigned x_divide = 32;
        const unsigned y_divide = 32;
        const unsigned x_shrunk = (xs + x_divide-1) / x_divide;
        const unsigned y_shrunk = (ys + y_divide-1) / y_divide;
        
        typedef std::map<SpotType, std::vector<size_t>,
            std::less<SpotType>,
            FSBAllocator<int> > RarityType;
        
        std::vector<RarityType> Rarities( x_shrunk * y_shrunk );
        
        for(unsigned p=0, y=0; y+4<=sy; ++y, p+=3)
            for(unsigned x=0; x+4<=sx; ++x, ++p)
                Rarities[ (y/y_divide) * x_shrunk + (x/x_divide) ]
                    [ spots[p] ].push_back(p);
        
        for(unsigned p=0, y=0; y<y_shrunk; ++y)
            for(unsigned x=0; x<x_shrunk; ++x, ++p)
            {
                /* Pick the SpotType that occurs the least number of times */
                const RarityType& r = Rarities[p];
                RarityType::const_iterator winner = r.begin();
                for(RarityType::const_iterator i = winner; ++i != r.end(); )
                    if(i->second.size() < winner->second.size())
                        winner = i;

                const size_t coordinate = winner->second.front();
                InterestingSpot spot =
                    { { xoffs+ coordinate%sx,
                        yoffs+ coordinate/sx },
                      winner->first };
                output.push_back(spot);
            }
    }
    
    AlignResult Align(
        const std::vector<InterestingSpot>& input_spots,
        const std::vector<InterestingSpot>& reference_spots,
        int org_x,
        int org_y)
    {
        typedef std::map<SpotType, std::vector<IntCoordinate>,
            std::less<SpotType>, FSBAllocator<int> > SpotLocSetType;

        SpotLocSetType input_spot_locations;
        for(size_t b=input_spots.size(), a=0; a<b; ++a)
            input_spot_locations[ input_spots[a].data ]
                .push_back( input_spots[a].where ];

        SpotLocSetType reference_spot_locations;
        for(size_t b=reference_spots.size(), a=0; a<b; ++a)
            reference_spot_locations[ reference_spots[a].data ]
                .push_back( reference_spots[a].where ];
        
        /* Find a set of possible offsets */
        std::map<RelativeCoordinate, unsigned,
            std::less<RelativeCoordinate>,
            FSBAllocator<int> > offset_suggestions;
        for(int y=-4; y<=4; ++y)
            for(int x=-4; x<=4; ++x)
                offset_suggestions.insert
                    (std::make_pair( RelativeCoordinate(x,y), 999 ));
        
        /* If a rarely occurring vista is found in the reference picture,
         * add the offset to the list of offsets to be tested
         */ 
        for(SpotLocSetType::const_iterator
            i = input_spot_locations.begin();
            i != input_spot_locations.end();
            ++i)
        {
            const std::vector<IntCoordinate>& coords = i->second;

            if(coords.size() > 5) continue;
            const SpotType& pixel = i->first;
            
            SpotLocSetType::const_iterator r
                = reference_spot_locations.find( pixel );
            if(r == reference_spot_locations.end()) continue;
            
            const std::vector<IntCoordinate>& rcoords = r->second;
            if(rcoords.size() > 20) continue;
            
            for(size_t c=0; c<coords.size(); ++c)
                for(size_t d=0; d<rcoords.size(); ++d)
                {
                    offset_suggestions[
                        RelativeCoordinate(
                            coords[c].x - rcoords[c].x - org_x,
                            coords[c].y - rcoords[c].y - org_y
                                      )] += 1;
                }
        }
        
        input_spot_locations.clear();
        reference_spot_locations.clear();

        typedef std::map<IntCoordinate, SpotType,
            std::less<IntCoordinate>, FSBAllocator<int> > LocSpotSetType;
        
        LocSpotSetType reference_location_spots;
        for(size_t b=reference_spots.size(), a=0; a<b; ++a)
            reference_location_spots.insert(
                std::make_pair( reference_spots[a].where,
                                reference_spots[a].data ) );

        /* For each candidate offset that has sufficient confidence,
         * find out the one that has most overlap in spots to the reference
         */
        size_t             best_match = 0;
        RelativeCoordinate best_coord;
        
        for(std::map<RelativeCoordinate, unsigned>::const_iterator
            i = offset_suggestions.begin();
            i != offset_suggestions.end();
            ++i)
        {
            if(i->second < 8) continue; // Not confident enough
            
            const RelativeCoordinate& relcoord = i->first;
            
            for(size_t b=input_spots.size(), a=0; a<b; ++a)
            {
                const SpotType& data       = input_spots[a].data;
                const IntCoordinate& coord = input_spots[a].where;
                
                IntCoordinate test_coord =
                    {
                        org_x + relcoord.x + coord.x,
                        org_y + relcoord.y + coord.y
                    };
                
                size_t n_match = 0;
                
                SpotLocSetType::const_iterator
                    r = reference_location_spots.find(test_coord);
                if(r == reference_location_spots.end()
                || !(r->second == data))
                    continue;
                
                ++n_match;
            }
            if(n_match > best_match)
            {
                best_match = n_match,
                best_coord = i->first;
            }
        }
        
        AlignResult result;
        result.suspect_reset = (best_match < input_spots.size()/16);
        result.offs_x        = best_coord.x;
        result.offs_y        = best_coord.y;
        return result;
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

    FitScreen(input,
        sx,sy,
        align.offs_x,
        align.offs_y,
        align.suspect_reset);
}
