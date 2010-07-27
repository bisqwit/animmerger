#include "tiletracker.hh"

#include <map>

extern "C" { 
unsigned VScroll=0;
unsigned HScroll=0;
static const unsigned MinScanLine = 0;
static const unsigned MaxScanLine = 272;
unsigned ScrollPointers[MaxScanLine] = { 0 };
static unsigned PrevScrollingDefault = 0;
void TILE_SaveScroll(unsigned H, unsigned V, int scanline)
{
/*
    printf("[%3d] %u,%u\n", scanline, H,V);
    fflush(stdout);
*/
    if(scanline < 0 || scanline >= (int)MaxScanLine) return;
    
    unsigned code = H*4000+V;
    ScrollPointers[scanline] = code + 1;
}
void GetScroll(unsigned& First, unsigned& Last)
{
    std::map<unsigned, unsigned> ScrollUsages;
    // Take the most common value that appeared during the frame.

    // snes9x: take the first value and duplicate it to the frame.
    for(unsigned a=0; a<MaxScanLine; ++a)
        if(ScrollPointers[a] != 0)
            { PrevScrollingDefault = ScrollPointers[a]; break; }
    
    unsigned prev = PrevScrollingDefault;
    unsigned max=0, result=0;
    for(unsigned a=MinScanLine; a<MaxScanLine; ++a)
    {
        unsigned value = ScrollPointers[a];
        if(value == 0) value = prev; else prev = --value;
        ScrollPointers[a] = value+1;
        
        unsigned usage = ++ScrollUsages[value];
        if(usage > max) { max = usage; result = value; }
    }
    PrevScrollingDefault = prev;
    
    // Find the region that has this scrolling value, stored in "result".
    int state=0;
    for(unsigned a=MinScanLine; a < MaxScanLine && state < 2; ++a)
    {
        unsigned value = ScrollPointers[a];
        if(value == 0) continue;
        --value;
        switch(state)
        {
            case 0: if(value == result) { First = a; state = 1; }  break;
            case 1: if(value != result) { Last  =a-1; state = 2; } break;
        }
    }
    
    VScroll = result % 4000;
    HScroll = result / 4000;

#if NESVIDEOS_TRACKING
    fprintf(stderr, "%u-%u: %u,%u\n", First,Last, HScroll,VScroll);
#endif

    if(First > 0 && First < 8)
    {
        //fprintf(stderr, "First < 8 !\n");
        
        VScroll = (VScroll + 240 - First) % 240;
        
        First = 0;
    }
    
    for(unsigned a=MinScanLine; a<MaxScanLine; ++a)
        ScrollPointers[a] = 0; // reset for next frame.
}
}

#if NESVIDEOS_TRACKING

#include <vector>
#include <gd.h>

#include "uncertainpixel.hh"

typedef unsigned int uint32 ;
typedef unsigned char uint8 ;
typedef unsigned short uint16;

#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG || PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
unsigned CurrentTimer = 0;
unsigned SequenceBegin = 0;
struct ScrollingPosition
{
    unsigned org_x, org_y;
    /*unsigned max_x, max_y;*/
};
static std::vector<ScrollingPosition> scrolls;

#endif

class TILE_Tracker
{
    /* In this tracker, alpha = visibility. 0=transparent */

    int org_x, org_y;
    
    int xmin,ymin;
    int xmax,ymax;
    bool first;
    
    unsigned count;
    
    int get_min_y() const { return ymin; }
    int get_max_y() const { return ymax; }
    int get_min_x() const { return xmin; }
    int get_max_x() const { return xmax; }
    
    typedef std::vector<UncertainPixel> vectype; 
    typedef std::map<int,vectype> xmaptype;
    typedef std::map<int,xmaptype> ymaptype;
    ymaptype screens;
    
    const std::vector<uint32> LoadScreen(int ox,int oy, unsigned sx,unsigned sy)
    {
        // Create the result vector filled with default pixel value
        std::vector<uint32> result(sy*sx, DefaultPixel);

        const int xbegin = ox;
        const int xend   = ox+sx-1;
        
        const int xscreen_begin = xbegin/256;
        const int xscreen_end   = xend  /256;
        
        const int ybegin = oy;
        const int yend   = oy+sy-1;
        
        const int yscreen_begin = ybegin/256;
        const int yscreen_end   = yend  /256;
        
/*
        fprintf(stderr, "Loading screens x(%d..%d)y(%d..%d)\n",
            xscreen_begin,xscreen_end,
            yscreen_begin,yscreen_end);
*/
        
        // Load each cube that falls into the requested region
        
        unsigned targetpos=0;
        unsigned this_cube_ystart = oy&255;
        for(int yscreen=yscreen_begin; yscreen<=yscreen_end; ++yscreen)
        {
            xmaptype& xmap = screens[yscreen];
            
            unsigned this_cube_yend = yscreen==yscreen_end ? ((oy+sy-1)&255) : 255;
            
            unsigned this_cube_xstart = ox&255;
            for(int xscreen=xscreen_begin; xscreen<=xscreen_end; ++xscreen)
            {
                unsigned this_cube_xend = xscreen==xscreen_end ? ((ox+sx-1)&255) : 255;
/*
                fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                    this_cube_xstart,this_cube_xend,
                    this_cube_ystart,this_cube_yend);
*/
                const vectype& cube = xmap[xscreen];
                /* If this screen is not yet initialized, we'll skip over
                 * it, since there's no real reason to initialize it at
                 * this point. */
                if(!cube.empty())
                {
                    /* Load this particular cube */
                    for(unsigned yp=this_cube_ystart, y=0; yp<=this_cube_yend; ++y, ++yp)
                    { 
                        unsigned srcp  = 256*yp + this_cube_xstart - this_cube_xstart;
                        unsigned destp =   sx*y + targetpos        - this_cube_xstart;
                        for(unsigned xp=this_cube_xstart; xp<=this_cube_xend; ++xp)
                            result[destp + xp] = cube[srcp + xp];
                    }
                }
                
                unsigned this_cube_xsize = (this_cube_xend-this_cube_xstart)+1;
                
                targetpos+= this_cube_xsize;
                
                this_cube_xstart=0;
            }
            
            unsigned this_cube_ysize = (this_cube_yend-this_cube_ystart)+1;
            
            targetpos += sx * (this_cube_ysize-1);
            
            this_cube_ystart=0;
        }
        
        return result;
    }

    void PutScreen(const uint16*const input, int ox,int oy, unsigned sx,unsigned sy)
    {
        /* Nearly the same as LoadScreen. */
        
        const int xbegin = ox;
        const int xend   = ox+sx-1;
        
        const int xscreen_begin = xbegin/256;
        const int xscreen_end   = xend  /256;
        
        const int ybegin = oy;
        const int yend   = oy+sy-1;
        
        const int yscreen_begin = ybegin/256;
        const int yscreen_end   = yend  /256;
        
/*
        fprintf(stderr, "Writing screens x(%d..%d)y(%d..%d)\n",
            xscreen_begin,xscreen_end,
            yscreen_begin,yscreen_end);
*/
        unsigned targetpos=0;
        unsigned this_cube_ystart = oy&255;
        for(int yscreen=yscreen_begin; yscreen<=yscreen_end; ++yscreen)
        {
            xmaptype& xmap = screens[yscreen];
            
            unsigned this_cube_yend = yscreen==yscreen_end ? ((oy+sy-1)&255) : 255;
            
            unsigned this_cube_xstart = ox&255;
            for(int xscreen=xscreen_begin; xscreen<=xscreen_end; ++xscreen)
            {
                unsigned this_cube_xend = xscreen==xscreen_end ? ((ox+sx-1)&255) : 255;
                
                vectype& cube = xmap[xscreen];
                /* If this screen is not yet initialized, we'll initialize it */
                if(cube.empty()) cube.resize(256*256);
                
/*
                fprintf(stderr, " Cube(%u,%u)-(%u,%u)\n",
                    this_cube_xstart,this_cube_xend,
                    this_cube_ystart,this_cube_yend);
*/
                /* Write this particular cube */
                for(unsigned yp=this_cube_ystart, y=0; yp<=this_cube_yend; ++y, ++yp)
                    for(unsigned xp=this_cube_xstart, x=0; xp<=this_cube_xend; ++x, ++xp)
                    {
                        uint16 pix16 = input[targetpos + x + y*sx];
                        unsigned r = (pix16 >> (11-3)) & 0xF8;
                        unsigned g = (pix16 >> (5-2))  & 0xFC;
                        unsigned b = (pix16 << -(0-3)) & 0xF8;
                        cube[xp + 256*yp].set(r,g,b);
                    }

                unsigned this_cube_xsize = (this_cube_xend-this_cube_xstart)+1;
                
                targetpos+= this_cube_xsize;
                
                this_cube_xstart=0;
            }
            
            unsigned this_cube_ysize = (this_cube_yend-this_cube_ystart)+1;
            
            targetpos += sx * (this_cube_ysize-1);
            
            this_cube_ystart=0;
        }
    }
public:
    TILE_Tracker() : count(0)
    {
        Reset();
    }

    ~TILE_Tracker()
    {
    }
    
#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG
    std::vector<uint32> LastScreen;
    std::string LastFilename;
    
    bool veq(const std::vector<uint32>& a, const std::vector<uint32>& b) const
    {
        if(a.size() != b.size()) return false;
        return memcmp(&a[0], &b[0], a.size()*sizeof(a[0])) == 0;
    }
#endif

    void Cleanup()
    {
        fprintf(stderr, "Compressing...\n");
        for(ymaptype::iterator y=screens.begin(); y!=screens.end(); ++y)
        {
            xmaptype& xmap = y->second;
            for(xmaptype::iterator x=xmap.begin(); x!=xmap.end(); ++x)
            {
                vectype& vec = x->second;
                for(unsigned a=0; a<vec.size(); ++a)
                    vec[a].Compress();
            }
        }
    }

    void SaveAndReset()
    {
        Cleanup();
        Save();
        Reset();
    }

    void Save()
    {
#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG || PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
        fprintf(stderr, "Saving(%d,%d)\n", first,CurrentTimer);
#else
        fprintf(stderr, "Saving(%d)\n", first);
#endif
        if(first) return;
        
#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG || PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
            static bool Saving = false;
            if(!Saving)
            {
                Saving = true;
                unsigned SavedTimer = CurrentTimer;
#  if PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
                if(SavedTimer >= LoopLength) SavedTimer = LoopLength;
#  endif
                for(CurrentTimer=0; CurrentTimer<SavedTimer; CurrentTimer += 1)
                {
                    count = CurrentTimer + SequenceBegin;
                    fprintf(stderr, "Saving frame %u/%u @ %u\n",
                        count-SequenceBegin,SavedTimer, SequenceBegin);
                    Save();
                }
                CurrentTimer  = SavedTimer;
                scrolls.clear();
                Saving = false;
            }
            else
            {
                printf("/*%u*/ %u,%u, %u,%u,\n",
                    count,
                    scrolls[CurrentTimer].org_x - get_min_x(),
                    scrolls[CurrentTimer].org_y - get_min_y(),/*
                    scrolls[CurrentTimer].max_x,
                    scrolls[CurrentTimer].max_y*/
                    0,0
                    );
                fflush(stdout);
            }
#endif
        
        int ymi = get_min_y(), yma = get_max_y();
        int xmi = get_min_x(), xma = get_max_x();

        unsigned wid = xma-xmi+1;
        unsigned hei = yma-ymi+1;
        
        if(wid <= 1 || hei <= 1) return;
        
        fprintf(stderr, " (%d,%d)-(%d,%d)\n", 0,0, xma-xmi, yma-ymi);
        
        char Filename[512];
        sprintf(Filename, "tile-%04u.png", count++);
        
        std::vector<uint32> screen = LoadScreen(xmi,ymi, wid,hei);
#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG
        if(veq(screen, LastScreen) && !LastFilename.empty())
        {
            fprintf(stderr, "->link (%u,%u)\n", screen.size(), LastScreen.size());
            std::string cmd = "ln "+LastFilename+" "+Filename;
            system(cmd.c_str());
            LastScreen   = screen;
            LastFilename = Filename;
            return;
        }
        LastScreen   = screen;
        LastFilename = Filename;
#endif

        gdImagePtr im = gdImageCreateTrueColor(wid,hei);
        
        for(unsigned p=0, y=0; y<hei; ++y)
            for(unsigned x=0; x<wid; ++x)
            {
                unsigned pix32 = screen[p++];
                unsigned pix = pix32/*gdTrueColor
                (
                    (pix32 >> 24) & 0xFF,
                    (pix32 >> 16) & 0xFF,
                    (pix32 >> 8) & 0xFF
                )*/;
                gdImageSetPixel(im, x,y, pix);
            }
        
        FILE* fp = fopen(Filename, "wb");
        gdImagePngEx(im, fp, 1);
        fclose(fp);
        gdImageDestroy(im);
    }

    void Reset()
    {
#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG || PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
        SequenceBegin += CurrentTimer;
        CurrentTimer = 0;
#endif
        
        fprintf(stderr, " Resetting\n");
        screens.clear();
        org_x = 0x40000000;
        org_y = 0x40000000;
        xmin=xmax=org_x;
        ymin=ymax=org_y;
        first = true;
    }

    void FitScreen(const uint16* buf,
                   unsigned max_x,
                   unsigned max_y,
                   int offs_x, int offs_y, bool suspect_reset,
                   int extra_offs_x=0,
                   int extra_offs_y=0
                  )
    {
#if !(PIXEL_METHOD==PIXEL_METHOD_CHANGELOG || PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG)
/*
        static unsigned framecounter=0;
        if(++framecounter == 600) { Save(); framecounter=0; }
*/
#endif
        
        if(offs_x != 0 || offs_y != 0)
        {
            fprintf(stderr, " Motion(%d,%d), Origo(%d,%d)\n", offs_x,offs_y, org_x,org_y);
        }

        org_x += offs_x; org_y += offs_y;
        
        int this_org_x = org_x + extra_offs_x;
        int this_org_y = org_y + extra_offs_y;
        
        if(suspect_reset)
        {
#if 0
            goto AlwaysReset;
#endif
            std::vector<uint32> oldbuf = LoadScreen(this_org_x,this_org_y, max_x,max_y);
            unsigned diff = 0;
            for(unsigned a=0; a<oldbuf.size(); ++a)
            {
                unsigned pix   = oldbuf[a];
                unsigned pix16 = buf[a];
                unsigned r = (pix16 >> (11-3)) & 0xF8;
                unsigned g = (pix16 >> (5-2))  & 0xFC;
                unsigned b = (pix16 << -(0-3)) & 0xF8;
                unsigned oldr = ((pix >> 16) & 0xFF);
                unsigned oldg = ((pix >>  8) & 0xFF);
                unsigned oldb = ((pix >>  0) & 0xFF);
                int rdiff = (int)(r-oldr); if(rdiff < 0)rdiff=-rdiff;
                int gdiff = (int)(g-oldg); if(gdiff < 0)gdiff=-gdiff;
                int bdiff = (int)(b-oldb); if(bdiff < 0)bdiff=-bdiff;
                unsigned absdiff = rdiff+gdiff+bdiff;
                diff += absdiff;
            }
            
            if(diff > oldbuf.size() * 128)
            {
#if 0
                /* Castlevania hack */
                static int org_diff = -180;
                org_y += org_diff;
                org_diff = -org_diff;
#else
#if 1
            AlwaysReset:
                SaveAndReset();
#endif
#endif
            }
        }
        
        if(first || this_org_x < xmin) xmin = this_org_x;
        if(first || this_org_y < ymin) ymin = this_org_y;
        int xtmp = this_org_x+max_x; if(first || xtmp > xmax) xmax=xtmp;
        int ytmp = this_org_y+max_y; if(first || ytmp > ymax) ymax=ytmp;
        first=false;
        
#if 0
        /* If the image geometry would exceed some bounds */
        if(xmax-xmin > 800 || ymax-ymin > 800)
        {
            SaveAndReset();
            first=true;
        }
#endif
        
        PutScreen(buf, this_org_x,this_org_y, max_x,max_y);
    }
    void NextFrame()
    {
#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG || PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
        ScrollingPosition s;
        s.org_x = org_x;
        s.org_y = org_y;
        /*
        s.max_x = max_x;
        s.max_y = max_y;
        */
        scrolls.push_back(s);

        ++CurrentTimer;
#endif
    }
} TILE_Tracker;

extern "C" { /******* PPU variables: **/

/* 0..7. Finetuning the scrolling. */
unsigned NTAWrites=0;

extern "C" { 
extern struct { unsigned char r,g,b,unused; } psdl[256]; // palette from sdl-video.c
}

void TILE_TrackFrame(
    const unsigned char*data, unsigned width, unsigned height,
    const int x_offs,
    const int y_offs
    )
{
    if(width > 256) return;
    if(height > 256) height=256;
    
    static uint16 EdgeEaten[256*256];
    static uint16 bpp16transbuf[256*256];
    uint16* buf = bpp16transbuf;
    for(unsigned p=0; p<width*height; ++p)
    {
        unsigned r = psdl[data[p]].r;
        unsigned g = psdl[data[p]].g;
        unsigned b = psdl[data[p]].b;
        buf[p] = (r*32/256)*32*64
               + (g*64/256)*32
               + (b*32/256);
    }
    
    const unsigned eat_left =0; //17
    const unsigned eat_right=0; //32

    if(eat_left || eat_right)
    {
        /* Eat edges */
        const unsigned eat_total=eat_left+eat_right;
        for(unsigned ipos=eat_left,opos=0,y=0; y<height;++y,ipos+=eat_total)
            for(unsigned x=0; x<width-eat_total;++x)
                EdgeEaten[opos++]=buf[ipos++];
        buf = EdgeEaten;
        width -= eat_total;
    }

#if 0
    /* Don't track anything, just create herd of PNGs */
    gdImagePtr im = gdImageCreateTrueColor(width,height);
    
    for(unsigned p=0, y=0; y<height; ++y)
        for(unsigned x=0; x<width; ++x)
        {
            unsigned pix16 = buf[p++];
            unsigned r = (pix16 >> (11-3)) & 0xF8;
            unsigned g = (pix16 >> (5-2))  & 0xFC;
            unsigned b = (pix16 << -(0-3)) & 0xF8;
            unsigned pix32 = gdTrueColor(r,g,b);
            gdImageSetPixel(im, x,y, pix32);
        }
    
    char Filename[512];
    static unsigned count=0;
    sprintf(Filename, "tile-%04u.png", count++);
    
    FILE* fp = fopen(Filename, "wb");
    gdImagePngEx(im, fp, 1);
    fclose(fp);
    gdImageDestroy(im);
    return;
#endif
    
    unsigned FirstLine = 0;
    unsigned LastLine  = height-1;
    
    GetScroll(FirstLine,LastLine);
    
    //LastLine -= 10*8;
    
    //if(LastLine >= height) LastLine=height-1;
#if 0
    FirstLine=0;
    LastLine =height-1;
#endif
    
#if 0 /* SMB3 */
    LastLine=194;
    FirstLine=0;
#endif
    
    static unsigned PrevV;
    static unsigned PrevH;
    static bool first=true;
    if(first) { PrevV=VScroll; PrevH=HScroll; first=false; }
    
    fprintf(stderr, "First&lastline %u,%u\n", FirstLine,LastLine);

    //VScroll += FirstLine;
    
    int vdiff = (VScroll+240-PrevV)%240; PrevV=VScroll;
    int hdiff = (HScroll+256-PrevH)%256; PrevH=HScroll;

    if(hdiff >= 128) hdiff -= 256;
    if(vdiff >= 120) vdiff -= 240;
    
    //long absdiff2 = (hdiff*hdiff) + (vdiff*vdiff);
    
    bool suspect_reset = false;
    if(/*absdiff2 > 20*20 || */NTAWrites > 100)
    {
       suspect_reset = true;
    }
    //suspect_reset=true;
    
/*
    if(NTAWrites != 0)
        fprintf(stderr, "%ld NTA writes\n", NTAWrites);
*/
    TILE_Tracker.FitScreen
    (
        buf + FirstLine*width,
        width, LastLine-FirstLine+1,
        hdiff, vdiff, suspect_reset,
        x_offs,
        y_offs
    );
    NTAWrites = 0;
}

void TILE_ForceSave()
{
    TILE_Tracker.SaveAndReset();
}

void TILE_NextFrame()
{
    TILE_Tracker.NextFrame();
}


}

#else
extern "C" { 
void TILE_ForceSave() { }
void TILE_TrackFrame(const unsigned char*data, unsigned width, unsigned height,int,int) { }
void TILE_NextFrame() { }
}
#endif

