#ifndef bqtTileTrackerPixelHH
#define bqtTileTrackerPixelHH

#include "types.hh"

/* This is the definite list of available pixel methods. */
/* The order is completely arbitrary and irrelevant.
 * Params: option flag, animation flags, name
 * animation flags & 1 = animated
 * animation flags & 2 = obeys looplength
 * animation flags & 4 = obeys blur
 * animation flags & 8 = obeys firstlastlength
 * animation flags &16 = obeys yuv flag
 */
#define DefinePixelMethods(callback) \
    callback(f,0x00,First) \
    callback(l,0x00,Last) \
    callback(F,0x08,FirstNMost) \
    callback(L,0x08,LastNMost) \
    callback(a,0x10,Average) \
    callback(A,0x00,TinyAverage) \
    callback(t,0x10,ActionAvg) \
    callback(m,0x00,MostUsed) \
    callback(e,0x00,LeastUsed) \
    callback(c,0x05,ChangeLog) \
    callback(v,0x17,LoopingAvg) \
    callback(o,0x07,LoopingLog)

/* Create it as an enum */
#define MakeEnum(o,f,name) pm_##name##Pixel,
enum PixelMethod { DefinePixelMethods(MakeEnum) };
#undef MakeEnum

/* Count them into NPixelMethods */
#define CountMethods(o,f,name) +1
enum { NPixelMethods = 0 DefinePixelMethods(CountMethods) };
#undef CountMethods


extern bool OptimizeChangeLog;
extern unsigned AnimationBlurLength;
extern unsigned LoopingLogLength;
extern int FirstLastLength;
extern bool AveragesInYUV;

extern unsigned long pixelmethods_result;
extern PixelMethod bgmethod;

#define MakeMask(o,flags,name) | ((flags&1) ? (1ul << pm_##name##Pixel) : 0)
static const unsigned long AnimatedPixelMethodsMask = 0 DefinePixelMethods(MakeMask);
#undef MakeMask
#define MakeMask(o,flags,name) | ((flags&2) ? (1ul << pm_##name##Pixel) : 0)
static const unsigned long LoopingPixelMethodsMask = 0 DefinePixelMethods(MakeMask);
#undef MakeMask
#define MakeMask(o,flags,name) | ((flags&4) ? (1ul << pm_##name##Pixel) : 0)
static const unsigned long BlurCapablePixelMethodsMask = 0 DefinePixelMethods(MakeMask);
#undef MakeMask
#define MakeMask(o,flags,name) | ((flags&8) ? (1ul << pm_##name##Pixel) : 0)
static const unsigned long FirstLastPixelMethodsMask = 0 DefinePixelMethods(MakeMask);
#undef MakeMask
#define MakeMask(o,flags,name) | ((flags&16) ? (1ul << pm_##name##Pixel) : 0)
static const unsigned long YUVCapablePixelMethodsMask = 0 DefinePixelMethods(MakeMask);
#undef MakeMask


#ifdef __GNUC__
# define FastPixelMethod __attribute__((regparm(3),optimize("O3,omit-frame-pointer")))
# define FasterPixelMethod FastPixelMethod __attribute__((always_inline))
#else
# define FastPixelMethod
# define FasterPixelMethod
#endif


/* A vector of 256x256 pixels. */
/* Each pixel has two traits:
 * the trait determined by pixelmethod (retrievable with GetLive()),
 * and the trait determined by bgmethod (retrievable with GetStatic()).
 */
struct Array256x256of_Base
{
    virtual ~Array256x256of_Base() { }

    virtual uint32 GetLive(PixelMethod method, unsigned index, unsigned timer) const FastPixelMethod = 0;

    virtual uint32 GetStatic(unsigned index) const FastPixelMethod;

    virtual void Set(unsigned index, uint32 p, unsigned timer) FastPixelMethod = 0;

    virtual void GetStaticInto(uint32* target, unsigned target_stride) const FastPixelMethod;

    virtual void GetLiveSectionInto
        (PixelMethod method, unsigned timer,
        uint32* target, unsigned target_stride,
        unsigned x1, unsigned y1,
        unsigned width, unsigned height) const FastPixelMethod;

    virtual void PutSectionInto
        (unsigned timer,
        const uint32* source, unsigned target_stride,
        unsigned x1, unsigned y1,
        unsigned width, unsigned height) FastPixelMethod;
};
class UncertainPixelVector256x256
{
public:
    // By default, the vector is uninitialized (empty)
    UncertainPixelVector256x256() : data(0)
    {
    }

    // Init: Resize the vector to 256x256 elements
    void init();

    // Copy constructor
    UncertainPixelVector256x256(const UncertainPixelVector256x256& b);

    // Assign operator (create copy of the other vector)
    UncertainPixelVector256x256& operator= (const UncertainPixelVector256x256& b);

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
        if(data) func(const_cast<const Array256x256of_Base&>(*data));
    }

    const Array256x256of_Base* operator->() const { return data; }
    Array256x256of_Base* operator->() { return data; }

    // Test whether vector is empty (uninitialized)
    bool empty() const { return !data; }

private:
    Array256x256of_Base* data;
};

unsigned GetPixelSizeInBytes();
unsigned GetPixelSizePenaltyInBytes();
const char* GetPixelSetupName();

#endif
