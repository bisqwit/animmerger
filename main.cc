#include <gd.h>
#include "canvas.hh"
#include "settype.hh"
#include "align.hh"
#include "palette.hh"
#include "dither.hh"

#include "mask.hh"

#include <cstdio>
#include <algorithm>
#include <cctype>

#include <string.h>
#include <getopt.h>

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
} // namespace

int main(int argc, char** argv)
{
    bool bgmethod0_chosen = false;
    bool bgmethod1_chosen = false;
    bool dithering_configured = false;
    std::string color_compare_formula;

    bool opt_exit = false;
    int exit_code = 0;
    for(;;)
    {
        int option_index = 0;
        static struct option long_options[] =
        {
            {"help",       0,0,'h'},
            {"longhelp",   0,0,3001},
            {"fullhelp",   0,0,3002},
            {"version",    0,0,'V'},
            {"mask",       1,0,'m'},
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
            {"gif",        2,0,'g'},
            {"verbose",    0,0,'v'},
            {"yuv",        0,0,'y'},
            {"noalign",    0,0,4002},
            {"quantize",   1,0,'Q'},
            {"dithmethod", 1,0,'D'},
            {"ditherror",  1,0,5001},  {"de",1,0,5001},
            {"dithmatrix", 1,0,5002},  {"dm",1,0,5002},
            {"dithcount",  1,0,5003},  {"dc",1,0,5003},
            {"dithcombine",1,0,5004}, {"dr",1,0,5004}, {"dithcontrast",1,0,5004},
            {"deltae",     2,0,5005},  {"cie",2,0,5005},
            {"gamma",      1,0,'G'},
            {"output",     1,0,'o'},
            {"transform",  1,0,6001},
            {0,0,0,0}
        };
        int c = getopt_long(argc, argv, "hVm:b:p:l:B:f:r:a:g::vyu:Q:G:D:o:", long_options, &option_index);
        if(c == -1) break;
        switch(c)
        {
            case 'V':
                std::printf("%s\n", VERSION);
                opt_exit = true;
                break;
            case 'h':
            case 3001:
            case 3002:
            {
                struct
                {
                    inline void operator<< (const char *s)
                    {
                        std::fwrite(s, 1, std::strlen(s), stdout);
                        //std::printf("%s", s);
                    }
                } O;

                int v = c=='h' ? 0 : c-3000;

        if(v>=0)O << "\
animmerger v"VERSION" - Copyright (C) 2011 Joel Yliluoma (http://iki.fi/bisqwit/)\n\
\n\
Usage: animmerger [<options>] <imagefile> [<...>]\n\
\n\
Merges animation frames together with motion shifting.\n";
        if(v>=0)O << "\n\
General options:\n";
        if(v==0)O << "\
 --help, -h\n\
     Short help on usage. Use --longhelp or --fullhelp for more/all options.\n";
        if(v>=1)O << "\
 --help, -h\n\
     Short help on usage\n\
 --longhelp\n\
     List most commandline options\n\
 --fullhelp\n\
     List all commandline options and detailed help on usage\n";
        if(v>=1)O << "\
 --version, -V\n\
     Displays version information\n\
 --verbose, -v\n\
     Increase verbosity\n";
                O << "\n\
Canvas affecting options:\n";
        if(v>=2)O << "\
 --mask, -m <defs>\n\
     Define a mask, see instructions below\n\
 --maskmethod, -u <value>\n\
     Specify how the masked content will be hidden, see instructions below\n";
        if(v>=2)O << "\
 --method, -p <mode>\n\
     Select pixel type, see below\n";
        if(v<2)O << "\
 --method, -p <mode>\n\
     Select pixel type (average/actionavg/mostused/changelog/loopingavg)\n\
     See full help for details.\n";
        if(v>=0)O << "\
 --looplength, -l <int>\n\
     Set loop length for the LOOPINGxx modes\n";
        if(v>=1)O << "\
 --motionblur, -B <int>\n\
     Set motion blur length for animated modes\n";
        if(v>=2)O << "\
 --firstlast, -f <int>\n\
     Set threshold for xxNMOST modes\n\
 --yuv, -y\n\
     Specifies that average-colors are to be calculated in the YUV\n\
     colorspace rather than the default RGB colorspace.\n";
        if(v>=0)O << "\n\
Image aligning options:\n";
        if(v>=1)O << "\
 --bgmethod, -b <mode>\n\
     Select pixel type for alignment tests\n";
        if(v>=2)O << "\
 --bgmethod0 <mode>\n\
     Explicit pixel mode for ChangeLog background before camera comes\n\
 --bgmethod1 <mode>\n\
     Explicit pixel mode for ChangeLog background after camera leaves\n";
        if(v>=1)O << "\
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
     only scroll horizontally and by 4 pixels at most per frame.\n";
        if(v>=0)O << "\
 --noalign\n\
     Disable automatic image aligner. Useful if you only want to\n\
     utilize the dithering and quantization features of animmerger,\n\
     or your images simply don't happen to form a nice 2D map.\n";
        if(v>=0)O << "\n\
Output options:\n\
 --output, -o <filename/pattern>\n\
     Output to given filename. The filename may also be a pattern.\n\
     The default value is \"%%2$s-%1$04u.%3$s\".\n\
        %%2$s gets replaced with pixel method name (such as \"Average\") or \"tile\".\n\
        %%3$s gets replaced with \"gif\" or \"png\" depending on output format.\n\
        %%1$04u gets replaced with a sequential frame number, padded to 4 zero digits.\n";
        if(v>=1)O << "\
 --gif, -g [=always|=never|=auto]\n\
     Control how GIF files are saved. Always/never/auto.\n";
        if(v>=1)O << "\
     In automatic mode (default), GIF is selected for animations\n\
     if quantization was configured, PNG otherwise.\n\
     Default: auto. --gif without parameter defaults to always.\n\
     See below on details on when and how GIF files are written\n\
     depending on this option.\n";
        if(v<2)O << "\
 --quantize, -Q <method>,<num_colors>\n\
     Reduce palette using method (diversity/neuquant). See full help for details.\n\
 --quantize, -Q <file>\n\
     Load palette from the given file (PNG or GIF, must be paletted)\n\
 --quantize, -Q <R>x<G>x<B>\n\
     Set a regular RGB palette with given dimensions (e.g. 6x8x5).\n\
 --dithmethod, -D <method>[,<method>]\n\
     Select dithering method (ky/y2/floyd). See full help for details.\n";
        if(v>=2)O << "\
 --quantize, -Q <method>,<num_colors>\n\
     Reduce palette, see instructions below\n\
 --quantize, -Q <file>\n\
     Load palette from the given file (PNG or GIF, must be paletted)\n\
 --quantize, -Q <R>x<G>x<B>\n\
     Set a regular RGB palette with given dimensions.\n\
     Examples: -Q2x2x2 (8 color RGB extremes)\n\
               -Q6x6x6 (so called web-safe palette)\n\
               -Q6x8x5, -Q6x7x6, -Q8x8x4, -Q7x9x4 (some ~256 color palettes)\n\
 --dithmethod, -D <method>[,<method>]\n\
     Select dithering method (see full help)\n";
        if(v>=2)O << "\
 --ditherror, --de <float>\n\
     Set error multiplication value for the dithering algorithm.\n\
     0.0 = disable dithering. 1.0 = full dithering.\n\
     Usable values lie somewhere in between. Default: 1.0\n";
        if(v>=1)O << "\
 --dithmatrix, --dm <x>,<y>[<,time>]\n\
     Set the Bayer matrix size to be used in dithering.\n\
     Common values include 2x2, 4x4 and 8x8. Default: 8x8x1.\n";
        if(v>=2)O << "\
 --dithcount, --dc <int>\n\
     Set maximum number of palette colors to use in dithering\n\
     of an uniform color area of the source picture.\n\
     Value 1 disables dithering.\n\
     This number should not be made larger than x*y*time.\n\
     Default: 32 or x*y*time, whichever is smaller.\n\
 --dithcombine, --dr <float>[,<combinationlimit>[,<changeslimit>]]]\n\
     Set the maximum contrast between two or more color items\n\
     that are pre-selected for combined candidates for dithering.\n\
     The value must be in range 0..3. Default value: 1.\n\
     See details below.\n";
        if(v==1)O << "\
 --deltae, --cie [=<type>|=<formula>]\n\
     Select color comparison method (rgb/76/94/2000/cmc/bfd).\n\
     See full help for details.\n";
        if(v>=2)O << "\
 --deltae, --cie [=<type>|=<formula>]\n\
     Select color comparison method, see below for details.\n";
        if(v>=0)O << "\
 --gamma [=<value>]\n\
     Select gamma to use in dithering. Default: 1.0\n";
        if(v>=2)O << "\
 --transform { r= | g= | b= }<function>\n\
     Transform red, green and blue color channel values according\n\
     to the given mathematical function. See details below.\n";
        if(v>=2)O << "\n\
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
     A combination of loopinglog and actionavg, also supports motion blur.\n";
        if(v>=2)O << "\n\
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
         Alias: blank, black, censor\n";
        if(v>=2)O << "\n\
REDUCING PALETTE\n\
\n\
  GIF files are restricted to 256 colors, but regardless of whether\n\
  you use GIF or PNG for the output format, you can use the --quantize\n\
  option to reduce your images to a particular number of colors.\n\
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
    Octree ( example: --quantize=octree,4 or -Qo,4 )\n\
      Currently broken, don't use\n\
\n\
    NeuQuant ( example: --quantize=neuquant,16 or -Qq,16 )\n\
      A self-balancing Kohonen neural network is used to generate\n\
      an optimal palette for the imageset. Fast and very high quality.\n\
      Especially good with color gradients. Not useful for smallest palettes.\n\
\n\
    Load from file ( example: --quantize=test.gif )\n\
      Animmerger will attempt to open the named file, read the\n\
      palette from it and append its colors to the current palette\n\
      (or replace with loaded palette if was the first -Q option).\n\
\n\
  Multiple quantization phases can be performed in a sequence.\n\
  For example, -Qb,32 -Qd,16 first reduces with \"blend-diversity\"\n\
  to 32 colors, then reduces the remaining set with \"diversity\" to 16 colors.\n\
  It is not an error to reduce to a larger set than 256 colors.\n\
  If you are creating a GIF file and the last explicitly chosen quantization\n\
  method yields more than 256 colors, the colormap will be implicitly reduced\n\
  with a method that picks the 256 most-used colors.\n\
  If necessary, the image will be dithered using a positional dithering method.\n\
  If you are making a GIF file and you do not specify any quantization options\n\
  at all, animmerger will use whatever method GD graphics library happens to use.\n\
  Note that the blending quantization methods are subject to the YUV selection.\n";
        if(v>=2)O << "\n\
DITHERING\n\
\n\
  Dithering methods\n\
     The following dithering methods are defined:\n\
      FULL NAME            SHORT NAME    Suitable for animation\n\
       Yliluoma1            y1            Yes (positional)\n\
       Yliluoma2            y2            Yes (positional)\n\
       Yliluoma3            y3            Yes (positional)\n\
       Yliluoma1Iterative   y1i / ky      Yes (positional)\n\
       Floyd-Steinberg      fs / floyd    No (diffuses +4 pixels)\n\
       Jarvis-Judice-Ninke  jjn           No (diffuses +12 pixels)\n\
       Stucki               s             No (diffuses +12 pixels)\n\
       Burkes               b             No (diffuses +7 pixels)\n\
       Sierra-3             s3            No (diffuses +10 pixels)\n\
       Sierra-2             s2            No (diffuses +7 pixels)\n\
       Sierra-2-4A          s24a          No (diffuses +3 pixels)\n\
       Stevenson-Arce       sa            No (diffuses +12 pixels)\n\
       Atkinson             a             No (diffuses +6 pixels, though only 75%)\n\
     To set the dithering method, use the --dithmethod or -D option.\n\
     Examples:\n\
       -Dy1i or --dithmethod=ky (default)\n\
       -Dfloyd-steinberg\n\
       -Ds24a,y2 --dr=2 --dc=4 --dm 8x8\n\
\n\
  Dithering matrix size\n\
     You can use an uneven ratio such as 8x2 to produce images\n\
     that are displayed on a device where pixels are not square.\n\
     The values should be powers of two, but it is not required.\n\
     A non-poweroftwo dimension will produce uneven dithering.\n\
     The third dimension, time, can be specified in order to\n\
     use temporal dithering, which will appear as flickering.\n\
     For example, 2x2x2 will use a 2x2 matrix plus 2 frames of\n\
     flickering to produce the average color. By default, temporal\n\
     dithering is done from the LSB of the color error, so as to minimize\n\
     the flicker with cost to spatial accuracy. By specifying a negative time\n\
     value, such as 2x2x-2, it will be done from the MSB, causing much more\n\
     prominent flickering, while improving the spatial accuracy.\n\
\n\
  Dithering combine control (--dithcombine)\n\
     The contrast is specified as a sliding scale where\n\
       0 means that no combinations are loaded.\n\
       1 represents the average luma difference between\n\
         two successive colors in luma-sorted palette,\n\
       2 represents the maximum luma difference between two\n\
         two successive colors in luma-sorted palette,\n\
       3 represents the maximum luma coverage of the\n\
         palette, in practice allowing all combinations.\n\
     In general, a higher number will produce more ugly dithering\n\
     and will slow down the dithering algorithm a great deal too,\n\
     so animmerger tries to choose a reasonable (low) default value.\n\
     If you have lots of time and you're rendering a high-resolution\n\
     picture, you can try 3. Otherwise, less than 1.3 is a safe bet.\n\
     Note that a low value of dithcount can make this option useless.\n\
     \n\
     --dithcombine takes up to three parameters:\n\
         <float>\n\
            Contrast, as described above.\n\
         <combinationlimit>\n\
            Maximum number of colors to combine.\n\
            This value defaults to DithCount, but it may be lower.\n\
            Lower values are faster. It must be at least 1.\n\
         <changeslimit>\n\
            Maximum number of _different_ colors to combine.\n\
            For example, depth 16 & changes 2 means that up to 16 colors\n\
            are mixed, but the list can contain only 2 distinct colors.\n\
            If you specify a negative value, it means that identical colors\n\
            are not mixed together at all; all candidate components are distinct.\n\
            It is the fastest option, though not best quality.\n\
     Examples:\n\
       --dithcombine 2,4,2\n\
          Select all combinations of up to 4 palette colors where:\n\
            - The difference between dimmest and brightest colors in the mix\n\
              is equal or less than 100% of the maximum difference between\n\
              consecutive elements in the luma-sorted palette\n\
            - Only 2 distinct palette indices may occur in the mix\n\
       --dithcombine 3,16,-1\n\
          Select all combinations of up to 16 palette colors where:\n\
            - All palette indices are distinct (same index does not appear twice)\n\
            - No restrictions on how bright&dim colors are mixed together\n\
       --dithcombine 0\n\
          No precalculated mixes. For Yliluoma1 and Yliluoma3 dithers,\n\
          this means that no dithering is done at all. For Yliluoma1Improved\n\
          and Yliluoma2, this is the fastest possible option.\n\
\n\
Ordered dithering method differences:\n\
\n\
  Yliluoma1\n\
     Completely reliant on --dithcombine to provide\n\
     the selection of mix candidates.\n\
     Ignores the --ditherror parameter.\n\
     Ignores the --dithcount parameter (which still controls the defaults\n\
                                        to --dithcombine though)\n\
     Fastest. Depending on --dithcombine, quality may be good or bad.\n\
  Yliluoma1Iterative\n\
     Can be improved with --dithcombine, but does not depend on it at all.\n\
     The only dither that uses --ditherror parameter.\n\
     Fast. Quality adequate in most cases.\n\
  Yliluoma2\n\
     Can be improved with --dithcombine.\n\
     Ignores the --ditherror parameter.\n\
     Slow.\n\
  Yliluoma3\n\
     Only uses color combinations of 1 or 2 items.\n\
     Ignores the --ditherror parameter.\n\
     Slow.\n";
        if(v>=2)O << "\n\
COLOR TRANSFORMATION FUNCTION\n\
\n\
  The option --transform can be used to transform the image's color.\n\
\n\
  The following identifiers are defined for the function:\n\
    r,g,b   Input color (0..255)\n\
    frameno Frame number (0..n)\n\
    x,y     Screen coordinates (x,y)\n\
\n\
  Note that when animmerger counts color, it\n\
  will pass bogus coordinates to the x,y values.\n\
  The output value is expected to be in range 0..255, though not required.\n\
  For a description of the accepted function syntax, see:\n\
         http://iki.fi/warp/FunctionParser/\n\
\n\
  Examples:\n\
    --transform 'r=g=b=(r*0.299+g*0.587+b*0.114)'\n\
        Renders grayscale rather than color.\n\
    --transform 'r=0x80+(r/2)'\n\
        Makes image considerably redder.\n\
    --transform 'r=128+127*sin(frameno*.1+x/40+y/90)' \\\n\
    --transform 'g=128+127*cos(frameno*.1+x/40+y/90)' \\\n\
    --transform 'b=128+127*sin(frameno*.1+x/40+y/90+20)'\n\
        Will make the screen cycle in colors.\n\
\n\
  Note that rendering with a transformation function is much\n\
  slower than rendering without it.\n";
        if(v>=2)O << "\n\
COLOR COMPARE METHODS\n\
\n\
  For dithering purposes, animmerger has to compare colors\n\
  and decide out of many options which combination represents\n\
  the desired color best.\n\
\n\
  The comparison algorithm can be selected from the following choices:\n\
  \n\
    default   = 0    = RGB                 e.g. --deltae=0 or --deltae=rgb\n\
    CIE76     = 76   = CIE76 Delta E       e.g. --deltae or --deltae=76\n\
    CIE94     = 94   = CIE94 Delta E       e.g. --deltae=94 or --deltae=cie94\n\
    CIEDE2000 = 2000 = CIEDE2000 Delta E   e.g. --deltae=2000\n\
    CMC              = CMC l:c Delta E     e.g. --deltae=cmc\n\
    BFD              = BFD l:c Delta E     e.g. --deltae=bfd\n\
    user-defined  (see below)\n\
  \n\
  When a CIE method is selected, colors are compared in the CIE L*a*b* colorspace.\n\
  Animmerger converts RGB values into CIE using an Adobe D65 illuminant profile or\n\
  a close equivalent.\n\
\n\
  Performance:\n\
     RGB and CIE76 are simple euclidean differences.\n\
       Because animmerger will calculate the CIE L*a*b* value\n\
       for each color regardless of whether you use RGB or not,\n\
       there is no speed difference between these two.\n\
     CIE94 includes more mathematics than RGB or CIE74.\n\
     CMC   is complex, and not always very good.\n\
     CIEDE2000 includes very complicated mathematics,\n\
               and can be expected to be very slow.\n\
     BFD   is very complex.\n\
  \n\
  It is also possible to use a homebrew color comparison formula.\n\
  Examples:\n\
    --deltae='(R1-R2)^2 + (G1-G2)^2 + (B1-B2)^2'\n\
      This is equivalent to --deltae=RGB, though --deltae=RGB is faster.\n\
      Note that it is not necessary to take the square root of the result,\n\
      because animmerger only cares about whether a deltae value is larger\n\
      or smaller than another, not about its exact value.\n\
    --deltae='(L1-L2)^2 + (a1-a2)^2 + (b1-b2)^2'\n\
      This is equivalent to --deltae=CIE76, though --deltae=CIE76 is faster.\n\
    --deltae='abs(luma1-luma2)'\n\
      This simply compares luminosity and disregards any color information.\n\
    --deltae='hypot(a1-a2, b1-b2)'\n\
      This simply compares chroma and disregards luminance.\n\
  Variables supported in the color comparison formula:\n\
    R1,G1,B1,A1     -- RGB+alpha value (0..1 range)\n\
    L1,a1,b1,C1,h1  -- L*a*b* and L*C*h[ab] values (unspecified range)\n\
                       Note that h is indicated in radians, not degrees\n\
    luma1           -- Equivalent to R1*.299 + G1*.587 + B1*.114\n\
    And the same for color 2 (replace 1 with 2)\n\
    gamma           -- Configured gamma correction rate\n\
  Functions supported in the color comparison formula:\n\
    Standard fparser functions such as cos,atan2,asinh,log10\n\
    g(x) is equivalent to x^gamma and ug(x) is equivalent to x^(1/gamma).\n";
        if(v>=2)O << "\n\
GIF VERSUS PNG AND WHAT ANIMMERGER CREATES\n\
  GIF is capable of paletted images of 256 colors or less.\n\
  PNG is capable of paletted images, as well as truecolor images.\n\
  \n\
  GIF selection  Quantization  Saves in format            Dithering used\n\
  --auto         -Q was used   Animations=GIF, other=PNG  For GIF, unless disabled\n\
  --auto         -Q NOT used   Always PNG                 Never\n\
  --never        -Q was used   Always paletted PNG        Yes, unless disabled\n\
  --never        -Q NOT used   Always trueclor PNG        Never\n\
  --always       -Q was used   Always GIF                 Yes, unless disabled\n\
  --always       -Q not used   Always GIF                 Never\n";
        if(v>=2)O << "\n\
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
implementation that provides all of the requested features.\n";
        if(v>=1)O << "\n\
When creating animations of video game content, please take\n\
all necessary steps to ensure that background stays fixed\n\
while characters move. Parallax animation is bad; If possible,\n\
please fix all background layers so that they scroll at even\n\
rate.\n\
\n";
                opt_exit = true;
                break;
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
                    opt_exit = true; exit_code = 1;
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
                    opt_exit = true; exit_code = 1;
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
                    opt_exit = true; exit_code = 1;
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
                    opt_exit = true; exit_code = 1;
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
                    opt_exit = true; exit_code = 1;
                }
                break;
            }
            case 'o':
            {
                OutputNameTemplate = optarg;
                break;
            }
            case 'Q':
            {
                char *arg = optarg;
                std::vector<uint32> colors;
                if(access(arg, R_OK) == 0)
                {
                    FILE* fp = fopen(arg, "rb");
                    if(!fp)
                    {
                        std::perror(arg);
                        opt_exit = true; exit_code = errno;
                        break;
                    }
                    gdImagePtr im = gdImageCreateFromPng(fp);
                    if(!im) { std::rewind(fp); im = gdImageCreateFromGif(fp); }
                    if(!im) { std::rewind(fp); im = gdImageCreateFromWBMP(fp); }
                    if(!im)
                    {
                        std::fprintf(stderr,
                            "%s: Not a PNG, GIF or WBMP file! Cannot read palette.\n",
                                arg);
                        opt_exit = true; exit_code = 1;
                    }
                    else
                    {
                        if(gdImageTrueColor(im))
                        {
                            std::fprintf(stderr,
                                "%s: Not a paletted picture! Will hastily use libGD to make a 256-color palette out of it regardless.\n",
                                arg);
                            gdImageTrueColorToPalette(im, 0, 256);
                        }
                        unsigned n_colors = gdImageColorsTotal(im);
                        colors.reserve(n_colors);
                        for(unsigned c=0; c<n_colors; ++c)
                        {
                            int r = gdImageRed(im,c);
                            int g = gdImageGreen(im,c);
                            int b = gdImageBlue(im,c);
                            int a = gdImageAlpha(im,c);
                            colors.push_back( gdTrueColorAlpha(r,g,b, a) );
                        }
                        if(verbose >= 1)
                            std::fprintf(stderr, "Loaded %u colors from %s\n", n_colors, arg);
                        gdImageDestroy(im);
                    }
                    std::fclose(fp);
                    goto opt_Q_handled;
                }
               {int r_dim=0;
                int g_dim=0;
                int b_dim=0;
                // Try parse as RxGxB expression
                char* q = arg;
                while(*q>='0' && *q<='9') { r_dim = r_dim*10 + (*q++ - '0'); }
                if(*q != 'x') goto not_rgb_Q; ++q;
                while(*q>='0' && *q<='9') { g_dim = g_dim*10 + (*q++ - '0'); }
                if(*q != 'x') goto not_rgb_Q; ++q;
                while(*q>='0' && *q<='9') { b_dim = b_dim*10 + (*q++ - '0'); }
                if(*q != '\0') goto not_rgb_Q;
                if(r_dim < 2 || g_dim < 2 || b_dim < 2
                || r_dim > 256 || g_dim > 256 || b_dim > 256)
                {
                    std::fprintf(stderr, "animmerger: Invalid parameter to -Q: %s\n", arg);
                    opt_exit = true; exit_code = 1;
                }
                else
                {
                    for(int r=0; r<r_dim; ++r)
                    {
                        unsigned rr = (((r*2)*255/(r_dim-1)+1)/2) << 16;
                        for(int g=0; g<g_dim; ++g)
                        {
                            unsigned gg = (((g*2)*255/(g_dim-1)+1)/2) << 8;
                            unsigned rrgg = rr + gg;
                            for(int b=0; b<b_dim; ++b)
                            {
                                unsigned bb = ((b*2)*255/(b_dim-1)+1)/2;
                                colors.push_back(rrgg+bb);
                            }
                        }
                    }
                    goto opt_Q_handled;
                }}
            not_rgb_Q:;
               {char *comma = std::strchr(arg, ',');
                if(!comma)
                {
                    std::fprintf(stderr, "animmerger: Invalid parameter to -Q: %s\n", arg);
                    opt_exit = true; exit_code = 1;
                }
                else
                {
                opt_q_got_comma:;
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
                        /* Check if the name looks like a hex color */
                        char* endpos = 0;
                        long colorvalue = strtol(arg, &endpos, 16);
                        if(comma == arg+6 && endpos == comma)
                        {
                            colors.push_back(colorvalue);
                            arg = comma+1;
                            comma = std::strchr(arg, ',');
                            if(comma) goto opt_q_got_comma;
                            colorvalue = strtol(arg, 0, 16);
                            colors.push_back(colorvalue);
                        }
                        else
                        {
                            std::fprintf(stderr, "animmerger: Unknown quantization mode: %s\n", arg);
                            opt_exit = true; exit_code = 1;
                        }
                    }
                    if(method.size)
                    {
                        long ncolors = strtol(comma+1, 0, 10);
                        if(ncolors < 1 || ncolors > (1u<<24))
                        {
                            std::fprintf(stderr, "animmerger: Invalid palette size: %ld\n", ncolors);
                            opt_exit = true; exit_code = 1;
                        }
                        else
                        {
                            method.size = ncolors;
                            PaletteReductionMethod.push_back(method);
                        }
                    }
                    #undef AddOption
                }}
            opt_Q_handled:;
                if(!colors.empty())
                {
                    if(verbose >= 1)
                        std::fprintf(stderr, "Adding fixed palette of %u colors\n",
                            (unsigned) colors.size());
                    PaletteMethodItem method;
                    method.size     = 0;
                    method.entries.swap(colors);
                    PaletteReductionMethod.push_back(method);
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
                    opt_exit = true; exit_code = 1;
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

            case 'D': // dithmethod, D
            {
                Diffusion = Diffusion_None;
                Dithering = Dither_Yliluoma1Iterative;
                bool positional_defined = false;
                std::string optarg_save(optarg);
                bool errors = false;
                for(char* arg = optarg; ; arg = 0)
                {
                    char* cm[128] = { 0 }; // List of character that exist in the input
                    char* section = std::strtok(arg, ",");
                    if(!section) break;
                    for(; *section; ++section)
                    {
                        unsigned char c = std::tolower(*section);
                        if(c < 128 && !cm[c]) cm[c] = section;
                    }
                    /**/ if(cm['k'] && cm['y']) { Dithering = Dither_Yliluoma1Iterative; positional_defined = true; }
                    else if(cm['y'] && cm['1']
                         && cm['i'] && cm['1'][1]=='i') 
                                                { Dithering = Dither_Yliluoma1Iterative; positional_defined = true; }
                    else if(cm['y'] && cm['1']) { Dithering = Dither_Yliluoma1; positional_defined = true; }
                    else if(cm['y'] && cm['2']) { Dithering = Dither_Yliluoma2; positional_defined = true; }
                    else if(cm['y'] && cm['3']) { Dithering = Dither_Yliluoma3; positional_defined = true; }
                    else if(cm['f'] && (cm['s']||cm['y']>cm['f']))
                                                Diffusion = Diffusion_FloydSteinberg;
                    else if(cm['j'] && cm['n']) Diffusion = Diffusion_JarvisJudiceNinke;
                    else if(cm['b'])            Diffusion = Diffusion_Burkes;
                    else if(cm['s'] && cm['4']) Diffusion = Diffusion_Sierra24A;
                    else if(cm['s'] && cm['3']) Diffusion = Diffusion_Sierra3;
                    else if(cm['s'] && cm['2']) Diffusion = Diffusion_Sierra2;
                    else if(cm['s'] && cm['a']
                         && cm['s'] <  cm['a']) Diffusion = Diffusion_StevensonArce;
                    else if(cm['a'])            Diffusion = Diffusion_Atkinson;
                    else if(cm['s'])            Diffusion = Diffusion_Stucki;
                    else errors = true;
                }
                if(!positional_defined && Diffusion != Diffusion_None)
                    { DitherMatrixWidth = DitherMatrixHeight = 1;
                      DitherColorListSize = 1; }
                if(errors)
                {
                    std::fprintf(stderr, "animmerger: Bad dither method selection: %s.\n", optarg_save.c_str());
                    opt_exit = true; exit_code = 1;
                }
                break;
            }

            case 5001: // ditherror, de
            {
                char* arg = optarg;
                double tmp = strtod(arg, 0);
                DitherErrorFactor = tmp;
                if(tmp < 0 || tmp >= 256.0)
                {
                    std::fprintf(stderr, "animmerger: Bad dither error multiplication value: %g. Valid range: 0..1, though 0..255 is permitted.\n", tmp);
                    opt_exit = true; exit_code = 1;
                    DitherErrorFactor = 1.0;
                }
                dithering_configured = true;
                break;
            }
            case 5002: // dithmatrix, dm
            {
                char* arg = optarg;
                for(char*s = arg; *s; ++s) if(*s == 'x') *s = ',';
                int dx,dy,dt;
                int result = sscanf(arg, "%d,%d,%d", &dx,&dy,&dt);
                if(result < 2)
                {
                    std::fprintf(stderr, "animmerger: Syntax error in '%s'\n", arg);
                    opt_exit = true; exit_code = 1;
                }
                else if(dx < 1 || dy < 1 || dx*dy > 65536)
                {
                    std::fprintf(stderr, "animmerger: Bad dither matrix size: %dx%d.\n", dx,dy);
                    opt_exit = true; exit_code = 1;
                }
                else
                {
                    DitherMatrixWidth  = dx;
                    DitherMatrixHeight = dy;
                }
                if(result >= 3)
                {
                    if(dt < -64 || dt > 64 || dt == -1 || dt == 0)
                    {
                        std::fprintf(stderr, "animmerger: Bad temporal dither length: %d. Valid range is -64..-2 and +1..+64, where +1 disables temporal dithering.\n", dt);
                        opt_exit = true; exit_code = 1;
                    }
                    else if(dt < 0)
                    {
                        TemporalDitherSize = -dt;
                        TemporalDitherMSB  = true;
                    }
                    else
                    {
                        TemporalDitherSize = dt;
                        TemporalDitherMSB  = false;
                    }
                }
                dithering_configured = true;
                break;
            }
            case 5003: // dithcount, dc
            {
                char* arg = optarg;
                long tmp = strtol(arg, 0, 10);
                DitherColorListSize = tmp;
                if(tmp != DitherColorListSize || tmp < 1 || tmp > 65536)
                {
                    std::fprintf(stderr, "animmerger: Bad dither color list size: %ld. Valid range: 1..65536\n", tmp);
                    DitherColorListSize = 0;
                    opt_exit = true; exit_code = 1;
                }
                dithering_configured = true;
                break;
            }
            case 5004: // dithcombine, dr
            {
                char* arg = optarg;
                double   contrast       = -1;
                long     recursionlimit = 0;
                long     changeslimit   = 0;
                int result = sscanf(arg, "%lf,%ld,%ld", &contrast,&recursionlimit,&changeslimit);
                DitherCombinationContrast = -1.0; // re-enable heuristic.
                if(result < 1)
                {
                    std::fprintf(stderr, "animmerger: Syntax error in '%s'\n", arg);
                    opt_exit = true; exit_code = 1;
                }
                else if(contrast < 0.0 || contrast > 3.0)
                {
                    std::fprintf(stderr, "animmerger: Bad dither contrast parameter: %g. Valid range: 0..3\n", contrast);
                    opt_exit = true; exit_code = 1;
                }
                else
                    DitherCombinationContrast = contrast;
                if(result >= 2)
                {
                    if(recursionlimit < 1)
                    {
                        std::fprintf(stderr, "animmerger: Bad dither combination limit: %ld\n", recursionlimit);
                        opt_exit = true; exit_code = 1;
                    }
                    else
                        DitherCombinationRecursionLimit = recursionlimit;
                    if(result >= 3)
                    {
                        if(changeslimit < 0)
                            { DitherCombinationChangesLimit = ~0u;
                              DitherCombinationAllowSame    = false; }
                        else if(changeslimit < 1)
                        {
                            std::fprintf(stderr, "animmerger: Bad dither combination changes limit: %ld\n", changeslimit);
                            opt_exit = true; exit_code = 1;
                        }
                        else
                        {
                            DitherCombinationChangesLimit = changeslimit;
                            DitherCombinationAllowSame    = true;
                        }
                    }
                }
                dithering_configured = true;
                break;
            }
            case 5005: // deltae
                if(optarg)
                {
                    if(std::strcmp(optarg, "default") == 0
                    || std::strcmp(optarg, "0") == 0
                    || std::strcmp(optarg, "rgb") == 0
                    || std::strcmp(optarg, "RGB") == 0)
                        ColorComparing = Compare_RGB;
                    else if(std::strcmp(optarg, "implied") == 0
                         || std::strcmp(optarg, "simple") == 0
                         || std::strcmp(optarg, "1") == 0
                         || std::strcmp(optarg, "76") == 0
                         || std::strcmp(optarg, "cie76") == 0
                         || std::strcmp(optarg, "CIE76") == 0)
                        ColorComparing = Compare_CIE76_DeltaE;
                    else if(std::strcmp(optarg, "94") == 0
                         || std::strcmp(optarg, "cie94") == 0
                         || std::strcmp(optarg, "CIE94") == 0)
                        ColorComparing = Compare_CIE94_DeltaE;
                    else if(std::strcmp(optarg, "cmc") == 0
                         || std::strcmp(optarg, "CMC") == 0)
                        ColorComparing = Compare_CMC_lc;
                    else if(std::strcmp(optarg, "bfd") == 0
                         || std::strcmp(optarg, "BFD") == 0)
                        ColorComparing = Compare_BFD_lc;
                    else if(std::strcmp(optarg, "2000") == 0
                         || std::strcmp(optarg, "ciede2000") == 0
                         || std::strcmp(optarg, "CIEDE2000") == 0
                         || std::strcmp(optarg, "cie2000") == 0
                         || std::strcmp(optarg, "CIE2000") == 0)
                        ColorComparing = Compare_CIEDE2000_DeltaE;
                    else
                    {
                        ColorComparing = Compare_fparser;
                        color_compare_formula += optarg;
                    }
                }
                else
                    ColorComparing = Compare_CIE76_DeltaE;
                break;

            case 'v':
                ++verbose;
                break;
            case 'y':
                AveragesInYUV = true;
                break;
            case 'g':
                if(optarg)
                {
                    if(std::strcmp(optarg, "auto") == 0)
                        SaveGif = -1;
                    else if(std::strcmp(optarg, "always") == 0
                         || std::strcmp(optarg, "yes") == 0
                         || std::strcmp(optarg, "1") == 0)
                        SaveGif = 1;
                    else if(std::strcmp(optarg, "never") == 0
                         || std::strcmp(optarg, "none") == 0
                         || std::strcmp(optarg, "no") == 0
                         || std::strcmp(optarg, "0") == 0)
                        SaveGif = 0;
                    else
                    {
                        std::fprintf(stderr, "animmerger: Invalid parameter to --gif: %s. Allowed values: auto, always, never\n",
                            optarg);
                        opt_exit = true; exit_code = 1;
                    }
                }
                else
                    SaveGif = 1;
                break;
            case 6001: // transform
            {
                char* arg = optarg;
                bool touch_r = false;
                bool touch_g = false;
                bool touch_b = false;
                for(;;)
                {
                    if(strncmp(arg, "r=", 2) == 0) { touch_r=true; arg+=2; continue; }
                    if(strncmp(arg, "g=", 2) == 0) { touch_g=true; arg+=2; continue; }
                    if(strncmp(arg, "b=", 2) == 0) { touch_b=true; arg+=2; continue; }
                    break;
                }
                if(!touch_r && !touch_g && !touch_b)
                {
                    transform_common += arg;
                }
                if(touch_r) transform_r = arg;
                if(touch_g) transform_g = arg;
                if(touch_b) transform_b = arg;
                break;
            }
            case 'G': // gamma
            {
                char* arg = optarg;
                double tmp = strtod(arg, 0);
                DitherGamma = tmp;
                if(tmp <= 0.0 || tmp > 50.0)
                {
                    std::fprintf(stderr, "animmerger: Bad dither gamma parameter: %g. Valid range: > 0\n", tmp);
                    DitherGamma = 1.0;
                    opt_exit = true; exit_code = 1;
                }
                break;
            }
        }
    }
    SetColorTransformations();

    if(!color_compare_formula.empty())
        SetColorCompareFormula(color_compare_formula);

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

    if(dithering_configured && PaletteReductionMethod.empty())
    {
        std::fprintf(stderr,
            "animmerger: Warning: Dithering will not be done when the palette is not reduced by animmerger. Even if you specify GIF output format but no quantization methods (--quantize), dithering will not be used.\n");
    }

    if((DitherMatrixWidth & (DitherMatrixWidth-1))
    || (DitherMatrixHeight & (DitherMatrixHeight-1)))
    {
        std::fprintf(stderr, "animmerger: Warning: Dither matrix dimensions should be given as powers of two. %dx%d will work, but it might not look pretty, because animmerger generates the matrix with an algorithm designed for power-of-two dimensions.\n",
            DitherMatrixWidth,DitherMatrixHeight);
    }

    if(DitherColorListSize > DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize)
    {
        std::fprintf(stderr, "animmerger: Warning: It is probably pointless to generate %u candidate colors for dithering when the dithering matrix will only utilize at most %u at any time (%ux%ux%u). Recommend option: --dc%u\n",
            DitherColorListSize,
            DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize,
            DitherMatrixWidth, DitherMatrixHeight, TemporalDitherSize,
            DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize);
    }
    if(DitherErrorFactor > 1.0)
    {
        std::fprintf(stderr, "animmerger: Warning: Dither error multiplication factor should not be greater than 1.0. You chose %g. Hope you know what you are doing.\n",
            DitherErrorFactor);
    }
    if(DitherColorListSize == 0)
        DitherColorListSize = std::min(32u,
           DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize);

    if(DitherMatrixWidth == 4
    && DitherMatrixHeight == 4
    && TemporalDitherSize == 1
    && DitherColorListSize == 16
    && DitherCombinationContrast == 0.0
    && Diffusion == Diffusion_None
    && Dithering == Dither_Yliluoma1Iterative)
    {
        std::fprintf(stderr,
            "animmerger: Sorry, I cannot obey this combination of options.\n"
            "            The specific configuration you requested happens to be\n"
            "            patented by Adobe Systems Incorporated (US patent 6606166).\n"
            "            And due to that patent, this method cannot be implemented\n"
            "            in free software. Method patents are annoying, we all know.\n");
        opt_exit = true;
        exit_code = -1;
    }

    if(opt_exit)
        return exit_code;


    switch(Dithering)
    {
        case Dither_Yliluoma1:
            // Finding the single best-matching candidate
            if(DitherCombinationRecursionLimit == 1
            || DitherCombinationChangesLimit==1)
            {
                fprintf(stderr, "animmerger: Warning: For Yliluoma1 dithering to work properly, more than 1 distinct elements should be allowed in a mix (--dithcombine).\n");
            }
            break;
        case Dither_Yliluoma1Iterative:
            // Finding the best-matching candidate, repeat with error diffusion
            if(DitherCombinationAllowSame
            && DitherCombinationChangesLimit > DitherCombinationRecursionLimit)
            {
            }
            break;
        case Dither_Yliluoma2:
            // Find the best-matching candidate, repeat find best-improving candidate with count
            break;
        case Dither_Yliluoma3:
            // Subdivision of DitherColorListSize, replacing with colorpairs or colors
            if(DitherCombinationRecursionLimit == 0)
                DitherCombinationRecursionLimit = 2;
            else if(DitherCombinationRecursionLimit > 2)
            {
                fprintf(stderr, "animmerger: Warning: For Yliluoma3 dithering, upper limit for combination limit is 2.\n");
                DitherCombinationRecursionLimit = 2;
            }
            if(DitherCombinationChangesLimit == 0)
                DitherCombinationChangesLimit = 2;
            if(DitherCombinationChangesLimit != 2)
            {
                fprintf(stderr, "animmerger: Warning: For Yliluoma3 dithering to work properly, changes limit should be 2.\n");
                if(DitherCombinationChangesLimit > 2)
                    DitherCombinationRecursionLimit = 2;
            }
            DitherCombinationAllowSame = false;
            break;
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

        if(!PaletteReductionMethod.empty())
        {
            std::printf("\tPalette reduction if necessary: ");
            for(size_t b = PaletteReductionMethod.size(), a=0; a<b; ++a)
            {
                if(a) std::printf(", followed by ");
                if(!PaletteReductionMethod[a].entries.empty())
                {
                    unsigned n_colors = PaletteReductionMethod[a].entries.size();
                    std::printf("fixed palette of %u colors", n_colors);
                    if(verbose >= 2)
                    {
                        std::printf(":");
                        for(unsigned c=0; c<n_colors; ++c)
                            std::printf(" %06X",
                                (unsigned) PaletteReductionMethod[a].entries[c]);
                    }
                    continue;
                }
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

            if(DitherColorListSize == 1
            || DitherErrorFactor   == 0.0
            || (DitherCombinationRecursionLimit==1 &&
                           (Dithering == Dither_Yliluoma1
                         || Dithering == Dither_Yliluoma3))
              )
            {
                std::printf(
                    "\tPositional dithering disabled\n");
            }
            else
            {
                const char* methodname = "?";
                switch(Dithering)
                {
                    case Dither_Yliluoma1:
                        methodname = "Yliluoma1 ordered dithering (single-shot)"; break;
                    case Dither_Yliluoma2:
                        methodname = "Yliluoma2 positional dithering"; break;
                    case Dither_Yliluoma3:
                        methodname = "Yliluoma3 positional dithering"; break;
                    case Dither_Yliluoma1Iterative:
                        methodname = "Yliluoma1 ordered dithering (iterative)"; break;
                }
                std::printf(
                    "\tDithering method: %s\n"
                    "\tDithering combination control: contrast=%g, recursionlimit=%u%s, maxchanges=%u%s, allowsame=%s\n"
                    "\tDithering with %u color choices, Bayer matrix size: %ux%u",
                    methodname,
                    DitherCombinationContrast,
                    DitherCombinationRecursionLimit,
                        DitherCombinationRecursionLimit?"":" (auto)",
                    DitherCombinationChangesLimit,
                        DitherCombinationChangesLimit  ?"":" (auto)",
                    DitherCombinationAllowSame ? "yes" : "no",
                    DitherColorListSize,
                    DitherMatrixWidth, DitherMatrixHeight);
                if(DitherMatrixWidth * DitherMatrixHeight * TemporalDitherSize == 1)
                    std::printf(" (DEFUNCT");
                if(TemporalDitherSize > 1)
                {
                    std::printf(" + %u-frame temporal dithering (with %s)",
                        TemporalDitherSize,
                        TemporalDitherMSB ? "MSB" : "LSB");
                }
                std::printf("\n");
                if(Dithering == Dither_Yliluoma1Iterative)
                {
                    std::printf("\tDither color error spectrum size: %g\n",
                        DitherErrorFactor);
                }
            }
            if(Diffusion != Diffusion_None)
            {
                const char* methodname = "?";
                switch(Diffusion)
                {
                    case Diffusion_None: break;
                    case Diffusion_FloydSteinberg:
                        methodname = "Floyd-Steinberg"; break;
                    case Diffusion_JarvisJudiceNinke:
                        methodname = "Jarvis-Judice-Ninke"; break;
                    case Diffusion_Stucki:
                        methodname = "Stucki"; break;
                    case Diffusion_Burkes:
                        methodname = "Burkes"; break;
                    case Diffusion_Sierra3:
                        methodname = "Sierra-3"; break;
                    case Diffusion_Sierra2:
                        methodname = "Sierra-2"; break;
                    case Diffusion_Sierra24A:
                        methodname = "Sierra-2-4A"; break;
                    case Diffusion_StevensonArce:
                        methodname = "Stevenson-Arce"; break;
                    case Diffusion_Atkinson:
                        methodname = "Atkinson"; break;
                }
                std::printf(
                    "\tError-diffusion method (dithering): %s\n", methodname);
            }
        }
        else if(SaveGif == 1)
        {
            std::printf(
                "\tPalette reduction done by libGD if necessary (no dithering)\n");
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

    estimated_num_frames = argc-optind;

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
      #pragma omp parallel for schedule(static)
        for(unsigned y=0; y<sy; ++y)
            for(unsigned p=y*sx, x=0; x<sx; ++x)
                pixels[p+x] = gdImageGetTrueColorPixel(im, x,y);

        gdImageDestroy(im);

        MaskImage(pixels, sx,sy);

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
