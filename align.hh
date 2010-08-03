#ifndef bqtTileTrackerAlignHH
#define bqtTileTrackerAlignHH

#include <vector>
#include "pixel.hh" // For uint32 and uint64

/* A two-dimensional coordinate */
struct IntCoordinate
{
    int x, y;
    bool operator< (const IntCoordinate& b) const
    {
        if(x != b.x) return x < b.x;
        return y < b.y;
    }
    bool operator== (const IntCoordinate& b) const
    {
        return x == b.x && y == b.y;
    }
};

/* SpotType describes the contents of a particular
 * location in the image. See the implementation
 * of FindInterestingSpots() on how it works.
 */
typedef std::pair<uint64,uint64> SpotType;
struct InterestingSpot
{
    IntCoordinate   where;
    SpotType        data;
};

/* Find points of interest from the input,
 * and insert them into the given vector.
 *    output:      Target vector to store the points in
 *    input:       Input bitmap
 *    sx,sy:       Input bitmap dimensions
 *    xoffs,yoffs:
 *        Global coordinates corresponding to input bitmap's (0,0)
 *        This tells where in the global map is the input bitmap located.
 *    force_all_pixels:
 *        true = this is input picture, false = this is background picture
 */
void FindInterestingSpots(
    VecType<InterestingSpot>& output,
    const uint32* input,
    int xoffs, int yoffs,
    unsigned sx, unsigned sy,
    bool force_all_pixels);

/* Attempts to align the input picture into the background picture
 *    input_spots:
 *        Points of interest concerning the input picture
 *        As collected by FindInterestingSpots()
 *    reference_spots:
 *        Points of interest concerning the background picture
 *        As collected by FindInterestingSpots()
 *    org_x, org_y:
 *        Offset of the previous frame within the background picture
 *        This is used for optimization: It is assumed that the next
 *        frame is likely somewhere near the previous frame.
 * Result:
 *    offs_x, offs_y:
 *        Suggested placement of the input picture
 *        within the background picture
 *    suspect_reset:
 *        Set to true if there's a reason to suspect
 *        that the images do not quite align right
 */
struct AlignResult Align(
    const VecType<InterestingSpot>& input_spots,
    const VecType<InterestingSpot>& reference_spots,
    int org_x,
    int org_y);
struct AlignResult
{
    int  offs_x;
    int  offs_y;
    bool suspect_reset; // Scene change alert or scene shift alert
};


/* These parameters control the aligner. */
extern unsigned x_divide_input;
extern unsigned y_divide_input;
extern unsigned x_divide_reference;
extern unsigned y_divide_reference;

extern int mv_xmin, mv_ymin, mv_xmax, mv_ymax;

#endif
