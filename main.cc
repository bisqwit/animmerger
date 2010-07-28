#include <gd.h>
#include "canvas.hh"

int main(int argc, char** argv)
{
    TILE_Tracker tracker;
    
    //tracker.NextFrame();
    
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
        
        tracker.FitScreenAutomatic(&pixels[0], sx,sy);
        tracker.NextFrame();
    }
    tracker.SaveAndReset();
}
