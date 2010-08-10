#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"

/* List of pixel classes. Only list those pixel
 * classes that have an unique set of Traits.
 */
#define DefinePixelClasses(callback) \
    callback(LastPixel) \
    callback(FirstPixel) \
    callback(AveragePixel) \
    callback(MostUsedPixel) \
    callback(ChangeLogPixel) \
    callback(LoopingLogPixel)
