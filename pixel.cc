#include "types.hh"

unsigned LoopingLogLength = 16;

#ifdef __GNUC__
# define FastPixelMethod __attribute__((regparm(6)))
# define FasterPixelMethod __attribute__((regparm(6),always_inline))
#else
# define FastPixelMethod
# define FasterPixelMethod
#endif

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
    inline void set(uint32 p, unsigned timer) FastPixelMethod
    {
        pixel1.set(p, timer);
        pixel2.set(p, timer);
    }
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return pixel1.get(timer); }
    inline uint32 get_pixel2(unsigned timer) const FasterPixelMethod { return pixel2.get(timer); }
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
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return Pix::get(timer); }
    inline uint32 get_pixel2(unsigned timer) const FasterPixelMethod { return Pix::get(timer); }
};

template<typename Pixel1,typename Pixel2>
class SwapTwoPixels: private TwoPixels<Pixel1,Pixel2>
{
    typedef TwoPixels<Pixel1,Pixel2> TwoPix;
public:
    using TwoPix::set;
    using TwoPix::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return TwoPix::get_pixel2(timer); }
    inline uint32 get_pixel2(unsigned timer) const FasterPixelMethod { return TwoPix::get_pixel1(timer); }
};

#define DefineBasePair(basetype, type1, type2) \
    template<> \
    class TwoPixels<type1##Pixel, type2##Pixel>: private basetype \
    { \
    public: \
        using basetype::set; \
        using basetype::Compress; \
        inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return basetype::Get##type1(timer); } \
        inline uint32 get_pixel2(unsigned timer) const FasterPixelMethod { return basetype::Get##type2(timer); } \
    }; \
    template<> \
    class TwoPixels<type2##Pixel, type1##Pixel>: \
        public SwapTwoPixels<type1##Pixel, type2##Pixel> \
    { \
    };

#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/mostusedwithinpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"
// ActionAvgPixel is defined in MostUsedPixel AND ChangeLogPixel
// LoopingAvgPixel is defined in ChangeLogPixel
// LoopingLastPixel is defined in ChangeLogPixel

#undef DefineBasePair

/* Postponing pixel.hh inclusion here to ensure that
 * the pixel implementations do not depend on any globals.
 */
#include "pixel.hh"

enum PixelMethod pixelmethod = pm_MostUsedPixel;
enum PixelMethod bgmethod    = pm_MostUsedPixel;


template<typename T>
struct Array256x256of: public Array256x256of_Base
{
public:
    T data[256*256];
public:
    Array256x256of() { }
    virtual ~Array256x256of() { }
public:
    virtual uint32 GetLive(unsigned index, unsigned timer) const
    {
        return data[index].get_pixel1(timer);
    }

    virtual uint32 GetStatic(unsigned index) const
    {
        return data[index].get_pixel2(0);
    }

    virtual void Set(unsigned index, uint32 p, unsigned timer)
    {
        data[index].set(p, timer);
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
        typedef FirstPixel              t2;
        typedef MostUsedPixel           t3;
        typedef MostUsedWithinPixel<16> t4;
        typedef ActionAvgPixel          t5;
        typedef ChangeLogPixel          t6;
        typedef LoopingLogPixel         t7;
        typedef LoopingAvgPixel         t8;
        typedef LoopingLastPixel        t9;

        template<typename Type1>
        class SubTables
        {
            template<typename Type2>
            struct s : public FactoryMethods<TwoPixels<Type1,Type2> > { };
            // s is akin to a templated typedef.
            // It simplifies the syntax in the definition of methods[].
            // It is a short name, but its visibility is private to Subtables.
        public:
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
                SubTables<t5>::methods,
                SubTables<t6>::methods,
                SubTables<t7>::methods,
                SubTables<t8>::methods,
                SubTables<t9>::methods
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
        { s<t5>::Construct, s<t5>::Copy, s<t5>::Assign },
        { s<t6>::Construct, s<t6>::Copy, s<t6>::Assign },
        { s<t7>::Construct, s<t7>::Copy, s<t7>::Assign },
        { s<t8>::Construct, s<t8>::Copy, s<t8>::Assign },
        { s<t9>::Construct, s<t9>::Copy, s<t9>::Assign }
    };
}

void UncertainPixelVector256x256::init()
{
    /* Construct the type of object determined by the globals "pixelmethod" and "bgmethod" */
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
