#ifndef bqtTileTrackerPixelHH
#define bqtTileTrackerPixelHH

typedef unsigned int uint32;
typedef unsigned char uint8 ;
typedef unsigned short uint16;

static const uint32 DefaultPixel = 0x404041;

extern unsigned CurrentTimer;
/* Timer value for set() and get methods in ChangeLog and LoopingLog methods */

#include "pixels/averagepixel.hh"
#include "pixels/lastpixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/mostusedwithinpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"

#include "alloc/FSBAllocator.hh"

enum PixelMethod
{
    AveragePixel,
    LastPixel,
    MostUsedPixel,
    MostUsed16Pixel,
    ChangeLogPixel,
    LoopingLogPixel
};

class UncertainPixel
{
    #define DoCases(code) \
        switch(PixelMethod) \
        { \
            case AveragePixel: \
                code(AveragePixel); break; \
            case LastPixel: \
                code(LastPixel);    break; \
            case MostUsedPixel: \
                code(MostUsedPixel); break; \
            case MostUsed16Pixel: \
                code(MostUsedWithinPixel<16> ); break; \
            case ChangeLogPixel: \
                code(ChangeLogPixel); break; \
            case LoopingLogPixel: \
                code(LoopingLogPixel<90> ); break; \
        }
public:
    UncertainPixel()
    {
        #define init(type) data = (void*) new type
        DoCases(init);
        #undef init
    }
    UncertainPixel(const UncertainPixel& b)
    {
        #define copy(type) data = (void*) new type(*(type*)b.data)
        DoCases(copy);
        #undef copy
    }
    UncertainPixel& operator= (const UncertainPixel& b)
    {
        #define assign(type) *(type*)data = *(type*)b.data
        DoCases(assign);
        #undef assign
    }
    
    ~UncertainPixel()
    {
        #define done(type) delete (type*)data
        DoCases(done);
        #undef done
    }
    operator uint32() const
    {
        #define get(type) return *((type*)data)
        DoCases(get);
        #undef get
        return DefaultPixel;
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        #define put(type) ((type*)data)->set(R,G,B)
        DoCases(put);
        #undef put
    }
    void Compress()
    {
        #define act(type) ((type*)data)->Compress()
        DoCases(act);
        #undef act
    }
    
    static bool is_changelog()
    {
        return PixelMethod == ChangeLogPixel;
    }
    static bool is_loopinglog()
    {
        return PixelMethod == ChangeLogPixel;
    }
    static bool is_animated()
    {
        return is_changelog() || is_loopinglog();
    }
    static unsigned GetLoopLength()
    {
        if(!is_loopinglog()) return 0;
        return 90;
    }
    
private:
    #undef DoCases
    void* data;
};

#endif
