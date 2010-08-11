#ifndef bqtTileTrackerPixelsHH
#define bqtTileTrackerPixelsHH

#include "pixel.hh"

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

template<unsigned id, typename Base>
struct PixelMethodImpl { typedef void result; };

template<typename T>
struct PixelMethodImplName { };

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

template<unsigned Value, unsigned Basevalue=0, bool found = !Value || (Value&1)>
struct GetLowestBit { enum { result = GetLowestBit<Value/2, Basevalue+1>::result }; };
template<unsigned Value, unsigned Basevalue>
struct GetLowestBit<Value, Basevalue, true> { enum { result = Basevalue }; };

/* Creates a combination class of pixel methods matching the requested bitmask. */
/* The combination is created through chain-inheritance. */
/* "PixelMethodImplementationCombination" would be somewhat wordy.
 * Hence abbreviated.
 */
template<unsigned bitmask,
         unsigned lowestbit = GetLowestBit<bitmask>::result,
         unsigned remainingbits = bitmask & ~(1ul << lowestbit)>
struct PixelMethodImplComb
    : public PixelMethodImpl
        <lowestbit,
         typename PixelMethodImplComb<remainingbits>::result>
{ };

template<>
struct PixelMethodImplComb<0> { typedef DummyPixel result; };

#endif
