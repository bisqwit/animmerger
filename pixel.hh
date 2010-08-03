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

#include "pixels/averagepixel.hh"
#include "pixels/lastpixel.hh"
#include "pixels/mostusedpixel.hh"
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
    #define DoCases(code) \
        switch(method) \
        { \
            case pm_AveragePixel: \
                { code(AveragePixel); break; } \
            case pm_LastPixel: \
                { code(LastPixel);    break; } \
            case pm_MostUsedPixel: \
                { code(MostUsedPixel); break; } \
            case pm_MostUsed16Pixel: \
                { code(MostUsedWithinPixel<16> ); break; } \
            case pm_ChangeLogPixel: \
                { code(ChangeLogPixel); break; } \
            case pm_LoopingLogPixel: \
                { code(LoopingLogPixel ); break; } \
        }
public:
    UncertainPixel()
    {
        #define init(type) \
            FSBAllocator<type> a; data = (void*)a.allocate(1); new(data) type()
        DoCases(init);
        #undef init
    }
    UncertainPixel(const UncertainPixel& b)
    {
        #define copy(type) \
            FSBAllocator<type> a; data = (void*)a.allocate(1); new(data) type(*(const type*)b.data)
        DoCases(copy);
        #undef copy
    }
    UncertainPixel& operator= (const UncertainPixel& b)
    {
        #define assign(type) *(type*)data = *(type*)b.data
        DoCases(assign);
        #undef assign
        return *this;
    }

    ~UncertainPixel()
    {
        #define done(type) \
            FSBAllocator<type> a; a.destroy( (type*) data); a.deallocate( (type*)data, 1 )
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
    void set(uint32 p)
    {
        #define put(type) ((type*)data)->set(p)
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
    
    void* GetPtr() const { return data; }

private:
    #undef DoCases
    void* data;
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
};

class UncertainPixelVector256x256
{
    #define DoCases(code) \
        switch(method) \
        { \
            case pm_AveragePixel: \
                { code(AveragePixel, Array256x256of<AveragePixel> ); break; } \
            case pm_LastPixel: \
                { code(LastPixel,    Array256x256of<LastPixel> );    break; } \
            case pm_MostUsedPixel: \
                { code(MostUsedPixel, Array256x256of<MostUsedPixel> ); break; } \
            case pm_MostUsed16Pixel: \
                { code(MostUsedWithinPixel<16>, Array256x256of<MostUsedWithinPixel<16> > ); break; } \
            case pm_ChangeLogPixel: \
                { code(ChangeLogPixel, Array256x256of<ChangeLogPixel> ); break; } \
            case pm_LoopingLogPixel: \
                { code(LoopingLogPixel, Array256x256of<LoopingLogPixel> ); break; } \
        }
public:
    UncertainPixelVector256x256() : data(0)
    {
    }
    void init()
    {
        if(data) return;
        #define init(type, type2) \
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
        #define copy(type, type2) \
            std::allocator<type2> a; \
            type2* aptr = a.allocate(1); \
            aptr->Construct( *(const type2*) b.data ); \
            data = (void*) aptr
        DoCases(copy);
        #undef copy
    }
    UncertainPixelVector256x256& operator= (const UncertainPixelVector256x256& b)
    {
        #define assign(type, type2) \
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
    }

    ~UncertainPixelVector256x256()
    {
        if(!data) return;
        #define done(type, type2) \
            type2* aptr = (type2*) data; \
            std::allocator<type2> a; \
            aptr->Destruct(); \
            a.deallocate(aptr, 1)
        DoCases(done);
        #undef done
    }
    uint32 operator[] (unsigned index) const
    {
        #define get(type, type2) return ((const type2*)data)->data[index]
        DoCases(get);
        #undef get
        return DefaultPixel;
    }
    void set(unsigned index, uint32 p)
    {
        #define put(type, type2) ((type2*)data)->data[index].set(p)
        DoCases(put);
        #undef put
    }
    void Compress()
    {
        if(!data) return;
        #define act(type, type2) \
            type2* aptr = (type2*)data; \
            for(unsigned a=0; a<256*256; ++a) aptr->data[a].Compress()
        DoCases(act);
        #undef act
    }

    void* GetPtr() const { return data; }
    bool empty() const { return !data; }
    unsigned size() const { return data ? 256*256 : 0; }

private:
    #undef DoCases
    void* data;
};

#endif
