#include "types.hh"

bool     OptimizeChangeLog   = true;
unsigned AnimationBlurLength = 4;
unsigned LoopingLogLength    = 16;
int      FirstLastLength     = 16;

#ifdef __GNUC__
# define FastPixelMethod __attribute__((regparm(6)))
# define FasterPixelMethod __attribute__((regparm(6),always_inline))
#else
# define FastPixelMethod
# define FasterPixelMethod
#endif

#include "pixel.hh"

#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"
// LeastUsedPixel is defined in MostUsedPixel
// ActionAvgPixel is defined in MostUsedPixel
// LoopingAvgPixel is defined in ChangeLogPixel
// LoopingLastPixel is defined in ChangeLogPixel
// LastNPixel is defined in ChangeLogPixel
// FirstNPixel is defined in ChangeLogPixel
// In addition, ChangeLog can emulate all of these.

struct DummyBasePixel
{
    static inline uint32 GetAverage(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetLast(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetFirst(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetMostUsed(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetLeastUsed(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetActionAvg(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetChangeLog(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetLoopingLog(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetLoopingAvg(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetLoopingLast(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetLastNMost(unsigned=0) FasterPixelMethod { return 0; }
    static inline uint32 GetFirstNMost(unsigned=0) FasterPixelMethod { return 0; }
};
template<typename T1>
struct MultiBasePixel1: public DummyBasePixel, public T1
{
    void set(uint32 p, unsigned timer) { T1::set(p,timer); }
};
template<typename T1, typename T2>
struct MultiBasePixel2: public DummyBasePixel, public T1, public T2
{
    void set(uint32 p, unsigned timer) { T1::set(p,timer); T2::set(p,timer); }
};
template<typename T1, typename T2, typename T3>
struct MultiBasePixel3: public DummyBasePixel, public T1, public T2, public T3
{
    void set(uint32 p, unsigned timer) { T1::set(p,timer); T2::set(p,timer); T3::set(p,timer); }
};
template<typename T1, typename T2, typename T3, typename T4>
struct MultiBasePixel4: public DummyBasePixel, public T1, public T2, public T3, public T4
{
    void set(uint32 p, unsigned timer) { T1::set(p,timer); T2::set(p,timer); T3::set(p,timer); T4::set(p,timer); }
};



#undef DefineBasePair


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
};

namespace
{
    template<typename T>
    struct IsAnimatedType { enum { result = false }; };
    template<>
    struct IsAnimatedType<ChangeLogPixel> { enum { result = true }; };
    template<>
    struct IsAnimatedType<LoopingLogPixel> { enum { result = true }; };
    template<>
    struct IsAnimatedType<LoopingAvgPixel> { enum { result = true }; };
    template<>
    struct IsAnimatedType<LoopingLastPixel> { enum { result = true }; };

    template<typename Type1,typename Type2,bool IsAnimatedSecondType>
    struct Select2ndType { typedef Type2 result; };
    template<typename Type1,typename Type2>
    struct Select2ndType<Type1,Type2,true> { typedef Type1 result; };

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
        typedef LeastUsedPixel          t4;
        typedef ActionAvgPixel          t5;
        typedef ChangeLogPixel          t6;
        typedef LoopingLogPixel         t7;
        typedef LoopingAvgPixel         t8;
        typedef LoopingLastPixel        t9;
        typedef LastNMostPixel          tA;
        typedef FirstNMostPixel         tB;

        template<typename Type1>
        class SubTables
        {
            // s is supposed to be FactoryMethods<TwoPixels<Type1,Type2> >.
            // However, we know that Type2 can never be an animated type.
            // To avoid having the compiler synthesize those pairs that
            // are never used, change Type2 into Type1 whenever it is an animated type.
            // If Type1 is an animated type as well, oh-well; it does not matter.
            template<typename Type2>
            struct s : public FactoryMethods<TwoPixels<Type1,
                                    typename Select2ndType<Type1,Type2,
                                       IsAnimatedType<Type2>::result
                                                 >::result> > { };
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
                SubTables<t9>::methods,
                SubTables<tA>::methods,
                SubTables<tB>::methods
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
        { s<t9>::Construct, s<t9>::Copy, s<t9>::Assign },
        { s<tA>::Construct, s<tA>::Copy, s<tA>::Assign },
        { s<tB>::Construct, s<tB>::Copy, s<tB>::Assign }
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
