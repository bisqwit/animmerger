#ifndef bqtTileTrackerCanvasHH
#define bqtTileTrackerCanvasHH

#include "pixel.hh"

#include <vector>
#include <map>
#include <cstring> // std::memcmp


extern unsigned CurrentTimer = 0;       // For animated
extern unsigned SequenceBegin = 0;      // For animated
struct ScrollingPosition         // For animated
{
    unsigned org_x, org_y;
};
extern std::vector<ScrollingPosition> scrolls; // For animated

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
    
    struct cubetype
    {
        bool changed;
        vectype                    pixels;
        std::vector<MostUsedPixel> mostused;
    };

    typedef std::map<int,vectype> xmaptype;
    typedef std::map<int,xmaptype> ymaptype;
    ymaptype screens;
    
    const std::vector<uint32> LoadScreen(int ox,int oy, unsigned sx,unsigned sy);
    void PutScreen(const uint32*const input, int ox,int oy, unsigned sx,unsigned sy);
 
public:
    TILE_Tracker() : count(0)
    {
        Reset();
    }

    ~TILE_Tracker()
    {
    }
    
    std::vector<uint32> LastScreen;  // For ChangeLog
    std::string LastFilename;        // For ChangeLog
    
    bool veq(const std::vector<uint32>& a,
             const std::vector<uint32>& b) const // For ChangeLog
    {
        if(a.size() != b.size()) return false;
        return std::memcmp(&a[0], &b[0], a.size()*sizeof(a[0])) == 0;
    }

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
        if(UncertainPixel::is_animated())
            fprintf(stderr, "Saving(%d,%d)\n", first,CurrentTimer);
        else
            fprintf(stderr, "Saving(%d)\n", first);
        if(first) return;
        
        if(UncertainPixel::is_animated())
        {
            static bool Saving = false;
            if(!Saving)
            {
                Saving = true;
                unsigned SavedTimer = CurrentTimer;
                
                if(UncertainPixel::is_loopinglog())
                {
                    const unsigned LoopLength = UncertainPixel::GetLoopLength();
                    if(SavedTimer >= LoopLength) SavedTimer = LoopLength;
                }

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
                    scrolls[CurrentTimer].org_y - get_min_y(),
                    0,0
                    );
                fflush(stdout);
            }
        }
        
        int ymi = get_min_y(), yma = get_max_y();
        int xmi = get_min_x(), xma = get_max_x();

        unsigned wid = xma-xmi+1;
        unsigned hei = yma-ymi+1;
        
        if(wid <= 1 || hei <= 1) return;
        
        fprintf(stderr, " (%d,%d)-(%d,%d)\n", 0,0, xma-xmi, yma-ymi);
        
        char Filename[512];
        sprintf(Filename, "tile-%04u.png", count++);
        
        std::vector<uint32> screen = LoadScreen(xmi,ymi, wid,hei);
        
        if(UncertainPixel::is_changelog())
        {
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
        }

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
        if(UncertainPixel::is_animated())
        {
            SequenceBegin += CurrentTimer;
            CurrentTimer = 0;
        }
        
        fprintf(stderr, " Resetting\n");
        screens.clear();
        org_x = 0x40000000;
        org_y = 0x40000000;
        xmin=xmax=org_x;
        ymin=ymax=org_y;
        first = true;
    }

    void FitScreenAutomatic(const uint32*const input, unsigned sx,unsigned sy);

    void FitScreen(const uint32* buf,
                   unsigned max_x,
                   unsigned max_y,
                   int offs_x, int offs_y, bool suspect_reset,
                   int extra_offs_x=0,
                   int extra_offs_y=0
                  )
    {
        if(! UncertainPixel::is_animated())
        {
/*
        static unsigned framecounter=0;
        if(++framecounter == 600) { Save(); framecounter=0; }
*/
        }
        
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
                unsigned oldpix = oldbuf[a];
                unsigned pix   = buf[a];
                unsigned r = (pix >> 16);
                unsigned g = (pix >> 8) & 0xFF;
                unsigned b = (pix) & 0xFF;
                unsigned oldr = (oldpix >> 16);
                unsigned oldg = (oldpix >> 8) & 0xFF;
                unsigned oldb = (oldpix) & 0xFF;
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
        if(UncertainPixel::is_animated())
        {
            ScrollingPosition s;
            s.org_x = org_x;
            s.org_y = org_y;
            scrolls.push_back(s);

            ++CurrentTimer;
        }
    }
} TILE_Tracker;

#endif
