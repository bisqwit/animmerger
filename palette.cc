#include <cstdio>
#include <algorithm>

#include "palette.hh"
#include "pixel.hh"
#include "pixels/averagepixel.hh"

std::vector<PaletteMethodItem> PaletteReductionMethod;

namespace
{
    typedef std::pair<uint32, unsigned> HistItem;

    unsigned ColorDiff(int r1,int g1,int b1, int r2,int g2,int b2)
    {
        /*
        double y1 = 0.299*r1+0.587*g1+0.114*b1;
        double c1 =-0.299*r1-0.587*g1+0.886*b1;
        double C1 = 0.701*r1-0.587*g1-0.114*b1;
        double y2 = 0.299*r2+0.587*g2+0.114*b2;
        double c2 =-0.299*r2-0.587*g2+0.886*b2;
        double C2 = 0.701*r2-0.587*g2-0.114*b2;
        double ydiff = y1-y2, cdiff = c1-c2, Cdiff = C1-C2;
        return unsigned ( (ydiff*ydiff + (cdiff*cdiff + Cdiff*Cdiff)) + 0.5 );
        */
        int rdiff = r1-r2, gdiff = g1-g2, bdiff = b1-b2;
        return rdiff*rdiff + gdiff*gdiff + bdiff*bdiff;
        /**/
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
    struct AdaptiveSlot
    {
        unsigned begin, count;
        unsigned pixels;
    };
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

    void ReduceHistogram_Diversity(HistogramType& Histogram, unsigned target_colors,
                                   bool blend=false)
    {
        if(Histogram.size() <= target_colors) return;

        std::vector<HistItem> VecHistogram( Histogram.begin(), Histogram.end() );
        std::vector<uint32> MinDist( VecHistogram.size(), 0x7FFFFFFFu );
        std::vector<unsigned> Closest( VecHistogram.size() );
        /* 1. initialize min_dist and sort the colors in order of popularity. */
        std::sort(VecHistogram.begin(), VecHistogram.end(), CompareSecondRev);
        /* 2. Choose colors one at a time */
        unsigned nadapt;
        for(nadapt=0; nadapt<target_colors; ++nadapt)
        {
            /* 2.1. Choose the color to be added */
            unsigned chosen = 0;
            if(nadapt == 0 || (nadapt >= 10 && nadapt % 2 == 0))
            {
                /* 2.1a. Choose based on popularity from unchosen colors; we've
                 * sorted them on popularity, so just choose the first in the list */
                for(; chosen < VecHistogram.size(); ++chosen)
                    if(MinDist[chosen]) break;
            }
            else
            {
                /* 2.1b. Choose based on diversity from unchosen colors */
                uint32 chosen_dist = 0;
                for(unsigned i=0; i<VecHistogram.size(); ++i)
                    if(MinDist[i] > chosen_dist)
                        chosen_dist = MinDist[chosen = i];
            }
            /* 2.2. Add the color */
            MinDist[chosen] = 0;
            Closest[chosen] = nadapt;
            /* 2.3. Adjust the MinDist array */
            uint32 chosencolor = VecHistogram[chosen].first;
            for(unsigned i=0; i<VecHistogram.size(); ++i)
                if(MinDist[i])
                {
                    unsigned dist = ColorDiff(chosencolor, VecHistogram[i].first);
                    if(dist < MinDist[i])
                    {
                        MinDist[i] = dist;
                        Closest[i] = nadapt;
                    }
                }
        }
        /* 3. Make the new palette by choosing one color from each slot. */
        Histogram.clear();
        if(blend)
        {
            for(unsigned i=0; i<nadapt; ++i)
            {
                AveragePixel total;
                unsigned match=0, pixel_total=0, mismatch_pixel_total=0;
                for(unsigned j=0; j<VecHistogram.size(); ++j)
                    if(Closest[j] == i)
                    {
                        total.set_n( VecHistogram[j].first,
                                     VecHistogram[j].second );
                        pixel_total += VecHistogram[j].second;
                        if(MinDist[j])
                            mismatch_pixel_total += VecHistogram[j].second;
                        else
                            match = j;
                    }
                /* Only blend if total number of mismatched pixels
                 * exceeds total number of matched pixels by a large margin. */
                if(3*mismatch_pixel_total <= 2*pixel_total)
                    Histogram[ VecHistogram[match].first ] += 1;
                else
                {
                    unsigned pixel = VecHistogram[match].second * 2;
                    total.set_n( VecHistogram[match].first, pixel );
                    Histogram[ total.get() ] += 1;
                }
            }
        }
        else
        {
            for(unsigned i=0; i<nadapt; ++i)
            {
                unsigned match = 0;
                for(unsigned j=0; j<VecHistogram.size(); ++j)
                    if(Closest[j] == i && !MinDist[j])
                        { match = j; break; }
                Histogram[ VecHistogram[match].first ] += 1;
            }
        }
        fprintf(stderr, "%s-reduced to %u colors (aimed for %u)\n",
            blend ? "Blend-diversity" : "Diversity",
            (unsigned) Histogram.size(),
            nadapt);
    }

    void ReduceHistogram_BlendDiversity(HistogramType& Histogram, unsigned target_colors)
    {
        ReduceHistogram_Diversity(Histogram, target_colors, true);
    }

    void ReduceHistogram_MedianCut(HistogramType& Histogram, unsigned target_colors)
    {
        if(Histogram.size() <= target_colors) return;

        std::vector<AdaptiveSlot> slots(target_colors);
        std::vector<HistItem> VecHistogram( Histogram.begin(), Histogram.end() );
        std::sort(VecHistogram.begin(), VecHistogram.end(), CompareSecond);

        unsigned n_pixels = 0;
        for(unsigned a=0; a<VecHistogram.size(); ++a)
            n_pixels += VecHistogram[a].second;

        /* 1. Set up the first slot, containing all pixels */
        slots[0].begin  = 0;
        slots[0].count  = VecHistogram.size();
        slots[0].pixels = n_pixels;
        /* 2. Split slots until we have enough */
        unsigned nadapt;
        for(nadapt=1; nadapt<target_colors; ++nadapt)
        {
            /* 2.1. Pick the slot to split. */
            unsigned splitpos=0, splitpixels=0;
            for(unsigned i=0; i<nadapt; ++i)
                if(slots[i].count >= 2 && slots[i].pixels >= splitpixels)
                    { splitpos=i; splitpixels=slots[i].pixels; }
            if(!splitpixels) break;
            /* 2.2. Find its extent. */
            AdaptiveSlot& split = slots[splitpos];
            HistItem* slice_begin = &VecHistogram[split.begin];
            HistItem* slice_end   = slice_begin + split.count;
            int minr=255,ming=255,minb=255, maxr=0,maxg=0,maxb=0;
            for(HistItem* s = slice_begin; s != slice_end; ++s)
            {
                int r=(s->first>>16)&0xFF, g=(s->first>>8)&0xFF, b=s->first&0xFF;
                if(r<minr) minr=r; if(r>maxr) maxr=r;
                if(g<ming) ming=g; if(g>maxg) maxg=g;
                if(b<minb) minb=b; if(b>maxb) maxb=b;
            }
            /* 2.3. Sort colors according to the channel that has largest luminance */
            double rdiff = 0.299 * (maxr-minr);
            double gdiff = 0.587 * (maxg-ming);
            double bdiff = 0.114 * (maxb-minb);
            std::sort(slice_begin, slice_end,
                (rdiff >= gdiff && rdiff >= bdiff) ? CompareRed
              : (gdiff >= bdiff) ? CompareGreen
              : CompareBlue);
            /* 2.4 Decide where to split the slot and split it there. */
            unsigned half_pixels = split.pixels/2;
            unsigned pixel_accum = slice_begin->second;
            unsigned i=1;
            for(; i<split.count-1 && pixel_accum<half_pixels; ++i)
                pixel_accum += slice_begin[i].second;
            unsigned diff1 = 2*pixel_accum - split.count;
            unsigned diff2 = split.count - 2*(pixel_accum - slice_begin[i-1].second);
            if(diff2 < diff1 && i > 1) pixel_accum -= slice_begin[--i].second;
            slots[nadapt].begin = split.begin + i;
            slots[nadapt].count = split.count - i;
            slots[nadapt].pixels = split.pixels - pixel_accum;
            split.count  = i;
            split.pixels = pixel_accum;
        }
        /* 3. Make a new palette by choosing one color from each slot. */
        Histogram.clear();
        for(unsigned i=0; i<nadapt; ++i)
        {
            AveragePixel pix;
            for(unsigned a=0; a<slots[i].count; ++a)
                pix.set_n( VecHistogram[ slots[i].begin+a ].first,
                           VecHistogram[ slots[i].begin+a ].second );
            Histogram[ pix.get() ] += 1;
        }

        fprintf(stderr, "Median-cut-reduced to %u colors (aimed for %u)\n",
            (unsigned) Histogram.size(),
            nadapt);
    }

    void ReduceHistogram_Merging(HistogramType& Histogram, unsigned target_colors)
    {
        while(Histogram.size() > target_colors)
        {
            fprintf(stderr, "%u colors, %u wanted\n",
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
            pix.set(i->first);
            pix.set(j->first);
            Histogram.erase(i);
            Histogram.erase(j);
            Histogram[ pix.get() ] += combined_count;
        }
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
    if(PaletteSize <= 32)
    {
        const double input_luma = rin*0.299 + gin*0.587 + bin*0.114;

        std::vector<double> luma_table(PaletteSize);
        for(unsigned a=0; a<PaletteSize; ++a)
        {
            const uint32 pix = Palette[a];
            int r1 = (pix >> 16) & 0xFF, g1 = (pix >> 8) & 0xFF, b1 = (pix) & 0xFF;
            luma_table[a] = r1*0.299 + g1*0.587 + b1*0.114;
        };

        unsigned best_diff=0;
        for(unsigned pa=0; pa<PaletteSize; ++pa)
        {
            double luma1 = luma_table[pa];
            if(luma1 > input_luma) continue;
            const uint32 pix1 = Palette[pa];
            int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;

            for(unsigned pb=0; pb<PaletteSize; ++pb)
            {
                double luma2 = luma_table[pb];
                if(luma2 < luma1) continue;
                if(luma2 < input_luma) continue;
                const uint32 pix2 = Palette[pb];
                int r2 = (pix2 >> 16) & 0xFF, g2 = (pix2 >> 8) & 0xFF, b2 = (pix2) & 0xFF;

                /*if(ColorDiff(r1,g1,b1, r2,g2,b2) >= 255*255)
                {
                    // Don't combine too different colors
                    continue;
                }*/
                /**/
                double result;
                if(luma1==luma2)
                {
                    double result_r = (r1==r2 ? 0.5 : ((r1-rin) / double(r1-r2)));
                    double result_g = (g1==g2 ? 0.5 : ((g1-gin) / double(g1-g2)));
                    double result_b = (b1==b2 ? 0.5 : ((b1-bin) / double(b1-b2)));
                    if(result_r < 0 || result_r > 1) result_r = 0.5;
                    if(result_g < 0 || result_g > 1) result_g = 0.5;
                    if(result_b < 0 || result_b > 1) result_b = 0.5;
                    result = (result_r + result_g + result_b) / 3.0;
                }
                else
                    result = (luma1-input_luma) / (luma1-luma2);
                // ^ Don't combine if the desired color is either darker or brighter
                //   than both of the candidate colors.

                //if(result < 0) result = 0; if(result > 1) result = 1;
                //result = ((int)(result*64)) / 64.0;
                int got_r = r1 + result*(r2-r1);
                int got_g = g1 + result*(g2-g1);
                int got_b = b1 + result*(b2-b1);
                unsigned diff =
                    ColorDiff(rin,gin,bin, got_r,got_g,got_b)
                  + ColorDiff(rin,gin,bin, r1,g1,b1)*2
                  + ColorDiff(rin,gin,bin, r2,g2,b2)*2;
                if(diff < best_diff || best_diff == 0)
                {
                    best_diff   = diff;
                    output.result = result;
                    output.entry1 = pa;
                    output.entry2 = pb;
                    output_chosen = true;
                }
            }
        }
    }

    if(!output_chosen)
    {
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
        output.result = (result_r + result_g + result_b) / 3.0;
    }
    return output;
}

void ReduceHistogram(HistogramType& Histogram)
{
    for(size_t b=PaletteReductionMethod.size(), a=0; a<b; ++a)
    {
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
    return n;
}
