#ifndef bqtTileTrackerCanvasHH
#define bqtTileTrackerCanvasHH

#include "pixel.hh"
#include "alloc/FSBAllocator.hh"

#include <vector>
#include <map>
#include <cstring> // std::memcmp
#include <string>
#include <cstdio>

extern unsigned CurrentTimer;       // For animated
extern unsigned SequenceBegin;      // For animated
struct ScrollingPosition            // For animated
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

    typedef std::map<int,cubetype, std::less<int>, FSBAllocator<int> > xmaptype;
    typedef std::map<int,xmaptype, std::less<int>, FSBAllocator<int> > ymaptype;
    ymaptype screens;

    const std::vector<uint32> LoadScreen(int ox,int oy, unsigned sx,unsigned sy);
    void PutScreen(const uint32*const input, int ox,int oy, unsigned sx,unsigned sy);

public:
    TILE_Tracker() : count(0), LastFilename()
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
        std::fprintf(stderr, "Compressing...\n");
        for(ymaptype::iterator y=screens.begin(); y!=screens.end(); ++y)
        {
            xmaptype& xmap = y->second;
            for(xmaptype::iterator x=xmap.begin(); x!=xmap.end(); ++x)
            {
                vectype& vec = x->second.pixels;
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

    void Save();

    void Reset()
    {
        if(UncertainPixel::is_animated())
        {
            SequenceBegin += CurrentTimer;
            CurrentTimer = 0;
        }

        std::fprintf(stderr, " Resetting\n");
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
                  );

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
};

#endif
