//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"
#include "maptype.hh"

/* undef or define. */
static const bool CHANGELOG_GUESS_OUTSIDES = true;

#define CHANGELOG_USE_LASTTIMESTAMP 0

class ChangeLogPixel
{
    MapType<unsigned, uint32> history;
    MostUsedPixel most_used;
#if CHANGELOG_USE_LASTTIMESTAMP
    unsigned last_time;
#endif

public:
    ChangeLogPixel() : history(), most_used()
#if CHANGELOG_USE_LASTTIMESTAMP
                        , last_time(0)
#endif
    {
    }
    void set(unsigned R,unsigned G,unsigned B, unsigned timer) FastPixelMethod
    {
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set(p, timer);
    }
    void set(uint32 p, unsigned timer) FastPixelMethod
    {
        /*if(timer == 0)
        {
            // Ignore first frame. It's gray.
            return;
        }*/
        most_used.set(p);
#if CHANGELOG_USE_LASTTIMESTAMP
        if(timer > last_time) last_time = timer;
#endif
        // Store the value into the history.
        // However, do not store three consecutive identical values.
        // Only store the first timer value where it occurs,
        // and the last timer value where it occurs.
        MapType<unsigned, uint32>::iterator
            i = history.lower_bound(timer);
        if(i != history.end() && i->first == timer)
        {
            // Redefining what happened at [timer]
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
        if(i != history.begin())
        {
            MapType<unsigned, uint32>::iterator prev1(i);
            --prev1;
#if CHANGELOG_USE_LASTTIMESTAMP
            if(prev1->second == p)
            {
                // We've got a repeat.
                return; // Ignore repeating value
            }
#else
            if(prev1->second == p)
            {
                // We've got a repeat.
                if(prev1 != history.begin())
                {
                    MapType<unsigned, uint32>::iterator prev2(prev1);
                    --prev2;
                    if(prev1->second == prev2->second)
                    {
                        // Preceding two are duplicates. Here's a third.
                        // Do not insert the third one. Instead, update
                        // the second duplicate's timestamp to current's.
                        prev1->first = timer;
                        return;
                    }
                }
                // Previous two weren't duplicates.
                // Only the previous one was. Add a second one, so
                // that we know how long the duplicateness lasts.
            }
            else
            {
                if(prev1 != history.begin())
                {
                    MapType<unsigned, uint32>::iterator prev2(prev1);
                    --prev2;
                    if(prev1->second == prev2->second)
                    {
                        // Preceding two are duplicates; remove
                        // the latter, for it is redundant.
                        history.erase(prev1);
                        i = prev2; ++i;
                    }
                }
            }
#endif
        }

        history.insert(i,
            std::pair<unsigned, uint32> (timer, p)
                      );
    }

    inline uint32 get(unsigned timer) const FasterPixelMethod
    {
        return Find(timer);
    }
    inline uint32 GetMostUsed() const FasterPixelMethod
    {
        return most_used.get();
    }

    inline uint32 GetLast() const FastPixelMethod
    {
        return history.empty() ? DefaultPixel : history.rbegin()->second;
    }

    inline uint32 GetAverage() const FasterPixelMethod
    {
        return most_used.GetAverage();
    }

    void Compress()
    {
        most_used.Compress();
    }

private:
    uint32 Find(unsigned time) const FastPixelMethod
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
                ? most_used.get()
                : DefaultPixel;
        }

        /* Post-end value: Special handling */
        bool last = (i == history.end());

        /* Anything else. Take the value. */
        --i;
        if(i->first < time && last
#if CHANGELOG_USE_LASTTIMESTAMP
                                   && time > last_time
#endif
          )
        {
            return CHANGELOG_GUESS_OUTSIDES
                ? most_used.get()
                : DefaultPixel;
        }
        return i->second;
    }
};

template<>
class TwoPixels<ChangeLogPixel, MostUsedPixel>: private ChangeLogPixel
{
public:
    using ChangeLogPixel::set;
    using ChangeLogPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned)       const FasterPixelMethod { return GetMostUsed(); }
};

template<>
class TwoPixels<ChangeLogPixel, LastPixel>: private ChangeLogPixel
{
public:
    using ChangeLogPixel::set;
    using ChangeLogPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned)       const FasterPixelMethod { return GetLast(); }
};

template<>
class TwoPixels<ChangeLogPixel, AveragePixel>: private ChangeLogPixel
{
public:
    using ChangeLogPixel::set;
    using ChangeLogPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned)       const FasterPixelMethod { return GetAverage(); }
};

template<>
class TwoPixels<MostUsedPixel, ChangeLogPixel>
    : public SwapTwoPixels<ChangeLogPixel, MostUsedPixel>
{
};

template<>
class TwoPixels<LastPixel, ChangeLogPixel>
    : public SwapTwoPixels<ChangeLogPixel, LastPixel>
{
};

template<>
class TwoPixels<AveragePixel, ChangeLogPixel>
    : public SwapTwoPixels<ChangeLogPixel, AveragePixel>
{
};
