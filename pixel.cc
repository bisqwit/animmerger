#include "pixel.hh"

enum PixelMethod pixelmethod = pm_MostUsedPixel;

unsigned LoopingLogLength = 16;

bool NeedsMostUsedPixel = true;

#include "pixels/mostusedpixel.hh"

#include "pixels/averagepixel.hh"
#include "pixels/lastpixel.hh"
#include "pixels/mostusedwithinpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"

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

template<typename T>
class PixelButNotMostUsedPixel: public T
{
public:
    inline uint32 get_pixel() const { return T::operator uint32(); }
    inline uint32 get_mostused() const { return get_pixel(); }
};

namespace
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
            // ChangeLog and LoopingLog contain a built-in MostUsedPixel,
            // so adding a PixelButNotMostUsedPixel for those types
            // wouldn't do any good.
            typedef FactoryMethods<PixelButNotMostUsedPixel<AveragePixel> > q0;
            typedef FactoryMethods<PixelButNotMostUsedPixel<LastPixel> > q1;
            typedef FactoryMethods<MostUsedPixelAndMostUsedPixel> q2;
            typedef FactoryMethods<PixelButNotMostUsedPixel<MostUsedWithinPixel<16> > > q3;
            typedef FactoryMethods<ChangeLogPixelAndMostUsedPixel> q4;
            typedef FactoryMethods<LoopingLogPixelAndMostUsedPixel> q5;
            // Table
            static const FactoryType methods[] =
            {
                // Just Pixel:
                { q0::Construct, q0::Copy, q0::Assign },
                { q1::Construct, q1::Copy, q1::Assign },
                { q2::Construct, q2::Copy, q2::Assign },
                { q3::Construct, q3::Copy, q3::Assign },
                { q4::Construct, q4::Copy, q4::Assign },
                { q5::Construct, q5::Copy, q5::Assign },
                // Pixel and MostUsedPixel:
                { t0::Construct, t0::Copy, t0::Assign },
                { t1::Construct, t1::Copy, t1::Assign },
                { t2::Construct, t2::Copy, t2::Assign },
                { t3::Construct, t3::Copy, t3::Assign },
                { t4::Construct, t4::Copy, t4::Assign },
                { t5::Construct, t5::Copy, t5::Assign }
            };
            return &methods[pixelmethod + 6*NeedsMostUsedPixel];
        }
        // This encapsulation allows us to change the static
        // array into a std::map or MapType if we'd like to.
    };
}

void UncertainPixelVector256x256::init()
{
    /* Construct the type of object determined by the global enum "method" */
    if(!data) data = Get256x256pixelFactory()->Construct();
}

// Copy constructor
UncertainPixelVector256x256::UncertainPixelVector256x256
    (const UncertainPixelVector256x256& b) : data(0)
{
    if(b.data) data = Get256x256pixelFactory()->Copy(*b.data);
}

// Assign operator (create copy of the other vector)
UncertainPixelVector256x256&
    UncertainPixelVector256x256::operator=
        (const UncertainPixelVector256x256& b)
{
    if(!b.data) { delete data; data=0; }
    else if(data) Get256x256pixelFactory()->Assign(*data, *b.data);
    else data = Get256x256pixelFactory()->Copy(*b.data);
    return *this;
}
