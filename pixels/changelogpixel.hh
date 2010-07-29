//#include "pixels/mostusedpixel.hh"
#include "maptype.hh"

/* undef or define. */
static const bool CHANGELOG_GUESS_OUTSIDES = true;

#include <vector>
#include <algorithm>

class ChangeLogPixel
{
    MapType<unsigned, uint32> history;
    MostUsedPixel most_used;

public:
    ChangeLogPixel() : history(), most_used()
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set(p);
    }
    void set(uint32 p)
    {
        /*if(CurrentTimer == 0)
        {
            // Ignore first frame. It's gray.
            return;
        }*/
        most_used.set(p);
        // Store the value into the history.
        // However, do not store three consecutive identical values.
        // Only store the first timer value where it occurs,
        // and the last timer value where it occurs.
        MapType<unsigned, uint32>::iterator
            i = history.lower_bound(CurrentTimer);
        if(i != history.end() && i->first == CurrentTimer)
        {
            // Redefining what happened at [CurrentTimer]
            i->second = p;
            return;
        }

        //bool at_begin = i == history.begin();
        //bool at_end   = i == history.end();

        /*
            |=edge
            ?=anything
            S=same
            D=different
            ^=location

             SSS    ignore
              ^

            SSDS    update timestamp of ^-1
              ^

             SDSS   update timestamp of ^...
              ^

            anything else: insert at ^+0

         Note: We can do none of this if we are going
               to support later insertions into middle
               of timestream.
        */

        history.insert(i,
            std::pair<unsigned, uint32> (CurrentTimer, p)
                      );
    }

    operator uint32() const
    {
        return Find(CurrentTimer);
    }

    void Compress()
    {
    }

private:
    uint32 Find(unsigned time) const
    {
        // Find the pixel value that was present at the given time.
        /*
          map::lower_bound:
            Returns an iterator pointing to first element >= key, or end().
          map::upper_bound:
            Returns an iterator pointing to first element > key, or end().

          What we want is an iterator pointing
            to the last element that is <= key.
         */
        MapType<unsigned, uint32>::const_iterator
            i = history.upper_bound(time);

        /* Pre-begin value: reasonable default */
        if(i == history.begin())
        {
            return CHANGELOG_GUESS_OUTSIDES
                ? most_used
                : DefaultPixel;
        }

        /* Post-end value: Special handling */
        bool last = (i == history.end());

        /* Anything else. Take the value. */
        --i;
        if(i->first < time && last)
        {
            return CHANGELOG_GUESS_OUTSIDES
                ? most_used
                : DefaultPixel;
        }
        return i->second;
    }
};
