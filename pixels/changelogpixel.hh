//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"
#include "maptype.hh"

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

        if(i != history.begin() && OptimizeChangeLog)
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

    uint32 GetChangeLogOnly(unsigned timer) const FastPixelMethod
    {
        return Find(timer, DefaultPixel);
    }

    uint32 GetChangeLog(unsigned timer) const FastPixelMethod
    {
        if(AnimationBlurLength == 0) return Find(timer);

        const uint32 most = GetMostUsed();
        uint32 pix = Find(timer, most);

        if(pix != most) return pix;

        AveragePixel result;
        unsigned remaining_blur = AnimationBlurLength;
        result.set_n(pix, 1);

        for(; timer-- > 0 && remaining_blur-- > 0; )
        {
            pix = Find(timer, most);
            result.set_n(pix, 1);
            if(pix != most) break;
        }

        return result.get();
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

    template<typename SlaveType>
    uint32 GetAggregate(unsigned=0) const
    {
        SlaveType result;
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

    inline uint32 GetMostUsed(unsigned=0) const FastPixelMethod
    {
        return GetAggregate<MostUsedPixel> ();
    }
    inline uint32 GetLeastUsed(unsigned=0) const FastPixelMethod
    {
        return GetAggregate<LeastUsedPixel> ();
    }
    inline uint32 GetAverage(unsigned=0) const FastPixelMethod
    {
        return GetAggregate<AveragePixel> ();
    }
    inline uint32 GetLast(unsigned=0) const FastPixelMethod
    {
        return history.empty() ? DefaultPixel : history.rbegin()->second;
    }
    inline uint32 GetFirst(unsigned=0) const FastPixelMethod
    {
        return history.empty() ? DefaultPixel : history.begin()->second;
    }

    template<typename SlaveType>
    uint32 GetFirstNAggregate(unsigned n) const
    {
        const unsigned first_time = history.empty() ? 0 : history.begin()->first;
        const unsigned end_threshold = first_time + n;
        SlaveType result;
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
            if(begin >= end_threshold) break;
            unsigned n_hits = 0;
            while(duration--)
                if(begin++ < end_threshold)
                    ++n_hits;
            result.set_n(j->second, n_hits);
        }
        return result.get();
    }

    template<typename SlaveType>
    uint32 GetLastNAggregate(unsigned n) const
    {
#if !CHANGELOG_USE_LASTTIMESTAMP
        const unsigned last_time = history.empty() ? 0 : (history.rbegin()->first + 1);
#endif
        const unsigned begin_threshold = n < last_time ? last_time-n : 0;
        SlaveType result;
        MapType<unsigned, uint32>::const_iterator
            i ( history.upper_bound(begin_threshold) );
        if(i != history.begin()) --i;
        while(i != history.end())
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
            unsigned n_hits = 0;
            while(duration--)
                if(begin++ >= begin_threshold)
                    ++n_hits;
            result.set_n(j->second, n_hits);
        }
        return result.get();
    }

    uint32 GetFirstNMost(unsigned=0) const FastPixelMethod
    {
        if(FirstLastLength == 0) return GetFirstUncommon();
        if(FirstLastLength > 0)
            return GetFirstNAggregate<MostUsedPixel>(FirstLastLength);
        else
            return GetFirstNAggregate<LeastUsedPixel>(-FirstLastLength);
    }

    uint32 GetLastNMost(unsigned=0) const FastPixelMethod
    {
        if(FirstLastLength == 0) return GetLastUncommon();
        if(FirstLastLength > 0)
            return GetLastNAggregate<MostUsedPixel>(FirstLastLength);
        else
            return GetLastNAggregate<LeastUsedPixel>(-FirstLastLength);
    }

    uint32 GetFirstUncommon(unsigned=0) const FastPixelMethod
    {
        // Invoked with GetFirstNMost when FirstLastLength=0
        const uint32 most = GetMostUsed();
        for(MapType<unsigned, uint32>::const_iterator
            i = history.begin();
            i != history.end();
            ++i)
        {
            if(i->second != most) return i->second;
        }
        return most;
    }

    uint32 GetLastUncommon(unsigned=0) const FastPixelMethod
    {
        // Invoked with GetLastNMost when FirstLastLength=0
        const uint32 most = GetMostUsed();
        for(MapType<unsigned, uint32>::const_reverse_iterator
            i = history.rbegin();
            i != history.rend();
            ++i)
        {
            if(i->second != most) return i->second;
        }
        return most;
    }
private:
    uint32 Find(unsigned timer) const FastPixelMethod
    {
        if(!OptimizeChangeLog)
        {
            MapType<unsigned, uint32>::const_iterator
                i = history.find(timer);
            if(i == history.end() || i->first != timer)
                return GetMostUsed();
            return i->second;
        }
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

        /* Pre-begin value: Use background */
        if(i == history.begin())
        {
            return GetMostUsed();
        }

        bool last = (i == history.end());
        --i;
        if(i->first < timer && last
#if CHANGELOG_USE_LASTTIMESTAMP
                                   && timer > last_time
#endif
          )
        {
            /* Post-end value: Use background */
            return GetMostUsed();
        }
        /* Anything else. Take the value. */
        return i->second;
    }

    uint32 Find(unsigned timer, uint32 background) const FastPixelMethod
    {
        if(!OptimizeChangeLog)
        {
            MapType<unsigned, uint32>::const_iterator
                i = history.find(timer);
            if(i == history.end() || i->first != timer)
                return background;
            return i->second;
        }
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

        /* Pre-begin value: Use background */
        if(i == history.begin())
        {
            return background;
        }

        bool last = (i == history.end());
        --i;
        if(i->first < timer && last
#if CHANGELOG_USE_LASTTIMESTAMP
                                   && timer > last_time
#endif
          )
        {
            /* Post-end value: Use background */
            return background;
        }
        /* Anything else. Take the value. */
        return i->second;
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
struct FirstNMostPixel: public ChangeLogPixel
{
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetFirstNMost();
    }
};
struct LastNMostPixel: public ChangeLogPixel
{
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetLastNMost();
    }
};

/*
ChangeLog defines these:

    GetChangeLog
    GetActionAvg   (UNIQUE, BUT ALSO IMPLEMENTED IN "MOSTUSED")
    GetLoopingAvg  (UNIQUE)
    GetLoopingLast (UNIQUE)
    GetFirstNMost  (UNIQUE)
    GetLastNMost   (UNIQUE)
    GetMostUsed    (EMULATED, NOT UNIQUE)
    GetLast        (EMULATED, NOT UNIQUE)
    GetFirst       (EMULATED, NOT UNIQUE)
    GetAverage     (EMULATED, NOT UNIQUE)
*/

DefineBasePair(ChangeLogPixel, ChangeLog, ActionAvg)
DefineBasePair(ChangeLogPixel, ChangeLog, LoopingAvg)
DefineBasePair(ChangeLogPixel, ChangeLog, LoopingLast)
DefineBasePair(ChangeLogPixel, ChangeLog, FirstNMost)
DefineBasePair(ChangeLogPixel, ChangeLog, LastNMost)
DefineBasePair(ChangeLogPixel, ChangeLog, MostUsed)
DefineBasePair(ChangeLogPixel, ChangeLog, Last)
DefineBasePair(ChangeLogPixel, ChangeLog, First)
DefineBasePair(ChangeLogPixel, ChangeLog, Average)

DefineBasePair(ChangeLogPixel, ActionAvg, LoopingAvg)
DefineBasePair(ChangeLogPixel, ActionAvg, LoopingLast)
DefineBasePair(ChangeLogPixel, ActionAvg, FirstNMost)
DefineBasePair(ChangeLogPixel, ActionAvg, LastNMost)
//DefineBasePair(ChangeLogPixel, ActionAvg, MostUsed) -- used MostUsedPixel version instead
DefineBasePair(ChangeLogPixel, ActionAvg, Last)
DefineBasePair(ChangeLogPixel, ActionAvg, First)
//DefineBasePair(ChangeLogPixel, ActionAvg, Average) -- used MostUsedPixel version instead

DefineBasePair(ChangeLogPixel, LoopingAvg, LoopingLast)
DefineBasePair(ChangeLogPixel, LoopingAvg, FirstNMost)
DefineBasePair(ChangeLogPixel, LoopingAvg, LastNMost)
DefineBasePair(ChangeLogPixel, LoopingAvg, MostUsed)
DefineBasePair(ChangeLogPixel, LoopingAvg, Last)
DefineBasePair(ChangeLogPixel, LoopingAvg, First)
DefineBasePair(ChangeLogPixel, LoopingAvg, Average)

DefineBasePair(ChangeLogPixel, LoopingLast, FirstNMost)
DefineBasePair(ChangeLogPixel, LoopingLast, LastNMost)
DefineBasePair(ChangeLogPixel, LoopingLast, MostUsed)
DefineBasePair(ChangeLogPixel, LoopingLast, Last)
DefineBasePair(ChangeLogPixel, LoopingLast, First)
DefineBasePair(ChangeLogPixel, LoopingLast, Average)

DefineBasePair(ChangeLogPixel, FirstNMost, LastNMost)
DefineBasePair(ChangeLogPixel, FirstNMost, MostUsed)
DefineBasePair(ChangeLogPixel, FirstNMost, Last)
DefineBasePair(ChangeLogPixel, FirstNMost, First)
DefineBasePair(ChangeLogPixel, FirstNMost, Average)

DefineBasePair(ChangeLogPixel, LastNMost, MostUsed)
DefineBasePair(ChangeLogPixel, LastNMost, Last)
DefineBasePair(ChangeLogPixel, LastNMost, First)
DefineBasePair(ChangeLogPixel, LastNMost, Average)
