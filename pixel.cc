#include "types.hh"

bool     OptimizeChangeLog   = true;
unsigned AnimationBlurLength = 0;
unsigned LoopingLogLength    = 16;
int      FirstLastLength     = 16;

#include "pixel.hh"
#include "pixels.hh"

unsigned long pixelmethods_result = 1ul << pm_MostUsedPixel;
enum PixelMethod bgmethod = pm_MostUsedPixel;

namespace
{
/*
    template<typename T>
    struct PixelMethodImpls
    {
        static uint32 (T::*const methods[NPixelMethods])(unsigned)const;
    };
    template<typename T>
    uint32 (T::*const PixelMethodImpls<T>::methods[NPixelMethods])(unsigned)const =
    {
        #define MakeMethodPointer(n,f,name) &T::Get##name,
        DefinePixelMethods(MakeMethodPointer)
        #undef MakeMethodPointer
    };
*/
}

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

        //return (data[index].*(PixelMethodImpls<T>::methods[method]))(timer);
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

        //return (data[index].*(PixelMethodImpls<T>::methods[bgmethod]))(0);
    }

    virtual void Set(unsigned index, uint32 p, unsigned timer)
    {
        data[index].set(p, timer);
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
    template<unsigned impl_index>
    struct PixelImplFactory
    {
        typedef typename PixelMethodImplComb<impl_index>::result T;
        static const FactoryType data;
    };
    template<unsigned impl_index>
    const FactoryType PixelImplFactory<impl_index>::data =
    {
        FactoryMethods<T>::Construct,
        FactoryMethods<T>::Copy,
        FactoryMethods<T>::Assign,
        PixelMethodImplName<T>::getname,
        sizeof(T),
        T::SizePenalty
    };

#include "pixelfactory.inc"

    const FactoryType* Get256x256pixelFactory()
    {
        return FindFactory( pixelmethods_result | (1ul << bgmethod) );
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
