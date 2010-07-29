#include "align.hh"
#include "alloc/FSBAllocator.hh"

#include <map>
#include <set>

unsigned x_divide_input = 1;
unsigned y_divide_input = 1;

unsigned x_divide_reference = 32;
unsigned y_divide_reference = 32;

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
     */

    /* Convert the input image into a YUV image.
     * Actually save only Y value (luma/brightness); ignore U and V (chroma).
     * Also determine which pixels are transparent.
     */
    std::vector<char> Transparent(sx*sy, false);
    std::vector<unsigned char> Y(sx*sy);
    for(unsigned p=0, y=0; y<sy; ++y)
        for(unsigned x=0; x<sx; ++x, ++p)
        {
            if((input[p] & 0xFF000000u) == 0)
            {
                Y[p] = ((((input[p] >> 16) & 0xFF) * RY
                      +  ((input[p] >> 8)  & 0xFF) * GY
                      +  ((input[p]     )  & 0xFF) * BY
                        ) >> RGB2YUV_SHIFT) + Y_ADD;
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

    std::vector<SpotType> spots(sx*sy);
    //unsigned randkey = 1;
    for(unsigned p=0, y=0; y+4<=sy; ++y, p+=3)
        for(unsigned x=0; x+4<=sx; ++x, ++p)
        {
            if(Transparent[p]) continue;
            /* A sufficiently unique code describing this pixel
             * should be made by comparing the brightness of this
             * pixel to its immediate surroundings, scaled by the
             * average
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
                    InterestingSpot spot = { { xoffs+x, yoffs+y }, data };
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

    const unsigned x_shrunk = (sx + x_divide-1) / x_divide;
    const unsigned y_shrunk = (sy + y_divide-1) / y_divide;

    typedef std::map<SpotType, std::vector<unsigned>,
        std::less<SpotType>,
        FSBAllocator<int> > RarityType;

    std::vector<RarityType> Rarities( x_shrunk * y_shrunk );

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
    // Using multiset<> rather than set<> here
    // because it has a simpler insert() method.
    typedef std::multiset<IntCoordinate,
        std::less<IntCoordinate>, FSBAllocator<int> > CoordSetType;
    typedef std::map<SpotType, CoordSetType,
        std::less<SpotType>, FSBAllocator<int> > SpotLocSetType;

    SpotLocSetType input_spot_locations;
    for(size_t b=input_spots.size(), a=0; a<b; ++a)
    {
        /*std::fprintf(stderr, "in[%3u]: %16llX,%16llX @ %d,%d\n",
            (unsigned)a,
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
        const CoordSetType& coords = i->second;

        if(coords.size() > 15) continue;

        const SpotType& pixel = i->first;

        SpotLocSetType::const_iterator r
            = reference_spot_locations.find( pixel );
        if(r == reference_spot_locations.end()) continue;

        const CoordSetType& rcoords = r->second;
        typedef CoordSetType::const_iterator it;

        for(it c=coords.begin(); c!=coords.end(); ++c)
        {
            size_t rmax = 0;
            for(it d=rcoords.begin(); d!=rcoords.end(); ++d)
            {
                offset_suggestions[
                    RelativeCoordinate(
                        (d->x - org_x) - (c->x),
                        (d->y - org_y) - (c->y)
                                  )] += 1;
                if(++rmax >= 20) break;
            }
        }
    }

    /* For each candidate offset that has sufficient confidence,
     * find out the one that has most overlap in spots to the reference
     */
    size_t             best_match = 0;
    RelativeCoordinate best_coord(0,0);

    for(OffsetSuggestions::const_iterator
        i = offset_suggestions.begin();
        i != offset_suggestions.end();
        ++i)
    {
        if(i->second < 8) continue; // Not confident enough

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

        if(n_match > best_match)
        {
            best_match = n_match,
            best_coord = i->first;
        }
    }

    /*std::fprintf(stderr, "Choice: %d,%d: %u\n",
        best_coord.x, best_coord.y,
        (unsigned) best_match);*/

    AlignResult result;
    result.suspect_reset = best_coord.length() > (16+16)
                 //      && (best_match < input_spots.size()/16)
                       && ! UncertainPixel::is_loopinglog();
    result.offs_x        = best_coord.x;
    result.offs_y        = best_coord.y;
    return result;
}
