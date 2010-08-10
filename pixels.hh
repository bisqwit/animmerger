#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"

/* List of pixel classes. Only list those pixel
 * classes that have an unique set of Traits.
 * Sort them in the order of preference-of-use
 * when multiple classes implement the same method.
 */
#define DefinePixelClasses(callback) \
    callback(LastPixel) \
    callback(FirstPixel) \
    callback(AveragePixel) \
    callback(MostUsedPixel) \
    callback(LoopingLogPixel) \
    callback(ChangeLogPixel)
