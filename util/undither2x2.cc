#include <gd.h>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <cmath>

#define G(x) std::pow( (x)/255.0, 2.2)
#define Un(x) 255*std::pow( (x), 1/2.2)

static unsigned avg4pixels(unsigned a,unsigned b,unsigned c,unsigned d)
{
    int r1 = (a>>16)&0xFF, g1 = (a>>8)&0xFF, b1 = (a>>0)&0xFF;
    int r2 = (b>>16)&0xFF, g2 = (b>>8)&0xFF, b2 = (b>>0)&0xFF;
    int r3 = (c>>16)&0xFF, g3 = (c>>8)&0xFF, b3 = (c>>0)&0xFF;
    int r4 = (d>>16)&0xFF, g4 = (d>>8)&0xFF, b4 = (d>>0)&0xFF;
    int rr = Un((G(r1)+G(r2)+G(r3)+G(r4))/4);
    int gg = Un((G(g1)+G(g2)+G(g3)+G(g4))/4);
    int bb = Un((G(b1)+G(b2)+G(b3)+G(b4))/4);
    unsigned res = (rr<<16) + (gg<<8) + (bb);
    //printf("Averages %06X,%06X,%06X,%06X, becomes %06X\n", a,b,c,d,res); 
    return res;
}

static bool IsPair(unsigned pix1,unsigned pix2)
{
    int r1 = (pix1 >> 16) & 0xFF, g1 = (pix1 >> 8) & 0xFF, b1 = (pix1) & 0xFF;
    int r2 = (pix2 >> 16) & 0xFF, g2 = (pix2 >> 8) & 0xFF, b2 = (pix2) & 0xFF;
    int rdiff = std::abs(r1-r2);
    int gdiff = std::abs(g1-g2);
    int bdiff = std::abs(b1-b2);
    return (rdiff < 20 && gdiff < 12 && bdiff < 50)
       || (pix1 == 0xFC0000 && pix2 == 0x0034DC)
       || (pix2 == 0xFC0000 && pix1 == 0x0034DC);
}

static const unsigned NotOptimized = 0x7B123456;
static unsigned CheckOptimize(
    unsigned a,unsigned b,
    unsigned c,unsigned d,
    int recursion = 0)
{
    if((IsPair(a,d) && (recursion <= 1 || IsPair(b,c)))
    || (IsPair(b,c) && (recursion <= 1 || IsPair(a,d)))
      )
    {
        if(a != b || a != c || a != d
        || b != c || b != d
        || c != d)
            return avg4pixels(a,b,c,d);
    }
    return NotOptimized;
}

int main(int argc, char** argv)
{
    FILE* fp = fopen(argv[1], "rb");
    gdImagePtr srcim = gdImageCreateFromPng(fp);
    fclose(fp);

    unsigned w = gdImageSX(srcim), h = gdImageSY(srcim);
    gdImagePtr im = gdImageCreateTrueColor(w, h);

    #define GetP(x,y) gdImageGetTrueColorPixel(srcim, x,y)
    #define PSet(x,y,c) gdImageSetPixel(im, x,y,c)

    std::vector<bool> optimized(w*h, false);
    for(unsigned x=0; x<w; ++x)
        for(unsigned y=0; y<h; ++y)
        {
            // Try each of these four patterns
            // * = current pixel
            // . = other pixel which is identical to this one
            // x = two other pixels
            //  Zx    zX   zx   zx
            //  xy    xy   xY   Xy
            if(optimized[y*w+x]) continue;

            for(int xo=-1; xo<=+1; xo+=2)
                for(int yo=-1; yo<=+1; yo+=2)
                    if(int(x)+xo >= 0 && int(x)+xo < int(w)
                    && int(y)+yo >= 0 && int(y)+yo < int(h))
                    {
                        unsigned color = CheckOptimize(
                            GetP(x,   y   ),
                            GetP(x+xo,y   ),
                            GetP(x,   y+yo),
                            GetP(x+xo,y+yo));
                        if(color != NotOptimized)
                        {
                            PSet(x,   y,   color); optimized[(y   )*w+(x   )] = true;
                            PSet(x+xo,y,   color); optimized[(y   )*w+(x+xo)] = true;
                            PSet(x,   y+yo,color); optimized[(y+yo)*w+(x   )] = true;
                            PSet(x+xo,y+yo,color); optimized[(y+yo)*w+(x+xo)] = true;
                            goto done2x2;
                        }
                    }
            PSet(x,y, GetP(x,y));
           done2x2:;
        }

    // Then do the same using 2x2 pixels
    bool changed=true;
    for(int recursion=1; recursion<=1 && changed; ++recursion)
    {
        changed=false;

        gdImageDestroy(srcim);
        srcim = im;
        im = gdImageCreateTrueColor(w,h);

        std::vector<bool> optimized2(w*h, false);
        for(unsigned x=0; x<w; ++x)
            for(unsigned y=0; y<h; ++y)
            {
                if(optimized2[y*w+x]) continue;
                if(x+1 < w && y+1 < h
                && IsPair(GetP(x,y), GetP(x,y+1))
                && IsPair(GetP(x,y), GetP(x+1,y))
                && IsPair(GetP(x,y), GetP(x+1,y+1)))
                {
                    for(int xo=+2; xo>=-2; xo-=4)
                        for(int yo=+2; yo>=-2; yo-=4)
                        if(int(x)+xo >= 0 && int(x)+xo+1 < int(w)
                        && int(y)+yo >= 0 && int(y)+yo+1 < int(h))
                        {
                            if(IsPair(GetP(x+xo,y), GetP(x+xo,  y+1))
                            && IsPair(GetP(x+xo,y), GetP(x+xo+1,y))
                            && IsPair(GetP(x+xo,y), GetP(x+xo+1,y+1))
                            //
                            && IsPair(GetP(x+xo,y+yo), GetP(x+xo,  y+yo+1))
                            && IsPair(GetP(x+xo,y+yo), GetP(x+xo+1,y+yo))
                            && IsPair(GetP(x+xo,y+yo), GetP(x+xo+1,y+yo+1))
                            //
                            && IsPair(GetP(x,y+yo), GetP(x,  y+yo+1))
                            && IsPair(GetP(x,y+yo), GetP(x+1,y+yo))
                            && IsPair(GetP(x,y+yo), GetP(x+1,y+yo+1)))
                            {
                                int x1 = xo<0 ? x+xo : x, x2 = xo<0 ? x+1 : (x+xo+1);
                                int y1 = yo<0 ? y+yo : y, y2 = yo<0 ? y+1 : (y+yo+1);
                                /*
                                printf("x=%d,y=%d,xo=%d,yo=%d,x1=%d,y1=%d,x2=%d,y2=%d, w=%d,h=%d\n",
                                    x,y, xo,yo, x1,y1, x2,y2, w,h);
                                */
                                assert(x1 >= 0);
                                assert(y1 >= 0);
                                assert(x1 < w);
                                assert(y1 < h);
                                unsigned c1 = avg4pixels(GetP(x,y),GetP(x+1,y),GetP(x,y+1),GetP(x+1,y+1));
                                unsigned c2 = avg4pixels(GetP(x+xo,y),GetP(x+xo+1,y),GetP(x+xo,y+1),GetP(x+xo+1,y+1));
                                unsigned c3 = avg4pixels(GetP(x,y+yo),GetP(x+1,y+yo),GetP(x,y+yo+1),GetP(x+1,y+yo+1));
                                unsigned c4 = avg4pixels(GetP(x+xo,y+yo),GetP(x+xo+1,y+yo),GetP(x+xo,y+yo+1),GetP(x+xo+1,y+yo+1));

                                unsigned color = CheckOptimize(c1,c2,c3,c4, recursion);
                                if(color != NotOptimized)
                                {
                                    for(int yp=y1; yp<=y2; ++yp)
                                        for(int xp=x1; xp<=x2; ++xp)
                                        {
                                            PSet(xp,yp, color);
                                            optimized2[yp*w+xp] = true;
                                        }
                                    changed = true;
                                    goto done4x4;
                                }
                            }
                        }
                }
                PSet(x,y, GetP(x,y));
               done4x4:;
            }
    }

    fp = fopen(argv[2], "wb");
    gdImagePng(im, fp);
    fclose(fp); gdImageDestroy(im); gdImageDestroy(srcim);
}
