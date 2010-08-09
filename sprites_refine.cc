#include "openmp.hh"
#include "sprites.hh"

/* In any average animation, sometimes actors collide.
 * They shoot projectiles, in which case one actor splits into two.
 * They pass each others, in which case two actors become one, or
 * one becomes two, or both.
 * The job of this function is to find out which DifferingSections
 * refer to the same actor, and to ignore those DifferingSections
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
void SpriteLore::FindDistinctActors()
{
}
