#include "pixel.hh"

enum PixelMethod pixelmethod = pm_MostUsedPixel;
enum PixelMethod bgmethod    = pm_MostUsedPixel;

unsigned LoopingLogLength = 16;

/* This class wraps two pixel classes in one,
 * providing the functions of both. One is a "live"
 * pixel, one is a "static" (background) pixel.
 */
template<typename Pixel1,typename Pixel2>
class TwoPixels
{
    Pixel1 pixel1;
    Pixel2 pixel2;
public:
    inline void set(uint32 p)
    {
        pixel1.set(p);
        pixel2.set(p);
    }
    inline uint32 get_pixel1() const { return pixel1; }
    inline uint32 get_pixel2() const { return pixel2; }
    inline void Compress() { pixel1.Compress(); pixel2.Compress(); }
};

/* Specialize an optimized case for when the two pixel
 * types are the same. Only store one set of data.
 */
template<typename Pix>
class TwoPixels<Pix,Pix>: private Pix
{
public:
    using Pix::set;
    using Pix::Compress;
    inline uint32 get_pixel1() const { return Pix::operator uint32(); }
    inline uint32 get_pixel2() const { return Pix::operator uint32(); }
};

#include "pixels/lastpixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/averagepixel.hh"
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
    virtual uint32 GetLive(unsigned index) const
    {
        return data[index].get_pixel1();
    }

    virtual uint32 GetStatic(unsigned index) const
    {
        return data[index].get_pixel2();
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
        // Note: This must be in the same order as the Methods enum,
        //       and with no gaps.
        typedef AveragePixel            t0;
        typedef LastPixel               t1;
        typedef MostUsedPixel           t2;
        typedef MostUsedWithinPixel<16> t3;
        typedef ChangeLogPixel          t4;
        typedef LoopingLogPixel         t5;

        template<typename Type1>
        struct SubTables
        {
            template<typename Type2>
            struct s : public FactoryMethods<TwoPixels<Type1,Type2> > { };
            static const FactoryType methods[];
        };
    public:
        inline const FactoryType* operator-> () const
        {
            static const FactoryType*const tables[] =
            {
                /* Must match the number of enum items */
                SubTables<t0>::methods,
                SubTables<t1>::methods,
                SubTables<t2>::methods,
                SubTables<t3>::methods,
                SubTables<t4>::methods,
                SubTables<t5>::methods
            };
            return &tables[pixelmethod][bgmethod];
        }
    };
    template<typename Type1>
    const FactoryType Get256x256pixelFactory::SubTables<Type1>::methods[] =
    {
        /* Must match the number of enum items */
        { s<t0>::Construct, s<t0>::Copy, s<t0>::Assign },
        { s<t1>::Construct, s<t1>::Copy, s<t1>::Assign },
        { s<t2>::Construct, s<t2>::Copy, s<t2>::Assign },
        { s<t3>::Construct, s<t3>::Copy, s<t3>::Assign },
        { s<t4>::Construct, s<t4>::Copy, s<t4>::Assign },
        { s<t5>::Construct, s<t5>::Copy, s<t5>::Assign }
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
