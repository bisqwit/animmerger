#include "canvas.hh"
#include "alloc/FSBAllocator.hh"

#include <utility>
#include <set>

#include <gd.h>

unsigned CurrentTimer = 0;       // For animated
unsigned SequenceBegin = 0;      // For animated
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
            const vectype& cube  = xmap[xscreen].pixels;
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
            
            cubetype& cubedata = xmap[xscreen];
            vectype& cube  = cubedata.pixels;
            std::vector<MostUsedPixel>& cube2 = cubedata.mostused;
            cubedata.changed = true;
            
            /* If this screen is not yet initialized, we'll initialize it */
            if(cube.empty()) cube.resize(256*256);
            if(cube2.empty()) cube2.resize(256*256);
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
                    cube[xp + 256*yp].set(pix);
                    cube2[xp + 256*yp].set(pix);
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
                fprintf(stderr, "Saving frame %u/%u @ %u, savedtimer=%u, currenttimer=%u, scrolls.size=%u\n",
                    count-SequenceBegin,SavedTimer, SequenceBegin,
                    (unsigned) SavedTimer,
                    (unsigned) CurrentTimer,
                    (unsigned) scrolls.size());
                Save();
            }
            CurrentTimer  = SavedTimer;
            //scrolls.clear();
            Saving = false;
        }
        else
        {
            printf("/*%u*/ %d,%d, %d,%d,\n",
                (unsigned)count,
                (int)(scrolls[CurrentTimer].org_x - get_min_x()),
                (int)(scrolls[CurrentTimer].org_y - get_min_y()),
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
            fprintf(stderr, "->link (%u,%u)\n",
                (unsigned)screen.size(), (unsigned)LastScreen.size());
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
    /* For RGB2YUV: */

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

    /* For YUV2RGB: */

    static const int YUV2RGB_SHIFT = 13; /* highest value where UB still fits in signed short */

    static const int Y_REV = 9539; // ((int)( (  255 / 219.0 )     * (1<<YUV2RGB_SHIFT)+0.5));
    static const int VR = 14688;   // ((int)( ( 117504 / 65536.0 ) * (1<<YUV2RGB_SHIFT)+0.5));
    static const int VG = -6659;   // ((int)( ( -53279 / 65536.0 ) * (1<<YUV2RGB_SHIFT)+0.5));
    static const int UG = -3208;   // ((int)( ( -25675 / 65536.0 ) * (1<<YUV2RGB_SHIFT)+0.5));
    static const int UB = 16525;   // ((int)( ( 132201 / 65536.0 ) * (1<<YUV2RGB_SHIFT)+0.5));
}

namespace
{
    typedef std::pair<uint64, uint64> PixelLabel;
    
    struct IntCoordinate
    {
        int x, y;
        
        bool operator< (const IntCoordinate& b) const
        {
            if(x != b.x) return x < b.x;
            return y < b.y;
        }
        bool operator== (const IntCoordinate& b) const
        {
            return x==b.x && y==b.y;
        }
    };
    struct InterestingSpot
    {
        IntCoordinate where;
        PixelLabel    label;
    };
    
    struct AlignResult
    {
        int offs_x;
        int offs_y;
        bool suspect_reset;
    };
    
    void FindInterestingSpots(
        std::vector<InterestingSpot>& output,
        const uint32* input,
        int xoffs, int yoffs,
        unsigned sx, unsigned sy,
        bool every_pixel)
    {
        /* Calculate the type and interestingness for all pixels.
         *
         * PLAN 1:
         * Then narrow them down, by selecting the most interesting
         * spot from every 32x32 size region.
         * This gets 8x8, i.e. 64 spots for each cube,
         * and 8x7, i.e. 56 spots for input image.
         * 
         * PLAN 2:
         *
         * Sort the pixels in order of rarity, and take N most rare
         * ones, still sufficiently far apart and sufficiently many
         * to avoid just choosing all onscreen actors.
         *
         */
        std::vector<unsigned char> Y(sx*sy);
        //std::vector<unsigned char> U(sx*sy);
        //std::vector<unsigned char> V(sx*sy);
        for(unsigned p=0, y=0; y<sy; ++y)
            for(unsigned x=0; x<sx; ++x,++p)
            {
                uint32 i = input[p];
                unsigned r = (i >> 16);
                unsigned g = (i >> 8) & 0xFF;
                unsigned b = (i >> 0) & 0xFF;
                Y[p] = Y_ADD + ((RY*r + GY*g + BY*b) >> RGB2YUV_SHIFT);
            }
        
        /*unsigned offsets[8*8]; // relative offsets to address a 8x8 tile
        for(unsigned q=0, p=0, y=0; y<8; ++y, q+=sx)
            for(unsigned x=0; x<8; ++x,++p)
                offsets[p] = x+q;*/

        std::vector<PixelLabel> labels(sx*sy);
        for(unsigned p=0, y=0; y+4<=sy; ++y,p+=3)
            for(unsigned x=0; x+4<=sx; ++x,++p)
            {
                /* A sufficiently unique code describing this pixel
                 * should be made by comparing the brightness of this
                 * pixel to its immediate surroundings, scaled by the
                 * average
                 */
                uint64 pixels[4] = // each uint32 is four pixels horizontally
                    { *(uint32*)&Y[p+sx*0],
                      *(uint32*)&Y[p+sx*1],
                      *(uint32*)&Y[p+sx*2],
                      *(uint32*)&Y[p+sx*3] };
                
                labels[p].first  = pixels[0] | (uint64(pixels[1]) << 32);
                labels[p].second = pixels[2] | (uint64(pixels[3]) << 32);
            }
        
        if(every_pixel)
        {
            for(unsigned p=0, y=0; y+4<=sy; ++y,p+=3)
                for(unsigned x=0; x+4<=sx; ++x, ++p)
                {
                    InterestingSpot spot = { {x+xoffs, y+yoffs}, labels[p] };
                    output.push_back(spot);
                }
            return;
        }
        
        typedef std::map<PixelLabel, std::vector<unsigned>,
                         std::less<PixelLabel>,
                         FSBAllocator<int>
                        > Rarity;

        const unsigned y_shrink = 8;
        const unsigned x_shrink = 8;
        const unsigned rarity_x = (sx+x_shrink-1)/x_shrink;
        const unsigned rarity_y = (sy+y_shrink-1)/y_shrink;
        
        std::vector<Rarity> rarities( rarity_x * rarity_y );
        for(unsigned p=0, y=0; y+4<=sy; ++y, p+=3)
            for(unsigned x=0; x+4<=sx; ++x,++p)
                rarities[(y/y_shrink) * rarity_x + x/x_shrink][ labels[p] ].push_back(p);
    
        /* From each table, pick the least common PixelLabel,
         * and show the first coordinate for it */
        for(std::vector<Rarity>::iterator
            l = rarities.end(),
            k = rarities.begin(); k != l; ++k)
        {
            typedef Rarity::const_iterator it;
            Rarity& r = *k;
            
            // Find the PixelLabel that occurred in
            // the least number of coordinates (j)
            it j = r.begin();
            for(it i = j; ++i != r.end(); )
                if(i->second.size() < j->second.size())
                    j = i;
            
            if(j->second.size() == x_shrink * y_shrink) continue;
            
            size_t coordinate = j->second.front();
            InterestingSpot spot =
            {
                { (coordinate % sx) + xoffs,
                  (coordinate / sx) + yoffs },
                j->first
            };
            output.push_back(spot);
            r.clear();
        }
    }
    
    struct TryCoordinate
    {
        int x;
        int y;
        
        TryCoordinate(int xx,int yy): x(xx), y(yy) { }
        
        bool operator< (const TryCoordinate& b) const
        {
            int l = length(), bl = b.length();
            if(l != bl) return l < bl;
            if(x != b.x) return x < b.x;
            return y < b.y;
        }
        bool operator== (const TryCoordinate& b) const
        {
            return x==b.x && y==b.y;
        }
        
        int length() const
        {
            return (x<0 ? -x : x) + (y<0 ? -y : y);
        }
    };

    AlignResult Align(
        const std::vector<InterestingSpot>& input_spots,
        const std::vector<InterestingSpot>& reference_spots,
        int cur_x,
        int cur_y)
    {
        typedef std::map<PixelLabel, std::vector<IntCoordinate>,
                         std::less<PixelLabel>,
                         FSBAllocator<int>
                        > Rarity;
        
        Rarity input_rarity;
        Rarity reference_rarity;
        
        for(size_t b=input_spots.size(), a=0; a<b; ++a)
        {
            input_rarity[ input_spots[a].label ].push_back( input_spots[a].where );
            /*fprintf(stderr,
                "in[%3u]: %16llX,%16llX @ %d,%d\n",
                    (unsigned)a,
                    input_spots[a].label.first,
                    input_spots[a].label.second,
                    input_spots[a].where.x,
                    input_spots[a].where.y);*/
        }
        
        for(size_t b=reference_spots.size(), a=0; a<b; ++a)
        {
            reference_rarity[ reference_spots[a].label ].push_back( reference_spots[a].where );
            /*fprintf(stderr,
                "ref[%3u]: %16llX,%16llX @ %d,%d\n",
                    (unsigned)a,
                    reference_spots[a].label.first,
                    reference_spots[a].label.second,
                    reference_spots[a].where.x,
                    reference_spots[a].where.y);*/
        }
            
        typedef std::multimap
            <size_t, Rarity::iterator, std::less<size_t>,
             FSBAllocator<int> > CountSorted;
        CountSorted input_sorted;
        
        for(Rarity::iterator i=input_rarity.begin(); i!=input_rarity.end(); ++i)
            input_sorted.insert(
                std::make_pair<size_t, Rarity::iterator>
                ( i->second.size(), i ) );

        // TODO IMPLEMENTATION
        
        std::map<TryCoordinate, size_t> try_coordinates;
        for(int x=-5; x<=5; ++x)
            for(int y=-5; y<=5; ++y)
                try_coordinates[TryCoordinate(x,y)] = 999;

        for(CountSorted::iterator
            r = input_sorted.begin(); r != input_sorted.end(); ++r)
        {
            Rarity::iterator i = r->second;
            const PixelLabel& pixel                  = i->first;
            const std::vector<IntCoordinate>& coords = i->second;

            if(coords.size() >= 6) break;
            
            Rarity::const_iterator e = reference_rarity.find(pixel);
            if(e != reference_rarity.end())
            {
                const std::vector<IntCoordinate>& ecoords = e->second;
                if(ecoords.size() > 20) continue;

                for(size_t c=0; c<coords.size(); ++c)
                    for(size_t d=0; d<ecoords.size(); ++d)
                    {
                        TryCoordinate tmp(
                            ecoords[d].x - coords[c].x - cur_x,
                            ecoords[d].y - coords[c].y - cur_y);
                        /*fprintf(stderr, "Pair %u,%u for pixel that occurs %u times suggests %d,%d\n",
                            (unsigned)c, (unsigned)d,
                            (unsigned) coords.size(),
                            tmp.x, tmp.y);*/
                        try_coordinates[tmp] += 1;
                    }
            }
        }
        
        typedef std::map<IntCoordinate, PixelLabel> LocSorted;
        LocSorted reference_sorted;

        for(size_t b=reference_spots.size(), a=0; a<b; ++a)
            reference_sorted.insert(
                std::make_pair(reference_spots[a].where,
                               reference_spots[a].label) );

        TryCoordinate best_loc (0,0);
        size_t        best_n   = 0;
        
        for(std::map<TryCoordinate, size_t>::const_iterator
            i = try_coordinates.begin();
            i != try_coordinates.end();
            ++i)
        {
            /*fprintf(stderr, "Suggestion %d,%d (%u times suggested)\n",
                i->first.x,
                i->first.y,
                (unsigned) i->second);*/
            if(i->second < 8/*
            || i->second < input_spots.size() / 16*/) continue;
            const int try_x = cur_x + i->first.x;
            const int try_y = cur_y + i->first.y;
            size_t n_match = 0;
            
            for(size_t b=input_spots.size(), a=0; a<b; ++a)
            {
                IntCoordinate tmp = {
                    try_x + input_spots[a].where.x,
                    try_y + input_spots[a].where.y };
                LocSorted::const_iterator j
                    = reference_sorted.find(tmp);
                if(j != reference_sorted.end()
                && j->second == input_spots[a].label)
                    ++n_match;
            }
            /*fprintf(stderr, "For %d,%d, got %u\n",
                i->first.x,
                i->first.y,
                (unsigned) n_match);*/
            if(n_match > best_n)
            {
                best_n   = n_match;
                best_loc = i->first;
            }
        }
        
        fprintf(stderr, "Suppose best relative offset = %d,%d (best_n=%u, input_spots=%u, reference_spots=%u)\n",
            best_loc.x,
            best_loc.y,
            (unsigned) best_n,
            (unsigned) input_spots.size(),
            (unsigned) reference_spots.size() );
        
        AlignResult result;
        result.offs_x = best_loc.x + cur_x;
        result.offs_y = best_loc.y + cur_y;
        result.suspect_reset = best_n*3 < input_spots.size();
        return result;
    }
}

void
TILE_Tracker::PutScreenAutomatic(const uint32*const input, unsigned sx,unsigned sy)
{
    /* Find spots of interest within the reference image,
     * and within the input image.
     *
     * Select that offset which results in greatest overlap
     * between those two sets of spots.
     */
     
    std::vector<InterestingSpot> input_spots;
    std::vector<InterestingSpot> reference_spots;
    FindInterestingSpots(input_spots, input, 0,0, sx,sy, false);

    static std::map< std::pair<int,int>,
                     std::vector<InterestingSpot>
                   > cached_spots;

    /* For speed reasons, we don't use LoadScreen(), but
     * instead, work on cube-by-cube basis. This way, we
     * can also cache the list of interesting spots for
     * each cube.
     */
    for(ymaptype::iterator
        yi = screens.begin();
        yi != screens.end();
        ++yi)
    {
        const int y_screen_offset = yi->first * 256;
        
        for(xmaptype::iterator
            xi = yi->second.begin();
            xi != yi->second.end();
            ++xi)
        {
            const int x_screen_offset = xi->first  * 256;
            cubetype& cubedata  = xi->second;
            const std::vector<MostUsedPixel>& cube = cubedata.mostused;
            
            std::pair<int,int> cache_key(y_screen_offset, x_screen_offset);
            if(cubedata.changed)
            {
                cubedata.changed = false;

                const size_t old_n_spots = reference_spots.size();
                
                uint32 result[256*256];
                for(unsigned p=0; p<256*256; ++p)
                    result[p] = cube[p];

                FindInterestingSpots(reference_spots, result,
                    x_screen_offset,y_screen_offset,
                    256,256,
                    true);

                cached_spots[cache_key].assign
                    ( reference_spots.begin() + old_n_spots,
                      reference_spots.end() );
            }
            else
            {
                const std::vector<InterestingSpot>& cache
                    = cached_spots[cache_key];

                reference_spots.insert(
                    reference_spots.end(),
                    cache.begin(),
                    cache.end());
            }
        }
    }
    
    AlignResult res = Align(input_spots, reference_spots, org_x,org_y);
    
    FitScreen(input, sx,sy,
        res.offs_x - org_x,
        res.offs_y - org_y,
        res.suspect_reset);
}
