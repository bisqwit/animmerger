#include <gd.h>
#include "canvas.hh"

struct AlphaRange
{
    unsigned x1,y1, width,height;
    std::vector<unsigned> colors;
};

int main(int argc, char** argv)
{
    TILE_Tracker tracker;
    
    std::vector<AlphaRange> alpha_ranges;
    
    for(int a=1; a<argc; ++a)
    {
        const char* fn = argv[a];
        FILE* fp = fopen(fn, "rb");
        if(!fp)
        {
            perror(fn);
            continue;
        }
        
        gdImagePtr im = gdImageCreateFromPng(fp);
        if(!im) { rewind(fp); im = gdImageCreateFromGif(fp);
        if(!im) { rewind(fp); im = gdImageCreateFromJpeg(fp); } }
        fclose(fp);
        if(!im)
        {
            fprintf(stderr, "%s has unrecognized image type, ignoring file\n", fn);
            continue;
        }

        unsigned sx = gdImageSX(im), sy = gdImageSY(im);
        std::vector<uint32> pixels(sx*sy);
        for(unsigned p=0, y=0; y<sy; ++y)
            for(unsigned x=0; x<sx; ++x,++p)
                pixels[p] = gdImageGetTrueColorPixel(im, x,y);

        for(size_t r=0; r<alpha_ranges.size(); ++r)
        {
            const AlphaRange& a = alpha_ranges[r];
            AlphaRange tmp;
            tmp.x1 = a.x1 < sx ? a.x1 : sx;
            tmp.y1 = a.y1 < sy ? a.y1 : sy;
            tmp.width  = (tmp.x1 + a.width ) < sx ? a.width  : sx-tmp.x1;
            tmp.height = (tmp.y1 + a.height) < sy ? a.height : sy-tmp.y1;
            
            unsigned p = tmp.y1 * sx + tmp.x1;
            for(unsigned y=0; y<tmp.height; ++y, p += (sx-tmp.width))
                for(unsigned x=0; x<tmp.width; ++x, ++p)
                {
                    if(a.colors.empty())
                        blotout: pixels[p] |= 0xFF000000u;
                    else for(size_t b=0; b<a.colors.size(); ++b)
                        if(pixels[p] == a.colors[b])
                            goto blotout;
                }
        }

        tracker.FitScreenAutomatic(&pixels[0], sx,sy);
        tracker.NextFrame();
    }
    tracker.SaveAndReset();
}
