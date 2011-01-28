#include <cstdio>
#include <algorithm>
#include <cmath>
#include <gd.h>

#include "palette.hh"
#include "pixel.hh"
#include "pixels/averagepixel.hh"

std::vector<PaletteMethodItem> PaletteReductionMethod;

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

PalettePair FindBestPalettePair(int rin,int gin,int bin,
    const uint32* Palette, unsigned PaletteSize)
{
    PalettePair output;
    output.entry1 = 0;
    output.entry2 = 0;
    output.result = 0.5;
    bool output_chosen = false;
    if(PaletteSize >= 2 && PaletteSize <= 64)
    {
        /* O(n^2) algorithm description: Choose a pair of colors that
         * form a 3D line segment that is closest to the input color.
         * To optimize this, we use luma values rather than geometrical
         * transformations to scalarize the different coordinates.
         */

        int input_luma = rin*/*0.*/299 + gin*/*0.*/587 + bin*/*0.*/114;

        /* Note: Palette is sorted by luma by MakePalette(). */
        std::vector<int> luma_table(PaletteSize);
        for(unsigned a=0; a<PaletteSize; ++a)
        {
            const uint32 pix = Palette[a];
            int r1 = (pix >> 16) & 0xFF, g1 = (pix >> 8) & 0xFF, b1 = (pix) & 0xFF;
            luma_table[a] = r1*/*0.*/299 + g1*/*0.*/587 + b1*/*0.*/114;
        };
retry_modified_luma:;
        /* lower_bound(k) = find the first element that is >= k */
        /* upper_bound(k) = find the first element that is > k */
        unsigned first_index_to_consider = // First where luma >= input_luma
            std::lower_bound(luma_table.begin(), luma_table.end(),
                input_luma) - luma_table.begin();
        unsigned last_index_to_consider = // First where luma  > input_luma
            std::upper_bound(luma_table.begin(), luma_table.end(),
                input_luma) - luma_table.begin();
        /*if(PaletteSize <= 16)
        {
            first_index_to_consider = 0;
            last_index_to_consider = PaletteSize;
        }*/

        if(first_index_to_consider >= PaletteSize && input_luma != luma_table.back())
        {
            // If all palette colors are darker than the color that was
            // requested, accept a loss of luma and try again.
            input_luma = luma_table.back();
            goto retry_modified_luma;
        }
        if(last_index_to_consider <= 0 && input_luma != luma_table.front()+1)
        {
            input_luma = luma_table.front()+1;
            goto retry_modified_luma;
        }

        unsigned best_diff=0;
        for(unsigned pa=0; pa<last_index_to_consider; ++pa)
        {
            int luma1 = luma_table[pa];

            const uint32 pix1 = Palette[pa];
            int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;

            for(unsigned pb=first_index_to_consider; pb<PaletteSize; ++pb)
            {
                int luma2 = luma_table[pb];
                /*
                  At this point,
                     luma1 <= input_luma <= luma2.
                  There's no point in combining two colors,
                  if the desired color can in no way become
                  a product of those two colors.
                */

                const uint32 pix2 = Palette[pb];
                int r2 = (pix2 >> 16) & 0xFF, g2 = (pix2 >> 8) & 0xFF, b2 = (pix2) & 0xFF;

                double result;
                if(luma1==luma2)
                {
                    double result_r = (r1==r2 ? 0.5 : ((r1-rin) / double(r1-r2)));
                    double result_g = (g1==g2 ? 0.5 : ((g1-gin) / double(g1-g2)));
                    double result_b = (b1==b2 ? 0.5 : ((b1-bin) / double(b1-b2)));
                    if(result_r < 0) result_r = 0; if(result_r > 1) result_r = 1;
                    if(result_g < 0) result_g = 0; if(result_g > 1) result_g = 1;
                    if(result_b < 0) result_b = 0; if(result_b > 1) result_b = 1;
                    result = (result_r + result_g + result_b) / 3.0;
                }
                else
                    result = (luma1-input_luma) / double(luma1-luma2);

                //if(result < 0) result = 0; if(result > 1) result = 1;

                int DifferenceFromEitherEnd = 2;//1+FindNegativePowerOfTwoMultiple(result);
                // The farther the "result" is from 0.0, 0.5, 0.25/0.75, etc,
                // the more striking the individual pixels become and thus
                // the more important it becomes that they resemble the
                // intended pixel more closely.

                int got_r = r1 + result*(r2-r1);
                int got_g = g1 + result*(g2-g1);
                int got_b = b1 + result*(b2-b1);
                unsigned diff =
                    ColorDiff(rin,gin,bin, got_r,got_g,got_b)
                  + ( ColorDiff(rin,gin,bin, r1,g1,b1)
                    + ColorDiff(rin,gin,bin, r2,g2,b2)
                    ) * DifferenceFromEitherEnd;
                if(diff < best_diff || best_diff == 0)
                {
                    best_diff   = diff;
                    output.result = (float) result;
                    output.entry1 = pa;
                    output.entry2 = pb;
                    output_chosen = true;
                }
            }
        }
    }

    if(!output_chosen)
    {
        /* O(n) algorithm: Choose two colors that are most similar to input. */
        /* Note: There is no guarantee that mixing them can produce
         * an approximation of the input.
         */
        unsigned diff1=0, diff2=0;
        for(unsigned a=0; a<PaletteSize; ++a)
        {
            unsigned diff = ColorDiff(rin,gin,bin, Palette[a]);
            if(diff < diff1 || diff1 == 0)
                { diff2 = diff1; output.entry2 = output.entry1;
                  diff1 = diff; output.entry1 = a; }
            else if(diff < diff2)
                { diff2 = diff; output.entry2 = a; }
        }

        const uint32 pix1 = Palette[output.entry1];
        const uint32 pix2 = Palette[output.entry2];
        int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;
        int r2 = (pix2 >> 16) & 0xFF, g2 = (pix2 >> 8) & 0xFF, b2 = (pix2) & 0xFF;

        double result_r = (r1==r2 ? 0.5 : ((r1-rin) / double(r1-r2)));
        double result_g = (g1==g2 ? 0.5 : ((g1-gin) / double(g1-g2)));
        double result_b = (b1==b2 ? 0.5 : ((b1-bin) / double(b1-b2)));
        if(result_r < 0) result_r = 0; else if(result_r > 1) result_r = 1;
        if(result_g < 0) result_g = 0; else if(result_g > 1) result_g = 1;
        if(result_b < 0) result_b = 0; else if(result_b > 1) result_b = 1;
        output.result = (float) ((result_r + result_g + result_b) / 3.0);
    }
    return output;
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

unsigned MakePalette(uint32* Palette, const HistogramType& Histogram, unsigned MaxColors)
{
    if(Histogram.size() > MaxColors)
    {
        // Histogram is too large. Choose most popular colors.
        std::vector<HistItem> VecHistogram( Histogram.begin(), Histogram.end() );
        std::sort(VecHistogram.begin(), VecHistogram.end(), CompareSecondRev);
        for(unsigned n=0; n<MaxColors; ++n)
            Palette[n] = VecHistogram[n].first;
        std::sort(Palette, Palette+MaxColors, CompareLuma);
        return MaxColors;
    }
    unsigned n = 0;
    for(HistogramType::const_iterator
        i = Histogram.begin();
        i != Histogram.end();
        ++i)
    {
        if(n >= MaxColors) break;
        Palette[n++] = i->first;
    }
    SortPalette(Palette, n);
    return n;
}

void SortPalette(uint32* Palette, unsigned n)
{
    std::sort(Palette, Palette+n, CompareLuma);
}
