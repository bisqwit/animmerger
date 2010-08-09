#ifndef bqtTileTrackerSpritesHH
#define bqtTileTrackerSpritesHH

#include "types.hh"
#include "vectype.hh"
#include "settype.hh"
#include "maptype.hh"

struct BoundingBox
{
    unsigned x1,y1, wid,hei;

    bool operator< (const BoundingBox& b) const;
    bool operator== (const BoundingBox& b) const;
    bool Overlaps(const BoundingBox& b) const;
};

typedef VecType<BoundingBox> BoundingBoxListType;

/* Generate a set of rectangles that expresses all
 * differing pixels between the two images.
 */
BoundingBoxListType FindDifferences
    (const VecType<uint32>& background,
     const VecType<uint32>& screen,
     unsigned width);

/* In any average animation, sometimes actors collide.
 * They shoot projectiles, in which case one actor splits into two.
 * They pass each others, in which case two actors become one, or
 * one becomes two, or both.
 * The job of this function is to find out which BoundingBoxes
 * refer to the same actor, and to ignore those BoundingBoxes
 * that are ambiguous.
 * Note: Sometimes actors move on the screen at such intervals
 * that the bounding boxes in adjacent frames are disconnected.
 *
 * TODO: Later phase:
 * There is also the possibility that an actor is partially visible
 * (emerging from screen edges or from behind an object); one should
 * reconstruct the actor in those cases.
 *
 * TODO: Later phase:
 * An actor may also sometimes become partially transparent. This
 * may happen if it shares background colors. One should attempt
 * to merge the individual frames by checking if one pose
 * can be overlayed over another pose without conflicts.
 */
struct SpriteLore
{
    // Raw data:
    VecType<BoundingBoxListType> DifferencesEachFrame;

    // Refined data:
    typedef MapType<unsigned/*frame number*/, BoundingBox> ActorData;
    MapType<unsigned/*actorid*/, ActorData> Actors;

    // Refinement method
    void FindDistinctActors();
};

#endif
