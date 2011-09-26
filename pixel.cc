#include "types.hh"

bool     OptimizeChangeLog   = true;
unsigned AnimationBlurLength = 0;
unsigned LoopingLogLength    = 16;
int      FirstLastLength     = 16;
bool     AveragesInYUV       = false;
int      verbose             = 0;
bool     autoalign           = true;
unsigned estimated_num_frames = 0;

#include "pixel.hh"

unsigned long pixelmethods_result = 1ul << pm_MostUsedPixel;
enum PixelMethod bgmethod  = pm_MostUsedPixel;
enum PixelMethod bgmethod0 = pm_MostUsedPixel;
enum PixelMethod bgmethod1 = pm_MostUsedPixel;

#define DO_VERY_SPECIALIZED -1
/* Specialization values:
 *   1 = Specialized implementations for all Array256x256 methods
 *   0 = Specialized implementations only for those pixelmethods
 *       that have one feature
 *  -1 = Minimal set of virtual functions that still does the work
 *  -2 = Use GetLive to implement GetStatic, further reducing code size
 */
template<typename T>
struct Array256x256of;

/* List of pixel classes. Only list those pixel
 * classes that have an unique set of Traits.
 * Sort them in the order of preference-of-use
 * when multiple classes implement the same method.
 */
#define DefinePixelImpls(callback) \
    callback(Last) \
    callback(First) \
    callback(Solid) \
    callback(TinyAverage) \
    callback(Average) \
    callback(MostUsed) \
    callback(ChangeLog)

#define MakeEnum(name) impl_##name,
enum PixelMethodImpl { DefinePixelImpls(MakeEnum) };
#undef MakeEnum

#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/solidpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/tinyaveragepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/changelogpixel.hh"

/* Count them into NPixelImpls */
#define CountImpls(name) +1
enum { NPixelImpls = 0 DefinePixelImpls(CountImpls) };
#undef CountImpls

namespace
{
    /* Translate PixelMethodImpl into a type and a string */

    template<unsigned id> struct PixelMethodClass
    {
        typedef void result;
    };
    template<typename T> struct PixelMethodImplName
    {
        /*static const char* getname()
        #ifdef __GNUC__
            __attribute__((error("You have a PixelMethod that no MethodImpl implements!")))
        #endif
        ;{
            return "ERROR: NO IMPLEMENTATION FOUND FOR THIS SET OF FEATURES";
        }*/
    };
    struct PixelMetaInfoAux
    {
        template<size_t n> struct aux { char dummy[n]; };
        #define MakeTest(o,f,name) \
            template<typename C> \
            static char ttest##name(aux<sizeof(&C::Get##name)>*); \
            template<typename> \
            static aux<2> ttest##name(...);
        DefinePixelMethods(MakeTest)
        #undef MakeTest

        #define MakeTest(name) \
            static char ctest##name(const name##Pixel *); \
            static aux<2> ctest##name(...);
        DefinePixelImpls(MakeTest)
        #undef MakeTest
    };
    template<typename T>
    struct PixelMetaInfo
    {
        typedef T result;

        // Determine traits
        // E.g. Trait pm_FirstPixel is set if the class defines GetFirst().
        static const unsigned long Traits = 0
        #define MakeTest(o,f,name) \
             | ((sizeof(PixelMetaInfoAux::ttest##name<T> (0))==sizeof(char)) \
                  ? (1ul << pm_##name##Pixel) : 0ul)
        DefinePixelMethods(MakeTest)
        #undef MakeTest
        ;

        // Determine components
        // E.g. Component impl_First is set if the class is or inherits FirstPixel.
        static const unsigned Components = 0
        #define MakeTest(name) \
             | ((sizeof(PixelMetaInfoAux::ctest##name( (const T*) 0))==sizeof(char)) \
                  ? (1u << impl_##name) : 0u)
        DefinePixelImpls(MakeTest)
        #undef MakeTest
        ;

        // Determine size and cost
        static const unsigned Size = sizeof(T);
        static const unsigned Cost = Size + T::SizePenalty;
    };
    template<>
    struct PixelMetaInfo<void>
    {
        typedef void result;
        static const unsigned long Traits = 0ul;
        static const unsigned Size        = 0xffffu;
        static const unsigned Cost        = 0xffffu;
        static const unsigned Components  = ~0u;
    };
    #define MakeImpl(name) \
        template<> struct PixelMethodClass<impl_##name>\
                : public PixelMetaInfo<name##Pixel> { };\
        template<> struct PixelMethodImplName<name##Pixel> \
            {  static const char* getname() { return #name; } };
    DefinePixelImpls(MakeImpl)
    #undef MakeImpl

    /* Function to call a method if it is exists */

    #define MakeMethodCaller(n,f,name) \
    template<typename T, \
             bool HasMethod = PixelMetaInfo<T>::Traits & (1u << pm_##name##Pixel)> \
    struct CallGet##name##Helper \
    { \
        static uint32 call(const T& obj, unsigned timer=0) FasterPixelMethod \
        { \
            return obj.Get##name(timer); \
        } \
    }; \
    template<typename T> \
    struct CallGet##name##Helper<T,false> \
    { \
        /* If the method does not exist, use default method. */ \
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

    /* Utility */

    template<unsigned Value, unsigned Basevalue=0, bool found = !Value || (Value&1)>
    struct GetLowestBit : public GetLowestBit<Value/2, Basevalue+1> { };
    template<unsigned Value, unsigned Basevalue>
    struct GetLowestBit<Value, Basevalue, true> { enum { result = Basevalue }; };

    template<unsigned value, unsigned alreadycounted = 0>
    struct PopCount : public PopCount< value/2, alreadycounted+(value&1) > { };
    template<unsigned alreadycounted>
    struct PopCount<0, alreadycounted> { static const unsigned result = alreadycounted; };

    template<typename T1, typename T2, bool select1st>
    struct ChooseType { typedef T2 result; };
    template<typename T1, typename T2>
    struct ChooseType<T1,T2,true> { typedef T1 result; };

    /* Combine implementations */
    template<typename T1,typename T2>
    struct And: public T1, public T2
    {
        void set(uint32 p, unsigned timer=0) FastPixelMethod
        {
            T1::set(p,timer);
            T2::set(p,timer);
        }
        /* Legal combination of two classes */
        static const unsigned SizePenalty = T1::SizePenalty + T2::SizePenalty;
    };

    /* This converts an implementation index into a type. */
    template<typename idclass,
             typename Base> // Two or more classes
    struct PixelMethodImpl: public
        // Special case: Don't combine T1 and T2,
        // when T1 already inherits T2 or any of T2's ancestors,
        //   or T2 already inherits T1 or any of T1's ancestors.
        // This avoids gcc warning about
        // "direct base inaccessible due to ambiguity".
        ChooseType<
            PixelMetaInfo<void>,
            PixelMetaInfo<And<Base, typename idclass::result> >,
            (PixelMetaInfo<Base>::Components &
             PixelMetaInfo<typename idclass::result>::Components) != 0
        >::result
    {
    };
    template<typename idclass> // Single class
    struct PixelMethodImpl<idclass, void>: public idclass
    {
    };

    /* Creates a combination class of pixel methods matching the requested bitmask. */
    /* The combination is created through chain-inheritance. */
    /* "PixelMethodImplementationCombination" would be somewhat wordy.
     * Hence abbreviated.
     */
    template<unsigned bitmask = 1>
    struct PixelMethodImplComb: public
        PixelMethodImpl<
            PixelMethodClass<GetLowestBit<bitmask>::result>,
            typename PixelMethodImplComb<bitmask & (bitmask-1)>::result>
            // bitmask & (bitmask-1) clears the
            // least significant bit of the mask.
    {
    };
    template<>
    struct PixelMethodImplComb<0>: public PixelMetaInfo<void>
    {
    };

    /* This converts a composite type into a name (string). */
    template<typename T1, typename T2>
    struct PixelMethodImplName< And<T1,T2> >
    {
        static const char* getname()
        {
            static std::string name (
                PixelMethodImplName<T1>::getname()
                + std::string("+") +
                PixelMethodImplName<T2>::getname() );
            return name.c_str();
        }
    };

    /* Same as PixelMethodImplComb, but given a Traits value,
     * either picks the PixelMethodImplComb matching given bitmask,
     * or escalates to next bitmask, until an implementation
     * that has the requested Traits is found.
     * Using this class in BestPixelMethodImplComb_ForTraits
     * speeds up the compilation by some 50%.
     */
    template<unsigned long Traits,unsigned bm=1>
    struct NextPixelMethodImplComb_WithTraits
    {
        static const unsigned bitmask =
            (Traits & ~PixelMethodImplComb<bm>::Traits)
            ? // Escalate
              NextPixelMethodImplComb_WithTraits<Traits, bm+1>::bitmask
            : // Found
              bm; // matched traits.
    };
    template<unsigned long Traits>
    struct NextPixelMethodImplComb_WithTraits<Traits, (1ul<<NPixelImpls)>
    {   // Default case, ensures termination
        static const unsigned bitmask = 0;
    };

    /* Picks the smallest PixelMethodImplComb that fulfills the requested Traits. */
    template<unsigned long Traits,
             typename DType = PixelMethodImplComb<0>,
             unsigned bitmask = NextPixelMethodImplComb_WithTraits<Traits>::bitmask
            >
    struct BestPixelMethodImplComb_ForTraits
    {
        typedef PixelMethodImplComb<bitmask> CType;
        static const bool ChooseC =
           // !(Traits & ~CType::Traits) && // ensured by NextPixelMethod
           ((CType::Cost < DType::Cost
             // Because users may complain if First+MostUsed gives
             // a type of First+Last+MostUsed, even though both are
             // 24 bytes on x86_64, do extra work to get the type that
             // is more fundamental (shorter chain of inheritances).
             // Also, it will give a faster set() method.
          || (CType::Cost == DType::Cost && PopCount<CType::Components>::result
                                          < PopCount<DType::Components>::result)
           ));
        // Escalate.
        typedef typename BestPixelMethodImplComb_ForTraits<
            Traits,
            typename ChooseType<CType, DType, ChooseC>::result,
            NextPixelMethodImplComb_WithTraits<Traits, bitmask+1>::bitmask
            >::result result;
    };
    template<unsigned long Traits,typename T>
    struct BestPixelMethodImplComb_ForTraits<Traits,T, 0>
    {
        typedef T result;
    };

    /* Actual framework for constructing the pixels */

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
    struct FactoryMethods
    {
        typedef Array256x256of_Base ObjT;
        typedef Array256x256of<T> ResT;
        static ObjT* Construct()         { return new ResT; }
        static ObjT* Copy(const ObjT& b) { return new ResT ( (const ResT&) b ) ; }
        static void Assign(ObjT& tgt, const ObjT& b) { (ResT&) tgt = (const ResT&) b; }
    };
    /*template<>
    struct FactoryMethods<void>
    {
        static Array256x256of_Base* Construct() { return 0; }
        static Array256x256of_Base* Copy(const Array256x256of_Base&) { return 0; }
        static void Assign(Array256x256of_Base&, const Array256x256of_Base&) { }
    };*/
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
        PixelMetaInfo<T>::Size,
        PixelMetaInfo<T>::Cost - PixelMetaInfo<T>::Size
    };

    typedef
        BestPixelMethodImplComb_ForTraits< ((1ul<<NPixelMethods)-1) >
        ::result::result EverythingImplementer;

    template<unsigned bitmask=1u>
    struct FactoryFinder
    {
        static inline const FactoryType* FindForTraits
            (unsigned long req_traits,
             unsigned result_cost = PixelMetaInfo<EverythingImplementer>::Cost,
             const FactoryType* result =
                &PixelImplCombFactory<EverythingImplementer>::data
            ) FastPixelMethod
        {
            typedef typename BestPixelMethodImplComb_ForTraits
                <PixelMethodImplComb<bitmask>::Traits>::result imprfinder;

            /* Using BestPixelMethodImplComb_ForTraits here avoids
             * having to _instantiate_ all the different types.
             * Only those types that end up being returned here, are
             * instantiated. If it weren't for this, this duplicate
             * functionality would be redundant.
             */

            //if(  (imprfinder::Traits & req_traits) == req_traits)
            if( !( (imprfinder::Traits ^ req_traits) & req_traits))
            //if( ! (req_traits & ~imprfinder::Traits))
            {
                if(imprfinder::Cost < result_cost)
                {
                    result      = &PixelImplCombFactory
                        <typename imprfinder::result>::data;
                    result_cost = imprfinder::Cost;
                }
            }
            return FactoryFinder<bitmask+1>::FindForTraits(req_traits, result_cost, result);
        }
    };

    template<>
    struct FactoryFinder< (1u<<NPixelImpls) >
    {
        static inline const FactoryType* FindForTraits
            (unsigned long,unsigned, const FactoryType* result) FasterPixelMethod
        {
            return result;
        }
    };

    const FactoryType* Get256x256pixelFactory()
    {
        unsigned long Traits = pixelmethods_result
                            | (1ul << bgmethod)
                            | (1ul << bgmethod0)
                            | (1ul << bgmethod1);

        static unsigned long Prev = ~0ul;
        static const FactoryType* cache = 0;

        if(Prev == Traits) return cache;
        return cache = FactoryFinder<>::FindForTraits(Prev = Traits);
    };
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

void Array256x256of_Base::GetStaticSectionInto
    (uint32* target, unsigned target_stride,
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
            target[p++] = GetStatic(index+x);
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
            if( pix >= 0x7F000000 ) continue;
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
        unsigned width, unsigned height) const FastPixelMethod
    {
        const T* databegin = data      + (y1*256+x1);
        const T* dataend   = databegin + height*256;
    #if DO_VERY_SPECIALIZED>0
        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: \
                if(T::Traits & (1ul<<pm_##name##Pixel)) \
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

    virtual void GetStaticSectionInto(
        uint32* target, unsigned target_stride,
        unsigned x1, unsigned y1,
        unsigned width, unsigned height) const FastPixelMethod
    {
        const T* databegin = data      + (y1*256+x1);
        const T* dataend   = databegin + height*256;
    #if DO_VERY_SPECIALIZED>0
        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: \
                if(T::Traits & (1ul<<pm_##name##Pixel)) \
                    for(; databegin<dataend; \
                           target += target_stride-width, \
                           databegin += 256-width) \
                        for(unsigned x=width; x-->0; ) \
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
        method=method;
        for(; databegin<dataend;
               target += target_stride-width,
               databegin += 256-width)
            for(unsigned x=width; x-->0; )
                *target++ = databegin++->get(0);
    #endif
    }

    virtual void GetStaticInto(
        uint32* target, unsigned target_stride) const FastPixelMethod
    {
        const T* databegin = data;
        const T* dataend   = databegin + 256*256;
    #if DO_VERY_SPECIALIZED>0
        #define MakeMethodCase(n,f,name) \
            case pm_##name##Pixel: \
                if(T::Traits & (1ul<<pm_##name##Pixel)) \
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
        unsigned width, unsigned height) FastPixelMethod
    {
        unsigned p=0, index=y1*256+x1, maxindex=(y1+height)*256+x1;
        for(; index<maxindex; p+=target_stride, index+=256)
            for(unsigned x=0; x<width; ++x)
            {
                uint32 pix = source[p+x];
                if( pix >= 0x7F000000 ) continue;
                /* Do not plot completely transparent pixels */
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
    virtual uint32 GetStatic(unsigned index) const FastPixelMethod
    {
        return DoGetLive(bgmethod, index, 0);
    }
#endif

    virtual uint32 GetLive(PixelMethod method, unsigned index, unsigned timer) const FastPixelMethod
    {
        return DoGetLive(method, index, timer);
    }

    virtual void Set(unsigned index, uint32 p, unsigned timer) FastPixelMethod
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
                if(!(PixelMetaInfo<T>::Traits & (1u << pm_##name##Pixel))) break; \
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
        switch((PixelMethod) GetLowestBit<PixelMetaInfo<T>::Traits>::result)
        {
            DefinePixelMethods(MakeDefaultCase);
            default: return DefaultPixel;
        }
        #undef MakeDefaultCase
    }
};

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
