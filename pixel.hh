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

class UncertainPixel
{
public:
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

template<typename T>
struct Array256x256of
{
public:
    T data[256*256];
private:
    Array256x256of();
    Array256x256of(const Array256x256of&);
    void operator=(const Array256x256of&);
    ~Array256x256of();
protected:
    friend class UncertainPixelVector256x256;
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
    inline uint32 GetPixel(unsigned index) const
        { return data[index].get_pixel(); }
    inline uint32 GetMostUsed(unsigned index) const
        { return data[index].get_mostused(); }
    inline void set(unsigned index, uint32 p)
        { data[index].set(p); }
};

class UncertainPixelVector256x256
{
    /* Precompiler polymorphism. */
    #define DoCode(code,t) typedef Array256x256of<t> type2; code(t,type2)
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
        #define init(type,type2) \
            std::allocator<type2> a; \
            /**/std::fprintf(stderr, "Allocating %lu bytes for %s\n", (unsigned long) sizeof(type2), #type2);/**/ \
            type2* aptr = a.allocate(1); \
            aptr->Construct(); \
            data = (void*) aptr
        DoCases(init);
        #undef init
    }
    UncertainPixelVector256x256(const UncertainPixelVector256x256& b) : data(0)
    {
        if(!b.data) return;
        #define copy(type,type2) \
            std::allocator<type2> a; \
            type2* aptr = a.allocate(1); \
            aptr->Construct( *(const type2*) b.data ); \
            data = (void*) aptr
        DoCases(copy);
        #undef copy
    }
    UncertainPixelVector256x256& operator= (const UncertainPixelVector256x256& b);/*
    {
        #define assign(type,type2) \
            std::allocator<type2> a; \
            if(!data) \
            { \
                if(b.data) \
                { \
                    type2* aptr = a.allocate(1); \
                    aptr->Construct( *(const type2*) b.data ); \
                    data = (void*) aptr; \
                } \
            } \
            else \
            { \
                type2* aptr = (type2*)data; \
                if(!b.data) \
                { \
                    aptr->Destruct(); \
                    a.deallocate(aptr, 1); \
                    aptr = 0; \
                } \
                else \
                { \
                    const type2* bptr = (const type2*) b.data; \
                    for(unsigned a=0; a<256*256; ++a) aptr->data[a] = bptr->data[a]; \
                } \
            }
        DoCases(assign);
        #undef assign
        return *this;
    }*/
    ~UncertainPixelVector256x256()
    {
        if(!data) return;
        #define done(type,type2) \
            type2* aptr = (type2*) data; \
            std::allocator<type2> a; \
            aptr->Destruct(); \
            a.deallocate(aptr, 1)
        DoCases(done);
        #undef done
    }

    /* Visit allows one to use freely the pixel methods
     * for an entire group of 256x256 pixels without
     * having to do the method selection on each pixel.
     * Because virtual functions cannot be template
     * functions, this must be done with templates and
     * preprocessor rather than with the virtual keyword.
     */
    template<typename F>
    void Visit(F func)
    {
        if(!data) return;
        #define act(type,type2) func(* (type2*) data)
        DoCases(act);
        #undef act
    }

    template<typename F>
    void Visit(F func) const
    {
        if(!data) return;
        #define act(type,type2) func(* (const type2*) data)
        DoCases(act);
        #undef act
    }

    void Compress()
    {
        if(!data) return;
        #define act(type,type2) \
            type2* aptr = (type2*)data; \
            for(unsigned a=0; a<256*256; ++a) aptr->data[a].Compress()
        DoCases(act);
        #undef act
    }

    bool empty() const { return !data; }

private:
    #undef DoCases
    #undef DoCode
    void* data;
};

#endif
