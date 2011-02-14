#include <cmath>
#include <vector>

#include "mask.hh"
#include "pixel.hh"
#include "pixels/averagepixel.hh"
#include "openmp.hh"

VecType<AlphaRange> alpha_ranges;
MaskMethod maskmethod = MaskHole;

namespace
{
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
}

void MaskImage(VecType<uint32>& pixels, unsigned sx, unsigned sy)
{
    for(size_t r=0; r<alpha_ranges.size(); ++r)
    {
        const AlphaRange& a = alpha_ranges[r];

        AlphaRange corrected_range ( a );

        if(a.x1 >= sx) corrected_range.x1 = sx;
        if(a.y1 >= sy) corrected_range.y1 = sy;

        if(corrected_range.x1 + a.width >= sx)
            corrected_range.width = sx - corrected_range.x1;

        if(corrected_range.y1 + a.height >= sy)
            corrected_range.height = sy - corrected_range.y1;

        /*std::fprintf(stderr, "%u,%u, %u,%u\n",
            corrected_range.x1,corrected_range.y1, corrected_range.width,corrected_range.height);
        for(size_t b=0; b<a.colors.size(); ++b)
            std::fprintf(stderr, "- %06X\n", a.colors[b]);*/

        unsigned long BlankCount = 0;
        unsigned p = corrected_range.y1 * sx + corrected_range.x1;
        for(unsigned y=0; y<corrected_range.height; ++y, p += sx)
            for(unsigned x=0; x<corrected_range.width; ++x)
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
                    BlurHUD(&pixels[0], sx, sy, corrected_range);
                    break;
                case MaskPattern:
                    ExtrapolateHUD(&pixels[0], sx, sy, corrected_range);
                    break;
            }
    }
}
