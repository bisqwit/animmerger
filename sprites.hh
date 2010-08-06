#ifndef bqtTileTrackerSpritesHH
#define bqtTileTrackerSpritesHH

#include "types.hh"
#include "vectype.hh"

struct DifferingSection
{
    unsigned x1,y1, wid,hei;

    bool operator< (const DifferingSection& b) const;
    bool operator== (const DifferingSection& b) const;
    bool Overlaps(const DifferingSection& b) const;
};

typedef VecType<DifferingSection> DifferencesOnFrame;

/* Generate a set of rectangles that expresses all
 * differing pixels between the two images.
 */
DifferencesOnFrame FindDifferences
    (const VecType<uint32>& background,
     const VecType<uint32>& screen,
     unsigned width);

#endif
