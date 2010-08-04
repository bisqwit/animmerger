#include "pixel.hh"

enum PixelMethod pixelmethod = pm_MostUsedPixel;

unsigned LoopingLogLength = 16;

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
