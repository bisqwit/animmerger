#include <cstdio>
#include <algorithm>
#include <cmath>
#include <stdarg.h>

#include <assert.h>
#include <gd.h>

#include "palette.hh"
#include "pixel.hh"
#include "pixels/averagepixel.hh"

std::vector<PaletteMethodItem> PaletteReductionMethod;

double DitherErrorFactor    = 1.0;
unsigned DitherMatrixWidth  = 8;
unsigned DitherMatrixHeight = 8;
unsigned TemporalDitherSize = 1; // 1 = no temporal dithering
bool     TemporalDitherMSB  = false;  // Use MSB rather than LSB for temporal dithering
unsigned DitherColorListSize = 0;
double   DitherCombinationContrast = -1.0;
ColorCompareMethod UseCIE = Compare_RGB;

namespace
{
    // A histogram item contains a pixel value and an occurrence count.
    typedef std::pair<uint32, unsigned> HistItem;

    void GetHSV(int r,int g,int b, double&h, double&s, double&v)
    {
        int Max = std::max(std::max(r,g), b);
        int Min = std::min(std::min(r,g), b);
        double Delta = Max-Min;
        v = (Max-Min)/(255.0);
        if(Max != 0.0)
            s = Delta / Max;
        else
            s = 0.0;
        if(s == 0.0)
            h = 0.0;
        else
        {
            /**/ if(r == Max) h = (g - b) / Delta;
            else if(g == Max) h=2+(b - r) / Delta;
            else if(b == Max) h=4+(r - g) / Delta;
            h /= 6.0; if(h < 0) ++h;
        }
    }
    void GetSpatialColor(int r,int g,int b, double&x, double&y, double&z)
    {
        double h,s,v;
        GetHSV(r,g,b, h,s,v);
        double angle = h * (3.141592653*2/6.0);
        const double m = 1.0/(255);
        double luma = (r*0.299*m + g*0.587*m + b*0.114*m);
        x = s * std::sin(angle);
        y = s * std::cos(angle);
        z = (v+luma)*0.5;
    }
    unsigned ColorDiff(int r1,int g1,int b1, int r2,int g2,int b2)
    {
        /*
        double x1,y1,z1, x2,y2,z2;
        GetSpatialColor(r1,g1,b1, x1,y1,z1);
        GetSpatialColor(r2,g2,b2, x2,y2,z2);
        double xdiff = x1-x2, ydiff=y1-y2, zdiff=z1-z2;
        return (xdiff*xdiff + ydiff*ydiff + zdiff*zdiff) * (3121200/3.0);
        */
        int luma1 = (r1*299 + g1*587 + b1*114);
        int luma2 = (r2*299 + g2*587 + b2*114);
        int lumadiff = (luma1-luma2)/1000;
        int avgluma = (luma1+luma2)/5000;
        int rdiff = r1-r2, gdiff = g1-g2, bdiff = b1-b2;
        return ((rdiff*rdiff + gdiff*gdiff + bdiff*bdiff)*avgluma)/4 + lumadiff*lumadiff;
    }

    unsigned ColorDiff(int r1,int g1,int b1, uint32 pix2)
    {
        int r2 = (pix2 >> 16) & 0xFF, g2 = (pix2 >> 8) & 0xFF, b2 = (pix2) & 0xFF;
        return ColorDiff(r1,g1,b1, r2,g2,b2);
    }
    unsigned ColorDiff(uint32 pix1, uint32 pix2)
    {
        int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;
        return ColorDiff(r1,g1,b1, pix2);
    }

    static bool CompareSecondRev(const HistItem& a, const HistItem& b)
    {
        return a.second > b.second;
    }
    inline bool CompareSecond(const HistItem& a, const HistItem& b)
    {
        return a.second < b.second;
    }
    inline bool CompareRed(const HistItem& a, const HistItem& b)
    {
        int r1 = (a.first >> 16) & 0xFF;
        int r2 = (b.first >> 16) & 0xFF;
        return r1 < r2;
    }
    inline bool CompareGreen(const HistItem& a, const HistItem& b)
    {
        int g1 = (a.first >> 8) & 0xFF;
        int g2 = (b.first >> 8) & 0xFF;
        return g1 < g2;
    }
    inline bool CompareBlue(const HistItem& a, const HistItem& b)
    {
        int b1 = (a.first) & 0xFF;
        int b2 = (b.first) & 0xFF;
        return b1 < b2;
    }
    bool CompareLuma(uint32 pix1, uint32 pix2)
    {
        int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;
        int r2 = (pix2 >> 16) & 0xFF, g2 = (pix2 >> 8) & 0xFF, b2 = (pix2) & 0xFF;
        const int luma1 = r1*/*0.*/299 + g1*/*0.*/587 + b1*/*0.*/114;
        const int luma2 = r2*/*0.*/299 + g2*/*0.*/587 + b2*/*0.*/114;
        return luma1 < luma2;
    }

    struct DiversityInfo
    {
        // Smallest distance from all so-far chosen colors
        unsigned min_dist;
        // Which color index (of so-far chosen colors) is most similar to this
        unsigned closest;
        // Color and use count
        unsigned count;
        uint32   color;

        DiversityInfo(const HistItem& i)
            : min_dist(~0u), closest(~0u),
              count(i.second), color(i.first) { }

        // Comparator for sorting by popularity
        bool operator< (const DiversityInfo& b) const
        {
            return count > b.count;
        }
    };

    // Code written based on implementation in GifSicle
    void ReduceHistogram_Diversity
        (HistogramType& Histogram, unsigned target_colors,
         bool blend=false)
    {
        const unsigned colors_originally = Histogram.size();
        if(colors_originally <= target_colors) return;

        /* 1. Initialize lore and sort the colors in order of popularity. */
        std::vector<DiversityInfo> Lore;
        Lore.reserve( colors_originally );
        for(HistogramType::iterator i = Histogram.begin(); i != Histogram.end(); ++i)
            Lore.push_back( DiversityInfo(*i) );
        std::sort(Lore.begin(), Lore.end());

        Histogram.clear();
        /* 2. Choose colors one at a time */
        unsigned nadapt;
        for(nadapt=0; nadapt<target_colors; ++nadapt)
        {
            /* 2.1. Choose the color to be added */
            unsigned chosen;
            if(nadapt == 0 || (nadapt >= 10 && nadapt % 2 == 0))
            {
                /* 2.1a. Choose the most popular remaining color */
                for(chosen=0; chosen < colors_originally; ++chosen)
                    if(Lore[chosen].min_dist != 0)
                        break;
            }
            else
            {
                /* 2.1b. Choose the color that is most different
                 *       to any color chosen so far */
                unsigned chosen_dist = 0;
                chosen = colors_originally;
                for(unsigned i=0; i<colors_originally; ++i)
                    if(Lore[i].min_dist > chosen_dist)
                        chosen_dist = Lore[chosen = i].min_dist;
            }
            if(chosen >= colors_originally) break;
            /* 2.2. Add the color */
            Lore[chosen].min_dist = 0;
            Lore[chosen].closest  = nadapt; // Closest to it is itself.
            uint32 chosencolor = Lore[chosen].color;
            /* 2.3. Adjust the distances for remaining colors */
            for(unsigned i=0; i<colors_originally; ++i)
                if(Lore[i].min_dist != 0)
                {
                    // If the distance to the newest addition is shorter
                    // than the previous one, mark it as the closest one.
                    unsigned dist = ColorDiff(chosencolor, Lore[i].color);
                    if(dist < Lore[i].min_dist)
                    {
                        Lore[i].min_dist = dist;
                        Lore[i].closest  = nadapt;
                    }
                }
        }
        /* 3. Make the new palette by choosing one color from each slot. */
        if(!blend)
        {
            for(unsigned i=0; i<nadapt; ++i)
            {
                HistItem chosen(DefaultPixel, 0);
                for(unsigned j=0; j<Lore.size(); ++j)
                    if(Lore[j].closest == i)
                    {
                        chosen.second += Lore[j].count;
                        if(Lore[j].min_dist == 0)
                            chosen.first = Lore[j].color;
                    }
                Histogram.insert(chosen);
            }
        }
        else
        {
            // Unblended palette has already been generated.
            // If we enabled blending, the palette is generated
            // a bit differently: For each selected palette entry,
            // all palette entries that were _not_ chosen but were
            // most similar to this one are blended together.
            for(unsigned i=0; i<nadapt; ++i)
            {
                AveragePixel total;
                HistItem chosen;
                unsigned count_total=0, mismatch_count_total=0;
                // For each original palette color
                // that most resembles this one...
                for(unsigned j=0; j<Lore.size(); ++j)
                    if(Lore[j].closest == i)
                    {
                        total.set_n( Lore[j].color, Lore[j].count );
                        count_total += Lore[j].count;
                        if(Lore[j].min_dist == 0)
                        {
                            chosen.first = Lore[j].color;
                            chosen.second = Lore[j].count;
                        }
                        else
                            mismatch_count_total += Lore[j].count;
                    }
                /* Only blend if total number of mismatched pixels
                 * exceeds total number of matched pixels by a large margin. */
                if(3*mismatch_count_total > 2*count_total)
                {
                    // Favor, by a small margin, the color the plain
                    // diversity algorithm would pick.
                    total.set_n( chosen.first, chosen.second * 2 );
                    chosen.first = total.get();
                }
                chosen.second = count_total;
                // By chance, it might be possible that blending produces
                // colors that are identical to other non-blended colors.
                // To not lose color-use counts, use []+= rather than insert.
                Histogram[chosen.first] += chosen.second;
                //Histogram.insert(chosen);
            }
        }
        std::fprintf(stderr, "%s-reduced to %u colors (aimed for %u)\n",
            blend ? "Blend-diversity" : "Diversity",
            (unsigned) Histogram.size(),
            nadapt);
    }

    void ReduceHistogram_BlendDiversity
        (HistogramType& Histogram, unsigned target_colors)
    {
        ReduceHistogram_Diversity(Histogram, target_colors, true);
    }

    struct AdaptiveSlot
    {
        std::vector<HistItem> items;
        unsigned pixels;

        AdaptiveSlot(): items(), pixels(0) { }

        template<typename T>
        void Sort(T func)
        {
            std::sort(items.begin(), items.end(), func);
        }
    };

    // Code written based on implementation in GifSicle
    void ReduceHistogram_MedianCut
        (HistogramType& Histogram, unsigned target_colors)
    {
        const unsigned colors_originally = Histogram.size();
        if(colors_originally <= target_colors) return;

        std::vector<AdaptiveSlot> slots( target_colors );

        /* 1. Set up the first slot, containing all pixels */
        slots[0].items.reserve(colors_originally);
        for(HistogramType::iterator
            i = Histogram.begin();
            i != Histogram.end(); )
        {
            HistogramType::iterator j(i); ++i;
            slots[0].items.push_back(*j);
            slots[0].pixels += j->second;
            Histogram.erase(j);
        }

        /* 2. Split slots until we have enough */
        unsigned nadapt;
        for(nadapt=1; nadapt<target_colors; ++nadapt)
        {
            /* 2.1. Pick the slot with largest population, to split it. */
            unsigned splitpos=0, largestsize=0;
            for(unsigned i=0; i<nadapt; ++i)
                if(slots[i].items.size() >= 2
                && slots[i].pixels > largestsize)
                    largestsize = slots[splitpos = i].pixels;

            if(!largestsize) break;
            AdaptiveSlot& slot = slots[splitpos];

            /* 2.2. Find its extent. */
            int minr=255,ming=255,minb=255, maxr=0,maxg=0,maxb=0;
            for(size_t b=slot.items.size(), a=0; a<b; ++a)
            {
                const uint32 pix = slot.items[a].first;
                int r=(pix>>16)&0xFF, g=(pix>>8)&0xFF, b=pix&0xFF;
                if(r<minr) minr=r; if(r>maxr) maxr=r;
                if(g<ming) ming=g; if(g>maxg) maxg=g;
                if(b<minb) minb=b; if(b>maxb) maxb=b;
            }
            /* 2.3. Sort colors according to the channel that
             * contains the largest distribution of luminance */
            int rdiff = /*0.*/299 * (maxr-minr);
            int gdiff = /*0.*/587 * (maxg-ming);
            int bdiff = /*0.*/114 * (maxb-minb);
            slot.Sort(
                (rdiff >= gdiff && rdiff >= bdiff) ? CompareRed
              : (gdiff >= bdiff) ? CompareGreen
              : CompareBlue);
            /* 2.4 Decide where to split the slot and split it there. */
            /*     Split so that both sides have roughly the same number of pixels. */
            AdaptiveSlot& nextslot = slots[nadapt];
            for(;;)
            {
                const HistItem& p = slot.items.back();
                unsigned cur_transaction = p.second;

                bool finished_splitting = false;
                if( nextslot.pixels+cur_transaction*2 >= slot.pixels)
                {
                    // This transaction would change the balances.
                    // Only do it unless it results in a more crooked
                    // balance than was before it.
                    unsigned balance_before = slot.pixels - nextslot.pixels;
                    unsigned balance_after  =
                        nextslot.pixels+cur_transaction*2 - slot.pixels;
                    if(nextslot.pixels
                    && balance_after >= balance_before) break;
                    finished_splitting = true;
                }

                nextslot.items.push_back(p);
                nextslot.pixels += cur_transaction;
                slot.items.pop_back();
                slot.pixels -= cur_transaction;

                if(finished_splitting) break;
            }
        }
        /* 3. Make a new palette by choosing one color from each slot. */
        for(unsigned i=0; i<nadapt; ++i)
        {
            AveragePixel pix;
            const AdaptiveSlot& slot = slots[i];
            for(size_t b = slot.items.size(), a=0; a<b; ++a)
                pix.set_n( slot.items[a].first, slot.items[a].second );
            Histogram[ pix.get() ] += slot.pixels;
        }

        std::fprintf(stderr, "Median-cut-reduced to %u colors (aimed for %u)\n",
            (unsigned) Histogram.size(),
            nadapt);
    }

    // Original implementation.
    // Note: Complexity is O(n^2) so only use on tiny palettes.
    void ReduceHistogram_Merging
        (HistogramType& Histogram, unsigned target_colors)
    {
        while(Histogram.size() > target_colors)
        {
            std::fprintf(stderr, "%u colors, %u wanted\n",
                (unsigned) Histogram.size(),
                target_colors);

            // Find two colors that are nearly identical, and merge them
            std::pair<HistogramType::iterator, HistogramType::iterator> best_pair;
            unsigned best_difference = 0;

            for(HistogramType::iterator
                i = Histogram.begin();
                i != Histogram.end();
                ++i)
            {
                uint32 pix1 = i->first;
                int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;

                for(HistogramType::iterator j = i;
                    ++j != Histogram.end(); )
                {
                    uint32 pix2 = j->first;
                    unsigned diff = ColorDiff(r1,g1,b1, pix2);
                    if(diff < best_difference || best_difference == 0)
                    {
                        best_pair.first  = i;
                        best_pair.second = j;
                        best_difference = diff;
                    }
                }
            }
            HistogramType::iterator i = best_pair.first;
            HistogramType::iterator j = best_pair.second;
            unsigned combined_count = i->second + j->second;
            AveragePixel pix;
            pix.set_n(i->first, i->second);
            pix.set_n(j->first, j->second);
            Histogram.erase(i);
            Histogram.erase(j);
            Histogram[ pix.get() ] += combined_count;
        }

        std::fprintf(stderr, "Merging-reduced to %u colors (aimed for %u)\n",
            (unsigned) Histogram.size(), target_colors);
    }

    struct Inserter
    {
        HistogramType& Histogram;
        Inserter(HistogramType& h) : Histogram(h) { }
        void operator() (uint32 pix)
        {
            Histogram[pix] += 1;
        }
    };

    namespace Octree
    {
        class node
        {
            static const unsigned TREE_DEPTH = 5;
        public:
            unsigned level, ref_count;
            AveragePixel result;
            node *child[8], *parent;

            node(uint32 pix, node *p)
                : level(p ? p->level+1 : 0), ref_count(1), parent(p)
            {
                result.set(pix);
                for(unsigned i=0; i<8; ++i) child[i] = 0;
            }
            node() : level(0), ref_count(0), parent(0)
            {
                for(unsigned i=0; i<8; ++i) child[i] = 0;
            }
            ~node()
            {
                reduce();
                if (parent) parent->reset_child(this);
            }

            void reset_child(node *c)
            {
                for (unsigned i=0; i<8;++i)
                    if (child[i] == c) { child[i] = 0; break; }
            }

            void add_color(uint32 pix)
            {
                result.set(pix);
                ++ref_count;

                if (level < TREE_DEPTH)
                {
                    int mask = 1 << (7-level);
                    size_t ndx=0;
                    unsigned red   = (pix >> 16) & 0xFF;
                    unsigned green = (pix >>  8) & 0xFF;
                    unsigned blue  = pix & 0xFF;
                    if((red   & mask)>0) ndx=4;
                    if((green & mask)>0) ndx|=2;
                    if((blue  & mask)>0) ndx|=1;
                    if (!child[ndx])
                        child[ndx] = new node(pix, this);
                    else
                        child[ndx]->add_color(pix);
                }
            }
            void reduce()          //converts this node to be a leaf.
            {
                for (unsigned i=0; i<8; ++i) delete child[i];
            }
            void reduce_one_leaf() //makes this node to have one leaf less.
            {
                if (is_leaf()) { delete this; return; }
                node *n = 0;
                for (unsigned i=0; i<8; ++i)
                    if (!n)
                        { if (child[i]) n = child[i]; }
                    else if (child[i]
                          && child[i]->ref_count < n->ref_count)
                                n = child[i];
                if (num_leafs() > 1)
                    n->reduce_one_leaf(); // n must always point to a node
                else
                    delete this;
            }

            template<typename Func>
            void assign_palette_entries(Func func)
            {
                if (is_leaf()) { func( result.get() ); return; }
                for (unsigned i=0; i<8; ++i)
                    if (child[i])
                        child[i]->assign_palette_entries( func );
            }
            bool is_leaf() const
            {
                for (unsigned i=0; i<8; ++i) if (child[i]) return false;
                return true;
            }
            unsigned num_leafs() const
            {
                if (is_leaf()) return 1;
                unsigned n=0;
                for (unsigned i=0; i<8; ++i)
                    if (child[i])
                        n += child[i]->num_leafs();
                return n;
            }
        };

        class reducer: public node
        {
        public:
            template<typename Func>
            void reduce(Func func, unsigned num_colors)
            {
                unsigned num_leafs = node::num_leafs();
                while (num_leafs > num_colors)
                {
                    node::reduce_one_leaf();
                    --num_leafs;
                }
                node::assign_palette_entries(func);
            }
        };
    }

    void ReduceHistogram_Octree
        (HistogramType& Histogram, unsigned target_colors)
    {
        Octree::reducer reducer;
        for(HistogramType::iterator i = Histogram.begin(); i != Histogram.end(); ++i)
            reducer.add_color( i->first );
        Histogram.clear();
        reducer.reduce( Inserter(Histogram), target_colors);

        std::fprintf(stderr, "Octree-reduced to %u colors (aimed for %u)\n",
            (unsigned) Histogram.size(), target_colors);
    }

    /* NeuQuant algorithm as implemented by Anthony Dekker in 1994.
     * Minimal steps have been taken to improve the sanity of the
     * code in this C++ port. Mandatory copyright blob below.
     * The inxbuild() and inxsearch() functions were removed as redundant.
     *-----------
     * Copyright (c) 1994 Anthony Dekker
     *
     * NEUQUANT Neural-Net quantization algorithm by Anthony Dekker, 1994.
     * See "Kohonen neural networks for optimal colour quantization"
     * in "Network: Computation in Neural Systems" Vol. 5 (1994) pp 351-367.
     * for a discussion of the algorithm.
     * See also  http://members.ozemail.com.au/~dekker/NEUQUANT.HTML
     *
     * Any party obtaining a copy of these files from the author, directly or
     * indirectly, is granted, free of charge, a full and unrestricted irrevocable,
     * world-wide, paid up, royalty-free, nonexclusive right and license to deal
     * in this software and documentation files (the "Software"), including without
     * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
     * and/or sell copies of the Software, and to permit persons who receive
     * copies from any such party to do so, with the only requirement being
     * that this copyright notice remain intact.
     */
    class NeuQuant
    {
        int netsize; // Number of colors
        struct pixel { double c[3]; };
        std::vector<pixel> network;  /* the network itself */
        std::vector<double> bias;
        std::vector<double> freq;     /* bias and freq arrays for learning */
    public:
        NeuQuant(unsigned ncolors) : netsize(ncolors),
            network(netsize),
            bias(netsize, 0.0),
            freq(netsize, 1.0/netsize)
        {
            for(int i=0; i<netsize; ++i)
            {
                pixel& pix = network[i];
                double grayscale = i*256.0/netsize;
                for(int n=0; n<3; ++n) pix.c[n] = grayscale;
            }
        }

        template<typename Func>
        void Retrieve(Func func) const
        {
            for(int j=0; j<netsize; ++j)
            {
                const pixel& pix = network[j];
                int c0 = pix.c[0]+0.5; if(c0 > 255) c0 = 255;
                int c1 = pix.c[1]+0.5; if(c1 > 255) c1 = 255;
                int c2 = pix.c[2]+0.5; if(c2 > 255) c2 = 255;
                func( (c0<<16) + (c1<<8) + c2 );
            }
        }

        /* Main Learning Loop */
        void Learn(const uint32* all_pixels, unsigned lengthcount)
        {
            // Note: "samplefac" hardcoded as 1 here. We give all pixels a fair chance...
            const unsigned ncycles = 100; /* no. of learning cycles */
            unsigned samplepixels = lengthcount;
            unsigned delta        = samplepixels/ncycles; // Interval to achieve ncycles
            double alpha  = 1.0;
            double radius = netsize / 8.0; // For 256 colors, radius starts at 32
            /* Alpha: How strongly the value is imprinted into affected neurons
             * Radius: How many neurons, around the chosen victim, are affected too
             */

            int rad = radius;

            std::fprintf(stderr,"beginning 1D learning: initial radius=%d\n", rad);

            /* Provide random pixels from the input image
             * until approximately all pixels have been covered
             */
            for(unsigned i=0; i < samplepixels; )
            {
                double c[3] =
                    { (double) ((all_pixels[i] >> 16) & 0xFF),
                      (double) ((all_pixels[i] >>  8) & 0xFF),
                      (double) ((all_pixels[i] >>  0) & 0xFF) };
                int j = contest(c);

                altersingle (alpha, j, c);
                if (rad > 1) alterneigh(alpha, rad,j, c);   /* alter neighbours */

                i++;
                if (i % delta == 0)
                {
                    alpha -= alpha / 30.0;
                    radius *= (29.0 / 30.0); // Decreases by a factor of 1/30 each cycle
                    rad = radius;
                }
            }
            std::fprintf(stderr,"finished 1D learning: final alpha=%f !\n", alpha);
        }
    private:
        /* Search for biased BGR values */
        int contest(const double c[3])
        {
            /* finds closest neuron (min dist) and updates freq */
            /* finds best neuron (min dist-bias) and returns position */
            /* for frequently chosen neurons, freq[i] is high and bias[i] is negative */
            /* bias[i] = gamma * ((1/netsize) - freq[i]) */
            const double beta = 1.0 /  1024.0;
            const double gamma = 1024.0;
            const double betagamma = beta * gamma;

            double bestd     = 1e99;
            double bestbiasd = bestd;
            int bestpos = -1;
            int bestbiaspos = bestpos;
            for (int i=0; i<netsize; i++)
            {
                pixel& pix = network[i];

                double dist = 0;
                for(int n=0; n<3; ++n)
                    dist += std::abs(pix.c[n] - c[n]);

                if (dist<bestd) {bestd=dist; bestpos=i;}
                double biasdist = dist - bias[i];
                if (biasdist<bestbiasd) {bestbiasd=biasdist; bestbiaspos=i;}

                bias[i] += betagamma * freq[i];
                freq[i] -= beta      * freq[i];
            }
            freq[bestpos] += beta;
            bias[bestpos] -= betagamma;
            return(bestbiaspos);
        }

        /* Move neuron i towards biased (b,g,r) by factor alpha */
        void altersingle(double alpha, int i, const double c[3])
        {
            pixel& pix = network[i]; /* alter hit neuron */
            for(int n=0; n<3; ++n)
                pix.c[n] += (alpha * (c[n] - pix.c[n]));
        }

        /* Move adjacent neurons by precomputed alpha*(1-((i-j)^2/[r]^2)) in radpower[|i-j|] */
        void alterneigh(double alpha, int rad,int i, const double c[3])
        {
            int lo = i-rad;   if (lo<-1) lo=-1;
            int hi = i+rad;   if (hi>netsize) hi=netsize;

            int j = i+1;
            int k = i-1;
            int radsquared = rad*rad;
            double alpha_div_radsquared = alpha / radsquared;
            for(int q=0; (j<hi) || (k>lo); ++q)
            {
                double a = alpha_div_radsquared * (radsquared - q*q);
                // Escape the center of change at both sides
                if (j < hi) altersingle(a, j++, c);
                if (k > lo) altersingle(a, k--, c);
            }
        }
    };

    void ReduceHistogram_NeuQuant
        (HistogramType& Histogram, unsigned target_colors)
    {
        std::vector<uint32> all_pixels;
        for(HistogramType::iterator i = Histogram.begin(); i != Histogram.end(); ++i)
            for(unsigned pix=i->first, c=i->second; c-- > 0; )
                all_pixels.push_back(pix);
        std::random_shuffle( all_pixels.begin(), all_pixels.end() );
        /* Randomly shuffle the input pixels to provide the most unbiased
         * learning method for the network. In original NeuQuant code,
         * the array was traversed with intervals decided by a set of
         * prime numbers, which achieved more or less the same effect.
         */
        NeuQuant worker(target_colors);
        worker.Learn(&all_pixels[0], all_pixels.size());

        Histogram.clear();
        worker.Retrieve( Inserter(Histogram) );

        std::fprintf(stderr, "NeuQuant-reduced to %u colors (aimed for %u)\n",
            (unsigned) Histogram.size(), target_colors);
    }

    // Finds the smallest value of n where value is a multiple of (2^-n).
    int FindNegativePowerOfTwoMultiple(double value)
    {
        int result = 0;
        double factor = 1.0;
        for(;;)
        {
            double remainder = std::fmod(value, factor);
            remainder = std::min(remainder, factor-remainder);
            if( remainder < (1/64.0) )  break;

            factor *= 0.5;
            result += 1;
        }
        return result;
    }
}

struct ComparePaletteLuma
{
    const Palette& pal;

    ComparePaletteLuma(const Palette& p): pal(p) { }

    bool operator() (unsigned a, unsigned b) const
        { return pal.GetLuma(a) < pal.GetLuma(b); }
};


/* From the paper "The CIEDE2000 Color-Difference Formula: Implementation Notes, */
/* Supplementary Test Data, and Mathematical Observations", by */
/* Gaurav Sharma, Wencheng Wu and Edul N. Dalal, */
/* Color Res. Appl., vol. 30, no. 1, pp. 21-30, Feb. 2005. */

/* Return the CIEDE2000 Delta E color difference measure squared, for two Lab values */
double ColorCompare(int r1,int g1,int b1, // 0..255
                    const LabAndLuma& meta1,
                    int r2,int g2,int b2, // 0..255
                    const LabAndLuma& meta2)
{
    switch(UseCIE)
    {
        default:
        case Compare_RGB:
        {
            /* Joel Yliluoma's slightly luma&saturation aware RGB compare function */
            int diffR = r1-r2, diffG = g1-g2, diffB = b1-b2;
            //const double chroma_coeff = 0.75;
            //double lumadiff = (meta1.luma - meta2.luma) * (1/255e3);
            return (diffR*diffR)//*(0.299/255/255*chroma_coeff)
                 + (diffG*diffG)//*(0.587/255/255*chroma_coeff)
                 + (diffB*diffB)//*(0.114/255/255*chroma_coeff)
                // + lumadiff*lumadiff
                 ;
        }
        case Compare_CIE76_DeltaE:
        {
            double dL = meta1.lab.L - meta2.lab.L;
            double da = meta1.lab.a - meta2.lab.a;
            double db = meta1.lab.b - meta2.lab.b;
            // CIE normal deltaE squared
            return dL*dL + da*da + db*db;
        }
        case Compare_CIE94_DeltaE:
        {
            double dL = meta1.lab.L - meta2.lab.L;
            double da = meta1.lab.a - meta2.lab.a;
            double db = meta1.lab.b - meta2.lab.b;
            /* Although CIE94 and CIEDE2000 are theoretically more developed
             * than the plain CIE distance, in practise both have some kind
             * of issue with violet/purple colors, for which they deem black
             * as the best substitute.
             * (Does it somehow think purple is ultraviolet or something?)
             * (Bisqwit's hue circle + EGA palette test.)
             */

            // CIE94 deltaE color difference squared
            double dLsq = dL*dL;
            double c12 = std::sqrt(meta1.lab.C * meta2.lab.C); // Symmetric chromance
            double dc = meta2.lab.C - meta1.lab.C;
            double dcsq = dc*dc;
            double dhsq = da*da + db*db - dcsq; // delta hue squared
            if(dhsq < 0.0) dhsq = 0.0;
            double sc = 1.0 + 0.048 * (c12); // weighting for delta chromance
            double sh = 1.0 + 0.014 * (c12); // weighting for delta hue
            return dLsq + dcsq / (sc*sc) + dhsq / (sh*sh);
        }
        case Compare_CMC_lc:
        {
            const double l = 1.5, c = 1; // Lightness and chroma parameters
            double dl = meta2.lab.L - meta1.lab.L;
            if(meta1.lab.L <= 0.0 || meta2.lab.L <= 0.0)
                return dl*dl;
            double da = meta2.lab.a - meta1.lab.a;
            double db = meta2.lab.b - meta1.lab.b;
            double dc = meta2.lab.C - meta1.lab.C;
            double T;
            double h1 = meta1.lab.h;
            if(h1 < 0) h1 += 2*M_PI;
            if(h1 >= 164*M_PI/180 && h1 <= 345*M_PI/180)
                T = 0.56 + std::fabs(0.2 * std::cos(h1 + 168*M_PI/180));
            else
                T = 0.36 + std::fabs(0.4 * std::cos(h1 + 35*M_PI/180));
            double C1p4 = std::pow(meta1.lab.C, 4);
            double F = std::sqrt(C1p4 / (C1p4 + 1900));
            double SL = meta1.lab.L < 16 ? 0.511 : (0.040975*meta1.lab.L
                                                  /(1+0.01765*meta1.lab.L));
            double SC = 0.0638*meta1.lab.C / (1 + 0.0131*meta1.lab.C) + 0.0638;
            double SH = SC * (F*T + 1 - F);
            double dLSL = dl / (l*SL);
            double dCSC = dc / (c*SC);
            double dHSH = da*da + db*db - dc*dc;
            if(dHSH <= 0) dHSH = 0.0; else dHSH = std::sqrt(dHSH)/SH;
            return dLSL*dLSL + dCSC*dCSC + dHSH*dHSH;
        }
        case Compare_BFD_lc:
        {
            const double l = 1.5, c = 1; // Lightness and chroma parameters
            if(meta1.lab.L <= 0 && meta2.lab.L <= 0.0) return 0;
            double da = meta2.lab.a - meta1.lab.a;
            double db = meta2.lab.b - meta1.lab.b;
            double y1 = (meta1.lab.L > 7.996969)
                        ? std::pow( (meta1.lab.L+16)/116, 3.0)*100
                        : 100 * (meta1.lab.L / 903.3);
            double y2 = (meta2.lab.L > 7.996969)
                        ? std::pow( (meta2.lab.L+16)/116, 3.0)*100
                        : 100 * (meta2.lab.L / 903.3);
            double bfd1 = 54.6 * std::log(y1+1.5) - 9.6;
            double bfd2 = 54.6 * std::log(y2+1.5) - 9.6;
            double deltaL = bfd2 - bfd1;
            double deltaC = meta2.lab.C - meta1.lab.C;
            double avgC = (meta2.lab.C + meta1.lab.C) * .5;
            double avgh = (meta2.lab.h + meta1.lab.h) * .5;
            double deltah = da*da + db*db - deltaC*deltaC;
            if(deltah <= 0.0) deltah = 0.0; else deltah = std::sqrt(deltah);
            double dc = 0.035 * avgC / (1 + 0.00365*avgC) + 0.521;
            double avgCp4 = std::pow(avgC, 4.0);
            double g = std::sqrt(avgCp4 / (avgCp4 + 14000));
            double t = 0.627 + 0.055 * std::cos(avgh - 254*M_PI/180)
                             - 0.040 * std::cos(2*avgh - 136*M_PI/180)
                             + 0.070 * std::cos(3*avgh - 32*M_PI/180)
                             + 0.049 * std::cos(4*avgh + 114*M_PI/180)
                             - 0.015 * std::cos(5*avgh - 103*M_PI/180);
            double dh = dc * (g*(t-1) + 1);
            double rh = -0.260 * std::cos(avgh - 308*M_PI/180)
                       - 0.379 * std::cos(2*avgh - 160*M_PI/180)
                       - 0.636 * std::cos(3*avgh + 254*M_PI/180)
                       + 0.226 * std::cos(4*avgh + 140*M_PI/180)
                       - 0.194 * std::cos(5*avgh + 280*M_PI/180);
            double avgCp6 = std::pow(avgC, 6.0);
            double rc = std::sqrt(avgCp6 / (avgCp6 + 823543e7));
            double rt = rh * rc;
            return std::sqrt(
                   (deltaL/l)*(deltaL/l)
                 + (deltaC/(c*dc))*(deltaC/(c*dc))
                 + (deltah/dh)*(deltah/dh)
                 + rt*(deltaC/dc)*(deltah/dh) );
        }
        case Compare_CIEDE2000_DeltaE:
        {
            double dL = meta1.lab.L - meta2.lab.L;
        #define RAD2DEG(xx) (180.0/M_PI * (xx))
        #define DEG2RAD(xx) (M_PI/180.0 * (xx))
          #if 0
            double deltaL_ = meta2.lab.L - meta1.lab.L;
            double Lcap = (meta1.lab.L + meta2.lab.L) * .5;
            double Ccap = (meta1.lab.C + meta2.lab.C) * .5;
            double Ccap7 = std::pow(Ccap, 7.0);
            double a1_ = meta1.lab.a + meta1.lab.a*.5*(1-std::sqrt(Ccap7/(Ccap7+6103515625)));
            double a2_ = meta2.lab.a + meta2.lab.a*.5*(1-std::sqrt(Ccap7/(Ccap7+6103515625)));
            double C1_ = std::sqrt(a1_*a1_ + meta1.lab.b*meta1.lab.b);
            double C2_ = std::sqrt(a2_*a2_ + meta2.lab.b*meta2.lab.b);
            double Ccap_ = (C1_+C2_)*.5, deltaC_ = C1_-C2_;
            double Ccap_7 = std::pow(Ccap_, 7.0);
            double h1_ = std::fmod(std::atan2(meta1.lab.b, meta1.lab.a), 2*M_PI);
            double h2_ = std::fmod(std::atan2(meta2.lab.b, meta2.lab.a), 2*M_PI);
            double absh_delta = std::fabs(h1_-h2_);
            double deltah_;
            if(absh_delta < M_PI) deltah_ = h2_ - h1_;
            else if(h2_ <= h1_)   deltah_ = h2_ - h1_ + 2*M_PI;
            else                  deltah_ = h2_ - h1_ - 2*M_PI;
            double deltaHcap_ = 2*std::sqrt(C1_*C2_)*std::sin(deltah_*.5);
            double Hcap_;
            if(absh_delta > M_PI) Hcap_ = (h1_ + h2_ + 2*M_PI) * 0.5;
            else                  Hcap_ = (h1_ + h2_)          * 0.5;
            double T = 1
              - 0.17 * std::cos(deltaHcap_ - M_PI/6)
              + 0.24 * std::cos(2*deltaHcap_)
              + 0.32 * std::cos(3*deltaHcap_ + M_PI/30)
              - 0.20 * std::cos(4*deltaHcap_ - 21*M_PI/60);
            double Lcapminus50 = Lcap - 50;
            double Lcapminus50sq = Lcapminus50*Lcapminus50;
            double SL = 0.015 * Lcapminus50sq / std::sqrt(20 + Lcapminus50sq);
            double SC = 1 + 0.045 * Ccap_, SH = 1 + 0.015 * Ccap_ * T;
            double Htmp = (Hcap_ - 275*M_PI/180) / 25;
            double RT = -2 * std::sqrt(Ccap_7 / (Ccap_7 + 6103515625))
                           * std::sin(M_PI/6*std::exp(-(Htmp*Htmp)));
            double deltaH_ = /* ??? MISSING in Wikipedia's description, assume: */ deltaHcap_;
            return (deltaL_/SL)*(deltaL_/SL)
                 + (deltaC_/SC)*(deltaC_/SC)
                 + (deltaH_/SH)*(deltaH_/SH)
                 + RT * deltaC_/SC * deltaH_/SH;
          #endif
            /* Compute Cromanance and Hue angles */
            double C1,C2, h1,h2;
            {
                double Cab = 0.5 * (meta1.lab.C + meta2.lab.C);
                double Cab7 = std::pow(Cab,7.0);
                double G = 0.5 * (1.0 - std::sqrt(Cab7/(Cab7 + 6103515625.0)));
                double a1 = (1.0 + G) * meta1.lab.a;
                double a2 = (1.0 + G) * meta2.lab.a;
                C1 = std::sqrt(a1 * a1 + meta1.lab.b * meta1.lab.b);
                C2 = std::sqrt(a2 * a2 + meta2.lab.b * meta2.lab.b);

                if (C1 < 1e-9)
                    h1 = 0.0;
                else {
                    h1 = RAD2DEG(std::atan2(meta1.lab.b, a1));
                    if (h1 < 0.0)
                        h1 += 360.0;
                }

                if (C2 < 1e-9)
                    h2 = 0.0;
                else {
                    h2 = RAD2DEG(std::atan2(meta2.lab.b, a2));
                    if (h2 < 0.0)
                        h2 += 360.0;
                }
            }

            /* Compute delta L, C and H */
            double /*dL = meta2.lab.L - meta1.lab.L, */dC = C2 - C1, dH;
            {
                double dh;
                if (C1 < 1e-9 || C2 < 1e-9) {
                    dh = 0.0;
                } else {
                    dh = h2 - h1;
                    if (dh > 180.0)
                        dh -= 360.0;
                    else if (dh < -180.0)
                        dh += 360.0;
                }

                dH = 2.0 * std::sqrt(C1 * C2) * std::sin(DEG2RAD(0.5 * dh));
            }

            double h;
            double L = 0.5 * (meta1.lab.L  + meta2.lab.L);
            double C = 0.5 * (C1 + C2);
            if (C1 < 1e-9 || C2 < 1e-9) {
                h = h1 + h2;
            } else {
                h = h1 + h2;
                if (fabs(h1 - h2) > 180.0) {
                    if (h < 360.0)
                        h += 360.0;
                    else if (h >= 360.0)
                        h -= 360.0;
                }
                h *= 0.5;
            }
            double T = 1.0
              - 0.17 * std::cos(DEG2RAD(h - 30.0))
              + 0.24 * std::cos(DEG2RAD(2.0 * h))
              + 0.32 * std::cos(DEG2RAD(3.0 * h + 6.0))
              - 0.2 * std::cos(DEG2RAD(4.0 * h - 63.0));
            double hh = (h - 275.0)/25.0;
            double ddeg = 30.0 * std::exp(-hh * hh);
            double C7 = std::pow(C,7.0);
            double RC = 2.0 * std::sqrt(C7/(C7 + 6103515625.0));
            double L50sq = (L - 50.0) * (L - 50.0);
            double SL = 1.0 + (0.015 * L50sq) / std::sqrt(20.0 + L50sq);
            double SC = 1.0 + 0.045 * C;
            double SH = 1.0 + 0.015 * C * T;
            double RT = -std::sin(DEG2RAD(2 * ddeg)) * RC;
            double dLsq = dL/SL, dCsq = dC/SC, dHsq = dH/SH;
            return dLsq*dLsq + dCsq*dCsq + dHsq*dHsq + RT*dCsq*dHsq;
        #undef RAD2DEG
        #undef DEG2RAD
        }
    }
}

PalettePair FindBestPalettePair(int rin,int gin,int bin, const Palette& pal)
{
    PalettePair result;
    const unsigned PaletteSize     = pal.Size();
    const unsigned NumCombinations = pal.NumCombinations();
    const double X = DitherErrorFactor; // Error multiplication value
    int rer=0, ger=0, ber=0;

    size_t GenerationLimit = DitherColorListSize;
    assert(GenerationLimit > 0);

    while(result.size() < GenerationLimit)
    {
        int ter = rin + rer*X, teg = gin + ger*X, teb = bin + ber*X;

        if(ter<0) ter=0; else if(ter>255)ter=255;
        if(teg<0) teg=0; else if(teg>255)teg=255;
        if(teb<0) teb=0; else if(teb>255)teb=255;

        LabAndLuma t_meta(ter,teg,teb);
        double least_penalty = -1;

        unsigned chosen = result.size() % PaletteSize;
        bool chosen_comb = false;

        for(unsigned i=0; i<PaletteSize; ++i)
        {
            const unsigned color = pal.GetColor(i);
            unsigned r = (color >> 16) & 0xFF, g = (color >> 8) & 0xFF, b = (color) & 0xFF;
            double penalty = ColorCompare(ter,teg,teb, t_meta,
                                          r,g,b,       pal.GetMeta(i));
            if(penalty < least_penalty || least_penalty < 0)
                { least_penalty = penalty; chosen = i; chosen_comb = false; }
        }

        size_t capacity = GenerationLimit - result.size();
        for(unsigned i=0; i<NumCombinations; ++i)
        {
            if(pal.Combinations[i].indexcount > capacity)
                continue;

            const unsigned color = pal.GetCombinationColor(i);
            unsigned r = (color >> 16) & 0xFF, g = (color >> 8) & 0xFF, b = (color) & 0xFF;
            double penalty = ColorCompare(ter,teg,teb, t_meta,
                                          r,g,b,       pal.GetCombinationMeta(i));
            if(penalty < least_penalty || least_penalty < 0)
                { least_penalty = penalty; chosen = i; chosen_comb = true; }
        }

        unsigned r,g,b;
        if(!chosen_comb)
        {
            result.push_back(chosen);
            pal.Data[chosen].SplitRGB(r,g,b);
        }
        else
        {
            for(unsigned i=0; i<pal.Combinations[chosen].indexcount; ++i)
                result.push_back( pal.Combinations[chosen].indexlist[i] );
            pal.Combinations[chosen].combination.SplitRGB(r,g,b);
        }
        rer += rin-r;
        ger += gin-g;
        ber += bin-b;
    }
    std::sort(result.begin(), result.end(), ComparePaletteLuma(pal));
    return result;
}

void ReduceHistogram(HistogramType& Histogram)
{
    for(size_t b=PaletteReductionMethod.size(), a=0; a<b; ++a)
    {
        const std::string& fn = PaletteReductionMethod[a].filename;
        if(!fn.empty())
        {
            FILE* fp = std::fopen(fn.c_str(), "rb");
            if(!fp)
                std::perror(fn.c_str());
            else
            {
                gdImagePtr im = gdImageCreateFromPng(fp);
                if(!im) { std::rewind(fp); im = gdImageCreateFromGif(fp); }
                if(!im) { std::rewind(fp); im = gdImageCreateFromWBMP(fp); }
                if(!im)
                {
                    std::fprintf(stderr,
                        "%s: Not a PNG, GIF or WBMP file! Cannot read palette.\n",
                            fn.c_str());
                }
                else
                {
                    if(a == 0) Histogram.clear();

                    if(gdImageTrueColor(im))
                    {
                        std::fprintf(stderr,
                            "%s: Not a paletted picture! Will hastily use libGD to make a 256-color palette out of it regardless.\n",
                            fn.c_str());
                        gdImageTrueColorToPalette(im, 0, 256);
                    }
                    unsigned n_colors = gdImageColorsTotal(im);
                    for(unsigned c=0; c<n_colors; ++c)
                    {
                        unsigned color =
                            gdTrueColor(gdImageRed(im,c),gdImageGreen(im,c),gdImageBlue(im,c));
                        Histogram[color] += 1024*768;
                    }
                    std::fprintf(stderr,
                        "Loaded %u colors from %s, now has %u colors\n",
                            n_colors, fn.c_str(),
                            (unsigned) Histogram.size() );
                }
                std::fclose(fp);
            }
            continue;
        }
        unsigned size = PaletteReductionMethod[a].size;
        switch(PaletteReductionMethod[a].method)
        {
            #define MakeCase(o,name) \
                case quant_##name: ReduceHistogram_##name(Histogram, size); break;
            DefinePaletteMethods(MakeCase)
            #undef MakeCase
        }
    }
}

Palette MakePalette(const HistogramType& Histogram, unsigned MaxColors)
{
    if(Histogram.size() > MaxColors)
    {
        // Histogram is too large. Choose most popular colors.
        std::vector<HistItem> VecHistogram( Histogram.begin(), Histogram.end() );
        std::sort(VecHistogram.begin(), VecHistogram.end(), CompareSecondRev);
        Palette result;
        result.Data.reserve(MaxColors);
        for(unsigned n=0; n<MaxColors; ++n)
            result.Data.push_back( VecHistogram[n].first );
        result.Analyze();
        return result;
    }

    Palette result;
    for(HistogramType::const_iterator
        i = Histogram.begin();
        i != Histogram.end();
        ++i)
    {
        if(result.Size() >= MaxColors) break;
        result.Data.push_back( i->first );;
    }
    result.Analyze();
    return result;
}

const double illuminants[4][3*3] =
  { { // CIE C illuminant
      0.488718, 0.176204, 0.000000,
      0.310680, 0.812985, 0.0102048,
      0.200602, 0.0108109, 0.989795 },
    { // Adobe D65 illuminant
      0.576700, 0.297361, 0.0270328,
      0.185556, 0.627355, 0.0706879,
      0.188212, 0.0752847, 0.99124 },
    { // What is this?
      0.412453, 0.357580, 0.180423,
      0.212671, 0.715160, 0.072169,
      0.019334, 0.119193, 0.950227 } };
static const double* GetIlluminant()
{
    switch(UseCIE)
    {
        case Compare_CIE76_DeltaE:
            return illuminants[2];
        default:
            return illuminants[0];
    }
}

template<typename T>
static XYZitem RGBtoXYZ(T r,T g,T b)
{
    const double* illum = GetIlluminant();
    XYZitem result = {
      ((illum[0])*r + (illum[3])*g + (illum[6])*b) / 255.0,
      ((illum[1])*r + (illum[4])*g + (illum[7])*b) / 255.0,
      ((illum[2])*r + (illum[5])*g + (illum[8])*b) / 255.0 };
    return result;
}
LabItem XYZtoLAB(const XYZitem& xyz)
{
    // Use Profile illuminant - D65 */
    const double* illum = GetIlluminant();
    double Xn = 1.0/(illum[0]+illum[1]+illum[2]);
    double Yn = 1.0/(illum[3]+illum[4]+illum[5]);
    double Zn = 1.0/(illum[6]+illum[7]+illum[8]);
    double x = xyz.X * Xn;
    double y = xyz.Y * Yn;
    double z = xyz.Z * Zn;
    const double threshold1 = (6*6*6.0)/(29*29*29.0);
    const double threshold2 = (29*29.0)/(6*6*3.0);
    double x1 = (x > threshold1) ? pow(x, 1.0/3.0) : (threshold2*x)+(4/29.0);
    double y1 = (y > threshold1) ? pow(y, 1.0/3.0) : (threshold2*y)+(4/29.0);
    double z1 = (z > threshold1) ? pow(z, 1.0/3.0) : (threshold2*z)+(4/29.0);
    LabItem result;
    result.L = (29*4)*y1 - (4*4);
    result.a = (500*(x1-y1) );
    result.b = (200*(y1-z1) );
    result.C = std::sqrt(result.a*result.a + result.b+result.b);
    result.h = std::atan2(result.b, result.a);
    return result;
}
LabItem RGBtoLAB(int r,int g,int b)
{
    return XYZtoLAB( RGBtoXYZ(r,g,b) );
}

void Palette::Analyze()
{
    const unsigned PaletteSize = Data.size();
    Combinations.clear();

    if(!PaletteSize) return;

    // Sort palette by luma.
    // Create combinations for all pairs where the difference
    // between the luma of the two is <= the average difference
    // between the luma of two successive palette elements.
    std::vector<unsigned> luma_order(PaletteSize);;
    for(unsigned a=0; a<PaletteSize; ++a) luma_order[a] = a;
    std::sort(luma_order.begin(), luma_order.end(), ComparePaletteLuma(*this));

    unsigned long average_luma_difference = 0;
    unsigned maximum_luma_difference = 0;
    for(unsigned a=1; a<PaletteSize; ++a)
    {
        unsigned diff = GetLuma(luma_order[a]) - GetLuma(luma_order[a-1]);
        average_luma_difference += diff;
        if(diff > maximum_luma_difference)
            maximum_luma_difference = diff;
    }
    average_luma_difference /= (PaletteSize - 1);

    unsigned total_luma = GetLuma(luma_order.back()) - GetLuma(luma_order.front());

    unsigned luma_threshold = 0;

    double p = DitherCombinationContrast;
    if(p < 0.0 || p > 3.0) p = 1.2; // "Reasonable default"

    if(p >= 0.0
    && p < 1.0)
        luma_threshold = average_luma_difference * p;
    else if(p < 2.0)
        luma_threshold = average_luma_difference +
            (p-1.0) * (maximum_luma_difference-average_luma_difference);
    else if(p <= 3.0)
        luma_threshold = maximum_luma_difference +
            (p-2.0) * (total_luma-maximum_luma_difference);

    if(verbose >= 2)
    {
        for(unsigned a=0; a<PaletteSize; ++a)
            printf("Luma[%3u]: %6u (%3u = #%06X)\n", a, GetLuma(luma_order[a]),
                luma_order[a], Data[luma_order[a]].rgb);
        printf("Average: %lu, max: %u, total: %u, threshold: %u\n",
            average_luma_difference,
            maximum_luma_difference,
            total_luma,
            luma_threshold);
    }

    // Create some combinations
    for(unsigned a=0; a+1<PaletteSize; ++a)
    {
        const unsigned index1 = luma_order[a];
        const unsigned pix1 = Data[index1].rgb;
        int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;

        for(unsigned b=a+1; b<PaletteSize; ++b)
        {
            const unsigned index2 = luma_order[b];

            if(GetLuma(index2) > GetLuma(index1) + luma_threshold)
                break;

            const unsigned pix2 = Data[index2].rgb;
            int r2 = (pix2 >> 16) & 0xFF, g2 = (pix2 >> 8) & 0xFF, b2 = (pix2) & 0xFF;

            int combined_r = (r1 + r2) / 2;
            int combined_g = (g1 + g2) / 2;
            int combined_b = (b1 + b2) / 2;

            if(verbose >= 3)
                printf("Combination: %06X + %06X%*s(luma diff %ld) = %02X%02X%02X\n",
                    Data[index1].rgb, Data[index2].rgb,
                    19,"",
                    (long) Data[index2].meta.luma - (long) Data[index1].meta.luma,
                    combined_r,combined_g,combined_b);

            Combination comb( index1,index2,
                (combined_r<<16)+(combined_g<<8)+combined_b );
            Combinations.push_back(comb);

            for(unsigned c=b+1; c<PaletteSize; ++c)
            {
                const unsigned index3 = luma_order[c];

                if(GetLuma(index3) > GetLuma(index1) + luma_threshold)
                    break;

                const unsigned pix3 = Data[index3].rgb;
                int r3 = (pix3 >> 16) & 0xFF, g3 = (pix3 >> 8) & 0xFF, b3 = (pix3) & 0xFF;

                int combined_r = (r1 + r2 + r3) / 3;
                int combined_g = (g1 + g2 + g3) / 3;
                int combined_b = (b1 + b2 + b3) / 3;

                if(verbose >= 3)
                    printf("Combination: %06X + %06X + %06X%*s(luma diff %ld) = %02X%02X%02X\n",
                        Data[index1].rgb,
                        Data[index2].rgb,
                        Data[index3].rgb,
                        10,"",
                        (long) Data[index3].meta.luma - (long) Data[index1].meta.luma,
                        combined_r,combined_g,combined_b);

                Combination comb( index1,index2,index3,
                    (combined_r<<16)+(combined_g<<8)+combined_b );
                Combinations.push_back(comb);

                for(unsigned d=c+1; d<PaletteSize; ++d)
                {
                    const unsigned index4 = luma_order[d];

                    if(GetLuma(index4) > GetLuma(index1) + luma_threshold)
                        break;

                    const unsigned pix4 = Data[index4].rgb;
                    int r4 = (pix4 >> 16) & 0xFF, g4 = (pix4 >> 8) & 0xFF, b4 = (pix4) & 0xFF;

                    int combined_r = (r1 + r2 + r3 + r4) / 4;
                    int combined_g = (g1 + g2 + g3 + g4) / 4;
                    int combined_b = (b1 + b2 + b3 + b4) / 4;

                    if(verbose >= 3)
                        printf("Combination: %06X + %06X + %06X + %06X (luma diff %ld) = %02X%02X%02X\n",
                            Data[index1].rgb,
                            Data[index2].rgb,
                            Data[index3].rgb,
                            Data[index4].rgb,
                            (long) Data[index4].meta.luma - (long) Data[index1].meta.luma,
                            combined_r,combined_g,combined_b);

                    Combination comb( index1,index2,index3,index4,
                        (combined_r<<16)+(combined_g<<8)+combined_b );
                    Combinations.push_back(comb);
        }   }   }
    }
    if(!Combinations.empty() && verbose >= 2)
    {
        printf("Total palette-color combinations chosen for dithering candidates: %u\n",
            (unsigned) Combinations.size());
        printf("Dithering will maximally do %u * (%u + %u) = %u * %u = %u tests per pixel.\n",
            DitherColorListSize,
            (unsigned) Data.size(),
            (unsigned) Combinations.size(),
            DitherColorListSize,
            (unsigned) (Data.size() + Combinations.size()),
            (unsigned) (DitherColorListSize * (Data.size() + Combinations.size()))
        );
    }
}

void Palette::SetHardcoded(unsigned n, ...)
{
    va_list ap;
    va_start(ap, n);
    Data.clear();
    Data.reserve(n);
    while(n--)
    {
        uint32 val = va_arg(ap, unsigned);
        Data.push_back( val );
    }
    va_end(ap);
    Analyze();
}

Palette Palette::GetSlice(unsigned begin, unsigned count) const
{
    Palette result;
    result.Data.assign( Data.begin()+begin, Data.begin()+begin+count );
    return result;
}

std::vector<unsigned> CreateOrderedDitheringMatrix(unsigned Width, unsigned Height)
{
    // Find M=ceil(log2(x)) and L=ceil(log2(y))
    unsigned M=0; while( Width  > (1u << M)) ++M;
    unsigned L=0; while( Height > (1u << L)) ++L;
    unsigned RoundedWidth  = 1 << M;
    unsigned RoundedHeight = 1 << L;
    std::vector<unsigned> result;
    result.reserve(Width * Height);

    for(unsigned y=0; y<Height; ++y)
        for(unsigned x=0; x<Width; ++x)
        {
            unsigned v = 0, offset = 0, xmask = M, ymask = L;
            if(M==0 || (M > L && L != 0))
            {
                unsigned xc = x ^ (y<<M>>L), yc = y;
                for(unsigned bit=0; bit < M+L; )
                {
                    v |= ((yc >> --ymask)&1) << bit++;
                    for(offset += M; offset >= L; offset -= L)
                        v |= ((xc >> --xmask)&1) << bit++;
                }
            }
            else
            {
                unsigned xc = x, yc = y ^ (x<<L>>M);
                for(unsigned bit=0; bit < M+L; )
                {
                    v |= ((xc >> --xmask)&1) << bit++;
                    for(offset += L; offset >= M; offset -= M)
                        v |= ((yc >> --ymask)&1) << bit++;
                }
            }
            result.push_back(v);
        }

    if(Height != RoundedHeight
    || Width  != RoundedWidth)
    {
        // If the user requested e.g. 3x3 and we used 4x4 algorithm,
        // convert the numbers so that the resulting range is ungapped:
        //
        // We generated:  We saved:   We compress the number range:
        //  0 12  3 15     0 12  3     0 7 3
        //  8  4 11  7     8  4 11     5 4 6
        //  2 14  1 13     2 14  1     2 8 1
        // 10  6  9  5
        unsigned max_value   = RoundedWidth * RoundedHeight;
        unsigned matrix_size = Height * Width;
        unsigned n_missing = 0;
        for(unsigned v=0; v<max_value; ++v)
        {
            bool found = false;
            for(unsigned a=0; a<matrix_size; ++a)
            {
                if(result[a] == v)
                {
                    result[a] -= n_missing;
                    found = true;
                    break;
                }
            }
            if(!found)
                ++n_missing;
        }
    }
    return result;
}

std::vector<unsigned> CreateDispersedDitheringMatrix()
{
    return CreateOrderedDitheringMatrix(DitherMatrixWidth, DitherMatrixHeight);
}

std::vector<unsigned> CreateTemporalDitheringMatrix()
{
    return CreateOrderedDitheringMatrix(1, TemporalDitherSize);
}
