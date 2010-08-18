#ifndef bqtTileTrackerAlignHH
#define bqtTileTrackerAlignHH

#include <vector>
#include "types.hh"

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

#if 0
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
    std::vector<InterestingSpot>& output,
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
    const std::vector<InterestingSpot>& input_spots,
    const std::vector<InterestingSpot>& reference_spots,
    int org_x,
    int org_y);
#endif

/* Attempts to align the input picture into the background picture
 *    input, inputwidth, inputheight:
 *        Input bitmap (rgb32 array, width, height)
 *        May contain holes, indicated by (pixel value >> 24) != 0
 *    background, backwidth, backheight:
 *        Background bitmap (rgb32 array, width, height)
 *        May contain holes, indicated by (pixel value >> 24) != 0
 *    org_x, org_y:
 *        Upper-left corner offset relative to the background
 *        picture where the previous frame was placed.
 *        This can be used for optimization: It is usually likely
 *        that the next frame will be located a short distance
 *        away from the previous frame.
 * Pixel order is like this (for a bitmap where width=4, height=2):
 *    0123     upper-left coordinate is 0,0
 *    4567     bottom-right coordinate is 3,1
 * Result:
 *    offs_x, offs_y:
 *        Suggested placement of the input picture
 *        within the background picture
 *    suspect_reset:
 *        Set to true if there's a reason to suspect
 *        that the images do not quite align right
 */
struct AlignResult Align(
    const uint32* background,
    unsigned backwidth, unsigned backheight,
    const uint32* input,
    unsigned inputwidth, unsigned inputheight,
    unsigned org_x,
    unsigned org_y);

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
