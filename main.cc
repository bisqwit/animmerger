#include <gd.h>
#include "canvas.hh"
#include "settype.hh"
#include "align.hh"
#include "openmp.hh"
#include "palette.hh"

#include "pixels/averagepixel.hh"

#include <cstdio>
#include <algorithm>
#include <cmath>

#include <string.h>
#include <getopt.h>

struct AlphaRange
{
    unsigned x1,y1, width,height;
    SetType<unsigned> colors;
};

#define MakePixName(o,f,name) #name,
static const char* const PixelMethodNames[NPixelMethods] =
{
     DefinePixelMethods(MakePixName)
};
#undef MakePixName

enum MaskMethod
{
    MaskHole,
    MaskCircularBlur,
    MaskPattern,
    MaskBlack
};

namespace
{
    int ParsePixelMethod(
        char* optarg,
        bool allow_multiple,
        bool allow_animated)
    {
        #define AddOption(optchar,f,name) \
            { #optchar , #name, pm_##name##Pixel, (f&1) },
        static const struct
        {
            const char* shortopt;
            const char* longopt;
            PixelMethod mode;
            bool animated;
        } opts[] =
        {
            DefinePixelMethods(AddOption)
            { "s" , "LoopingLast", pm_LoopingLogPixel, true }
        };
        #undef AddOption

        unsigned result = 0;
        for(; char* arg =
            allow_multiple ? std::strtok(optarg, ",")
                           : optarg; optarg=0)
        {
            if(allow_multiple && strcmp(arg, "*") == 0)
                result |= ((1ul << NPixelMethods) - 1);
            else
            {
                for(unsigned a=0; a < sizeof(opts)/sizeof(*opts); ++a)
                {
                    if(strcmp(arg, opts[a].shortopt) == 0
                    || strcasecmp(arg, opts[a].longopt) == 0)
                    {
                        if(!allow_animated && opts[a].animated) goto is_animated;
                        result |= (allow_multiple ? 1ul << opts[a].mode
                                                  : (unsigned) opts[a].mode);
                        goto found_mode;
                    }
                }

                std::fprintf(stderr, "animmerger: Unrecognized method: %s\n", arg);
                return -1;
            is_animated:
                std::fprintf(stderr, "animmerger: Background pixel method cannot be animated. Bad choice: %s\n", arg);
                return -1;
            found_mode:;
            }
        }
        if(allow_multiple && result == 0)
        {
            std::fprintf(stderr, "animmerger: Error: No pixel method defined\n");
            result = 1ul << pm_MostUsedPixel;
        }
        return result;
    }

    static inline bool is_masked_pixel(uint32 pixel)
    {
        return pixel & 0xFF000000u;
    }

    /* Find the smallest circle radius that finds
     * a non-logo pixel from the given position.
     * Multiply the result by 1.25 to avoid jitter that happens
     * when the selected distance randomly only hits one pixel;
     * at the cost of some blur.
     */
    int BlurHUD_FindDistance(
        const uint32* gfx, unsigned sx,unsigned sy,
        unsigned x, unsigned y,
        unsigned max_xdistance,
        unsigned max_ydistance)
    {
        const int rx = x, ry = y;
        int circle_radius_horizontal = max_xdistance+1;
        int circle_radius_vertical   = max_ydistance+1;
        // ^ Estimated maximum radius that needs to be calculated to
        //   find a pixel that does not belong to the HUD.
    try_again:;
        int minx = -circle_radius_horizontal, maxx = +circle_radius_horizontal;
        int miny = -circle_radius_vertical,   maxy = +circle_radius_vertical;
        if(minx+rx < 0) minx = -rx;
        if(miny+ry < 0) miny = -ry;
        if(maxx+rx >= (int)sx) maxx = int(sx)-rx-1;
        if(maxy+ry >= (int)sy) maxy = int(sy)-ry-1;

        int smallest_distance_squared = 0;
        const uint32* src = &gfx[(miny+ry)*sx + rx];
        for(int ciry=miny; ciry<=maxy; ++ciry, src += sx)
        {
            int ciry_squared = ciry*ciry;
            for(int cirx=minx; cirx<=maxx; ++cirx)
            {
                if(!is_masked_pixel( src[cirx] ))
                {
                    int distance_squared = cirx*cirx + ciry_squared;
                    if(smallest_distance_squared == 0
                    || distance_squared < smallest_distance_squared)
                    {
                        smallest_distance_squared = distance_squared;
                        if(smallest_distance_squared == 1)
                            goto found_smallest_possible;
                    }
                }
            }
        }
        if(smallest_distance_squared == 0)
        {
            // For some reason, we found nothing.
            // Just in case, double the circle_radius and try again!
            // This may happen when the max_?distance variables are
            // based on the edges of a HUD that happens to be positioned
            // on the screen edges.
            circle_radius_horizontal *= 2;
            circle_radius_vertical   *= 2;
            goto try_again;
        }
    found_smallest_possible:
        int result = (int) ( 0.5 + std::sqrt( (double) smallest_distance_squared ) * 1.25 );
        return result;
    }

    uint32 DecideBlurHUD(
        const uint32* gfx, unsigned sx,unsigned sy,
        const AlphaRange& bounds,
        unsigned x, unsigned y)
    {
        /* At each position that is inside the logo, non-logo
         * pixels are averaged together from a circular area
         * whose radius corresponds to the shortest distance
         * to a non-logo pixel, increased by a factor of 1.25.
         *
         * This is basically the same as what MPlayer's delogo
         * filter does, but written in 95% fewer lines.
         * Also, delogo uses manhattan distances rather than
         * euclidean distances.
         */
        const int circle_radius = BlurHUD_FindDistance
            ( gfx,sx,sy, x+bounds.x1,y+bounds.y1,
              std::min(bounds.width-x, x),
              std::min(bounds.height-y, y)
            );

        int rx = x + bounds.x1;
        int ry = y + bounds.y1;
        int minx = -circle_radius, maxx = +circle_radius;
        int miny = -circle_radius, maxy = +circle_radius;
        if(minx+rx < 0) minx = -rx;
        if(miny+ry < 0) miny = -ry;
        if(maxx+rx >= (int)sx) maxx = int(sx)-rx-1;
        if(maxy+ry >= (int)sy) maxy = int(sy)-ry-1;

        const uint32* src = &gfx[(miny+ry)*sx + rx];

        const int circle_radius_squared = circle_radius*circle_radius;

        AveragePixel output;
        for(int ciry=miny; ciry<=maxy; ++ciry, src += sx)
        {
            const int circle_threshold = circle_radius_squared - ciry*ciry;
            for(int cirx=minx; cirx<=maxx; ++cirx)
                if(!is_masked_pixel(src[cirx])
                && cirx*cirx <= circle_threshold)
                {
                    output.set(src[cirx]);
                }
        }
        return output.get();
    }

    void BlurHUD(
        uint32* gfx, unsigned sx,unsigned sy,
        const AlphaRange& bounds)
    {
        const std::vector<uint32> backup( gfx, gfx + sx*sy );

        /* Remove the HUD */
        #pragma omp parallel for schedule(static)
        for(unsigned /*q=bounds.y1*sx+bounds.x1,*/ y=0; y < bounds.height; ++y/*, q+=sx*/)
        {
            unsigned q = (bounds.y1 + y) * sx + bounds.x1;
            for(unsigned x=0; x < bounds.width; ++x)
            {
                if(is_masked_pixel( backup[q+x] ))
                    gfx[q+x] = DecideBlurHUD(&backup[0], sx,sy,bounds, x,y);
            } // for x
        } // for y
    } // BlurHUD

    unsigned CompareTiles(
        const uint32* gfx, unsigned sx,unsigned sy,
        int a_x, int a_y, // target
        int b_x, int b_y, // source
        unsigned tilesizex, unsigned tilesizey)
    {
        /* Compare two tiles of size tilesize
         * centered at a and b respectively.
         */
        a_x -= tilesizex/2; if(a_x < 0) a_x = 0;
        a_y -= tilesizey/2; if(a_y < 0) a_y = 0;
        b_x -= tilesizex/2; if(b_x < 0) b_x = 0;
        b_y -= tilesizey/2; if(b_y < 0) b_y = 0;
        if(a_x + tilesizex > sx) a_x = sx-tilesizex;
        if(a_y + tilesizey > sy) a_y = sy-tilesizey;
        if(b_x + tilesizex > sx) b_x = sx-tilesizex;
        if(b_y + tilesizey > sy) b_y = sy-tilesizey;
        const uint32* a_ptr = gfx + a_y * sx + a_x;
        const uint32* b_ptr = gfx + b_y * sx + b_x;
        const uint32* a_ptr_end = a_ptr + tilesizey*sx;

        unsigned n_match = 0;
        for(; a_ptr < a_ptr_end; a_ptr += sx, b_ptr += sx)
            for(unsigned x=0; x<tilesizex; ++x)
            {
                uint32 a_pix = a_ptr[x];
                uint32 b_pix = b_ptr[x];
                /* Truth table for comparison:
                 *                  transparent(B)
                 *   transparent(A)     no  yes
                 *               no    a==b true
                 *              yes    true false
                 */
                bool a_hud = is_masked_pixel(a_pix);
                bool b_hud = is_masked_pixel(b_pix);
                if(b_hud) return 0;
                if(a_hud || a_pix == b_pix) ++n_match;
                /*
                if(!(a_hud && b_hud)
                && (a_hud || b_hud || a_pix == b_pix))
                    ++n_match;*/
            }
        return n_match;
    }

    /*unsigned EvaluateDistinctiveness(
        const uint32* gfx, unsigned sx,unsigned sy,
        int a_x, int a_y,
        int b_x, int b_y,
        unsigned tilesizex, unsigned tilesizey)
    {
        a_x -= tilesizex/2; if(a_x < 0) a_x = 0;
        a_y -= tilesizey/2; if(a_y < 0) a_y = 0;
        b_x -= tilesizex/2; if(b_x < 0) b_x = 0;
        b_y -= tilesizey/2; if(b_y < 0) b_y = 0;
        if(a_x + tilesizex > sx) a_x = sx-tilesizex;
        if(a_y + tilesizey > sy) a_y = sy-tilesizey;
        if(b_x + tilesizex > sx) b_x = sx-tilesizex;
        if(b_y + tilesizey > sy) b_y = sy-tilesizey;
        const uint32* a_ptr = gfx + a_y * sx + a_x;
        const uint32* b_ptr = gfx + b_y * sx + b_x;
        const uint32* a_ptr_end = a_ptr + tilesizey*sx;

        SetType<uint32> distinct_pixels;
        distinct_pixels.reserve(16);
        //std::set<uint32, std::less<uint32>, FSBAllocator<uint32> > distinct_pixels;
        for(; a_ptr < a_ptr_end; a_ptr += sx, b_ptr += sx)
            for(unsigned x=0; x<tilesizex; ++x)
            {
                uint32 a_pix = a_ptr[x];
                uint32 b_pix = b_ptr[x];
                if(!is_masked_pixel(a_pix)) distinct_pixels.insert(a_pix);
                if(!is_masked_pixel(b_pix)) distinct_pixels.insert(b_pix);
            }
        return distinct_pixels.size();
    }*/

    double EvaluatePatternedness(const uint32* gfx, unsigned stride, unsigned npixels)
    {
        /* Evaluate how patterned the pixels along the given line are. */
        const uint32* const gfx_end = gfx + stride*npixels;

        double result = 0.0;
        for(unsigned pattern_width = 3;
                     pattern_width <= 32;
                     pattern_width <<= 1)
        {
            int match = 0, combo = 0;
            for(unsigned startpos = 0; startpos < pattern_width; ++startpos)
            {
                const uint32* p = gfx + stride*startpos;
                for(;;)
                {
                    const uint32* q = p + stride*pattern_width;
                    if(q >= gfx_end) break;
                    if(*p == *q)
                    {
                        // Count a combo unless all pixels between *q and *p are identical.
                        for(; &p[stride] != q; p+=stride)
                            if(p[stride] != *p)
                                goto combo_ok;
                        goto failed_combo;
                    combo_ok:;
                        ++combo;
                    }
                    else failed_combo:
                        { match += combo*combo; combo=0; }
                    p = q;
                }
            }
            match += combo*combo;
            double this_score = match * pattern_width;
            result += this_score;
        }
        return result / (double)npixels;
    }

    uint32 DecideExtrapolateHUD(
        const uint32* gfx, unsigned sx,unsigned sy,
        const AlphaRange& bounds,
        unsigned x, unsigned y)
    {
        const int absx = bounds.x1 + x;
        const int absy = bounds.y1 + y;

        const unsigned min_tilesize_power = 4;
        unsigned max_tilesize_power = 4;
    try_again:;
        int best_source_x=0, best_source_y=0;
        unsigned best_score=0;
      #ifdef _OPENMP
        MutexType score_lock;
      #endif

        const unsigned n_tilesize_powers =
            (max_tilesize_power-min_tilesize_power)+1;
        const unsigned n_tilesize_powers_squared =
            n_tilesize_powers * n_tilesize_powers;

        /* TODO: It is not very smart to do this decision individually
         *       for each and every pixel. Devise some way to use the
         *       same stamping-source for larger areas successively.
         */
        #pragma omp parallel for schedule(static)
        for(unsigned tilesize_counter=0;
            tilesize_counter < n_tilesize_powers_squared;
            ++tilesize_counter)
        {
            int tilesize_x = 1 << (min_tilesize_power + tilesize_counter/n_tilesize_powers);
            int tilesize_y = 1 << (min_tilesize_power + tilesize_counter%n_tilesize_powers);

            int peek_depth = 2;

            int min_x = absx, max_x = absx, min_y = absy, max_y = absy;
            while(min_x > int(bounds.x1)-tilesize_x*peek_depth) min_x -= tilesize_x;
            while(min_y > int(bounds.y1)-tilesize_y*peek_depth) min_y -= tilesize_y;
            while(max_x < int(bounds.x1+bounds.width)+tilesize_x*peek_depth) max_x += tilesize_x;
            while(max_y < int(bounds.y1+bounds.width)+tilesize_y*peek_depth) max_y += tilesize_y;

            int loop_mode = 0;
            int source_x = min_x;
            int source_y = absy;
            for(;; loop_mode ? (source_y+=tilesize_y) : (source_x+=tilesize_x) )
            {
                switch(loop_mode)
                {
                    case 0:
                        if(source_x <= max_x) break;
                        source_x = absx;
                        source_y = min_y;
                        loop_mode = 1; // passthru
                    default: case 1:
                        if(source_y <= max_y) break;
                        goto done_loop;
                }
                //std::fprintf(stderr, "try %d,%d (mode=%d, %d..%d,%d..%d)\n", source_x,source_y, loop_mode, min_x,max_x, min_y,max_y);
                if(source_x < 0 || source_x >= int(sx)
                || source_y < 0 || source_y >= int(sy)
                || is_masked_pixel( gfx[source_y*sx + source_x] ))
                {
                    continue;
                }
                unsigned score = CompareTiles(gfx,sx,sy, absx,absy, source_x,source_y, 8,8);
              #ifdef _OPENMP
                ScopedLock lck(score_lock);
              #endif
                #pragma omp flush(best_score,best_source_x,best_source_y)
                if(score > best_score
                || (score == best_score &&
                        (best_source_y < source_y
                      || (best_source_y == source_y && best_source_x < source_x))
                  ))
                {
                    // The coordinate comparison above is simply a tie-breaker
                    // in order to produce consistent results despite multithreading.
                    best_source_x = source_x;
                    best_source_y = source_y;
                    best_score = score;
                    #pragma omp flush(best_score,best_source_x,best_source_y)
                }
            }
         done_loop:;
        }
        if(best_score == 0)
        {
            /* If no sample could be found,
             * retry with a larger tile size limit.
             */
            max_tilesize_power += 1;
            //std::fprintf(stderr, "Extend tilesize to %u\n", 1u << max_tilesize_power);
            goto try_again;
        }
        /*fprintf(stderr, "Copy from %d,%d\n",
            (best_source_x-absx),
            (best_source_y-absy));*/
        return gfx[best_source_y*sx + best_source_x];
    }

    void ExtrapolateHUD(
        uint32* gfx, unsigned sx,unsigned sy,
        AlphaRange& bounds)
    {
        /* Reduce one edge at time, choosing that edge
         * which neighbors the most patterned-looking data.
         */
      #if 0
        while(bounds.width >= 2
           && bounds.height >= 2)
        {
            uint32* q0 = gfx + bounds.y1 * sx + bounds.x1;
            unsigned y2 = bounds.height-1;
            unsigned x2 = bounds.width-1;

            int selected_edge = 0;
            double best_score = 0;

            if(bounds.y1 > 0)
            {   double score = EvaluatePatternedness(q0-sx,1, bounds.width);
                if(score > best_score) { selected_edge = 0; best_score = score; } }
            if(bounds.x1 > 0)
            {   double score = EvaluatePatternedness(q0-1,sx, bounds.height);
                if(score > best_score) { selected_edge = 2; best_score = score; } }
            if(bounds.y1 + bounds.height < sy)
            {   double score = EvaluatePatternedness(q0+bounds.height*sx,1, bounds.width);
                if(score > best_score) { selected_edge = 1; best_score = score; } }
            if(bounds.x1 + bounds.width < sx)
            {   double score = EvaluatePatternedness(q0+bounds.width,sx, bounds.height);
                if(score > best_score) { selected_edge = 3; best_score = score; } }

            switch(selected_edge)
            {
                case 0: /* Top edge */
                    for(unsigned x=0; x<bounds.width; ++x)
                        if(is_masked_pixel(q0[x]))
                            q0[x] = DecideExtrapolateHUD(gfx,sx,sy,bounds, x,0);
                    bounds.y1 += 1;
                    bounds.height -= 1;
                    break;
                case 1: /* Bottom edge */
                    q0 += y2*sx;
                    for(unsigned x=0; x<bounds.width; ++x)
                        if(is_masked_pixel(q0[x]))
                            q0[x] = DecideExtrapolateHUD(gfx,sx,sy,bounds, x,y2);
                    bounds.height -= 1;
                    break;
                case 2: /* Left edge */
                    for(unsigned y=0; y<bounds.height; ++y, q0+=sx)
                        if(is_masked_pixel(*q0))
                            *q0 = DecideExtrapolateHUD(gfx,sx,sy,bounds, 0,y);
                    bounds.x1 += 1;
                    bounds.width -= 1;
                    break;
                case 3: /* Right edge */
                    q0 += x2;
                    for(unsigned y=0; y<bounds.height; ++y, q0+=sx)
                        if(is_masked_pixel(*q0))
                            *q0 = DecideExtrapolateHUD(gfx,sx,sy,bounds, x2,y);
                    bounds.width -= 1;
                    break;
            }
        }
      #endif
      #if 1
        /* Now reducing all edges roughly at a constant rate. */
        //int carry = (int)bounds.height - (int)bounds.width;
        while(bounds.width >= 2
           && bounds.height >= 2)
        {
            uint32* q0 = gfx + bounds.y1 * sx + bounds.x1;
            unsigned y2 = bounds.height-1;
            unsigned x2 = bounds.width-1;
            //fprintf(stderr, "%ux%u+%u+%u\n",
            //    bounds.width,bounds.height, bounds.x1,bounds.y1);
            double score_topbottom = 0;
            double score_leftright = 0;
            if(bounds.y1 > 0)
                score_topbottom += EvaluatePatternedness(q0-sx,1, bounds.width);
            if(bounds.x1 > 0)
                score_leftright += EvaluatePatternedness(q0-1,sx, bounds.height);
            if(bounds.y1 + bounds.height < sy)
                score_topbottom += EvaluatePatternedness(q0+bounds.height*sx,1, bounds.width);
            if(bounds.x1 + bounds.width < sx)
                score_leftright += EvaluatePatternedness(q0+bounds.width,sx, bounds.height);
            //if(carry >= 0)
            if(score_topbottom > score_leftright)
            {
                //carry -= bounds.width;
                if(bounds.y1 == 0 && bounds.height != sy)
                {
                    uint32* q1 = q0 + y2 * sx;
                    // Do bottom edge only
                    for(unsigned x=0; x<bounds.width; ++x)
                        if(is_masked_pixel(q1[x]))
                            q1[x] = DecideExtrapolateHUD(gfx,sx,sy,bounds, x,y2);
                    bounds.height -= 1;
                }
                else if(bounds.y1 != 0 && bounds.y1+bounds.height == sy)
                {
                    // Do top edge only
                    for(unsigned x=0; x<bounds.width; ++x)
                        if(is_masked_pixel(q0[x]))
                            q0[x] = DecideExtrapolateHUD(gfx,sx,sy,bounds, x,0);
                    bounds.y1 += 1;
                    bounds.height -= 1;
                }
                else
                {
                    uint32* q1 = q0 + y2 * sx;
                    // Do top & bottom horizontal edges
                    for(unsigned x=0; x<bounds.width; ++x)
                    {
                        if(is_masked_pixel(q0[x]))
                            q0[x] = DecideExtrapolateHUD(gfx,sx,sy,bounds, x,0);
                        if(is_masked_pixel(q1[x]))
                            q1[x] = DecideExtrapolateHUD(gfx,sx,sy,bounds, x,y2);
                    }
                    bounds.y1 += 1;
                    bounds.height -= 2;
                }
            }
            else
            {
                //carry += bounds.height;
                if(bounds.x1 == 0 && bounds.width != sx)
                {
                    uint32* q1 = q0 + x2;
                    // Do right edge only
                    for(unsigned y=0; y<bounds.height; ++y, q1+=sx)
                        if(is_masked_pixel(*q1))
                            *q1 = DecideExtrapolateHUD(gfx,sx,sy,bounds, x2,y);
                    bounds.width -= 1;
                }
                else if(bounds.x1 != 0 && bounds.x1+bounds.width == sx)
                {
                    // Do left edge only
                    for(unsigned y=0; y<bounds.height; ++y, q0+=sx)
                        if(is_masked_pixel(*q0))
                            *q0 = DecideExtrapolateHUD(gfx,sx,sy,bounds, 0,y);
                    bounds.x1 += 1;
                    bounds.width -= 1;
                }
                else
                {
                    uint32* q1 = q0 + x2;
                    // Do left & right vertical edges
                    for(unsigned y=0; y<bounds.height; ++y, q0+=sx, q1+=sx)
                    {
                        if(is_masked_pixel(*q0))
                            *q0 = DecideExtrapolateHUD(gfx,sx,sy,bounds, 0,y);
                        if(is_masked_pixel(*q1))
                            *q1 = DecideExtrapolateHUD(gfx,sx,sy,bounds, x2,y);
                    }
                    bounds.x1 += 1;
                    bounds.width -= 2;
                }
            }
        }
      #endif
        /* bounds.width or bounds.height may still be 1. */
        for(unsigned y=0; y<bounds.height; ++y)
        {
            uint32* q = gfx + (bounds.y1 + y) * sx + bounds.x1;
            for(unsigned x=0; x<bounds.width; ++x)
            {
                if(is_masked_pixel( q[x] ))
                    q[x] = DecideExtrapolateHUD(&gfx[0], sx,sy,bounds, x,y);
            }
        }
    }
} // namespace

int main(int argc, char** argv)
{
    VecType<AlphaRange> alpha_ranges;

    bool bgmethod0_chosen = false;
    bool bgmethod1_chosen = false;
    bool autoalign = true;
    MaskMethod maskmethod = MaskHole;

    for(;;)
    {
        int option_index = 0;
        static struct option long_options[] =
        {
            {"help",       0,0,'h'},
            {"version",    0,0,'V'},
            {"mask",       1,0,'m'},
            {"quantize",   1,0,'Q'},
            {"method",     1,0,'p'},
            {"bgmethod",   1,0,'b'},
            {"bgmethod0",  1,0,4000},
            {"bgmethod1",  1,0,4001},
            {"maskmethod", 1,0,'u'},
            {"looplength", 1,0,'l'},
            {"motionblur", 1,0,'B'},
            {"firstlast",  1,0,'f'},
            {"refscale",   1,0,'r'},
            {"mvrange",    1,0,'a'},
            {"gif",        0,0,'g'},
            {"verbose",    0,0,'v'},
            {"yuv",        0,0,'y'},
            {"noalign",    0,0,4002},
            {0,0,0,0}
        };
        int c = getopt_long(argc, argv, "hVm:b:p:l:B:f:r:a:gvyu:Q:", long_options, &option_index);
        if(c == -1) break;
        switch(c)
        {
            case 'V':
                std::printf("%s\n", VERSION);
                return 0;
            case 'h':
            {
                std::printf("%s", "\
animmerger v"VERSION" - Copyright (C) 2010 Joel Yliluoma (http://iki.fi/bisqwit/)\n\
\n\
Usage: animmerger [<options>] <imagefile> [<...>]\n\
\n\
Merges animation frames together with motion shifting.\n\
\n\
Options:\n\
 --help, -h\n\
     This help\n\
 --version, -V\n\
     Displays version information\n\
 --verbose, -v\n\
     Increase verbosity\n\
 --mask, -m <defs>\n\
     Define a mask, see instructions below\n\
 --maskmethod, -u <value>\n\
     Specify how the masked content will be hidden, see instructions below\n\
 --method, -p <mode>\n\
     Select pixel type, see below\n\
 --bgmethod, -b <mode>\n\
     Select pixel type for alignment tests\n\
 --bgmethod0 <mode>\n\
     Explicit pixel mode for ChangeLog background before camera comes\n\
 --bgmethod1 <mode>\n\
     Explicit pixel mode for ChangeLog background after camera leaves\n\
 --looplength, -l <int>\n\
     Set loop length for the LOOPINGxx modes\n\
 --motionblur, -B <int>\n\
     Set motion blur length for animated modes\n\
 --firstlast, -f <int>\n\
     Set threshold for xxNMOST modes\n\
 --yuv, -y\n\
     Specifies that average-colors are to be calculated in the YUV\n\
     colorspace rather than the default RGB colorspace.\n\
 --refscale, -r <x>,<y>\n\
     Change the grid size that controls\n\
     how many samples are taken from the background image\n\
     for comparing with the input image, for image alignment.\n\
     Smaller grid = more accurate but slower aligning.\n\
     Default: -r32,32\n\
     Set to e.g. -r8,8 if you experience misalignment problems.\n\
 --mvrange, -a <xmin>,<ymin>,<xmax>,<ymax>\n\
     Change the limits of motion vectors.\n\
     Default: -9999,-9999,9999,9999\n\
     Example: --mvrange -4,0,4,0 specifies that the screen may\n\
     only scroll horizontally and by 4 pixels at most per frame.\n\
 --quantize <method>,<num_colors>\n\
     Reduce palette, see instructions below\n\
 --noalign\n\
     Disable automatic image aligner\n\
 --gif, -g\n\
     Save GIF frames instead of PNG frames,\n\
     even in non-animated modes.\n\
\n\
animmerger will always output PNG files into the current\n\
working directory, with the filename pattern tile-####.png\n\
where #### is a sequential number beginning from 0000.\n\
\n\
AVAILABLE PIXEL TYPES\n\
\n\
  AVERAGE, long option: --method=average , short option: -pa\n\
     Produces a single image. Each pixel\n\
     is the average of all frames addressing that pixel.\n\
  TINYAVERAGE, long option: --method=tinyaverage , short option: -pA\n\
     A less accurate but more space-efficient version of \"average\".\n\
  LAST, long option: --method=last , short option: -pl\n\
     Produces a single image. Each pixel\n\
     records the latest color addressing that pixel.\n\
  FIRST, long option: --method=first , short option: -pf\n\
     Produces a single image. Each pixel\n\
     records whatever first appeared in that spot.\n\
  SOLID, long option:: --method=solid , short option: -pO\n\
     Produces a single image. Changing pixels are\n\
     made transparent until something unchanging appears.\n\
  MOSTUSED, long option: --method=mostused, short option: -pm\n\
     Produces a single image. Each pixel\n\
     records the color that most often occured in that location.\n\
     Use this option for making maps!\n\
  LEASTUSED, long option: --method=leastused, short option: -pe\n\
     Produces a single image. Each pixel\n\
     records the color that least commonly occured in that location.\n\
  LASTNMOST, long option: --method=lastnmost, short option: -pL\n\
     Combines \"mostused\" and \"last\". Set threshold using\n\
     the -f option. Example: -f16 -pL = most used of last 16 pixels.\n\
     If -f0, then selects the last not-common pixel value.\n\
     If -f value is negative, uses leastused instead of mostused.\n\
  FIRSTNMOST, long option: --method=firstnmost, short option: -pF\n\
     Combines \"mostused\" and \"first\". Set threshold using\n\
     the -f option. Example: -f16 -pF = most used of first 16 pixels.\n\
     If -f0, then selects the first not-common pixel value.\n\
     If -f value is negative, uses leastused instead of mostused.\n\
  ACTIONAVG, long option: --method=actionavg, short option: -pt\n\
     Similar to average, except that blurring of actors\n\
     over the background is avoided.\n\
  CHANGELOG, long option: --method=changelog, short option: -pc\n\
     Produces an animation. Also supports motion blur.\n\
  LOOPINGLOG, long option: --methods=loopinglog, short option: -po\n\
     Produces a time-restricted animation.\n\
     Also called, \"lemmings mode\".\n\
     Use the -l option to set loop length in frames. Supports motion blur.\n\
  LOOPINGAVG, long option: --methods=loopingavg, short option: -pv\n\
     A combination of loopinglog and actionavg, also supports motion blur.\n\
\n\
DEFINING MASKS\n\
\n\
  You can use masks to block out HUD / splitscreens\n\
  so that it will not intervene with the animation.\n\
  To define mask, use the --mask option, or -m for short.\n\
  Mask syntax: x1,y1,width,height,colors\n\
\n\
  Examples:\n\
\n\
    -m0,0,256,32\n\
       Mask out a 256x32 wide section at the top of screen\n\
\n\
    -m0,0,256,32,FFFFFF\n\
       From the 256x32 wide section at the top of screen,\n\
       mask out those pixels whose color is white (#FFFFFF)\n\
\n\
    -m16,16,8,40,000000,483D8B\n\
       From the 8x40 wide section at coordinates 16x16,\n\
       mask out those pixels whose color is either\n\
       black (#000000) or dark slate blue (#483D8B)\n\
\n\
  The masking method option --maskmethod, or -u for short,\n\
  can be used to control how masks are handled.\n\
\n\
     --maskmethod=hole or -uhole (default)\n\
         Make the masked areas entirely transparent. It will be as\n\
         if there was a hole in the image where the masked part was.\n\
         Alias: hole, alpha, transparent\n\
\n\
     --maskmethod=delogo or -ublur\n\
         Hide masked areas by blurring them with circular interpolation.\n\
         Alias: delogo, blur, interpolate\n\
\n\
     --maskmethod=pattern or -upattern\n\
         Hide masked areas by extrapolating a surrounding\n\
         pattern over the masked areas.\n\
         Alias: pattern, extrapolate\n\
\n\
     --maskmethod=blank or -ublack\n\
         Replace the masked areas with black pixels, \"censoring\" them.\n\
         Alias: blank, black, censor\n\
\n\
REDUCING PALETTE\n\
\n\
  When making GIF images, the means on how the palette is reduced\n\
  can be controlled with the --quantize option.\n\
  The following quantization methods are defined:\n\
\n\
    Median cut ( example: --quantize=mediancut,32  or -Qm,32 )\n\
      Heckbert quantization. Progressively bisects the colorspace\n\
      into roughly equal sizes per population until the number of\n\
      sections matches the required size. Then chooses the weighted\n\
      average of each section.\n\
\n\
    Diversity ( example: --quantize=diversity,10 or -Qd,10 )\n\
      XV's modified diversity algorithm. Tries to choose the\n\
      most diverse set of the original colors.\n\
\n\
    Blend-diversity ( example: --quantize=blenddiversity,64 or -Qb,64 )\n\
      Same as diversity, but sometimes makes up new colors by blending.\n\
\n\
    Merging ( example: --quantize=merging,4 or -Qg,4 )\n\
      A last-resort method which progressively finds two most similar\n\
      colors in the remaining colormap and averages them together.\n\
      Very slow, thus not recommended.\n\
\n\
  Multiple quantization phases can be performed in a sequence.\n\
  For example, -Qb,32 -Qd,16 first reduces with \"blend-diversity\"\n\
  to 32 colors, then reduces the remaining set with \"diversity\" to 16 colors.\n\
  It is not an error to reduce to a larger set than 256 colors.\n\
  If the last explicitly chosen quantization method yields more than 256\n\
  than 256 colors, the colormap will be implicitly reduced\n\
  with a method that picks the 256 most-used colors.\n\
  If necessary, the image will be dithered using a positional dithering method.\n\
  If no quantization methods are explicitly selected, animmerger\n\
  will use whatever method GD graphics library happens to use.\n\
  Note that the blending quantization methods are subject to the YUV selection.\n\
\n\
TIPS\n\
\n\
Converting a GIF animation into individual frame files:\n\
   gifsicle -U -E animation.gif\n\
   animmerger <...> animation.gif.*\n\
\n\
To create images with multiple methods in succession,\n\
you can use the multimode option. For example,\n\
    --method average,last,mostused, or -pa,l,m\n\
creates three images, corresponding to that if\n\
you ran animmerger with -pa, -pl, -pm options\n\
in succession. Note that all modes share the same\n\
other parameters (firstlast, looplength).\n\
The benefit in doing this is that the image alignment\n\
phase needs only be done once.\n\
\n\
Different combinations of pixel methods require different\n\
amounts of memory. Use the -v option to see how much memory\n\
is required per pixel when using different options.\n\
animmerger always strives to choose the smallest pixel\n\
implementation that provides all of the requested features.\n\
\n\
When creating animations of video game content, please take\n\
all necessary steps to ensure that background stays fixed\n\
while characters move. Parallax animation is bad; If possible,\n\
please fix all background layers so that they scroll at even\n\
rate.\n\
\n");
                return 0;
            }
            case 'm':
            {
                char* arg = optarg;
                AlphaRange range;
                int arg_offset=0;
                int result = sscanf(arg, "%u,%u,%u,%u%n",
                    &range.x1,
                    &range.y1,
                    &range.width,
                    &range.height,
                    &arg_offset);
                if(result < 4) m_opt_err:
                    std::fprintf(stderr, "animmerger: Syntax error in '%s'\n", arg);
                else
                {
                    arg += arg_offset;
                    for(;;)
                    {
                        if(*arg == ',') ++arg;
                        if(!*arg) break;
                        char* end = 0;
                        long color = strtol(arg, &end, 16);
                        range.colors.insert(color);
                        arg = end;
                        if(*arg && *arg != ',') goto m_opt_err;
                    }
                }
                alpha_ranges.push_back(range);
                break;
            }
            case 'f':
            {
                char* arg = optarg;
                long tmp = strtol(arg, 0, 10);
                FirstLastLength = tmp;
                if(tmp != FirstLastLength)
                {
                    std::fprintf(stderr, "animmerger: Bad first/last threshold: %ld\n", tmp);
                    FirstLastLength = 1;
                }
                break;
            }
            case 'l':
            {
                char* arg = optarg;
                long tmp = strtol(arg, 0, 10);
                LoopingLogLength = tmp;
                if(LoopingLogLength < 1 || tmp != LoopingLogLength)
                {
                    std::fprintf(stderr, "animmerger: Bad loop length: %ld\n", tmp);
                    LoopingLogLength = 1;
                }
                break;
            }
            case 'B':
            {
                char* arg = optarg;
                long tmp = strtol(arg, 0, 10);
                AnimationBlurLength = tmp;
                if(tmp < 0 || tmp != AnimationBlurLength)
                {
                    std::fprintf(stderr, "animmerger: Bad motion blur length: %ld\n", tmp);
                    AnimationBlurLength = 0;
                }
                break;
            }
            case 'r':
            {
                char* arg = optarg;
                int n = sscanf(arg, "%u,%u", &x_divide_reference,&y_divide_reference);
                if(x_divide_reference < 1 || y_divide_reference < 1
                || x_divide_reference > 1024 || y_divide_reference > 1024
                || n != 2)
                {
                    std::fprintf(stderr, "animmerger: Invalid parameter to -r: %s\n", arg);
                    x_divide_reference=32;
                    y_divide_reference=32;
                }
                break;
            }
            case 'a':
            {
                char* arg = optarg;
                int n = sscanf(arg, "%d,%d,%d,%d",
                    &mv_xmin,&mv_ymin,
                    &mv_xmax,&mv_ymax);
                if(n != 4)
                {
                    std::fprintf(stderr, "animmerger: Invalid parameter to -a: %s\n", arg);
                }
                break;
            }
            case 'Q':
            {
                char *arg = optarg;
                char *comma = std::strchr(arg, ',');
                if(!comma)
                    std::fprintf(stderr, "animmerger: Invalid parameter to -Q: %s\n", arg);
                else
                {
                    *comma = '\0';
                    PaletteMethodItem method;
                    method.size = 0;
                    #define AddOption(optchar,name) \
                        else if(strcmp(arg, #optchar) == 0 || strcasecmp(arg, #name) == 0) \
                            { method.size = 1; method.method = quant_##name; }
                    if(false) {}
                    DefinePaletteMethods(AddOption)
                    else
                    {
                        std::fprintf(stderr, "animmerger: Unknown quantization mode: %s\n", arg);
                    }
                    if(method.size)
                    {
                        long ncolors = strtol(comma+1, 0, 10);
                        if(ncolors < 1)
                            std::fprintf(stderr, "animmerger: Invalid palette size: %ld\n", ncolors);
                        else
                        {
                            method.size = ncolors;
                            PaletteReductionMethod.push_back(method);
                        }
                    }
                    #undef AddOption
                }
                break;
            }
            case 'p':
            {
                int res = ParsePixelMethod(optarg, true, true);
                if(res < 0) return res;
                pixelmethods_result = res;
                break;
            }
            case 'b':
            {
                int res = ParsePixelMethod(optarg, false, false);
                if(res < 0) return res;
                bgmethod = (PixelMethod) res;
                break;
            }
            case 'u':
            {
                if(strcasecmp(optarg, "hole") == 0
                || strcasecmp(optarg, "alpha") == 0
                || strcasecmp(optarg, "transparent") == 0
                )
                    maskmethod = MaskHole;
                else if(strcasecmp(optarg, "delogo") == 0
                     || strcasecmp(optarg, "blur") == 0
                     || strcasecmp(optarg, "interpolate") == 0
                       )
                    maskmethod = MaskCircularBlur;
                else if(strcasecmp(optarg, "pattern") == 0
                     || strcasecmp(optarg, "extrapolate") == 0
                       )
                    maskmethod = MaskPattern;
                else if(strcasecmp(optarg, "blank") == 0
                     || strcasecmp(optarg, "black") == 0
                     || strcasecmp(optarg, "censor") == 0
                       )
                    maskmethod = MaskBlack;
                else
                {
                    std::fprintf(stderr,
                        "animmerger: Unrecognized method: %s\n", optarg);
                    return -1;
                }
                break;
            }
            case 4000: // bgmethod0
            {
                int res = ParsePixelMethod(optarg, false, false);
                if(res < 0) return res;
                bgmethod0 = (PixelMethod) res;
                bgmethod0_chosen = true;
                break;
            }
            case 4001: // bgmethod1
            {
                int res = ParsePixelMethod(optarg, false, false);
                if(res < 0) return res;
                bgmethod1 = (PixelMethod) res;
                bgmethod1_chosen = true;
                break;
            }
            case 4002:
            {
                autoalign = false;
                break;
            }
            case 'v':
                ++verbose;
                break;
            case 'y':
                AveragesInYUV = true;
                break;
            case 'g':
                SaveGif = 1;
                break;
        }
    }

    if(!bgmethod0_chosen) bgmethod0 = bgmethod;
    if(!bgmethod1_chosen) bgmethod1 = bgmethod;

    if(bgmethod0_chosen || bgmethod1_chosen)
    {
        if(!(pixelmethods_result & (1ul << pm_ChangeLogPixel)))
        {
            std::fprintf(stderr,
                "animmerger: Warning: bgmethod0 or bgmethod1 only apply to ChangeLog, which was not selected.\n");
        }
        if(AnimationBlurLength != 0)
        {
            std::fprintf(stderr,
                "animmerger: Warning: bgmethod0 and bgmethod1 are ignored when motion blur is used.\n");
        }
    }

    if(verbose)
    {
        const unsigned long AllUsedMethods =
            pixelmethods_result
            | (1ul << bgmethod)
            | (1ul << bgmethod0)
            | (1ul << bgmethod1);

        std::printf("\tChosen background method: %s\n", PixelMethodNames[bgmethod]);
        if(bgmethod0 != bgmethod)
            std::printf("\tBackground for changelog before camera: %s\n", PixelMethodNames[bgmethod0]);
        if(bgmethod1 != bgmethod)
            std::printf("\tBackground for changelog after camera: %s\n", PixelMethodNames[bgmethod1]);

        std::printf("\tChosen foreground methods:");
        for(unsigned a=0; a<NPixelMethods; ++a)
            if(pixelmethods_result & (1ul << a))
                std::printf(" %s", PixelMethodNames[a]);
        std::printf("\n");

        if(AllUsedMethods & LoopingPixelMethodsMask)
            std::printf("\tLoop length: %u\n", LoopingLogLength);

        if(AllUsedMethods & FirstLastPixelMethodsMask)
            std::printf("\tFirstLastLength: %d\n", FirstLastLength);

        if(AllUsedMethods & BlurCapablePixelMethodsMask)
            std::printf("\tBlur length: %u\n", AnimationBlurLength);

        if(AllUsedMethods & YUVCapablePixelMethodsMask)
            std::printf("\tAverage color is calculated in: %s\n",
                AveragesInYUV ? "YUV" : "RGB");

        if(SaveGif == 1 || (AllUsedMethods & AnimatedPixelMethodsMask))
        {
            if(PaletteReductionMethod.empty())
                std::printf("\tPalette reduction done by libGD if necessary\n");
            else
            {
                std::printf("\tPalette reduction if necessary: ");
                for(size_t b = PaletteReductionMethod.size(), a=0; a<b; ++a)
                {
                    if(a) std::printf(", followed by ");
                    switch(PaletteReductionMethod[a].method)
                    {
                        #define MakeCase(o,name) \
                            case quant_##name: std::printf("%s", #name); break;
                        DefinePaletteMethods(MakeCase)
                        #undef MakeCase
                    }
                    std::printf(" to %u", PaletteReductionMethod[a].size);
                }
                std::printf("\n");
            }
        }
        else
        {
            if(PaletteReductionMethod.empty())
                std::printf(
                    "\tPalette reduction was specified, but it will be ignored,\n"
                    "\tbecause only truecolor PNGs will be generated.\n");
        }

        unsigned size = GetPixelSizeInBytes();
        int penalty = GetPixelSizePenaltyInBytes();
        std::printf("\tPixel size in bytes: %u", size);
        for(; penalty>=8; penalty-=16) std::printf("+n");
        std::printf(" (%s)\n", GetPixelSetupName());
    }

    TILE_Tracker tracker;

    if(optind == argc)
    {
        std::fprintf(stderr,
            "animmerger: No files given. Nothing to do. Exiting.\n"
            "Try animmerger --help\n");
        return 0;
    }

    VecType<uint32> pixels;

    for(int a=optind; a<argc; ++a)
    {
        const char* fn = argv[a];
        if(verbose) std::fprintf(stderr, "Reading %s\n", fn);
        FILE* fp = std::fopen(fn, "rb");
        if(!fp)
        {
            perror(fn);
            continue;
        }

        gdImagePtr im = gdImageCreateFromPng(fp);
        if(!im) { rewind(fp); im = gdImageCreateFromGif(fp);
        if(!im) { rewind(fp); im = gdImageCreateFromJpeg(fp); } }
        std::fclose(fp);
        if(!im)
        {
            std::fprintf(stderr, "animmerger: %s has unrecognized image type, ignoring file\n", fn);
            continue;
        }

        unsigned sx = gdImageSX(im), sy = gdImageSY(im);
        pixels.resize(sx*sy);
        for(unsigned p=0, y=0; y<sy; ++y)
            for(unsigned x=0; x<sx; ++x,++p)
                pixels[p] = gdImageGetTrueColorPixel(im, x,y);
        gdImageDestroy(im);

        for(size_t r=0; r<alpha_ranges.size(); ++r)
        {
            const AlphaRange& a = alpha_ranges[r];
            AlphaRange tmp;
            tmp.x1 = a.x1 < sx ? a.x1 : sx;
            tmp.y1 = a.y1 < sy ? a.y1 : sy;
            tmp.width  = (tmp.x1 + a.width ) < sx ? a.width  : sx-tmp.x1;
            tmp.height = (tmp.y1 + a.height) < sy ? a.height : sy-tmp.y1;

            /*std::fprintf(stderr, "%u,%u, %u,%u\n",
                tmp.x1,tmp.y1, tmp.width,tmp.height);
            for(size_t b=0; b<a.colors.size(); ++b)
                std::fprintf(stderr, "- %06X\n", a.colors[b]);*/

            unsigned BlankCount = 0;
            unsigned p = tmp.y1 * sx + tmp.x1;
            for(unsigned y=0; y<tmp.height; ++y, p += sx)
                for(unsigned x=0; x<tmp.width; ++x)
                {
                    if(a.colors.empty()
                    || a.colors.find(pixels[p+x]) != a.colors.end())
                    {
                        if(maskmethod == MaskBlack)
                            pixels[p+x] = 0;
                        else
                        {
                            pixels[p+x] |= 0xFF000000u;
                            ++BlankCount;
                        }
                    }
                }
            if(BlankCount != 0)
                switch(maskmethod)
                {
                    case MaskHole:
                    case MaskBlack:
                        break;
                    case MaskCircularBlur:
                        BlurHUD(&pixels[0], sx, sy, tmp);
                        break;
                    case MaskPattern:
                        ExtrapolateHUD(&pixels[0], sx, sy, tmp);
                        break;
                }
        }

        if(autoalign)
        {
            tracker.FitScreenAutomatic(&pixels[0], sx,sy);
        }
        else
        {
            AlignResult align;
            align.offs_x = 0;
            align.offs_y = 0;
            align.suspect_reset = false;
            tracker.FitScreen(&pixels[0], sx,sy, align);
        }

        tracker.NextFrame();
    }
    tracker.Save();
}
