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
    callback(2,AveragePixel) \
    callback(3,MostUsedPixel) \
    callback(4,LoopingLogPixel) \
    callback(5,ChangeLogPixel)

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
    static const unsigned long Traits = 0;
};

#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"

#endif
