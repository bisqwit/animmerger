#include "quantize.hh"
#include "palette.hh"

#include "pixel.hh"
#include "pixels/averagepixel.hh"

#include <algorithm>
#include <gd.h>
#include <cstdio>

std::vector<PaletteMethodItem>
    PaletteReductionMethod;

namespace
{
    // A histogram item contains a pixel value and an occurrence count.
    typedef std::pair<uint32, unsigned> HistItem;

    unsigned ColorDiff(int r1,int g1,int b1, int r2,int g2,int b2)
    {
        int rdiff = r1-r2, gdiff = g1-g2, bdiff = b1-b2;
        return (rdiff*rdiff + gdiff*gdiff + bdiff*bdiff);
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

    struct DiversityInfo
    {
        // Smallest distance from all so-far chosen colors
        double min_dist;
        // Which color index (of so-far chosen colors) is most similar to this
        unsigned closest;
        // Color and use count
        unsigned count;
        ColorInfo meta;

        DiversityInfo(const HistItem& i)
            : min_dist(1e99), closest(~0u),
              count(i.second), meta(i.first) { }

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
        if(colors_originally <= target_colors)
        {
            std::fprintf(stderr,
                "Not reducing histogram, aiming for %u colors and input has %u\n",
                target_colors, colors_originally);
            return;
        }

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
                    if(Lore[chosen].min_dist != 0.0)
                        break;
            }
            else
            {
                /* 2.1b. Choose the color that is most different
                 *       to any color chosen so far */
                double chosen_dist = 0;
                chosen = colors_originally;
                for(unsigned i=0; i<colors_originally; ++i)
                    if(Lore[i].min_dist > chosen_dist)
                        chosen_dist = Lore[chosen = i].min_dist;
            }
            if(chosen >= colors_originally) break;
            /* 2.2. Add the color */
            Lore[chosen].min_dist = 0;
            Lore[chosen].closest  = nadapt; // Closest to it is itself.
            const ColorInfo& chosen_meta = Lore[chosen].meta;

            /* 2.3. Adjust the distances for remaining colors */
            for(unsigned i=0; i<colors_originally; ++i)
                if(Lore[i].min_dist != 0)
                {
                    // If the distance to the newest addition is shorter
                    // than the previous one, mark it as the closest one.
                    double dist = ColorCompare(chosen_meta, Lore[i].meta);
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
                HistItem product(DefaultPixel, 0);
                for(unsigned j=0; j<Lore.size(); ++j)
                    if(Lore[j].closest == i)
                    {
                        product.second += Lore[j].count;
                        if(Lore[j].min_dist == 0.0)
                            product.first = Lore[j].meta.rgb;
                    }
                Histogram.insert(product);
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
                HistItem product;
                unsigned count_total=0, mismatch_count_total=0;
                // For each original palette color
                // that most resembles this one...
                for(unsigned j=0; j<Lore.size(); ++j)
                    if(Lore[j].closest == i)
                    {
                        total.set_n( Lore[j].meta.rgb, Lore[j].count );
                        count_total += Lore[j].count;
                        if(Lore[j].min_dist == 0.0)
                        {
                            product.first = Lore[j].meta.rgb;
                            product.second = Lore[j].count;
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
                    total.set_n( product.first, product.second * 2 );
                    product.first = total.get();
                }
                product.second = count_total;
                // By chance, it might be possible that blending produces
                // colors that are identical to other non-blended colors.
                // To not lose color-use counts, use []+= rather than insert.
                Histogram[product.first] += product.second;
                //Histogram.insert(product);
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
                {int r=(pix>>16)&0xFF, g=(pix>>8)&0xFF, b=pix&0xFF;
                if(r<minr) minr=r; if(r>maxr) maxr=r;
                if(g<ming) ming=g; if(g>maxg) maxg=g;
                if(b<minb) minb=b; if(b>maxb) maxb=b;}
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
                /*
                // HACK: Limit to 6-bit colors
                c0 = int(int(c0*63.0/255.0+0.5)*255/63+0.5);
                c1 = int(int(c1*63.0/255.0+0.5)*255/63+0.5);
                c2 = int(int(c2*63.0/255.0+0.5)*255/63+0.5);
                */
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
        std::fprintf(stderr,"Beginning NeuQuant learning. Creating pixel corpus...");
        std::fflush(stderr);

        size_t total = 0;
        for(HistogramType::iterator i = Histogram.begin(); i != Histogram.end(); ++i)
            total += i->second;

        std::fprintf(stderr," of size %lu...", (unsigned long) total);
        std::fflush(stderr);

        std::vector<uint32> all_pixels;
        all_pixels.reserve(total);
        for(HistogramType::iterator i = Histogram.begin(); i != Histogram.end(); ++i)
            all_pixels.resize( all_pixels.size() + i->second,
                               i->first );

        std::fprintf(stderr,", shuffled...");
        std::fflush(stderr);

        std::random_shuffle( all_pixels.begin(), all_pixels.end() );
        /* Randomly shuffle the input pixels to provide the most unbiased
         * learning method for the network. In original NeuQuant code,
         * the array was traversed with intervals decided by a set of
         * prime numbers, which achieved more or less the same effect.
         */

        std::fprintf(stderr,", done\n");
        std::fflush(stderr);

        NeuQuant worker(target_colors);
        worker.Learn(&all_pixels[0], all_pixels.size());

        Histogram.clear();
        worker.Retrieve( Inserter(Histogram) );

        std::fprintf(stderr, "NeuQuant-reduced to %lu colors (aimed for %u)\n",
            (unsigned long) Histogram.size(), target_colors);
        if(Histogram.size() != target_colors)
        {
            std::fprintf(stderr,
                "Differences between the aim and the result are usually\n"
                "caused by the neural network selecting the same color twice.\n");
        }
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


void ReduceHistogram(HistogramType& Histogram)
{
    for(size_t b=PaletteReductionMethod.size(), a=0; a<b; ++a)
    {
        if(!PaletteReductionMethod[a].entries.empty())
        {
            if(a == 0) Histogram.clear();
            unsigned n_colors = PaletteReductionMethod[a].entries.size();
            for(unsigned c=0; c<n_colors; ++c)
            {
                unsigned color = PaletteReductionMethod[a].entries[c];
                Histogram[color] += 1024*768;
            }
            std::fprintf(stderr,
                "Added a fixed palette of %u colors, now has %u colors\n",
                    n_colors, (unsigned) Histogram.size() );
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
            result.AddPaletteRGB( VecHistogram[n].first );
        result.Analyze();
        return result;
    }

    // First add those colors that were force-given on commandline.
    Palette result;
/*
    HistogramType done;
    for(size_t b=PaletteReductionMethod.size(), a=0; a<b; ++a)
    {
        if(!PaletteReductionMethod[a].entries.empty())
        {
            for(auto color: PaletteReductionMethod[a].entries)
            {
                ++done[color];
                if(result.Size() > MaxColors)
                {
                    goto skip_preset;
                }
                result.AddPaletteRGB(color);
            }
        }
    }
skip_preset:
*/
    for(HistogramType::const_iterator
        i = Histogram.begin();
        i != Histogram.end();
        ++i)
    {
        if(result.Size() >= MaxColors) break;
//        if(done.find(i->first) == done.end())
            result.AddPaletteRGB( i->first );
    }
    result.Analyze();
    return result;
}
