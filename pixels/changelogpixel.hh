//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"
#include "maptype.hh"

/* undef or define. */
static const bool CHANGELOG_GUESS_OUTSIDES = true;

#define CHANGELOG_USE_LASTTIMESTAMP 0

class ChangeLogPixel
{
protected:
    MapType<unsigned, uint32> history;
#if CHANGELOG_USE_LASTTIMESTAMP
    unsigned last_time;
#endif

public:
    ChangeLogPixel() : history()
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
#if CHANGELOG_USE_LASTTIMESTAMP
        if(timer > last_time) last_time = timer;
#endif
        // Store the value into the history.
        // However, do not store three consecutive identical values.
        // Only store the first timer value where it occurs,
        // and the last timer value where it occurs.
        MapType<unsigned, uint32>::iterator i = history.end();
        // The most likely chance is that we're appending
        // to the end of the history. So check that case
        // before using lower_bound().
        if(i != history.begin())
        {
            --i;
            if(timer < i->first)
                i = history.lower_bound(timer);
            else if(i->first < timer)
                ++i;
        }
        else
            i = history.lower_bound(timer);

        if(i != history.end() && i->first == timer)
        {
            // Redefining what happened at [timer]
            i->second = p;
            return;
        }

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
        return GetChangeLog(timer);
    }

    uint32 GetChangeLog(unsigned timer) const FastPixelMethod
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
            i = history.upper_bound(timer);

        /* Pre-begin value: reasonable default */
        if(i == history.begin())
        {
            return CHANGELOG_GUESS_OUTSIDES
                ? GetMostUsed()
                : DefaultPixel;
        }

        /* Post-end value: Special handling */
        bool last = (i == history.end());

        /* Anything else. Take the value. */
        --i;
        if(i->first < timer && last
#if CHANGELOG_USE_LASTTIMESTAMP
                                   && timer > last_time
#endif
          )
        {
            return CHANGELOG_GUESS_OUTSIDES
                ? GetMostUsed()
                : DefaultPixel;
        }
        return i->second;
    }

    uint32 GetActionAvg(unsigned=0) const FastPixelMethod
    {
        const uint32 most = GetMostUsed();

        AveragePixel result;
        for(MapType<unsigned, uint32>::const_iterator
            i = history.begin();
            i != history.end();
            )
        {
            MapType<unsigned, uint32>::const_iterator j(i); ++i;
            unsigned duration =
                (i != history.end()) ? (i->first - j->first) :
#if CHANGELOG_USE_LASTTIMESTAMP
                    (last_time - j->first) + 1
#else
                    1
#endif
                    ;

            if(j->second != most)
                result.set_n(j->second, duration);
        }
        if(most != DefaultPixel) result.set_n(most, 1);
        uint32 res = result.get();
        return (res != DefaultPixel) ? res : most;
    }

    uint32 GetLoopingAvg(unsigned timer) const FastPixelMethod
    {
        unsigned offs = timer % LoopingLogLength;
        const uint32 most = GetMostUsed();

        AveragePixel result;
        for(MapType<unsigned, uint32>::const_iterator
            i = history.begin();
            i != history.end();
            )
        {
            MapType<unsigned, uint32>::const_iterator j(i); ++i;
            unsigned begin    = j->first;
            unsigned duration =
                (i != history.end()) ? (i->first - j->first) :
#if CHANGELOG_USE_LASTTIMESTAMP
                    (last_time - j->first) + 1
#else
                    1
#endif
                    ;

            if(j->second != most)
            {
                unsigned n_hits = 0;
                while(duration--)
                    if(begin++ % LoopingLogLength == offs)
                        ++n_hits;
                result.set_n(j->second, n_hits);
            }
        }
        uint32 res = result.get();
        return (res != DefaultPixel) ? res : most;
    }

    uint32 GetLoopingLast(unsigned timer) const FastPixelMethod
    {
        unsigned offs = timer % LoopingLogLength;
        const uint32 most = GetMostUsed();
        uint32 result = most;
        for(MapType<unsigned, uint32>::const_iterator
            i = history.begin();
            i != history.end();
            )
        {
            MapType<unsigned, uint32>::const_iterator j(i); ++i;
            unsigned begin    = j->first;
            unsigned duration =
                (i != history.end()) ? (i->first - j->first) :
#if CHANGELOG_USE_LASTTIMESTAMP
                    (last_time - j->first) + 1
#else
                    1
#endif
                    ;

            if(j->second != most)
            {
                while(duration--)
                    if(begin++ % LoopingLogLength == offs)
                        { result = j->second; break; }
            }
        }
        return result;
    }

    uint32 GetMostUsed(unsigned=0) const FastPixelMethod
    {
        MostUsedPixel result;
        for(MapType<unsigned, uint32>::const_iterator
            i = history.begin();
            i != history.end();
            )
        {
            MapType<unsigned, uint32>::const_iterator j(i); ++i;
            unsigned duration =
                (i != history.end()) ? (i->first - j->first) :
#if CHANGELOG_USE_LASTTIMESTAMP
                    (last_time - j->first) + 1
#else
                    1
#endif
                    ;
            result.set_n(j->second, duration);
        }
        return result.get();
    }

    inline uint32 GetLast(unsigned=0) const FastPixelMethod
    {
        return history.empty() ? DefaultPixel : history.rbegin()->second;
    }

    inline uint32 GetFirst(unsigned=0) const FastPixelMethod
    {
        return history.empty() ? DefaultPixel : history.begin()->second;
    }

    uint32 GetAverage(unsigned=0) const FastPixelMethod
    {
        AveragePixel result;
        for(MapType<unsigned, uint32>::const_iterator
            i = history.begin();
            i != history.end();
            )
        {
            MapType<unsigned, uint32>::const_iterator j(i); ++i;
            unsigned count =
                (i != history.end()) ? (i->first - j->first) :
#if CHANGELOG_USE_LASTTIMESTAMP
                    (last_time - j->first) + 1
#else
                    1
#endif
                    ;

            result.set_n(j->second, count);
        }
        return result.get();
    }
};

/*** CHANGELOG VARIANTS ***/

struct LoopingAvgPixel: public ChangeLogPixel
{
    inline uint32 get(unsigned timer) const FasterPixelMethod
    {
        return GetLoopingAvg(timer);
    }
};
struct LoopingLastPixel: public ChangeLogPixel
{
    inline uint32 get(unsigned timer) const FasterPixelMethod
    {
        return GetLoopingLast(timer);
    }
};

/*
ChangeLog defines these:

    GetChangeLog
    GetActionAvg   (UNIQUE, BUT ALSO IMPLEMENTED IN "MOSTUSED")
    GetLoopingAvg  (UNIQUE)
    GetLoopingLast (UNIQUE)
    GetMostUsed    (EMULATED, NOT UNIQUE)
    GetLast        (EMULATED, NOT UNIQUE)
    GetFirst       (EMULATED, NOT UNIQUE)
    GetAverage     (EMULATED, NOT UNIQUE)
*/

DefineBasePair(ChangeLogPixel, ChangeLog, ActionAvg)
DefineBasePair(ChangeLogPixel, ChangeLog, LoopingAvg)
DefineBasePair(ChangeLogPixel, ChangeLog, LoopingLast)
DefineBasePair(ChangeLogPixel, ChangeLog, MostUsed)
DefineBasePair(ChangeLogPixel, ChangeLog, Last)
DefineBasePair(ChangeLogPixel, ChangeLog, First)
DefineBasePair(ChangeLogPixel, ChangeLog, Average)

DefineBasePair(ChangeLogPixel, ActionAvg, LoopingAvg)
DefineBasePair(ChangeLogPixel, ActionAvg, LoopingLast)
//DefineBasePair(ChangeLogPixel, ActionAvg, MostUsed) -- used MostUsedPixel version instead
DefineBasePair(ChangeLogPixel, ActionAvg, Last)
DefineBasePair(ChangeLogPixel, ActionAvg, First)
//DefineBasePair(ChangeLogPixel, ActionAvg, Average) -- used MostUsedPixel version instead

DefineBasePair(ChangeLogPixel, LoopingAvg, LoopingLast)
DefineBasePair(ChangeLogPixel, LoopingAvg, MostUsed)
DefineBasePair(ChangeLogPixel, LoopingAvg, Last)
DefineBasePair(ChangeLogPixel, LoopingAvg, First)
DefineBasePair(ChangeLogPixel, LoopingAvg, Average)

DefineBasePair(ChangeLogPixel, LoopingLast, MostUsed)
DefineBasePair(ChangeLogPixel, LoopingLast, Last)
DefineBasePair(ChangeLogPixel, LoopingLast, First)
DefineBasePair(ChangeLogPixel, LoopingLast, Average)
