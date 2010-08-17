#include <gd.h>
#include "canvas.hh"
#include "settype.hh"
#include "align.hh"

#include <cstdio>
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

int main(int argc, char** argv)
{
    VecType<AlphaRange> alpha_ranges;

    int verbose=0;

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
            {"looplength", 1,0,'l'},
            {"motionblur", 1,0,'B'},
            {"firstlast",  1,0,'f'},
            {"refscale",   1,0,'r'},
            {"mvrange",    1,0,'a'},
            {"gif",        0,0,'g'},
            {"verbose",    0,0,'v'},
            {0,0,0,0}
        };
        int c = getopt_long(argc, argv, "hVm:b:p:l:B:f:r:a:gv", long_options, &option_index);
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
                    " --looplength, -l <int> Set loop length for the LOOPINGxx modes\n"
                    " --motionblur, -B <int> Set motion blur length for CHANGELOG mode\n"
                    " --firstlast, -f <int>  Set threshold for xxNMOST modes\n"
                    " --version, -V          Displays version information\n"
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
                    "     Produces an animation.\n"
                    "  LOOPINGLOG, long option: --methods=loopinglog, short option: -po\n"
                    "     Produces a time-restricted animation.\n"
                    "     Also called, \"lemmings mode\".\n"
                    "     Use the -l option to set loop length in frames.\n"
                    "  LOOPINGAVG, long option: --methods=loopingavg, short option: -pv\n"
                    "     A combination of loopinglog and actionavg.\n"
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
                pixelmethods_result = 0;
                for(; char* arg = std::strtok(optarg, ","); optarg=0)
                {
                    if(strcmp(arg, "*") == 0)
                        pixelmethods_result |= ((1ul << NPixelMethods) - 1);
                    #define TestMethod(optchar,f,name) \
                        else if(strcmp(arg, #optchar) == 0 || strcasecmp(arg, #name) == 0) \
                            pixelmethods_result |= 1ul << pm_##name##Pixel;
                    else if(strcmp(arg, "s") == 0 || strcasecmp(arg, "loopinglast") == 0)
                        pixelmethods_result |= 1ul << pm_LoopingLogPixel;
                    DefinePixelMethods(TestMethod)
                    #undef TestMethod
                    else
                    {
                        std::fprintf(stderr, "animmerger: Unrecognized method: %s\n", arg);
                        return -1;
                    }
                }
                if(!pixelmethods_result)
                {
                    std::fprintf(stderr, "animmerger: Error: No pixel method defined\n");
                    pixelmethods_result = 1ul << pm_MostUsedPixel;
                }
                break;
            }
            case 'b':
            {
                char* arg = optarg;
                if(false) {}
                #define TestMethod(optchar,f,name) \
                    else if(strcmp(arg, #optchar) == 0 || strcasecmp(arg, #name) == 0) \
                    { \
                        if(f & 1) goto is_animated; \
                        bgmethod = pm_##name##Pixel; \
                    }
                DefinePixelMethods(TestMethod)
                #undef TestMethod
                else
                {
                    std::fprintf(stderr, "animmerger: Unrecognized bgmethod: %s\n", arg);
                    return -1;
                is_animated:
                    std::fprintf(stderr, "animmerger: Background pixel method cannot be animated. Bad choice: %s\n", arg);
                }
                break;
            }
            case 'v':
                ++verbose;
                break;
            case 'g':
                SaveGif = 1;
                break;
        }
    }

    if(verbose)
    {
        const unsigned long AllUsedMethods = (1ul << bgmethod) | pixelmethods_result;

        std::printf("\tChosen background method: %s\n", PixelMethodNames[bgmethod]);
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

            unsigned p = tmp.y1 * sx + tmp.x1;
            for(unsigned y=0; y<tmp.height; ++y, p += (sx-tmp.width))
                for(unsigned x=0; x<tmp.width; ++x, ++p)
                {
                    if(a.colors.empty()
                    || a.colors.find(pixels[p]) != a.colors.end())
                    {
                        pixels[p] |= 0xFF000000u;
                    }
                }
        }

        tracker.FitScreenAutomatic(&pixels[0], sx,sy);

        //bgmethod = pixelmethod; NeedsBackgroundPixel = false;
        //tracker.FitScreen(&pixels[0], sx,sy,   0,0, false);

        tracker.NextFrame();
    }
    tracker.Save();
}
