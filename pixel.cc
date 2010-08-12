#include "types.hh"

bool     OptimizeChangeLog   = true;
unsigned AnimationBlurLength = 0;
unsigned LoopingLogLength    = 16;
int      FirstLastLength     = 16;

#include "pixel.hh"

unsigned long pixelmethods_result = 1ul << pm_MostUsedPixel;
enum PixelMethod bgmethod = pm_MostUsedPixel;


/* List of pixel classes. Only list those pixel
 * classes that have an unique set of Traits.
 * Sort them in the order of preference-of-use
 * when multiple classes implement the same method.
 */
#define DefinePixelClasses(callback) \
    callback(0,LastPixel) \
    callback(1,FirstPixel) \
    callback(2,TinyAveragePixel) \
    callback(3,AveragePixel) \
    callback(4,MostUsedPixel) \
    callback(5,LoopingLogPixel) \
    callback(6,ChangeLogPixel)

/* DummyPixel is a method that implements the interface
 * for all pixel methods but with no actual features
 * whatsoever. */
struct DummyPixel
{
    //inline uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    static inline void set(uint32,unsigned=0) FasterPixelMethod { }

    #define CreateDummyAliases(o,f,name) \
    static inline uint32 Get##name(unsigned=0) FasterPixelMethod { return 0; }
    DefinePixelMethods(CreateDummyAliases)
    #undef CreateDummyAliases
/////////
    static const unsigned long Traits = 0ul;
    static const unsigned SizePenalty = 0ul;
};

#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/tinyaveragepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/loopinglogpixel.hh"
#include "pixels/changelogpixel.hh"


template<typename T>
struct Array256x256of: public Array256x256of_Base
{
public:
    T data[256*256];
public:
    Array256x256of() { }
    virtual ~Array256x256of() { }
public:
    virtual uint32 GetLive(PixelMethod method, unsigned index, unsigned timer) const FastPixelMethod
    {
        #define MakeMethodCase(n,f,name) \
        if(T::Traits == (1ul << pm_##name##Pixel)) \
            return data[index].Get##name(0);
        DefinePixelMethods(MakeMethodCase);
        #undef MakeMethodCase

        // This switchcase is actually pretty optimal
        // compared to the method pointer table, because
        // those cases that are not implemented in T
        // actually expand to inline code that results in 0.
        // It could still be improved though, by somehow
        // removing the check for those methods that never
        // can occur here.

        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: return data[index].Get##name(timer);
        switch(method)
        {
            DefinePixelMethods(MakeMethodCase);
            default: return 0;
        }
        #undef MakeMethodCase
    }

    virtual uint32 GetStatic(unsigned index) const
    {
        #define MakeMethodCase(n,f,name) \
        if(T::Traits == (1ul << pm_##name##Pixel)) \
            return data[index].Get##name(0);
        DefinePixelMethods(MakeMethodCase);
        #undef MakeMethodCase

        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: return data[index].Get##name(0);
        switch(bgmethod)
        {
            DefinePixelMethods(MakeMethodCase);
            default: return 0;
        }
        #undef MakeMethodCase
    }

    virtual void Set(unsigned index, uint32 p, unsigned timer)
    {
        data[index].set(p, timer);
    }
};

namespace
{
    /* This converts an implementation index into a type. */
    template<unsigned id, typename Base>
    struct PixelMethodImpl { typedef void result; };

    /* This converts a type into a name (string). */
    template<typename T>
    struct PixelMethodImplName
    {
        static inline const char* getname()
        {
            return "ERROR: NO IMPLEMENTATION FOUND FOR THIS SET OF FEATURES";
        }
    };

    #define MakePixelMethodImpl(id,name) \
        template<typename Base> \
        struct PixelMethodImpl<id, Base> \
        { \
            typedef name<Base> result; \
        }; \
        template<typename Base> \
        struct PixelMethodImplName< name<Base> > \
        { \
            static const char* getname() \
            { \
                static const std::string n = \
                    #name + std::string("+") + PixelMethodImplName<Base>::getname(); \
                return n.c_str(); \
            } \
        }; \
        template<> \
        struct PixelMethodImplName< name<DummyPixel> > \
        { \
            static inline const char* getname() { return #name; } \
        };
    DefinePixelClasses(MakePixelMethodImpl)
    #undef MakePixelMethodImpl

    template<unsigned n=0, typename Obj = typename PixelMethodImpl<n,DummyPixel>::result>
    struct GetMethodImplCount : public GetMethodImplCount<n+1> { };
    template<unsigned n>
    struct GetMethodImplCount<n,void> { enum { result = n }; };

    template<unsigned Value, unsigned Basevalue=0, bool found = !Value || (Value&1)>
    struct GetLowestBit : public GetLowestBit<Value/2, Basevalue+1> { };
    template<unsigned Value, unsigned Basevalue>
    struct GetLowestBit<Value, Basevalue, true> { enum { result = Basevalue }; };

    template<unsigned value, unsigned alreadycounted = 0>
    struct PopCount : public PopCount< value/2, (alreadycounted + (value&1)) > { };
    template<unsigned alreadycounted>
    struct PopCount<0, alreadycounted> { enum { result = alreadycounted }; };

    template<typename T1, typename T2, bool select1st>
    struct ChooseType { typedef T2 result; };
    template<typename T1, typename T2>
    struct ChooseType<T1,T2,true> { typedef T1 result; };

    struct FactoryType
    {
        typedef Array256x256of_Base ObjT;
        ObjT* (*Construct)();
        ObjT* (*Copy)(const ObjT& b);
        void  (*Assign)(ObjT& tgt, const ObjT& b);
        const char* (*GetName)();
        unsigned short Size;
        unsigned short SizePenalty;
    };
    template<typename T>
    struct FactoryMethods: public T
    {
        typedef Array256x256of_Base ObjT;
        typedef Array256x256of<T> ResT;
        static ObjT* Construct()         { return new ResT; }
        static ObjT* Copy(const ObjT& b) { return new ResT ( (const ResT&) b ) ; }
        static void Assign(ObjT& tgt, const ObjT& b) { (ResT&) tgt = (const ResT&) b; }
        static const unsigned long Size = sizeof(T);
    };
    template<>
    struct FactoryMethods<void>
    {
        static Array256x256of_Base* Construct() { return 0; }
        static Array256x256of_Base* Copy(const Array256x256of_Base&) { return 0; }
        static void Assign(Array256x256of_Base&, const Array256x256of_Base&) { }
        static const unsigned long Size = 0xffffu, SizePenalty = 0;
    };
    template<typename T>
    struct PixelImplCombFactory
    {
        static const FactoryType data;
    };
    template<typename T>
    const FactoryType PixelImplCombFactory<T>::data =
    {
        FactoryMethods<T>::Construct,
        FactoryMethods<T>::Copy,
        FactoryMethods<T>::Assign,
        PixelMethodImplName<T>::getname,
        FactoryMethods<T>::Size,
        FactoryMethods<T>::SizePenalty
    };

    /* Creates a combination class of pixel methods matching the requested bitmask. */
    /* The combination is created through chain-inheritance. */
    /* "PixelMethodImplementationCombination" would be somewhat wordy.
     * Hence abbreviated.
     */
    template<unsigned bitmask = 1,
             unsigned lowestbit = GetLowestBit<bitmask>::result,
             unsigned remainingbits = bitmask & ~(1ul << lowestbit)
            >
    struct PixelMethodImplComb: public
        PixelMethodImpl<lowestbit, typename PixelMethodImplComb<remainingbits>::result>
    {
        enum {
            nmasks = 1ul << GetMethodImplCount<>::result,
            nextbitmask = (bitmask+1) & (nmasks-1)
        };
    };
    template<>
    struct PixelMethodImplComb<0>
    {
        typedef DummyPixel result;
        enum { nextbitmask = 0 };
    };

    /* Same as PixelMethodImplComb, but given a Traits value,
     * either picks the PixelMethodImplComb matching given bitmask,
     * or escalates to next bitmask, until an implementation
     * that has the requested Traits is found.
     */
    template
        <unsigned long Traits,unsigned bitmask=1,
         bool traits_ok =
          (!(Traits & ~PixelMethodImplComb<bitmask>::result::Traits) || !bitmask)>
    struct NextPixelMethodImplComb_WithTraits: public PixelMethodImplComb<bitmask>
    { // Traits match = pick the implementation
    };
    template<unsigned long Traits,unsigned bitmask>
    struct NextPixelMethodImplComb_WithTraits<Traits,bitmask,false>
        : public NextPixelMethodImplComb_WithTraits
            <Traits, PixelMethodImplComb<bitmask>::nextbitmask>
    { // Traits mismatch = try next implementation
    };

    /* Picks the smallest PixelMethodImplComb that fulfills the requested Traits. */
    template<unsigned long Traits,
             typename cand1info = NextPixelMethodImplComb_WithTraits<Traits>,
             typename cand1 = typename cand1info::result>
    struct BestPixelMethodImplComb_ForTraits
    {
        typedef NextPixelMethodImplComb_WithTraits<Traits, cand1info::nextbitmask> cand2info;
        typedef BestPixelMethodImplComb_ForTraits<Traits, cand2info> next;
        typedef typename next::result cand2;
        static const unsigned long cost = sizeof(cand1) + cand1::SizePenalty;
        static const unsigned overqualification = PopCount<cand1::Traits &~Traits>::result;
        static const bool is_better =
             (cost < next::cost)
             /* Because users may complain if First+MostUsed gives
              * a type of First+Last+MostUsed, even though both are
              * 24 bytes on x86_64, do extra work to get the type
              * that contains the minimal feature set required.
              * Also, it will give a faster set() method.
              */
          || (cost == next::cost && overqualification < next::overqualification);
        typedef typename ChooseType<cand1, cand2, is_better>::result result;
    };
    template<unsigned long T, typename c>
    struct BestPixelMethodImplComb_ForTraits<T,c, DummyPixel>
    {
        // DummyPixel is our fallback: It implements everything,
        // but we'll make it the worst possible choice, so it
        // never gets chosen unless there's really no "real" solution.
        typedef DummyPixel result;
        static const unsigned long cost = 0xfffffful;
        static const unsigned overqualification = 0;
        // Even then, it never gets instantiated, because of
        // PixelImplCombFactory<void>, so we could really use
        // any type here.
    };

    template<unsigned bitmask>
    static inline const FactoryType* FindFactoryForTraits(unsigned) FasterPixelMethod;
    template<unsigned bitmask>
    static inline const FactoryType* FindFactoryForTraits(unsigned Traits)
    {
        if(!bitmask) return &PixelImplCombFactory<void>::data;
        typedef PixelMethodImplComb<bitmask> implfinder;
        typedef typename implfinder::result thistype;
        typedef typename BestPixelMethodImplComb_ForTraits<thistype::Traits>::result improvedtype;
        //if( (thistype::Traits & Traits) == Traits)
        if(!( (thistype::Traits ^ Traits) & Traits) )
        //if (! (~thistype::Traits & Traits))
            return &PixelImplCombFactory<improvedtype>::data;
        return FindFactoryForTraits<implfinder::nextbitmask>(Traits);
    };

    const FactoryType* Get256x256pixelFactory()
    {
        unsigned long Traits = pixelmethods_result | (1ul << bgmethod);

        static unsigned long Prev = ~0ul;
        static const FactoryType* cache = 0;

        if(Prev == Traits) return cache;
        return cache = FindFactoryForTraits<1>(Prev = Traits);
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

unsigned GetPixelSizeInBytes()
{
    return Get256x256pixelFactory()->Size;
}
unsigned GetPixelSizePenaltyInBytes()
{
    return Get256x256pixelFactory()->SizePenalty;
}
const char* GetPixelSetupName()
{
    return Get256x256pixelFactory()->GetName();
}
