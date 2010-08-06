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

//extern unsigned CurrentTimer;       // For animated
//extern unsigned SequenceBegin;      // For animated
extern bool SaveGif;

class TILE_Tracker
{
    int org_x, org_y;

    int xmin,ymin;
    int xmax,ymax;
    bool first;

    typedef UncertainPixelVector256x256 vectype;

    struct cubetype
    {
        bool changed;
        vectype pixels;
    };

    typedef std::map<int,cubetype, std::less<int>, FSBAllocator<int> > xmaptype;
    typedef std::map<int,xmaptype, std::less<int>, FSBAllocator<int> > ymaptype;
    ymaptype screens;

    VecType<uint32> LastScreen;  // For ChangeLog
    std::string LastFilename;    // For ChangeLog
    unsigned SequenceBegin;
    unsigned CurrentTimer;

public:
    TILE_Tracker() : LastFilename(), SequenceBegin(0), CurrentTimer(0)
    {
        Reset();
    }

    ~TILE_Tracker()
    {
    }

    void SaveAndReset()
    {
        Save();
        Reset();
    }

    void Save();
    void SaveFrame(unsigned timer, unsigned imgcounter);

    void Reset();

    const VecType<uint32> LoadScreen(int ox,int oy, unsigned sx,unsigned sy,
                                     unsigned timer) const;

    void PutScreen(const uint32*const input, int ox,int oy, unsigned sx,unsigned sy,
                   unsigned timer);

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
