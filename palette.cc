#include <cstdio>
#include <algorithm>
#include <cmath>
#include <stdarg.h>

#include <assert.h>
#include <gd.h>

#include "palette.hh"
#include "pixel.hh"
#include "pixels/averagepixel.hh"
#include "fparser.hh"

std::vector<PaletteMethodItem> PaletteReductionMethod;

static const char colordiff_paramstr[] =
    "R1,G1,B1,A1,luma1,L1,a1,b1,C1,h1,"
    "R2,G2,B2,A2,luma2,L2,a2,b2,C2,h2";

static class FunctionParserWithPaletteCCVariables: public FunctionParser
{
public:
    FunctionParserWithPaletteCCVariables()
    {
        AddFunction("g",   FPGammaCorrect, 1);
        AddFunction("ug",  FPGammaUncorrect, 1);
        AddConstant("pi",  M_PI);
        AddConstant("e",   M_E);
    }
    static double FPGammaCorrect(const double* vars)
    {
        return GammaCorrect(vars[0]);
    }
    static double FPGammaUncorrect(const double* vars)
    {
        return GammaUncorrect(vars[0]);
    }
} colordiff_parser;

double DitherErrorFactor    = 1.0;
unsigned DitherMatrixWidth  = 8;
unsigned DitherMatrixHeight = 8;
unsigned TemporalDitherSize = 1; // 1 = no temporal dithering
bool     TemporalDitherMSB  = false;  // Use MSB rather than LSB for temporal dithering
unsigned DitherColorListSize = 0;
double   DitherCombinationContrast = -1.0;
double   DitherGamma         = 1.0;

ColorCompareMethod ColorComparing = Compare_RGB;
DitheringMethod Dithering = Dither_Yliluoma1;
DiffusionMethod Diffusion = Diffusion_None;

void SetColorCompareFormula(const std::string& expr)
{
    colordiff_parser.AddConstant("gamma", DitherGamma);
    int error = colordiff_parser.Parse(expr.c_str(), colordiff_paramstr);
    if(error >= 0)
    {
        fprintf(stderr, "Parse error (%s) in color difference formula:\n%s\n%*s\n",
            colordiff_parser.ErrorMsg(), expr.c_str(), error+1, "^");
        ColorComparing = Compare_RGB;
    }
    else
        colordiff_parser.Optimize();
}

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

double ColorCompare(const ColorInfo& meta1,
                    const ColorInfo& meta2)
{
  #define DEG2RAD(xx) (M_PI/180.0 * (xx))
    switch(ColorComparing)
    {
        default:
        case Compare_RGB:
        {
            int r1 = meta1.R, r2 = meta2.R;
            int g1 = meta1.G, g2 = meta2.G;
            int b1 = meta1.B, b2 = meta2.B;
            int a1 = meta1.A, a2 = meta2.A;
            int diffR = r1-r2, diffG = g1-g2, diffB = b1-b2, diffA = a1-a2;
            return diffR*diffR + diffG*diffG + diffB*diffB + diffA*diffA;
        }
        case Compare_CIE76_DeltaE:
        {
            int a1 = meta1.A, a2 = meta2.A;
            double dL = meta1.lab.L - meta2.lab.L;
            double da = meta1.lab.a - meta2.lab.a;
            double db = meta1.lab.b - meta2.lab.b;
            int diffA = a1-a2;
            // CIE normal deltaE squared
            return dL*dL + da*da + db*db + diffA*diffA;
        }
        case Compare_CIE94_DeltaE:
        {
            int a1 = meta1.A, a2 = meta2.A;
            double dL = meta1.lab.L - meta2.lab.L;
            double da = meta1.lab.a - meta2.lab.a;
            double db = meta1.lab.b - meta2.lab.b;
            int diffA = a1-a2;
            /* Although CIE94 and CIEDE2000 are theoretically more developed
             * than the plain CIE distance, in practise both have some kind
             * of issue with violet/purple colors, for which they deem black
             * as the best substitute.
             * (Does it somehow think purple is ultraviolet or something?)
             * (Bisqwit's hue circle + EGA palette test.)
             */

            // CIE94 deltaE color difference squared
            double C12 = std::sqrt(meta1.lab.C * meta2.lab.C); // Symmetric chromance
            double dC = meta2.lab.C - meta1.lab.C;
            double dCsq = dC*dC;
            double dHsq = da*da + db*db - dCsq; // delta hue squared
            if(dHsq < 0.0) dHsq = 0.0;
            double SC = 1.0 + 0.048 * C12; // weighting for delta chromance
            double SH = 1.0 + 0.014 * C12; // weighting for delta hue
            return dL*dL + dCsq / (SC*SC) + dHsq / (SH*SH) + diffA*diffA;
        }
        case Compare_CMC_lc:
        {
            const double l = 0.5, c = 1; // Lightness and chroma parameters
            int a1 = meta1.A, a2 = meta2.A;
            int diffA = a1-a2;
            double dl = meta2.lab.L - meta1.lab.L;
            if(meta1.lab.L <= 0.0 || meta2.lab.L <= 0.0)
                return dl*dl + diffA*diffA;
            double da = meta2.lab.a - meta1.lab.a;
            double db = meta2.lab.b - meta1.lab.b;
            double dC = meta2.lab.C - meta1.lab.C;
            double h1 = meta1.lab.h;
            if(h1 < 0) h1 += 2*M_PI;
            double T = (h1 >= DEG2RAD(164) && h1 <= DEG2RAD(345))
                ? 0.56 + std::fabs(0.2 * std::cos(h1 + DEG2RAD(168)))
                : 0.36 + std::fabs(0.4 * std::cos(h1 + DEG2RAD(35)));
            double C1p4 = meta1.lab.C; C1p4*=C1p4; C1p4*=C1p4; // C1 ^ 4
            double F = std::sqrt(C1p4 / (C1p4 + 1900));
            double SL = meta1.lab.L < 16 ? 0.511 : (0.040975*meta1.lab.L
                                                  /(1+0.01765*meta1.lab.L));
            double SC = 0.0638*meta1.lab.C / (1 + 0.0131*meta1.lab.C) + 0.0638;
            double SH = SC * (F*T + 1 - F);
            double dLSL = dl / (l*SL);
            double dCSC = dC / (c*SC);
            double dHsq = da*da + db*db - dC*dC;
            if(dHsq < 0.0) dHsq = 0.0;
            return dLSL*dLSL + dCSC*dCSC + dHsq / (SH*SH) + diffA*diffA;
        }
        case Compare_BFD_lc:
        {
            const double l = 1.5, c = 1; // Lightness and chroma parameters
            int a1 = meta1.A, a2 = meta2.A;
            int diffA = a1-a2;
            /*double bfd1 = 54.6 * std::log(Y1+1.5) - 9.6;
            double bfd2 = 54.6 * std::log(Y2+1.5) - 9.6;
            double deltaBFD = bfd2 - bfd1;*/
            double deltaBFD_l = (54.6/l) * (std::log((meta2.xyz.Y+1.5) / (meta1.xyz.Y+1.5)));
            double da = meta2.lab.a - meta1.lab.a;
            double db = meta2.lab.b - meta1.lab.b;
            double dC = meta2.lab.C - meta1.lab.C;
            double avgC = (meta2.lab.C + meta1.lab.C) * .5;
            double avgh = (meta2.lab.h + meta1.lab.h) * .5;
            double dc = 0.035 * avgC / (1 + 0.00365*avgC) + 0.521;
            double avgCp4 = avgC; avgCp4*=avgCp4; avgCp4*=avgCp4; // avgC ^ 4
            double g = std::sqrt(avgCp4 / (avgCp4 + 14000));
            double t = 0.627 + 0.055 * std::cos(  avgh - DEG2RAD(254))
                             - 0.040 * std::cos(2*avgh - DEG2RAD(136))
                             + 0.070 * std::cos(3*avgh - DEG2RAD(32))
                             + 0.049 * std::cos(4*avgh + DEG2RAD(114))
                             - 0.015 * std::cos(5*avgh - DEG2RAD(103));
            double dh = dc * (g*(t-1) + 1);
            double RH = -0.260 * std::cos(avgh   - DEG2RAD(308))
                       - 0.379 * std::cos(2*avgh - DEG2RAD(160))
                       - 0.636 * std::cos(3*avgh + DEG2RAD(254))
                       + 0.226 * std::cos(4*avgh + DEG2RAD(140))
                       - 0.194 * std::cos(5*avgh + DEG2RAD(280));
            double avgCp6 = avgC*avgC*avgC; avgCp6*=avgCp6; // avgC ^ 6
            double RC = std::sqrt(avgCp6 / (avgCp6 + 7e7));
            double dHsq = da*da + db*db - dC*dC;
            if(dHsq <= 0.0) dHsq = 0.0;
            double dCcdc = dC/(c*dc);
            return deltaBFD_l*deltaBFD_l + dCcdc*dCcdc + dHsq/(dh*dh)
                    + RH*RC*dC*std::sqrt(dHsq)/(dc*dh) + diffA*diffA;
        }

        case Compare_CIEDE2000_DeltaE:
        {
            /* From the paper "The CIEDE2000 Color-Difference Formula: Implementation Notes, */
            /* Supplementary Test Data, and Mathematical Observations", by */
            /* Gaurav Sharma, Wencheng Wu and Edul N. Dalal, */
            /* Color Res. Appl., vol. 30, no. 1, pp. 21-30, Feb. 2005. */
            /* Return the CIEDE2000 Delta E color difference measure squared, for two Lab values */

            int a1 = meta1.A, a2 = meta2.A;
            int diffA = a1-a2;
            const double v25p7 = 6103515625.0; // 25^7
            const double d180 = DEG2RAD(180), d360 = DEG2RAD(360);
            /* Compute Cromanance and Hue angles */
            double avgC = 0.5 * (meta1.lab.C + meta2.lab.C);
            double avgC2 = avgC; avgC2*=avgC2;
            double avgC7 = avgC2; avgC7*=avgC7; avgC7*=avgC2*avgC; // avgC ^ 7
            double Gt = 0.5 * std::sqrt(avgC7/(avgC7 + v25p7));
            double A1 = (1.5 - Gt) * meta1.lab.a;
            double A2 = (1.5 - Gt) * meta2.lab.a;
            double c1 = std::sqrt(A1 * A1 + meta1.lab.b * meta1.lab.b);
            double c2 = std::sqrt(A2 * A2 + meta2.lab.b * meta2.lab.b);
            double H1,H2;                   // Hue in 0..360 degrees range
            if(c1 < 1e-9) H1 = 0.0; else { H1=std::atan2(meta1.lab.b, A1); if(H1<0.0) H1+=d360; }
            if(c2 < 1e-9) H2 = 0.0; else { H2=std::atan2(meta2.lab.b, A2); if(H2<0.0) H2+=d360; }
            double dH = H2 - H1;
            /**/ if(dH < -d180) dH += d360;
            else if(dH >  d180) dH -= d360; // Hue difference in -180..180 degrees range
            double avgH = H1 + H2;
            if(c1 >= 1e-9 && c2 >= 1e-9)    // There are two hues rather than one; average them
            {
                // Average the closest way, i.e. the average
                // between 30 degrees and 350 degrees is not 190 degrees, but
                // rather, it is 20 degrees, because 350 degrees = -10 degrees.
                if(std::fabs(H1-H2) > d180) avgH += (avgH < d360) ? d360 : -d360;
                avgH *= 0.5;
                // The result is in 0..360 degrees range.
            }                               // Average hue in 0..360 degrees range
            /* Compute delta L, C and H */
            double DH = 2.0 * std::sqrt(c1 * c2) * std::sin(0.5 * dH);
            double avgL = 0.5 * (meta1.lab.L  + meta2.lab.L);
            double avgc = 0.5 * (c1 + c2);
            double T = 1.0
              - 0.17 * std::cos(avgH - DEG2RAD(30.0))
              + 0.24 * std::cos(2.0 * avgH)
              + 0.32 * std::cos(3.0 * avgH + DEG2RAD(6))
              - 0.2 * std::cos(4.0 * avgH - DEG2RAD(63));
            double hh = (avgH - DEG2RAD(275)) / DEG2RAD(25); // range: -11..+3.4
            double avgc2 = avgc; avgc2*=avgc2;
            double avgc7 = avgc2; avgc7*=avgc7; avgc7*=avgc2*avgc; // avgc ^ 7
            double L50sq = (avgL - 50.0); L50sq *= L50sq;
            double SL = 1.0 + (0.015 * L50sq) / std::sqrt(20.0 + L50sq);
            double SC = 1.0 + 0.045 * avgc;
            double SH = 1.0 + 0.015 * avgc * T;
            double RC = -2.0 * std::sqrt(avgc7/(avgc7 + v25p7));
            double RT = std::sin(DEG2RAD(30) * std::exp(-hh*hh)) * RC;
            double dLSL = (meta2.lab.L-meta1.lab.L)/SL, dcSC = (c2-c1) / SC, DHSH = DH/SH;
            return dLSL*dLSL + dcSC*dcSC + DHSH*DHSH - RT*dcSC*DHSH + diffA*diffA;
        }

        case Compare_fparser:
        {
            const double div255 = 1/255.0, div127 = 1/127.0, div255000 = 1/255e3;
            const double params[] =
            {
                meta1.R*div255, meta1.G*div255, meta1.B*div255, meta1.A*div127,
                meta1.luma*div255000,
                meta1.lab.L, meta1.lab.a, meta1.lab.b, meta1.lab.C, meta1.lab.h,
                meta2.R*div255, meta2.G*div255, meta2.B*div255, meta2.A*div127,
                meta2.luma*div255000,
                meta2.lab.L, meta2.lab.a, meta2.lab.b, meta2.lab.C, meta2.lab.h
            };
            return colordiff_parser.Eval(params);
        }
    }
  #undef DEG2RAD
}

std::pair<unsigned,double>
    Palette::FindClosestCombinationIndex(const ColorInfo& test_lab) const
{
    switch(ColorComparing)
    {
        case Compare_RGB:
        {
            KDTree<unsigned,4>::KDPoint q( test_lab.R, test_lab.G, test_lab.B, test_lab.A);
            return CombinationTree.nearest_info(q);
        }
        case Compare_CIE76_DeltaE:
        {
            KDTree<unsigned,4>::KDPoint q( test_lab.lab.L, test_lab.lab.a, test_lab.lab.b, test_lab.A );
            return CombinationTree.nearest_info(q);
        }
        default:
        {
            std::pair<unsigned,double> result(0u, -1.0);
            for(unsigned limit = Combinations.size(), index=0; index<limit; ++index)
            {
                double penalty = ColorCompare(test_lab, GetCombinationMeta(index));
                if(penalty < result.second || result.second == -1.0)
                    { result.first  = index;
                      result.second = penalty; }
            }
            return result;
        }
    }
}


static MixingPlan FindBestMixingPlan_KnollYliluoma
    (const ColorInfo& input,
     const Palette& pal)
{
    const double X = DitherErrorFactor; // Error multiplication value

    size_t GenerationLimit = DitherColorListSize;
    assert(GenerationLimit > 0);

    double rin_gamma = input.gammac.r; // What we want to accomplish
    double gin_gamma = input.gammac.g;
    double bin_gamma = input.gammac.b;
    double ain_gamma = input.gammac.a;
    double rer=rin_gamma, ger=gin_gamma, ber=bin_gamma, aer=ain_gamma;

    MixingPlan result;
    while(result.size() < GenerationLimit)
    {
        if(rer<0) rer=0; if(ger<0) ger=0; if(ber<0) ber=0; if(aer<0) aer=0;
        if(rer>1) rer=1; if(ger>1) ger=1; if(ber>1) ber=1; if(aer>1) aer=1;
        int ter = /**/GammaUncorrect/**/(rer)*255.0;
        int teg = /**/GammaUncorrect/**/(ger)*255.0;
        int teb = /**/GammaUncorrect/**/(ber)*255.0;
        int tea = /**//**/(aer)*127.0;
        int test = gdTrueColorAlpha(ter,teg,teb,tea);

        /*fprintf(stderr, "want:%.3f,%.3f,%.3f; test: %02X%02X%02X (%.3f,%.3f,%.3f)\n",
            rin_gamma,gin_gamma,bin_gamma,
            ter,teg,teb,
            rer,ger,ber);*/

        ColorInfo test_lab(test, rer,ger,ber,aer);

        //size_t capacity = GenerationLimit - result.size();

        std::pair<unsigned,double>
            comb_finding = pal.FindClosestCombinationIndex(test_lab);
        unsigned chosen = comb_finding.first;
        if(chosen >= pal.Combinations.size())
        {
            fprintf(stderr, "ERROR: chosen=%u/%u\n",
                chosen, (unsigned) pal.Combinations.size());
        }
        for(size_t i=0; i<pal.Combinations[chosen].indexlist.size(); ++i)
            result.push_back( pal.Combinations[chosen].indexlist[i] );
        rer += (rin_gamma - pal.Combinations[chosen].combination.gammac.r) * X;
        ger += (gin_gamma - pal.Combinations[chosen].combination.gammac.g) * X;
        ber += (bin_gamma - pal.Combinations[chosen].combination.gammac.b) * X;
        aer += (ain_gamma - pal.Combinations[chosen].combination.gammac.a) * X;
    }
    /*fprintf(stderr, "For %02X%02X%02X:", rin,gin,bin);
    for(size_t a=0; a<result.size(); ++a)
        fprintf(stderr, " [%2u]%06X",
            result[a],
            pal.GetColor(result[a]));
    fprintf(stderr, "\n");*/
    std::sort(result.begin(), result.end(), ComparePaletteLuma(pal));
    return result;
}

static MixingPlan FindBestMixingPlan_Yliluoma1
    (const ColorInfo& input,
     const Palette& pal)
{
    std::pair<unsigned,double>
        comb_finding = pal.FindClosestCombinationIndex(input);

    unsigned chosen = comb_finding.first;
    const std::vector<unsigned>& tmp = pal.Combinations[chosen].indexlist;
    return MixingPlan( tmp.begin(), tmp.end() );
}

static MixingPlan FindBestMixingPlan_Yliluoma2
    (const ColorInfo& input,
     const Palette& pal)
{
    const unsigned NumCombinations = pal.NumCombinations();

    size_t GenerationLimit = DitherColorListSize;
    assert(GenerationLimit > 0);

    MixingPlan result;
    double so_far[4] = {0,0,0,0};
    unsigned proportion_total = 0;

    while(result.size() < GenerationLimit)
    {
        unsigned chosen_amount = 1;
        unsigned chosen        = 0;

        double least_penalty = -1;

        for(unsigned i=0; i<NumCombinations; ++i)
        {
            unsigned count = pal.Combinations[i].indexlist.size();
            const unsigned max_test_count_comb =
                count==1
                    ? (proportion_total ? proportion_total : 1)
                    : std::min(proportion_total,
                               (unsigned)GenerationLimit-proportion_total-count);

            double sum[4] = { so_far[0], so_far[1], so_far[2], so_far[3] };
            double add[4] = { pal.Combinations[i].combination.gammac.r*count,
                              pal.Combinations[i].combination.gammac.g*count,
                              pal.Combinations[i].combination.gammac.b*count,
                              pal.Combinations[i].combination.gammac.a*count };
            for(unsigned p=1; p<=max_test_count_comb; p*=2)
            {
                for(unsigned c=0; c<4; ++c) sum[c] += add[c];
                for(unsigned c=0; c<4; ++c) add[c] += add[c];
                double t = 1.0 / (proportion_total + p*count);

                int test = gdTrueColorAlpha( (int)(255*GammaUncorrect(sum[0]*t)),
                                             (int)(255*GammaUncorrect(sum[1]*t)),
                                             (int)(255*GammaUncorrect(sum[2]*t)),
                                             (int)(127*(sum[3]*t)) );
                ColorInfo test_lab(test, sum[0]*t, sum[1]*t, sum[2]*t, sum[3]*t);
                double penalty = ColorCompare(input, test_lab);
                if(penalty < least_penalty || least_penalty < 0)
                    { least_penalty = penalty;
                      chosen = i;
                      chosen_amount = p; }
            }
        }

        unsigned count = pal.Combinations[chosen].indexlist.size();
        for(unsigned i=0; i<count; ++i)
            result.resize(result.size() + chosen_amount,
                          pal.Combinations[chosen].indexlist[i]);

        proportion_total += chosen_amount*count;

        so_far[0] += pal.Combinations[chosen].combination.gammac.r * (chosen_amount*count);
        so_far[1] += pal.Combinations[chosen].combination.gammac.g * (chosen_amount*count);
        so_far[2] += pal.Combinations[chosen].combination.gammac.b * (chosen_amount*count);
        so_far[3] += pal.Combinations[chosen].combination.gammac.a * (chosen_amount*count);
    }

    std::sort(result.begin(), result.end(), ComparePaletteLuma(pal));
/*
    fprintf(stderr, "For %02X%02X%02X:", rin,gin,bin);
    for(size_t a=0; a<result.size(); ++a)
        fprintf(stderr, " [%2u]%06X",
            result[a],
            pal.GetColor(result[a]));
    fprintf(stderr, "\n");*/
    return result;
}

static MixingPlan FindBestMixingPlan_Yliluoma3
    (const ColorInfo& input,
     const Palette& pal)
{
/***

IDEA:
    Result is a map<color, count>.

    For a 8x8 map, it begins with <closest color, 64>.

    For each more-than-1 color in the result,
    figure out if there exists a pair of colors,
    that could replace that color in 50%-50% proportion.
    Such as 64x blue --> 32x red + 32x violet
            32x red --> 16x someother + 16x yadah.
    Possibly, only consider colors that do not
    currently exist in the map.

*******/

    const unsigned NumCombinations = pal.NumCombinations();

    std::map<
        unsigned, unsigned,
        std::less<unsigned>, FSBAllocator<int>
    > Solution;

    // The penalty of our currently "best" solution.
    double current_penalty = -1;

    // First, find the closest color to the input color.
    if(1)
    {
        std::pair<unsigned,double>
            comb_finding = pal.FindClosestCombinationIndex(input);
        unsigned chosen = comb_finding.first;
        unsigned count = pal.Combinations[chosen].indexlist.size();
        unsigned proportion = 0;
        for(unsigned i=0; i<count; ++i)
        {
            unsigned index = pal.Combinations[chosen].indexlist[i];
            proportion += DitherColorListSize;
            unsigned ThisProportion = proportion / count;
            proportion %= count;
            if(ThisProportion > 0)
                Solution[index] = ThisProportion;
        }
    }

    double dbllimit = 1.0 / DitherColorListSize;
    while(current_penalty > 0.0)
    {
        // Find out if there is a region in Solution that
        // can be split in two for benefit.
        double   best_penalty      = current_penalty;
        unsigned best_splitfrom    = ~0u;
        unsigned best_split_to[2]  = { 0,0};

        //printf("pen=%g, cur=%g\n", best_penalty, current_penalty);

        double current_sum[4] = {0,0,0,0};
        for(std::map<unsigned,unsigned>::iterator
            j = Solution.begin();
            j != Solution.end();
            ++j)
        {
            current_sum[0] += pal.Data[ j->first ].gammac.r * j->second * dbllimit;
            current_sum[1] += pal.Data[ j->first ].gammac.g * j->second * dbllimit;
            current_sum[2] += pal.Data[ j->first ].gammac.b * j->second * dbllimit;
            current_sum[3] += pal.Data[ j->first ].gammac.a * j->second * dbllimit;
        }

        for(std::map<unsigned,unsigned>::iterator
            i = Solution.begin();
            i != Solution.end();
            ++i)
        {
            //if(i->second <= 1) continue;
            //printf("Try split %06X:%u...\n", i->first,i->second);
            unsigned split_color = i->first;
            unsigned split_count = i->second;
            double portion_total = split_count * dbllimit;

            double sum[4] = {
                current_sum[0] - pal.Data[split_color].gammac.r * portion_total,
                current_sum[1] - pal.Data[split_color].gammac.g * portion_total,
                current_sum[2] - pal.Data[split_color].gammac.b * portion_total,
                current_sum[3] - pal.Data[split_color].gammac.a * portion_total };

            if(true)
            {
                double portion1 = (split_count / 2            ) * dbllimit;
                double portion2 = (split_count - split_count/2) * dbllimit;
                for(unsigned c=0; c<NumCombinations; ++c)
                {
                    if(pal.Combinations[c].indexlist.size() == 1
                    || (split_count > 1
                     && pal.Combinations[c].indexlist.size() == 2)
                      )
                    {
                        unsigned a = pal.Combinations[c].indexlist.front();
                        unsigned b = pal.Combinations[c].indexlist.back();
                        if(a == split_color || b == split_color) continue;
                        if(portion1 == portion2 || a == b)
                        {
                            const double R = pal.Combinations[c].combination.gammac.r * portion_total;
                            const double G = pal.Combinations[c].combination.gammac.g * portion_total;
                            const double B = pal.Combinations[c].combination.gammac.b * portion_total;
                            const double A = pal.Combinations[c].combination.gammac.a * portion_total;

                            int test = gdTrueColorAlpha( (int)(255*GammaUncorrect(sum[0]+R)),
                                                         (int)(255*GammaUncorrect(sum[1]+G)),
                                                         (int)(255*GammaUncorrect(sum[2]+B)),
                                                         (int)(127*(sum[3]+A)) );

                            // Figure out if this split is better than what we had
                            ColorInfo test_lab(test, sum[0]+R, sum[1]+G, sum[2]+B, sum[3]+A);
                            double penalty = ColorCompare(input, test_lab);

                            if(penalty < best_penalty)
                            {
                                best_penalty   = penalty;
                                best_splitfrom = split_color;
                                best_split_to[0] = a;
                                best_split_to[1] = b;
                            }
                        }
                        else // portion1 != portion2 (split_count was uneven).
                        {
                          if(1)
                          {
                            const double R = pal.Data[a].gammac.r*portion1 + pal.Data[b].gammac.r*portion2;
                            const double G = pal.Data[a].gammac.g*portion1 + pal.Data[b].gammac.g*portion2;
                            const double B = pal.Data[a].gammac.b*portion1 + pal.Data[b].gammac.b*portion2;
                            const double A = pal.Data[a].gammac.a*portion1 + pal.Data[b].gammac.a*portion2;

                            int test = gdTrueColorAlpha( (int)(255*GammaUncorrect(sum[0]+R)),
                                                         (int)(255*GammaUncorrect(sum[1]+G)),
                                                         (int)(255*GammaUncorrect(sum[2]+B)),
                                                         (int)(127*(sum[3]+A)) );

                            // Figure out if this split is better than what we had
                            ColorInfo test_lab(test, sum[0]+R, sum[1]+G, sum[2]+B, sum[3]+A);
                            double penalty = ColorCompare(input, test_lab);

                            if(penalty < best_penalty)
                            {
                                best_penalty   = penalty;
                                best_splitfrom = split_color;
                                best_split_to[0] = a;
                                best_split_to[1] = b;
                            }
                          }
                          if(1)
                          {
                            const double R = pal.Data[b].gammac.r*portion1 + pal.Data[a].gammac.r*portion2;
                            const double G = pal.Data[b].gammac.g*portion1 + pal.Data[a].gammac.g*portion2;
                            const double B = pal.Data[b].gammac.b*portion1 + pal.Data[a].gammac.b*portion2;
                            const double A = pal.Data[b].gammac.a*portion1 + pal.Data[a].gammac.a*portion2;

                            int test = gdTrueColorAlpha( (int)(255*GammaUncorrect(sum[0]+R)),
                                                         (int)(255*GammaUncorrect(sum[1]+G)),
                                                         (int)(255*GammaUncorrect(sum[2]+B)),
                                                         (int)(127*(sum[3]+A)) );

                            // Figure out if this split is better than what we had
                            ColorInfo test_lab(test, sum[0]+R, sum[1]+G, sum[2]+B, sum[3]+A);
                            double penalty = ColorCompare(input, test_lab);

                            if(penalty < best_penalty)
                            {
                                best_penalty   = penalty;
                                best_splitfrom = split_color;
                                best_split_to[0] = b;
                                best_split_to[1] = a;
                            }
                          }
                        }
                    } // == 2
                } // for NumCombinations
            }
        }
        if(best_penalty == current_penalty) break; // No better solution was found.

        std::map<unsigned,unsigned>::iterator i = Solution.find(best_splitfrom);
        unsigned split_count = i->second;
        unsigned split1 = split_count/2;
        unsigned split2 = split_count - split1;
        Solution.erase(i);
        if(split1 > 0) Solution[best_split_to[0]] += split1;
        if(split2 > 0) Solution[best_split_to[1]] += split2;
        current_penalty = best_penalty;
    }

    // Sequence the solution.
    MixingPlan result;
    for(std::map<unsigned,unsigned>::iterator
        i = Solution.begin(); i != Solution.end(); ++i)
    {
        result.resize(result.size() + i->second, i->first);
    }
    // Sort the colors according to luminance
    std::sort(result.begin(), result.end(), ComparePaletteLuma(pal));
    return result;
}

MixingPlan FindBestMixingPlan(const ColorInfo& input, const Palette& pal)
{
    switch(Dithering)
    {
        case Dither_Yliluoma1:
            return FindBestMixingPlan_Yliluoma1(input, pal);
        case Dither_KnollYliluoma:
            return FindBestMixingPlan_KnollYliluoma(input, pal);
        case Dither_Yliluoma2:
            return FindBestMixingPlan_Yliluoma2(input, pal);
        case Dither_Yliluoma3:
            return FindBestMixingPlan_Yliluoma3(input, pal);
    }
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
                        int r = gdImageRed(im,c);
                        int g = gdImageGreen(im,c);
                        int b = gdImageBlue(im,c);
                        int a = gdImageAlpha(im,c);
                        unsigned color = gdTrueColorAlpha(r,g,b, a);
                        Histogram[color] += 1024*768;
                    }
                    std::fprintf(stderr,
                        "Loaded %u colors from %s, now has %u colors\n",
                            n_colors, fn.c_str(),
                            (unsigned) Histogram.size() );
                    gdImageDestroy(im);
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
            result.AddPaletteRGB( VecHistogram[n].first );
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
        result.AddPaletteRGB( i->first );
    }
    result.Analyze();
    return result;
}

// How VERY awkward it is when one wants to change a nested loops
// algorithm into a recursive form so that the number of nested loops
// can be arbitrary. Dozens of variables...
static void MakeCombinations(
    const std::vector<Palette::PaletteItem>& palData,
    const std::vector<int>& palDifferences,
    std::vector<Palette::Combination>& Combinations,
    unsigned begin, std::vector<unsigned>& indices,
    double rsum, double gsum, double bsum, double asum,
    size_t RecursionLimit,
    size_t ChangesLimit,
    const int MaxDifference)
{
    if(RecursionLimit <= 0) return;

    double mul = 1.0 / (indices.size() + 1);

    const unsigned RecursionLevel = indices.size();
    indices.push_back(0);
    const unsigned PaletteSize = palData.size();

    for(unsigned b = begin; b < PaletteSize; ++b)
    {
        bool too_big = false;
        for(unsigned a=0; a<RecursionLevel; ++a)
            if( palDifferences[ b*PaletteSize + indices[a] ] > MaxDifference)
            {
                too_big = true;
                break;
            }
        if(too_big) continue;

        const double rn = palData[b].gammac.r; // Gamma corrected R,G,B
        const double gn = palData[b].gammac.g;
        const double bn = palData[b].gammac.b;
        const double an = palData[b].gammac.a;

        double our_sum_r = rsum+rn;
        double our_sum_g = gsum+gn;
        double our_sum_b = bsum+bn;
        double our_sum_a = asum+an;
        indices.back() = b;

        if(true)
        {
            double combined_r = our_sum_r * mul;
            double combined_g = our_sum_g * mul;
            double combined_b = our_sum_b * mul;
            double combined_a = our_sum_a * mul;
            const int cr = ((int)(255*GammaUncorrect(combined_r)+0.5));
            const int cg = ((int)(255*GammaUncorrect(combined_g)+0.5));
            const int cb = ((int)(255*GammaUncorrect(combined_b)+0.5));
            const int ca = ((int)(127*              (combined_a)+0.5));

            uint32 uncorrected_rgb = gdTrueColorAlpha(cr,cg,cb, ca);
            //^  Gamma uncorrected R,G,B

            // Do not insert combinations that yield the lone component color
            if(RecursionLevel == 0
            || uncorrected_rgb != palData[b].rgb)
            {
                if(verbose >= 3)
                {
                    printf("Candidate %u, %06X(%.4f,%.4f,%.4f): ",
                        unsigned(Combinations.size()),
                        uncorrected_rgb,
                        combined_r,combined_g,combined_b);
                    for(size_t a=0; a<indices.size(); ++a)
                        printf(" %06X", palData[ indices[a] ].rgb);
                    printf("\n");
                }
                Combinations.push_back( Palette::Combination(
                    indices,
                    uncorrected_rgb,
                    combined_r,combined_g,combined_b,combined_a // Gamma corrected R,G,B
                ) );
            }
        }

        size_t WasSame = RecursionLevel == 0 ? 0
                       : b == indices[indices.size()-2];

        unsigned next_begin = b;
        if(Dithering != Dither_Yliluoma1
        && Dithering != Dither_KnollYliluoma) // || RecursionLimit > 2)
            ++next_begin;

        MakeCombinations(palData, palDifferences,
            Combinations,
            next_begin,indices, our_sum_r,our_sum_g,our_sum_b,our_sum_a,
            RecursionLimit-1,
            ChangesLimit - WasSame,
            MaxDifference);
    }
    indices.pop_back();
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

    int    average_luma_difference = 0;
    int    maximum_luma_difference = 0;
    for(unsigned a=1; a<PaletteSize; ++a)
    {
        int    diff = GetLuma(luma_order[a]) - GetLuma(luma_order[a-1]);
        average_luma_difference += diff;
        if(diff > maximum_luma_difference)
            maximum_luma_difference = diff;
    }
    average_luma_difference /= (PaletteSize - 1);

    int    total_luma = GetLuma(luma_order.back()) - GetLuma(luma_order.front());

    int    luma_threshold = 0;

    double p = DitherCombinationContrast;
    if(p < 0.0 || p > 3.0)
    {
        switch(Dithering)
        {
            case Dither_Yliluoma1:
                p = 2.0;
                break;
            case Dither_Yliluoma2:
                p = 1.0;
                break;
            case Dither_Yliluoma3:
                p = 1.0;
                break;
            case Dither_KnollYliluoma:
                p = 0.0; // "Reasonable default"
        }
    }

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
            std::printf("Luma[%3u]: %6d (%3u = #%06X)\n", a, GetLuma(luma_order[a]),
                luma_order[a], Data[luma_order[a]].rgb);
        std::printf("Average: %d, max: %d, total: %d, threshold: %d\n",
            average_luma_difference,
            maximum_luma_difference,
            total_luma,
            luma_threshold);
    }

    // Create some combinations
    size_t RecursionLimit = 4;
    size_t ChangesLimit   = RecursionLimit;
    switch(Dithering)
    {
        case Dither_Yliluoma1:
            RecursionLimit = DitherColorListSize;
            ChangesLimit   = DitherColorListSize;
            break;
        case Dither_Yliluoma2:
        case Dither_Yliluoma3:
            RecursionLimit = 2;
            break;
        case Dither_KnollYliluoma:
            RecursionLimit = DitherColorListSize > 2
                                 ? (DitherColorListSize+1)/2
                                 : DitherColorListSize;
            if(RecursionLimit > 8) RecursionLimit = 8;
            ChangesLimit   = RecursionLimit;
            if(ChangesLimit > 3) ChangesLimit = 3;
    }

    if(verbose >= 1)
    {
        std::printf("Calculating palette-color combinations... ");
        std::fflush(stdout);
    }

    std::vector<int> palDifferences( PaletteSize*PaletteSize, 0 );
    for(unsigned a=0; a<PaletteSize; ++a)
    for(unsigned b=a+1; b<PaletteSize; ++b)
    {
        palDifferences[a*PaletteSize+b] =
        palDifferences[b*PaletteSize+a] =
            std::abs( GetLuma(a) - GetLuma(b) );
    }

    std::vector<unsigned> temp;
    MakeCombinations(Data,
        palDifferences,
        Combinations,
        0, temp, 0,0,0,0,
        RecursionLimit,
        ChangesLimit,
        luma_threshold);

    if(Dithering == Dither_Yliluoma1)
    {
      #pragma omp parallel for schedule(static)
        for(size_t a=0; a<Combinations.size(); ++a)
        {
            std::vector<unsigned>& indices = Combinations[a].indexlist;
            std::sort(indices.begin(), indices.end(), ComparePaletteLuma(*this));
        }
    }

    if(verbose >= 1)
    {
        std::printf("Done.\n");
        std::fflush(stdout);
    }

    if(verbose >= 2)
    {
        std::printf("Total palette-color combinations chosen for dithering candidates: %u\n",
            (unsigned) Combinations.size());
        switch(Dithering)
        {
            case Dither_Yliluoma1: break;
            case Dither_KnollYliluoma:
                std::printf("Dithering will maximally do %u * (%u + %u) = %u * %u = %u tests per pixel.\n",
                    DitherColorListSize,
                    (unsigned) Data.size(),
                    (unsigned) Combinations.size(),
                    DitherColorListSize,
                    (unsigned) (Data.size() + Combinations.size()),
                    (unsigned) (DitherColorListSize * (Data.size() + Combinations.size()))
                );
            case Dither_Yliluoma2:break;
            case Dither_Yliluoma3:break;
        }
        std::fflush(stdout);
    }

    for(size_t a=0; a<Combinations.size(); ++a)
    {
        if(ColorComparing == Compare_RGB)
        {
            const ColorInfo& meta = Combinations[a].combination;
            KDTree<unsigned,4>::KDPoint p( meta.R, meta.G, meta.B, meta.A );
            CombinationTree.insert(p, a);
        }
        else if(ColorComparing == Compare_CIE76_DeltaE)
        {
            const ColorInfo& meta = Combinations[a].combination;
            KDTree<unsigned,4>::KDPoint p( meta.lab.L, meta.lab.a, meta.lab.b, meta.A );
            CombinationTree.insert(p, a);
        }
    }

    if(verbose >= 2
    && (ColorComparing == Compare_RGB
     || ColorComparing == Compare_CIE76_DeltaE) )
    {
        std::printf("Palette search trees created.\n");
        std::fflush(stdout);
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
        AddPaletteRGB(val);
    }
    va_end(ap);
    Analyze();
}

void Palette::AddPaletteRGB(uint32 val)
{
    Data.push_back( val);
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
    switch(ColorComparing)
    {
        case Compare_CIE76_DeltaE:
            return illuminants[2];
        default:
            return illuminants[0];
    }
}

template<bool HaveLuma, bool HaveGamma, bool HaveXYZ, bool HaveLAB>
void ColorInfo::MakeNeeded()
{
    // Luma is always needed (Analyze and most dithers break without it)
    if(!HaveLuma)
    {
        luma = R*299 + G*587 + B*114;
    }

    // Gamma is always needed (all dithers use it, including error diffusion)
    if(!HaveGamma)
    {
        gammac.r = GammaCorrect( R/255.0 );
        gammac.g = GammaCorrect( G/255.0 );
        gammac.b = GammaCorrect( B/255.0 );
        gammac.a = A/127.0;
    }

    // Don't calculate L*a*b* if we will not need it.
    // Only RGB comparison doesn't need it.
    if(!HaveLAB
    && ColorComparing != Compare_RGB)
    {
        if(!HaveXYZ)
        {
            const double* illum = GetIlluminant();
            // double Rn = gammac.r, Gn = gammac.g, Bn = gammac.b;
            double Rn = R/255.0, Gn=G/255.0, Bn=B/255.0;
            xyz.X = (illum[0])*Rn + (illum[3])*Gn + (illum[6])*Bn;
            xyz.Y = (illum[1])*Rn + (illum[4])*Gn + (illum[7])*Bn;
            xyz.Z = (illum[2])*Rn + (illum[5])*Gn + (illum[8])*Bn;
        }
        // Use Profile illuminant - D65 */
        const double* illum = GetIlluminant();
        double Xn = 1.0 / (illum[0]+illum[1]+illum[2]);
        double Yn = 1.0 / (illum[3]+illum[4]+illum[5]);
        double Zn = 1.0 / (illum[6]+illum[7]+illum[8]);
        double x = xyz.X * Xn;
        double y = xyz.Y * Yn;
        double z = xyz.Z * Zn;
        const double threshold1 = (6*6*6.0)/(29*29*29.0);
        const double threshold2 = (29*29.0)/(6*6*3.0);
        double x1 = (x > threshold1) ? pow(x, 1.0/3.0) : (threshold2*x)+(4/29.0);
        double y1 = (y > threshold1) ? pow(y, 1.0/3.0) : (threshold2*y)+(4/29.0);
        double z1 = (z > threshold1) ? pow(z, 1.0/3.0) : (threshold2*z)+(4/29.0);
        lab.L = (29*4)*y1 - (4*4);
        lab.a = (500*(x1-y1) );
        lab.b = (200*(y1-z1) );

        // Of color comparison algorithms that
        // might need L*a*b*, only CIE76 doesn't need C & h
        if(ColorComparing != Compare_CIE76_DeltaE)
        {
            lab.C = std::sqrt(lab.a*lab.a + lab.b+lab.b);
            lab.h = std::atan2(lab.b, lab.a);
        }
    }
}

ColorInfo::ColorInfo(uint32 val)
{
    rgb = val;
    MakeNeeded<false,false,false,false> ();
}

ColorInfo::ColorInfo(uint32 val, double rr,double gg,double bb,double aa)
{
    rgb = val;
    gammac.r = rr;
    gammac.g = gg;
    gammac.b = bb;
    gammac.a = aa;
    MakeNeeded<false,true,false,false> ();
}

ColorInfo::ColorInfo(int rr,int gg,int bb, int aa)
{
    rgb = gdTrueColorAlpha(rr,gg,bb, aa);
    MakeNeeded<false,false,false,false> ();
}

Palette Palette::GetSlice(unsigned begin, unsigned count) const
{
    Palette result;
    result.Data.assign( Data.begin()+begin, Data.begin()+begin+count );
    return result;
}

std::vector<unsigned> CreatePowerofTwoDitheringMatrix(unsigned Width, unsigned Height)
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
    return CreatePowerofTwoDitheringMatrix(DitherMatrixWidth, DitherMatrixHeight);
}

std::vector<unsigned> CreateTemporalDitheringMatrix()
{
    return CreatePowerofTwoDitheringMatrix(1, TemporalDitherSize);
}
