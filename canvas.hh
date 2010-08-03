#ifndef bqtTileTrackerCanvasHH
#define bqtTileTrackerCanvasHH

#include "pixel.hh"
#include "vectype.hh"
#include "alloc/FSBAllocator.hh"

#include <vector>
#include <map>
#include <cstring> // std::memcmp
#include <string>
#include <cstdio>

extern unsigned CurrentTimer;       // For animated
extern unsigned SequenceBegin;      // For animated

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

    typedef UncertainPixelVector256x256 vectype;

    struct cubetype
    {
        bool changed;
        vectype pixels;
    };

    typedef std::map<int,cubetype, std::less<int>, FSBAllocator<int> > xmaptype;
    typedef std::map<int,xmaptype, std::less<int>, FSBAllocator<int> > ymaptype;
    ymaptype screens;

    const VecType<uint32> LoadScreen(int ox,int oy, unsigned sx,unsigned sy) const;
    void PutScreen(const uint32*const input, int ox,int oy, unsigned sx,unsigned sy);

public:
    TILE_Tracker() : count(0), LastFilename()
    {
        Reset();
    }

    ~TILE_Tracker()
    {
    }

    VecType<uint32> LastScreen;  // For ChangeLog
    std::string LastFilename;    // For ChangeLog

    void Cleanup();

    void SaveAndReset()
    {
        Cleanup();
        Save();
        Reset();
    }

    void Save();

    void Reset();

    void FitScreenAutomatic(const uint32*const input, unsigned sx,unsigned sy);

    void FitScreen(const uint32* buf,
                   unsigned max_x,
                   unsigned max_y,
                   int offs_x, int offs_y, bool suspect_reset,
                   int extra_offs_x=0,
                   int extra_offs_y=0
                  );

    void NextFrame();
};

#endif
