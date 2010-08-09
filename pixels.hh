#ifdef __GNUC__
# define FastPixelMethod __attribute__((regparm(6)))
# define FasterPixelMethod __attribute__((regparm(6),always_inline))
#else
# define FastPixelMethod
# define FasterPixelMethod
#endif

#include "pixels/lastpixel.hh"
#include "pixels/firstpixel.hh"
#include "pixels/averagepixel.hh"
#include "pixels/mostusedpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"
// LeastUsedPixel is defined in MostUsedPixel
// ActionAvgPixel is defined in MostUsedPixel
// LoopingAvgPixel is defined in ChangeLogPixel
// LoopingLastPixel is defined in ChangeLogPixel
// LastNPixel is defined in ChangeLogPixel
// FirstNPixel is defined in ChangeLogPixel
// In addition, ChangeLog can emulate all of these.
