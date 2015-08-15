#include "align.hh"
#include "alloc/FSBAllocator.hh"

#include <map>
#include <set>
#include <cstdio>

#include "settype.hh"
#include "pixel.hh" // For pixelmethod
#include "openmp.hh"

unsigned x_divide_input = 1;
unsigned y_divide_input = 1;

unsigned x_divide_reference = 32;
unsigned y_divide_reference = 32;

bool always_align_with_canvas = false;

int mv_xmin = -9999;
int mv_ymin = -9999;
int mv_xmax = +9999;
int mv_ymax = +9999;

namespace
{
    static const unsigned RY = 8;  // 0.299*27 ~ 16/27 = 0.296{296...}
    static const unsigned GY = 16; // 0.587*27 ~ 32/27 = 0.592{592...}
    static const unsigned BY = 3;  // 0.114*27 ~  6/27 = 0.111{111...}
    static const unsigned RGB2YUV_MUL = RY+GY+BY;
    // Factor 27 chosen for minimal bit count (fastest to calculate).


    /* RelativeCoordinate is like IntCoordinate,
     * but in sorting, values closer to origo (0,0)
     * are processed before values farther from origo.
     */
    struct RelativeCoordinate
    {
        int x, y;

        RelativeCoordinate(int xx,int yy) : x(xx), y(yy) { }

        bool operator< (const RelativeCoordinate& b) const
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
}

static inline unsigned char GetLuma(uint32 pix)
{
    return (((pix&0xFF0000u)*RY/(1<<16))
          + ((pix&0x00FF00u)*GY/(1<<8))
          + ((pix&0x0000FFu)*BY/(1<<0))
          + RGB2YUV_MUL/2
           ) / RGB2YUV_MUL;
    // Get the luma in 0..255 range.
}

void FindInterestingSpots(
    std::vector<InterestingSpot>& output,
    const uint32* input,
    int xoffs, int yoffs,
    unsigned sx, unsigned sy,
    bool force_all_pixels)
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
     * What is actually done here is a mixture of these two plans.
     * For each cell in the cube (i.e. 8x8 for a 256x256 screen),
     * we save the one spot that occurs most rarely.
     */

    /* Convert the input image into a YUV image.
     * Actually save only Y value (luma/brightness); ignore U and V (chroma).
     * Also determine which pixels are transparent.
     */
    VecType<char> Transparent(sx*sy, false);
    VecType<unsigned char> Y(sx*sy);
    for(unsigned p=0, y=0; y<sy; ++y)
        for(unsigned x=0; x<sx; ++x, ++p)
        {
            if((input[p] & 0xFF000000u) == 0)
            {
                Y[p] = GetLuma(input[p]);
            }
            else
            {
                /* A single transparent pixel affects
                 * a 4x4 section extending left and up
                 * thereof, because Spots are calculated
                 * from 4x4 squares.
                 */
                for(unsigned yos=0, yo=0; yo<4 && yo<=y; ++yo, yos+=sx)
                    for(unsigned xo=0; xo<4 && xo<=x; ++xo)
                        Transparent[p - xo - yos] = true;
            }
        }

    const unsigned x_divide = force_all_pixels ? x_divide_input : x_divide_reference;
    const unsigned y_divide = force_all_pixels ? y_divide_input : y_divide_reference;
    const unsigned x_shrunk = (sx + x_divide-1) / x_divide;
    const unsigned y_shrunk = (sy + y_divide-1) / y_divide;

    output.reserve(output.size() + x_shrunk * y_shrunk);

    VecType<SpotType> spots(sx*sy);
    //unsigned randkey = 1;
    for(unsigned p=0, y=0; y+4<=sy; ++y, p+=3)
        for(unsigned x=0; x+4<=sx; ++x, ++p)
        {
            if(Transparent[p]) continue;
            /* A sufficiently unique code describing this pixel
             * should be made by comparing the brightness of this
             * pixel to its immediate surroundings, scaled by the
             * average.
             * That was the plan. The implementation here is simpler,
             * and simply takes a 4x4 pixel block of Y (brightness) values
             * and stores them verbatim into 16 bytes (two 8-byte integers).
             */
            uint32 pix[4] = { *(uint32*)&Y[p     ],
                              *(uint32*)&Y[p+sx  ],
                              *(uint32*)&Y[p+sx*2],
                              *(uint32*)&Y[p+sx*3] };
            SpotType data ( pix[0] | (uint64(pix[1]) << 32),
                            pix[2] | (uint64(pix[3]) << 32) );

            if(x_divide==1 && y_divide==1)
            {
                //randkey = randkey*0x8088405+1;
                //if((randkey % 3) == 0 || (x&3)==0)
                {
                    InterestingSpot spot { { int(xoffs+x), int(yoffs+y) }, data };
                    output.push_back(spot);
                }
            }
            else
            {
                spots[p] = data;
            }
        }

    if(x_divide==1 && y_divide==1)
        return;

    typedef std::map<SpotType, VecType<unsigned>,
        std::less<SpotType>,
        FSBAllocator<int> > RarityType;

    VecType<RarityType> Rarities( x_shrunk * y_shrunk );

    for(unsigned p=0, y=0; y+4<=sy; ++y, p+=3)
        for(unsigned x=0; x+4<=sx; ++x, ++p)
            Rarities[ (y/y_divide) * x_shrunk + (x/x_divide) ]
                [ spots[p] ].push_back(p);

    /* From each table, pick the least common PixelLabel,
     * and show the first coordinate for it */
    for(unsigned p=0, y=0; y<y_shrunk; ++y)
        for(unsigned x=0; x<x_shrunk; ++x, ++p)
        {
            /* Pick the SpotType that occurs the least number of times */
            const RarityType& r = Rarities[p];
            RarityType::const_iterator winner = r.begin();
            for(RarityType::const_iterator i = winner; ++i != r.end(); )
                if(i->second.size() < winner->second.size())
                    winner = i;

            const unsigned coordinate = winner->second.front();
            InterestingSpot spot
                { { int(xoffs+ coordinate%sx),
                    int(yoffs+ coordinate/sx) },
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
    // Using multiset<> rather than set<> here
    // because it has a simpler insert() method.
    typedef std::multiset<IntCoordinate,
        std::less<IntCoordinate>, FSBAllocator<int> > CoordSetType;
    typedef std::map<SpotType, CoordSetType,
        std::less<SpotType>, FSBAllocator<int> > SpotLocSetType;

    SpotLocSetType input_spot_locations;
    for(size_t b=input_spots.size(), a=0; a<b; ++a)
    {
        /*std::fprintf(stderr, "in[%3u/%3u]: %16llX,%16llX @ %d,%d\n",
            (unsigned)a, (unsigned)b,
            input_spots[a].data.first,
            input_spots[a].data.second,
            input_spots[a].where.x,
            input_spots[a].where.y);*/
        input_spot_locations[ input_spots[a].data ]
            .insert( input_spots[a].where );
    }

    SpotLocSetType reference_spot_locations;
    for(size_t b=reference_spots.size(), a=0; a<b; ++a)
    {
        /*std::fprintf(stderr, "ref[%3u]: %16llX,%16llX @ %d,%d\n",
            (unsigned)a,
            reference_spots[a].data.first,
            reference_spots[a].data.second,
            reference_spots[a].where.x,
            reference_spots[a].where.y);*/
        reference_spot_locations[ reference_spots[a].data ]
            .insert( reference_spots[a].where );
    }

    /* Find a set of possible offsets */
    typedef std::map<RelativeCoordinate, unsigned,
        std::less<RelativeCoordinate>,
        FSBAllocator<int> > OffsetSuggestions;
    OffsetSuggestions offset_suggestions;
    #ifdef _OPENMP
    MutexType offset_suggestions_lock;
    #endif

    for(int y=-4; y<=4; ++y)
        for(int x=-4; x<=4; ++x)
            offset_suggestions.insert
                (std::make_pair( RelativeCoordinate(x,y), 999 ));

    /* If a rarely occurring vista is found in the reference picture,
     * add the offset to the list of offsets to be tested
     */
  #pragma omp parallel
  {
   #pragma omp single
   {
    typedef
    std::multimap<size_t, SpotLocSetType::const_iterator,
                  std::less<size_t>, FSBAllocator<int> > InputSpotCounts;
    InputSpotCounts input_spot_counts;
    for(SpotLocSetType::const_iterator
        i = input_spot_locations.begin();
        i != input_spot_locations.end();
        ++i)
    {
        input_spot_counts.insert(
            std::pair<size_t, SpotLocSetType::const_iterator>
                ( i->second.size(), i ) );
    }
    size_t n_inputs = 0;
    for(InputSpotCounts::const_iterator
        itmp = input_spot_counts.begin();
        itmp != input_spot_counts.end();
        ++itmp)
    {
        SpotLocSetType::const_iterator i ( itmp->second );
        if(n_inputs < 50 || itmp->first <= 4)
        {
            ++n_inputs;
          #pragma omp task firstprivate(i)
          {
            const CoordSetType& coords = i->second;
            const SpotType& pixel = i->first;

            SpotLocSetType::const_iterator r
                = reference_spot_locations.find( pixel );
            if(r != reference_spot_locations.end())
            {
                const CoordSetType& rcoords = r->second;
                typedef CoordSetType::const_iterator it;

                for(it c=coords.begin(); c!=coords.end(); ++c)
                {
                    size_t rmax = 0;
                    for(it d=rcoords.begin(); d!=rcoords.end(); ++d)
                    {
                        int rx = (d->x - org_x) - (c->x);
                        int ry = (d->y - org_y) - (c->y);
                        if(rx < mv_xmin || rx > mv_xmax
                        || ry < mv_ymin || ry > mv_ymax
                        /*
                        || (rx&&ry)*/) continue;
                      {
                        #ifdef _OPENMP
                        ScopedLock lck(offset_suggestions_lock);
                        #endif
                        offset_suggestions[RelativeCoordinate(rx,ry)] += 1;
                      }
                        if(++rmax >= 80) break;
                    }
                }
            }
          } // omp task
        }
    }
   } // omp single
  } // omp parallel

    /* For each candidate offset that has sufficient confidence,
     * find out the one that has most overlap in spots to the reference
     */
    size_t             best_match = 0;
    RelativeCoordinate best_coord(0,0);
    #ifdef _OPENMP
    MutexType best_lock;
    #endif

  #pragma omp parallel
  {
   #pragma omp single
   {
    for(OffsetSuggestions::const_iterator
        i = offset_suggestions.begin();
        i != offset_suggestions.end();
        ++i)
    {
        //if(i->second < 8) continue; // Not confident enough
        if(i->first.x < mv_xmin
        || i->first.x > mv_xmax
        || i->first.y < mv_ymin
        || i->first.y > mv_ymax/*
        || (i->first.x && i->first.y)*/) continue; // Out of range

      #pragma omp task firstprivate(i) shared(best_match,best_coord)
      {
        const RelativeCoordinate& relcoord = i->first;

        size_t n_match = 0;

        for(SpotLocSetType::const_iterator
            j = input_spot_locations.begin();
            j != input_spot_locations.end();
            ++j)
        {
            const SpotType& data = j->first;

            SpotLocSetType::const_iterator
                r = reference_spot_locations.find(data);
            if(r == reference_spot_locations.end())
                continue;

            const CoordSetType& coords = j->second;
            const CoordSetType& rcoords = r->second;
            typedef CoordSetType::const_iterator it;

            for(it k = coords.begin(); k != coords.end(); ++k)
            {
                const IntCoordinate& coord = *k;
                IntCoordinate test_coord =
                    {
                        coord.x + org_x + relcoord.x,
                        coord.y + org_y + relcoord.y
                    };

                it l = rcoords.find(test_coord);
                if(l != rcoords.end())
                {
                    ++n_match;
                }
            }
        }

        /*std::fprintf(stderr, "Suggestion %d,%d (%u): %u\n",
            i->first.x, i->first.y, i->second,
            (unsigned) n_match);*/
        #ifdef _OPENMP
        ScopedLock lck(best_lock);
        #endif
        if(n_match > best_match)
        {
            best_match = n_match,
            best_coord = i->first;
        }
      } // omp task
    }
   } // omp single
  } // omp parallel

    /*std::fprintf(stderr, "Choice: %d,%d: %u\n",
        best_coord.x, best_coord.y,
        (unsigned) best_match);*/

    AlignResult result;
    result.suspect_reset = best_coord.length() > (16+16)
                 //      && (best_match < input_spots.size()/16)
                 //      && (pixelmethod != pm_LoopingLogPixel)
                         && false;
    result.offs_x        = best_coord.x;
    result.offs_y        = best_coord.y;
    return result;
}

AlignResult Align(
    const uint32* background,
    unsigned backwidth, unsigned backheight,
    const uint32* input,
    unsigned inputwidth, unsigned inputheight,
    int org_x,
    int org_y)
{
    /* TODO: Figure out where,
     * within background[], does input[] overlap.
     * Presume that somewhere near org_x,org_y
     * is most likely.
     * Preferrably, work also with tint/brightness changes...
     */

    if(!backwidth || !backheight)
    {
        AlignResult result;
        result.suspect_reset = false;
        result.offs_x        = org_x;
        result.offs_y        = org_y;
        return result;
    }

    /* Find a set of possible offsets */
    typedef VecType<std::pair<RelativeCoordinate, unsigned> > OffsetSuggestions;
    OffsetSuggestions offset_suggestions;
    #ifdef _OPENMP
    MutexType offset_suggestions_lock;
    #endif

    for(int y=-37; y<=37; ++y)
    for(int x=-37; x<=37; ++x)
        offset_suggestions.push_back
            (std::make_pair( RelativeCoordinate(x-org_x, y-org_y), 0 ));

    VecType<IntCoordinate> rand_spots;
    const unsigned x_divide = x_divide_reference;
    const unsigned y_divide = y_divide_reference;
    const unsigned n_rand_spots_per = ((x_divide*y_divide+59) / 60);
    const unsigned x_shrunk = (inputwidth  + x_divide-1) / x_divide;
    const unsigned y_shrunk = (inputheight + y_divide-1) / y_divide;

    for(unsigned y=0; y<y_shrunk; ++y)
        for(unsigned x=0; x<x_shrunk; ++x)
        {
            const unsigned sx = x*x_divide, sy = y*y_divide;
            unsigned cw = x_divide; if(sx+cw > inputwidth ) cw = inputwidth-sx;
            unsigned ch = y_divide; if(sy+ch > inputheight) ch = inputheight-sy;
            for(unsigned n=0; n<n_rand_spots_per; ++n)
            {
                IntCoordinate c { int(sx + (rand()%cw)), int(sy + (rand()%ch)) };
                rand_spots.push_back(c);
            }
        }
    /*fprintf(stderr, "org=%d,%d, nspots=%u, noffs=%lu (%d..%d, %d..%d), back=%u,%u, in=%u,%u\n",
        org_x,org_y,
        n_rand_spots, offset_suggestions.size(),
        -(int)inputheight, backheight,
        -(int)inputwidth, backwidth,
        backwidth,backheight,
        inputwidth,inputheight);*/

    unsigned best=0, worst=0;

    /*VecType<unsigned char> InputLuma( inputwidth * inputheight );
    #pragma omp parallel for schedule(static)
    for(unsigned a=0; a<InputLuma.size(); ++a)
        InputLuma[a] = GetLuma( input[a] );*/

    #pragma omp parallel for schedule(guided)
    for(unsigned a=0; a<offset_suggestions.size(); ++a)
    {
        OffsetSuggestions::iterator i = offset_suggestions.begin() + a;

        int rx = i->first.x;
        int ry = i->first.y;
        if(rx < mv_xmin || rx > mv_xmax
        || ry < mv_ymin || ry > mv_ymax
        /*
        || (rx&&ry)*/) continue;

        unsigned n_match = 0;
        for(unsigned b=0; b<rand_spots.size(); ++b)
        {
            const int ix = rand_spots[b].x;
            const int iy = rand_spots[b].y;
            const int bx = (rand_spots[b].x + rx + org_x);
            const int by = (rand_spots[b].y + ry + org_y);
            if(bx < 0 || bx >= (int)backwidth
            || by < 0 || by >= (int)backheight)
            {
                if(bx >= -32 && by >= -32
                && bx <= (int)(backwidth+32)
                && by <= (int)(backheight+32))
                    ++n_match;
                continue;
            }
            const uint32* inptr = &input[iy*inputwidth + ix];
            const uint32* bgptr = &background[by*backwidth + bx];
            if( (*inptr & 0xFF000000u) )
            {
                continue;
            }
            if( (*bgptr & 0xFF000000u) )
            {
                ++n_match;
                continue;
            }

            if(*inptr == *bgptr)
            {
                ++n_match;
            }
            /*else
            {
                // Check if they still match when comparing their average
                // relative brightnesses to their surroundings
                unsigned in_luma = InputLuma[inptr-input];
                unsigned bg_luma = GetLuma(*bgptr);
                unsigned in_luma_sum = 0, in_luma_count = 0;
                unsigned bg_luma_sum = 0, bg_luma_count = 0;
                for(int offsx=-1; offsx<=1; ++offsx)
                for(int offsy=-1; offsy<=1; ++offsy)
                {
                    if(bx+offsx >= 0 && bx+offsy < (int)backwidth
                    && by+offsy >= 0 && by+offsy < (int)backheight)
                        { bg_luma_sum += GetLuma(bgptr[offsy*(int)backwidth+offsx]);
                          ++bg_luma_count; }
                    if(ix+offsx >= 0 && ix+offsy < (int)inputwidth
                    && iy+offsy >= 0 && iy+offsy < (int)inputheight)
                        { bg_luma_sum += InputLuma[ &inptr[offsy*(int)inputwidth+offsx] - input ];
                          ++bg_luma_count; }
                }
                double in_luma_ratio = in_luma / (in_luma_sum / (double)in_luma_count);
                double bg_luma_ratio = bg_luma / (bg_luma_sum / (double)bg_luma_count);
                if(in_luma_ratio == bg_luma_ratio)
                    ++n_match;
            }*/
        }
        #ifdef _OPENMP
        ScopedLock lck(offset_suggestions_lock);
        #endif
        i->second += n_match;
        if(i->second > offset_suggestions[best].second)
            best = a;
        if(i->second < offset_suggestions[worst].second)
            worst = a;
    }

    const unsigned maxval = x_shrunk*y_shrunk*n_rand_spots_per;
    const unsigned bestval = offset_suggestions[best].second;
    const unsigned worstval =offset_suggestions[worst].second;
    const unsigned graphwidth = 10;

    const RelativeCoordinate& best_coord = offset_suggestions[best].first;

    AlignResult result;
    result.suspect_reset = bestval < (maxval * 0.80);
    result.offs_x        = best_coord.x + org_x;
    result.offs_y        = best_coord.y + org_y;

    /* >= 94% = very reliable
     * >= 80% = parallax motion, but still relatively reliable
     */
    if(verbose >= 3)
    {
        std::fprintf(stderr,
            "---------------------------\n"
            "Max: %u, Best: %u (%.1f%%%s), Worst: %u(%.1f%%), estimate %d,%d\n",
            maxval,
            bestval,  bestval*100.0/maxval,
            result.suspect_reset ? "" : " [OK]",
            worstval, worstval*100.0/maxval,
            result.offs_x, result.offs_y
        );
        int power[graphwidth];
        unsigned maxpower=0;
        for(unsigned x=0; x<graphwidth; ++x)
        {
            unsigned prevlimit = worstval + (bestval-worstval+1)*(x  )/graphwidth;
            unsigned limit     = worstval + (bestval-worstval+1)*(x+1)/graphwidth;
            unsigned n = 0;
            for(unsigned a=0; a<offset_suggestions.size(); ++a)
                if(offset_suggestions[a].second >= prevlimit
                && offset_suggestions[a].second <  limit)
                    ++n;
            power[x] = n;
            if(n > maxpower) maxpower = n;
        }
        for(int y=8; y>=0; --y)
        {
            int threshold = (int)((y)*maxpower/8);
            for(unsigned x=0; x<graphwidth; ++x)
                std::fputc((power[x]>threshold) ? '#' : '.', stderr);
            std::fprintf(stderr, "\n");
        }
        /**/
        int prev=-8;
        for(unsigned a=0; a<offset_suggestions.size(); ++a)
        {
            if(offset_suggestions[a].first.y != prev)
            {
                std::fprintf(stderr, "\n");
                prev = offset_suggestions[a].first.y;
            }
            if(best == a)
                std::fprintf(stderr, "[%4u]", offset_suggestions[a].second);
            else
                std::fprintf(stderr, "%5u ", offset_suggestions[a].second);
        }/**/
        std::fprintf(stderr, "\n");
    }

    if(!bestval)
    {
        result.suspect_reset = false;
        result.offs_x        = org_x;
        result.offs_y        = org_y;
        return result;
    }
    return result;
}
