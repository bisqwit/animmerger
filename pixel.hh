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

extern enum PixelMethod
{
    pm_AveragePixel,
    pm_LastPixel,
    pm_MostUsedPixel,
    pm_MostUsed16Pixel,
    pm_ChangeLogPixel,
    pm_LoopingLogPixel
} pixelmethod;

struct Array256x256of_Base
{
    virtual ~Array256x256of_Base() { }
    virtual uint32 GetPixel(unsigned index) const = 0;
    virtual uint32 GetMostUsed(unsigned index) const = 0;
    virtual void set(unsigned index, uint32 p) = 0;
    virtual void Compress() = 0;
};
template<typename T>
struct Array256x256of: public Array256x256of_Base
{
public:
    T data[256*256];
public:
    Array256x256of() { }
    virtual ~Array256x256of() { }
public:
    virtual uint32 GetPixel(unsigned index) const
    {
        return data[index].get_pixel();
    }
    virtual uint32 GetMostUsed(unsigned index) const
    {
        return data[index].get_mostused();
    }
    virtual void set(unsigned index, uint32 p)
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
    struct FactoryType
    {
        typedef Array256x256of_Base ObjT;
        ObjT* (*Construct)();
        ObjT* (*Copy)(const ObjT& b);
        void  (*Assign)(ObjT& tgt, const ObjT& b);
    };
    template<typename T>
    struct FactoryMethods
    {
        typedef Array256x256of_Base ObjT;
        typedef Array256x256of<T> ResT;
        static ObjT* Construct()         { return new ResT; }
        static ObjT* Copy(const ObjT& b) { return new ResT ( (const ResT&) b ) ; }
        static void Assign(ObjT& tgt, const ObjT& b) { (ResT&) tgt = (const ResT&) b;    }
    };
    class Get256x256pixelFactory
    {
    public:
        inline const FactoryType* operator-> () const
        {
            // Note: This must be in the same order as the Methods enum,
            //       and with no gaps.
            typedef FactoryMethods<AveragePixelAndMostUsedPixel> t0;
            typedef FactoryMethods<LastPixelAndMostUsedPixel> t1;
            typedef FactoryMethods<MostUsedPixelAndMostUsedPixel> t2;
            typedef FactoryMethods<MostUsedWithinAndMostUsedPixel<16> > t3;
            typedef FactoryMethods<ChangeLogPixelAndMostUsedPixel> t4;
            typedef FactoryMethods<LoopingLogPixelAndMostUsedPixel> t5;
            static const FactoryType methods[] =
            {
                { t0::Construct, t0::Copy, t0::Assign },
                { t1::Construct, t1::Copy, t1::Assign },
                { t2::Construct, t2::Copy, t2::Assign },
                { t3::Construct, t3::Copy, t3::Assign },
                { t4::Construct, t4::Copy, t4::Assign },
                { t5::Construct, t5::Copy, t5::Assign },
            };
            return &methods[pixelmethod];
        }
    };
public:
    // By default, the vector is uninitialized (empty)
    UncertainPixelVector256x256() : data(0)
    {
    }
    // Init: Resize the vector to 256x256 elements
    void init()
    {
        /* Construct the type of object determined by the global enum "method" */
        if(!data) data = Get256x256pixelFactory()->Construct();
    }
    // Copy constructor
    UncertainPixelVector256x256(const UncertainPixelVector256x256& b) : data(0)
    {
        if(b.data) data = Get256x256pixelFactory()->Copy(*b.data);
    }
    // Assign operator (create copy of the other vector)
    UncertainPixelVector256x256& operator= (const UncertainPixelVector256x256& b)
    {
        if(!b.data) { delete data; data=0; }
        else if(data) Get256x256pixelFactory()->Assign(*data, *b.data);
        else data = Get256x256pixelFactory()->Copy(*b.data);
        return *this;
    }
    // Destructor: Deallocate vector of alloated
    ~UncertainPixelVector256x256()
    {
        delete data;
    }
    // Visit: Run user-given functor, give reference to the actual vector data as param
    template<typename F>
    inline void Visit(F func)
    {
        if(data) func(*data);
    }
    template<typename F>
    inline void Visit(F func) const
    {
        if(data) func(*data);
    }

    // Run Compress() method on each vector element
    inline void Compress()
    {
        if(data) data->Compress();
    }

    // Test whether vector is empty (uninitialized)
    bool empty() const { return !data; }

private:
    Array256x256of_Base* data;
};

#endif
