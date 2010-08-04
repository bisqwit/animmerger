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

int main(int argc, char** argv)
{
    VecType<AlphaRange> alpha_ranges;

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
            {"refscale",   1,0,'r'},
            {"mvrange",    1,0,'a'},
            {"gif",        0,0,'g'},
            {0,0,0,0}
        };
        int c = getopt_long(argc, argv, "hVm:b:p:l:r:a:g", long_options, &option_index);
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
                    "                        Tip: Use -bl for some memory usage reduction\n"
                    " --looplength, -l <int> Set loop length for the LOOPINGLOG mode\n"
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
                    "  LAST, long option: --method=long , short option: -pl\n"
                    "     Produces a single image. Each pixel\n"
                    "     records the latest color addressing that pixel.\n"
                    "  MOSTUSED, long option: --method=mostused, short option: -pm\n"
                    "     Produces a single image. Each pixel\n"
                    "     records the color that most often occured in that location.\n"
                    "     Use this option for making maps!\n"
                    "  CHANGELOG, long option: --method=changelog, short option: -pc\n"
                    "     Produces an animation.\n"
                    "  LOOPINGLOG, long option: --methods=loopinglog, short option: -po\n"
                    "     Produces a time-restricted animation.\n"
                    "     Also called, \"lemmings mode\".\n"
                    "     Use the -l option to set loop length in frames.\n"
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
                    "Converting tile-*.png into a GIF animation:\n"
                    "   mogrify -format gif tile-????.png\n"
                    "   gifsicle -O2 -o animation.gif -l0 -d3 tile-????.gif\n"
                    "\n"
                    "Converting a GIF animation into individual frame files:\n"
                    "   gifsicle -U -E animation.gif\n"
                    "   animmerger <...> animation.gif.*\n"
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
            case 'l':
            {
                char* arg = optarg;
                LoopingLogLength = strtol(arg, 0, 10);
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
            case 'e':
            case 'p':
            {
                char* arg = optarg;
                if(strcmp(arg, "a") == 0 || strcmp(arg, "average") == 0)
                    pixelmethod = pm_AveragePixel;
                else if(strcmp(arg, "l") == 0 || strcmp(arg, "last") == 0)
                    pixelmethod = pm_LastPixel;
                else if(strcmp(arg, "m") == 0 || strcmp(arg, "mostused") == 0)
                    pixelmethod = pm_MostUsedPixel;
                else if(strcmp(arg, "c") == 0 || strcmp(arg, "changelog") == 0)
                    pixelmethod = pm_ChangeLogPixel;
                else if(strcmp(arg, "o") == 0 || strcmp(arg, "loopinglog") == 0)
                    pixelmethod = pm_LoopingLogPixel;
                else
                {
                    std::fprintf(stderr, "animmerger: Unrecognized method: %s\n", arg);
                    return -1;
                }
                break;
            }
            case 'b':
            {
                char* arg = optarg;
                if(strcmp(arg, "a") == 0 || strcmp(arg, "average") == 0)
                    bgmethod = pm_AveragePixel;
                else if(strcmp(arg, "l") == 0 || strcmp(arg, "last") == 0)
                    bgmethod = pm_LastPixel;
                else if(strcmp(arg, "m") == 0 || strcmp(arg, "mostused") == 0)
                    bgmethod = pm_MostUsedPixel;
                else if(strcmp(arg, "c") == 0 || strcmp(arg, "changelog") == 0)
                    bgmethod = pm_ChangeLogPixel;
                else if(strcmp(arg, "o") == 0 || strcmp(arg, "loopinglog") == 0)
                    bgmethod = pm_LoopingLogPixel;
                else
                {
                    std::fprintf(stderr, "animmerger: Unrecognized bgmethod: %s\n", arg);
                    return -1;
                }
                break;
            }
            case 'g':
                SaveGif = true;
                break;
        }
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
    tracker.SaveAndReset();
}
