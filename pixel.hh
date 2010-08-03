#ifndef bqtTileTrackerPixelHH
#define bqtTileTrackerPixelHH

typedef unsigned int uint32;
typedef unsigned char uint8 ;
typedef unsigned short uint16;
typedef unsigned long long uint64;

static const uint32 DefaultPixel = 0xFF404041u;

extern unsigned LoopingLogLength;

extern unsigned CurrentTimer;
/* Timer value for set() and get methods in ChangeLog and LoopingLog methods */

#include "pixels/mostusedpixel.hh"

#include "pixels/averagepixel.hh"
#include "pixels/lastpixel.hh"
#include "pixels/mostusedwithinpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"

#include "alloc/FSBAllocator.hh"

#include <cstdio>

extern enum PixelMethod
{
    pm_AveragePixel,
    pm_LastPixel,
    pm_MostUsedPixel,
    pm_MostUsed16Pixel,
    pm_ChangeLogPixel,
    pm_LoopingLogPixel
} method;

struct UncertainPixel
{
    static bool is_changelog()
    {
        return method == pm_ChangeLogPixel;
    }
    static bool is_loopinglog()
    {
        return method == pm_LoopingLogPixel;
    }
    static bool is_animated()
    {
        return is_changelog() || is_loopinglog();
    }
    static unsigned GetLoopLength()
    {
        if(!is_loopinglog()) return 0;
        return LoopingLogLength;
    }
};

struct Array256x256of_Base
{
public:
    virtual uint32 GetPixel(unsigned index) const = 0;
    virtual uint32 GetMostUsed(unsigned index) const = 0;
    virtual void Set(unsigned index, uint32 p) = 0;
    virtual void Compress() = 0;
};

template<typename T>
struct Array256x256of: public Array256x256of_Base
{
public:
    T data[256*256];
public:
    Array256x256of()
    {
    }
    virtual ~Array256x256of()
    {
    }
public:    
    void Construct()
    {
        for(unsigned a=0; a<256*256; ++a)
            new ((void*)&data[a]) T();
    }
    void Construct(const Array256x256of<T>& b)
    {
        for(unsigned a=0; a<256*256; ++a)
            new ((void*)&data[a]) T( b.data[a] );
    }
    void Destruct()
    {
        for(unsigned a=0; a<256*256; ++a)
            data[a].~T();
    }
public:
    virtual uint32 GetPixel(unsigned index) const
    {
        return data[index].get_pixel();
    }
    virtual uint32 GetMostUsed(unsigned index) const
    {
        return data[index].get_pixel();
    }
    virtual void Set(unsigned index, uint32 p)
    {
        data[index].set(p);
    }
    virtual void Compress()
    {
        for(unsigned a=0; a<256*256; ++a)
            data[a].Compress();
    }
};

class UncertainPixelVector256x256
{
    #define DoCode(code,type) code(type, Array256x256of<type >)
    #define DoCases(code) \
        switch(method) \
        { \
            case pm_AveragePixel: \
                { DoCode(code,AveragePixelAndMostUsedPixel ); break; } \
            case pm_LastPixel: \
                { DoCode(code,LastPixelAndMostUsedPixel );    break; } \
            case pm_MostUsedPixel: \
                { DoCode(code,MostUsedPixelAndMostUsedPixel ); break; } \
            case pm_MostUsed16Pixel: \
                { DoCode(code,MostUsedWithinAndMostUsedPixel<16> ); break; } \
            case pm_ChangeLogPixel: \
                { DoCode(code,ChangeLogPixelAndMostUsedPixel ); break; } \
            case pm_LoopingLogPixel: \
                { DoCode(code,LoopingLogPixelAndMostUsedPixel ); break; } \
        }
public:
    UncertainPixelVector256x256() : data(0)
    {
    }
    void init()
    {
        if(data) return;
        #define init(type, type2) data = new type2
        DoCases(init);
        #undef init
    }
    UncertainPixelVector256x256(const UncertainPixelVector256x256& b) : data(0)
    {
        if(!b.data) return;
        #define copy(type, type2) data = new type2( *(const type2*) b.data )
        DoCases(copy);
        #undef copy
    }
    UncertainPixelVector256x256& operator= (const UncertainPixelVector256x256& b)
    {
        if(!b.data) { if(data) { delete data; data=0; } return *this; }
        #define assign(type, type2) \
            if(data) *data = *(const type2*) b.data; \
            else     data = new type2( *(const type2*) b.data )
        DoCases(assign);
        #undef assign
        return *this;
    }
    ~UncertainPixelVector256x256()
    {
        if(!data) return;
        #define done(type, type2) delete data
        DoCases(done);
        #undef done
    }
    inline uint32 GetPixel(unsigned index) const
    {
        return data ? data->GetPixel(index) : DefaultPixel;
    }
    inline uint32 GetMostUsed(unsigned index) const
    {
        return data ? data->GetMostUsed(index) : DefaultPixel;
    }
    inline void set(unsigned index, uint32 p)
    {
        if(data) data->Set(index, p);
    }
    inline void Compress()
    {
        if(data) data->Compress();
    }

    bool empty() const { return !data; }
    unsigned size() const { return data ? 256*256 : 0; }

private:
    #undef DoCases
    #undef DoCode
    Array256x256of_Base* data;
};

#endif
