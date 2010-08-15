#include "types.hh"

bool     OptimizeChangeLog   = true;
unsigned AnimationBlurLength = 0;
unsigned LoopingLogLength    = 16;
int      FirstLastLength     = 16;

#include "pixel.hh"

unsigned long pixelmethods_result = 1ul << pm_MostUsedPixel;
enum PixelMethod bgmethod = pm_MostUsedPixel;

#define DO_VERY_SPECIALIZED -1
/* Specialization values:
 *   1 = Specialized implementations for all Array256x256 methods
 *   0 = Specialized implementations only for those pixelmethods
 *       that have one feature
 *  -1 = Minimal set of virtual functions that still does the work
 *  -2 = Use GetLive to implement GetStatic, further reducing code size
 */

/* List of pixel classes. Only list those pixel
 * classes that have an unique set of Traits.
 * Sort them in the order of preference-of-use
 * when multiple classes implement the same method.
 */
#define DefinePixelClasses(callback) \
    callback(0,Last) \
    callback(1,First) \
    callback(2,TinyAverage) \
    callback(3,Average) \
    callback(4,MostUsed) \
    callback(5,LoopingLog) \
    callback(6,ChangeLog)

#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/tinyaveragepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/loopinglogpixel.hh"
#include "pixels/changelogpixel.hh"

namespace
{
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

    #define MakeMethodCaller(n,f,name) \
    template<typename T, bool HasMethod = T::Traits & (1u << pm_##name##Pixel)> \
    struct CallGet##name##Helper \
    { \
        static uint32 call(const T& obj, unsigned timer=0) FasterPixelMethod \
            { return obj.Get##name(timer); } \
    }; \
    template<typename T> \
    struct CallGet##name##Helper<T,false> \
    { \
        static uint32 call(const T&, unsigned=0) FasterPixelMethod \
            { return DefaultPixel; } \
    }; \
    template<typename T> \
    uint32 CallGet##name(const T& obj, unsigned timer=0) FasterPixelMethod; \
    template<typename T> \
    uint32 CallGet##name(const T& obj, unsigned timer=0) \
    { \
        return CallGet##name##Helper<T>::call(obj, timer); \
    }
    DefinePixelMethods(MakeMethodCaller)
    #undef MakeMethodCaller
}

uint32 Array256x256of_Base::GetStatic(unsigned index) const
{
    return GetLive(bgmethod, index, 0);
}

void Array256x256of_Base::GetStaticInto
    (uint32* target, unsigned target_stride) const
{
    unsigned index    = 0;
    unsigned endindex = 256*256;
    for(unsigned p=0; index<endindex;
            index += 256,
            p += target_stride-256)
    {
        for(unsigned x=0; x<256; ++x)
            target[p++] = GetStatic(index+x);
    }
}

void Array256x256of_Base::GetLiveSectionInto
    (PixelMethod method, unsigned timer,
    uint32* target, unsigned target_stride,
    unsigned x1, unsigned y1,
    unsigned width, unsigned height) const
{
    unsigned index    = y1*256+x1;
    unsigned endindex = index + height*256;
    for(unsigned p=0; index<endindex;
            index += 256,
            p += target_stride-width)
    {
        for(unsigned x=0; x<width; ++x)
            target[p++] = GetLive(method, index+x, timer);
    }
}

void Array256x256of_Base::PutSectionInto
    (unsigned timer,
    const uint32* source, unsigned target_stride,
    unsigned x1, unsigned y1,
    unsigned width, unsigned height)
{
    unsigned p=0, index=y1*256+x1, maxindex=index+height*256;
    for(; index<maxindex; p+=target_stride, index+=256)
        for(unsigned x=0; x<width; ++x)
        {
            uint32 pix = source[p+x];
            if(pix & 0xFF000000u) continue;
            /* Do not plot transparent pixels */
            Set(index+x, pix, timer);
        }
}

template<typename T,
    bool ManyImpl =
#if DO_VERY_SPECIALIZED == 0
(PopCount<T::Traits>::result > 1)
#else
    false
#endif
>
struct Array256x256ofImpl: public Array256x256of_Base
{
public:
    T data[256*256];
};

#if DO_VERY_SPECIALIZED >= 0
template<typename T>
struct Array256x256ofImpl<T,false>: public Array256x256of_Base
{
public:
    T data[256*256];

    virtual void GetLiveSectionInto(PixelMethod method, unsigned timer,
        uint32* target, unsigned target_stride,
        unsigned x1, unsigned y1,
        unsigned width, unsigned height) const
    {
        const T* databegin = data      + (y1*256+x1);
        const T* dataend   = databegin + height*256;
    #if DO_VERY_SPECIALIZED>0
        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: \
                if(T::Traits & (1<<pm_##name##Pixel)) \
                    for(; databegin<dataend; \
                           target += target_stride-width, \
                           databegin += 256-width) \
                        for(unsigned x=width; x-->0; ) \
                            *target++ = CallGet##name(*databegin++, timer); \
                break;
        switch(method)
        {
            DefinePixelMethods(MakeMethodCase);
            default: break;
        }
        #undef MakeMethodCase
    #else
        // This implementation works when T only has one feature
        method=method;
        for(; databegin<dataend;
               target += target_stride-width,
               databegin += 256-width)
            for(unsigned x=width; x-->0; )
                *target++ = databegin++->get(timer);
    #endif
    }

    virtual void GetStaticInto(
        uint32* target, unsigned target_stride) const
    {
        const T* databegin = data;
        const T* dataend   = databegin + 256*256;
    #if DO_VERY_SPECIALIZED>0
        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: \
                if(T::Traits & (1<<pm_##name##Pixel)) \
                    for(; databegin<dataend; \
                           target += target_stride-256) \
                        for(unsigned x=256; x-->0; ) \
                            *target++ = CallGet##name(*databegin++, 0); \
                break;
        switch(bgmethod)
        {
            DefinePixelMethods(MakeMethodCase);
            default: break;
        }
        #undef MakeMethodCase
    #else
        // This implementation works when T only has one feature
        for(; databegin<dataend;
               target += target_stride-256)
            for(unsigned x=256; x-->0; )
                *target++ = databegin++->get(0);
    #endif
    }

    virtual void PutSectionInto
        (unsigned timer,
        const uint32* source, unsigned target_stride,
        unsigned x1, unsigned y1,
        unsigned width, unsigned height)
    {
        unsigned p=0, index=y1*256+x1, maxindex=(y1+height)*256+x1;
        for(; index<maxindex; p+=target_stride, index+=256)
            for(unsigned x=0; x<width; ++x)
            {
                uint32 pix = source[p+x];
                if(pix & 0xFF000000u) continue;
                /* Do not plot transparent pixels */
                data[index+x].set(pix, timer);
            }
    }
};
#endif


template<typename T>
struct Array256x256of: public Array256x256ofImpl<T>
{
    typedef Array256x256ofImpl<T> rep;
public:
#if DO_VERY_SPECIALIZED >= -1
    virtual uint32 GetStatic(unsigned index) const
    {
        return DoGetLive(bgmethod, index, 0);
    }
#endif

    virtual uint32 GetLive(PixelMethod method, unsigned index, unsigned timer) const FastPixelMethod
    {
        return DoGetLive(method, index, timer);
    }

    virtual void Set(unsigned index, uint32 p, unsigned timer)
    {
        rep::data[index].set(p, timer);
    }

private:
    uint32 DoGetLive(PixelMethod method, unsigned index, unsigned timer) const FastPixelMethod
    {
        // This switchcase is actually pretty optimal
        // compared to the method pointer table, because
        // those cases that are not implemented in T
        // actually expand to inline code that results in 0.
        // It could still be improved though, by somehow
        // removing the check for those methods that never
        // can occur here.

        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: \
                if(!(T::Traits & (1u << pm_##name##Pixel))) break; \
            case##name: \
                return CallGet##name(rep::data[index], timer);
        switch(method)
        {
            DefinePixelMethods(MakeMethodCase);
            default: break;
        }
        #undef MakeMethodCase
        /* In case of invalid "method" parameter, jump to
         * to the first _valid_ implementation. This code
         * actually reduces the generated code size.
         */
        #define MakeDefaultCase(n,f,name) \
            case pm_##name##Pixel: goto case##name;
        switch((PixelMethod) GetLowestBit<T::Traits>::result)
        {
            DefinePixelMethods(MakeDefaultCase);
            default: return DefaultPixel;
        }
        #undef MakeDefaultCase
    }
};

namespace
{
    /* This converts an implementation index into a type. */
    template<unsigned id, typename Base>
    struct PixelMethodImpl { typedef void result; enum { Traits = 0 }; };

    /* This converts a type into a name (string). */
    template<typename T>
    struct PixelMethodImplName
    {
        static inline const char* getname()
        {
            return "ERROR: NO IMPLEMENTATION FOUND FOR THIS SET OF FEATURES";
        }
    };

    template<typename T1,typename T2, bool force=false>
    struct CombinePixels: public T1, public T2
    {
        void set(uint32 p, unsigned timer=0) FastPixelMethod
        {
            T1::set(p,timer);
            T2::set(p,timer);
        }
        static const unsigned long Traits = T1::Traits | T2::Traits;
        static const unsigned SizePenalty = T1::SizePenalty + T2::SizePenalty;
    };
    template<typename T1>
    class CombinePixels<T1, LoopingLogPixel, false>
        : public CombinePixels<T1, LoopingLogPixel, true>
    {
    public:
        /* LoopingLogPixel<ChangeLogPixel> has visibility
         * ambiguity on these methods. Make the order explicit.
         */
        using LoopingLogPixel::GetLoopingLog;
        using LoopingLogPixel::GetActionAvg;
        using LoopingLogPixel::GetMostUsed;
        using LoopingLogPixel::GetLeastUsed;
        using LoopingLogPixel::GetAverage;
        using LoopingLogPixel::GetTinyAverage;
    };

    #define MakePixelMethodImpl(id,name) \
        template<typename Base> \
        struct name: public CombinePixels<Base,name##Pixel> \
        { \
        }; \
        template<typename Base> \
        struct PixelMethodImpl<id, Base> \
        { \
            typedef name<Base> result; \
            static const unsigned long Traits = result::Traits; \
        }; \
        template<> \
        struct PixelMethodImpl<id, void> \
        { \
            typedef name##Pixel result; \
            static const unsigned long Traits = result::Traits; \
        }; \
        template<typename Base> \
        struct PixelMethodImplName<name<Base> > \
        { \
            static const char* getname() \
            { \
                static const std::string n = \
                    #name + std::string("+") + PixelMethodImplName<Base>::getname(); \
                return n.c_str(); \
            } \
        }; \
        template<> \
        struct PixelMethodImplName<name##Pixel> \
        { \
            static inline const char* getname() { return #name; } \
        };
    DefinePixelClasses(MakePixelMethodImpl)
    #undef MakePixelMethodImpl

    template<unsigned n=0, typename Obj = typename PixelMethodImpl<n,void>::result>
    struct GetMethodImplCount : public GetMethodImplCount<n+1> { };
    template<unsigned n>
    struct GetMethodImplCount<n,void> { enum { result = n }; };

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
        typedef void result;
        enum { nextbitmask = 0, Traits = 0 };
    };

    /* Same as PixelMethodImplComb, but given a Traits value,
     * either picks the PixelMethodImplComb matching given bitmask,
     * or escalates to next bitmask, until an implementation
     * that has the requested Traits is found.
     */
    template
        <unsigned long Traits,unsigned bitmask=1,
         bool traits_ok =
          (!(Traits & ~PixelMethodImplComb<bitmask>::Traits) || !bitmask)>
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
        static const unsigned nb = cand1info::nextbitmask;
        typedef NextPixelMethodImplComb_WithTraits<Traits, nb> cand2info;
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
    struct BestPixelMethodImplComb_ForTraits<T,c, void>
    {
        typedef void result;
        static const unsigned long cost = 0xfffffful;
        static const unsigned overqualification = 0;
    };

    template<unsigned bitmask>
    static inline const FactoryType* FindFactoryForTraits(unsigned) FasterPixelMethod;
    template<unsigned bitmask>
    static inline const FactoryType* FindFactoryForTraits(unsigned Traits)
    {
        if(!bitmask) return &PixelImplCombFactory<void>::data;
        typedef PixelMethodImplComb<bitmask> implfinder;
        typedef typename implfinder::result thistype;
        typedef typename BestPixelMethodImplComb_ForTraits<implfinder::Traits>::result improvedtype;
        //if( (implfinder::Traits & Traits) == Traits)
        if(!( (implfinder::Traits ^ Traits) & Traits) )
        //if (! (~implfinder::Traits & Traits))
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
