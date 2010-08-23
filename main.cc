#include <gd.h>
#include "canvas.hh"
#include "settype.hh"
#include "align.hh"

#define impl_Average 0 /* dummy constant for AveragePixel */
#include "pixels/averagepixel.hh"

#include <cstdio>
#include <algorithm>
#include <cmath>

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
                    if(strcasecmp(arg, opts[a].shortopt) == 0
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

    static inline bool is_hud_pixel(uint32 pixel)
    {
        return pixel & 0xFF000000u;
    }

    /* Find the smallest circle radius that finds
     * a non-logo pixel from the given position.
     * Multiply the result by 1.25 to avoid jitter that happens
     * when the selected distance randomly only hits one pixel;
     * at the cost of some blur.
     */
    static int BlurHUD_FindDistance(
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
    try_again:
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
                if(!is_hud_pixel( src[cirx] ))
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

    static void BlurHUD(
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
                if(!is_hud_pixel( backup[q+x] ))
                {
                    gfx[q+x] = backup[q+x];
                    continue;
                }
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
                int circle_radius = BlurHUD_FindDistance
                    ( &backup[0],sx,sy, x+bounds.x1,y+bounds.y1,
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

                const uint32* src = &backup[(miny+ry)*sx + rx];

                const int circle_radius_squared = circle_radius*circle_radius;

                AveragePixel output;
                for(int ciry=miny; ciry<=maxy; ++ciry, src += sx)
                {
                    const int circle_threshold = circle_radius_squared - ciry*ciry;
                    for(int cirx=minx; cirx<=maxx; ++cirx)
                        if(!is_hud_pixel(src[cirx])
                        && cirx*cirx <= circle_threshold)
                        {
                            output.set(src[cirx]);
                        }
                }
                gfx[q+x] = output.get();
            } // for x
        } // for y
    } // BlurHUD
} // namespace

int main(int argc, char** argv)
{
    VecType<AlphaRange> alpha_ranges;

    bool bgmethod0_chosen = false;
    bool bgmethod1_chosen = false;
    bool HudBlurring = false;
    bool autoalign = true;

    for(;;)
    {
        int option_index = 0;
        static struct option long_options[] =
        {
            {"help",       0,0,'h'},
            {"version",    0,0,'V'},
            {"mask",       1,0,'m'},
            {"method",     1,0,'p'},
            {"bgmethod",   1,0,'b'},
            {"bgmethod0",  1,0,4000},
            {"bgmethod1",  1,0,4001},
            {"blurhud",    0,0,'u'},
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
        int c = getopt_long(argc, argv, "hVm:b:p:l:B:f:r:a:gvyu", long_options, &option_index);
        if(c == -1) break;
        switch(c)
        {
            case 'V':
                std::printf("%s\n", VERSION);
                return 0;
            case 'h':
            {
                std::printf("%s",
                    "animmerger v"VERSION" - Copyright (C) 2010 Joel Yliluoma (http://iki.fi/bisqwit/)\n"
                    "\n"
                    "Usage: animmerger [<options>] <imagefile> [<...>]\n"
                    "\n"
                    "Merges animation frames together with motion shifting.\n"
                    "\n"
                    " --help, -h             This help\n"
                    " --mask, -m <defs>      Define a mask, see instructions below\n"
                    " --method, -p <mode>    Select pixel type, see below\n"
                    " --bgmethod, -b <mode>  Select pixel type for alignment tests\n"
                    " --bgmethod0 <mode>     Explicit ChangeLog background before camera comes\n"
                    " --bgmethod1 <mode>     Explicit ChangeLog background after camera leaves\n"
                    " --looplength, -l <int> Set loop length for the LOOPINGxx modes\n"
                    " --motionblur, -B <int> Set motion blur length for animated modes\n"
                    " --firstlast, -f <int>  Set threshold for xxNMOST modes\n"
                    " --version, -V          Displays version information\n"
                    " --yuv, -y              Sets YUV mode for average-color calculations\n"
                    "                        Affects AVERAGE, ACTIONAVG and LOOPINGAVG.\n"
                    " --blurhud, -u          Hide masked areas with circular interpolation\n"
                    "                        rather than by making them entirely transparent.\n"
                    " --refscale, -r <x>,<y>\n"
                    "     Change the grid size that controls\n"
                    "     how many samples are taken from the background image\n"
                    "     for comparing with the input image, for image alignment.\n"
                    "     Smaller grid = more accurate but slower aligning.\n"
                    "     Default: -r32,32\n"
                    "     Set to e.g. -r8,8 if you experience misalignment problems.\n"
                    " --mvrange, -a <xmin>,<ymin>,<xmax>,<ymax>\n"
                    "     Change the limits of motion vectors.\n"
                    "     Default: -9999,-9999,9999,9999\n"
                    "     Example: --mvrange -4,0,4,0 specifies that the screen may\n"
                    "     only scroll horizontally and by 4 pixels at most per frame.\n"
                    " --noalign              Disable image aligner\n"
                    " --gif, -g              Save GIF frames instead of PNG frames\n"
                    " --verbose, -v          Increase verbosity\n"
                    "\n"
                    "animmerger will always output PNG files into the current\n"
                    "working directory, with the filename pattern tile-####.png\n"
                    "where #### is a sequential number beginning from 0000.\n"
                    "\n"
                    "AVAILABLE PIXEL TYPES\n"
                    "\n"
                    "  AVERAGE, long option: --method=average , short option: -pa\n"
                    "     Produces a single image. Each pixel\n"
                    "     is the average of all frames addressing that pixel.\n"
                    "  TINYAVERAGE, long option: --method=tinyaverage , short option: -pA\n"
                    "     A less accurate but more space-efficient version of \"average\".\n"
                    "  LAST, long option: --method=last , short option: -pl\n"
                    "     Produces a single image. Each pixel\n"
                    "     records the latest color addressing that pixel.\n"
                    "  FIRST, long option: --method=first , short option: -pf\n"
                    "     Produces a single image. Each pixel\n"
                    "     records whatever first appeared in that spot.\n"
                    "  SOLID, long option:: --method=solid , short option: -pO\n"
                    "     Produces a single image. Changing pixels are\n"
                    "     made transparent until something unchanging appears.\n"
                    "  MOSTUSED, long option: --method=mostused, short option: -pm\n"
                    "     Produces a single image. Each pixel\n"
                    "     records the color that most often occured in that location.\n"
                    "     Use this option for making maps!\n"
                    "  LEASTUSED, long option: --method=leastused, short option: -pe\n"
                    "     Produces a single image. Each pixel\n"
                    "     records the color that least commonly occured in that location.\n"
                    "  LASTNMOST, long option: --method=lastnmost, short option: -pL\n"
                    "     Combines \"mostused\" and \"last\". Set threshold using\n"
                    "     the -f option. Example: -f16 -pL = most used of last 16 pixels.\n"
                    "     If -f0, then selects the last not-common pixel value.\n"
                    "     If -f value is negative, uses leastused instead of mostused.\n"
                    "  FIRSTNMOST, long option: --method=firstnmost, short option: -pF\n"
                    "     Combines \"mostused\" and \"first\". Set threshold using\n"
                    "     the -f option. Example: -f16 -pF = most used of first 16 pixels.\n"
                    "     If -f0, then selects the first not-common pixel value.\n"
                    "     If -f value is negative, uses leastused instead of mostused.\n"
                    "  ACTIONAVG, long option: --method=actionavg, short option: -pt\n"
                    "     Similar to average, except that blurring of actors\n"
                    "     over the background is avoided.\n"
                    "  CHANGELOG, long option: --method=changelog, short option: -pc\n"
                    "     Produces an animation. Also supports motion blur.\n"
                    "  LOOPINGLOG, long option: --methods=loopinglog, short option: -po\n"
                    "     Produces a time-restricted animation.\n"
                    "     Also called, \"lemmings mode\".\n"
                    "     Use the -l option to set loop length in frames. Supports motion blur.\n"
                    "  LOOPINGAVG, long option: --methods=loopingavg, short option: -pv\n"
                    "     A combination of loopinglog and actionavg, also supports motion blur.\n"
                    "\n"
                    "DEFINING MASKS\n"
                    "\n"
                    "  You can use masks to block out HUD / splitscreens\n"
                    "  so that it will not intervene with the animation.\n"
                    "  To define mask, use the --mask option, or -m for short.\n"
                    "  Mask syntax: x1,y1,width,height,colors\n"
                    "  Examples:\n"
                    "    -m0,0,256,32\n"
                    "       Mask out a 256x32 wide section at the top of screen\n"
                    "    -m0,0,256,32,FFFFFF\n"
                    "       From the 256x32 wide section at the top of screen,\n"
                    "       mask out those pixels whose color is white (#FFFFFF)\n"
                    "    -m16,16,8,40,000000,483D8B\n"
                    "       From the 8x40 wide section at coordinates 16x16,\n"
                    "       mask out those pixels whose color is either\n"
                    "       black (#000000) or dark slate blue (#483D8B)\n"
                    "\n"
                    "TIPS\n"
                    "\n"
                    "Converting a GIF animation into individual frame files:\n"
                    "   gifsicle -U -E animation.gif\n"
                    "   animmerger <...> animation.gif.*\n"
                    "\n"
                    "To create images with multiple methods in succession,\n"
                    "you can use the multimode option. For example,\n"
                    "    --method average,last,mostused, or -pa,l,m\n"
                    "creates three images, corresponding to that if\n"
                    "you ran animmerger with -pa, -pl, -pm options\n"
                    "in succession. Note that all modes share the same\n"
                    "other parameters (firstlast, looplength).\n"
                    "The benefit in doing this is that the image alignment\n"
                    "phase needs only be done once.\n"
                    "\n"
                    "Different combinations of pixel methods require different\n"
                    "amounts of memory. Use the -v option to see how much memory\n"
                    "is required per pixel when using different options.\n"
                    "animmerger always strives to choose the smallest pixel\n"
                    "implementation that provides all of the requested features.\n"
                    "\n"
                    "When creating animations of video game content, please take\n"
                    "all necessary steps to ensure that background stays immobile\n"
                    "while characters move. Parallax animation is bad; If possible,\n"
                    "please fix all background layers so that they scroll at even\n"
                    "rate.\n"
                    "\n");
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
                    fprintf(stderr, "animmerger: Bad first/last threshold: %ld\n", tmp);
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
                    fprintf(stderr, "animmerger: Bad loop length: %ld\n", tmp);
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
                    fprintf(stderr, "animmerger: Bad motion blur length: %ld\n", tmp);
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
                    fprintf(stderr, "animmerger: Invalid parameter to -r: %s\n", arg);
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
                    fprintf(stderr, "animmerger: Invalid parameter to -a: %s\n", arg);
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
                HudBlurring = true;
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
        if(verbose) fprintf(stderr, "Reading %s\n", fn);
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
                fprintf(stderr, "- %06X\n", a.colors[b]);*/

            unsigned BlankCount = 0;
            unsigned p = tmp.y1 * sx + tmp.x1;
            for(unsigned y=0; y<tmp.height; ++y, p += sx)
                for(unsigned x=0; x<tmp.width; ++x)
                {
                    if(a.colors.empty()
                    || a.colors.find(pixels[p+x]) != a.colors.end())
                    {
                        pixels[p+x] |= 0xFF000000u;
                        ++BlankCount;
                    }
                }
            if(HudBlurring && BlankCount != 0)
            {
                BlurHUD(&pixels[0], sx, sy, tmp);
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
