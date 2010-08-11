#ifndef bqtTileTrackerPixelHH
#define bqtTileTrackerPixelHH

#include "types.hh"

/* This is the definite list of available pixel methods. */
/* The order is arbitrary, but chosen so as to minimize
 * the size of the generated FindFactory() function.
 * Params: option flag, animation flags, name
 * animation flags & 1 = animated
 * animation flags & 2 = obeys looplength
 * animation flags & 4 = obeys blur
 * animation flags & 8 = obeys firstlastlength
 */
#define DefinePixelMethods(callback) \
    callback(f,0,First) \
    callback(l,0,Last) \
    callback(o,3,LoopingLog) \
    callback(a,0,Average) \
    callback(A,0,TinyAverage) \
    callback(m,0,MostUsed) \
    callback(e,0,LeastUsed) \
    callback(t,0,ActionAvg) \
    callback(c,5,ChangeLog) \
    callback(v,3,LoopingAvg) \
    callback(s,3,LoopingLast) \
    callback(F,8,FirstNMost) \
    callback(L,8,LastNMost)

/* Create it as an enum */
#define MakeEnum(o,f,name) pm_##name##Pixel,
enum PixelMethod
{
    DefinePixelMethods(MakeEnum)
    NPixelMethods
};
#undef MakeEnum


extern bool OptimizeChangeLog;
extern unsigned AnimationBlurLength;
extern unsigned LoopingLogLength;
extern int FirstLastLength;


extern unsigned long pixelmethods_result;
extern PixelMethod bgmethod;

#define MakeMask(o,flags,name) ((flags&1) ? (1ul << pm_##name##Pixel) : 0) |
static const unsigned long AnimatedPixelMethodsMask =
    DefinePixelMethods(MakeMask)
0;
#undef MakeMask
#define MakeMask(o,flags,name) ((flags&2) ? (1ul << pm_##name##Pixel) : 0) |
static const unsigned long LoopingPixelMethodsMask =
    DefinePixelMethods(MakeMask)
0;
#undef MakeMask
#define MakeMask(o,flags,name) ((flags&4) ? (1ul << pm_##name##Pixel) : 0) |
static const unsigned long BlurCapablePixelMethodsMask =
    DefinePixelMethods(MakeMask)
0;
#undef MakeMask
#define MakeMask(o,flags,name) ((flags&8) ? (1ul << pm_##name##Pixel) : 0) |
static const unsigned long FirstLastPixelMethodsMask =
    DefinePixelMethods(MakeMask)
0;
#undef MakeMask


#ifdef __GNUC__
# define FastPixelMethod __attribute__((regparm(3)))
# define FasterPixelMethod __attribute__((regparm(3),always_inline))
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

    virtual uint32 GetStatic(unsigned index) const = 0;

    virtual void Set(unsigned index, uint32 p, unsigned timer) = 0;

    virtual unsigned GetPixelSize() const = 0;
    virtual unsigned GetPixelSizePenalty() const = 0;
    virtual const char* GetPixelSetupName() const = 0;
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
        if(data) func(*data);
    }

    // Test whether vector is empty (uninitialized)
    bool empty() const { return !data; }

private:
    Array256x256of_Base* data;
};

unsigned GetPixelSizeInBytes();
unsigned GetPixelSizePenaltyInBytes();
const char* GetPixelSetupName();

#endif
