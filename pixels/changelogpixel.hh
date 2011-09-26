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
    void set(uint32 p, unsigned timer) FastPixelMethod
    {
        if(history.empty() && !autoalign)
        {
            // Preallocate frame buffers to prevent
            // repeated reallocations later on.
            // Since autoaligning is disabled, we can
            // can assume that every image pixel
            // addresses one changelog pixel.
            history.reserve(estimated_num_frames);
        }
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

    inline uint32 GetChangeLogBackground(int sign=0) const
    {
        #define DoCase(o,f,name) \
            case pm_##name##Pixel: \
                if(f&1) break; else return Get##name(0);
        switch((sign <= 0 ? bgmethod0 : bgmethod1))
        {
            DefinePixelMethods(DoCase)
        }
        #undef DoCase
        return GetMostUsed();
    }
    uint32 GetChangeLog(unsigned timer) const FastPixelMethod
    {
        if(AnimationBlurLength == 0) return Find(timer);

        uint32 pix = Find(timer, DefaultPixel);
        if(pix != DefaultPixel) return pix;

        const uint32 most = GetChangeLogBackground();

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

    template<typename SlaveType>
    uint32 GetTimerAggregate(unsigned timer=0, uint32 background=DefaultPixel) const
    {
        if(background == DefaultPixel) background = GetMostUsed();
        SlaveType result(timer, background);
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
            result.set_n(j->first, j->second, duration);
        }
        return result.get();
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

    template<typename SlaveType>
    uint32 GetLoopingAggregate(unsigned timer) const
    {
        uint32 most = GetMostUsed();
        uint32 res = GetTimerAggregate<SlaveType> (timer, most);
        if(AnimationBlurLength != 0)
        {
            if(res == most)
            {
                AveragePixel result;
                unsigned remaining_blur = AnimationBlurLength;
                result.set_n(res, 1);

                timer += LoopingLogLength * AnimationBlurLength;
                for(; timer-- > 0 && remaining_blur-- > 0; )
                {
                    res = GetTimerAggregate<SlaveType> (timer, most);
                    result.set_n(res, 1);
                    if(res != most) break;
                }
                return result.get();
            }
        }
        return res;
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

    template<typename AvgType>
    struct ActionAvgSlave
    {
        AvgType      result;
        uint32       most;
        ActionAvgSlave(unsigned, uint32 m): result(), most(m)
        {
            if(most != DefaultPixel) result.set_n(most, 1);
        }
        void set_n(unsigned, uint32 pix, unsigned duration) FasterPixelMethod
        {
            if(pix == most) return;
            result.set_n(pix, duration);
        }
        inline uint32 get() const FasterPixelMethod
        {
            uint32 res = result.get();
            return (res != DefaultPixel) ? res : most;
        }
    };

    template<typename AvgType>
    struct LoopingAvgSlave
    {
        AvgType      result;
        uint32       most;
        unsigned     offs;
        LoopingAvgSlave(unsigned timer, uint32 m)
            : result(), most(m), offs(timer % LoopingLogLength) { }
        void set_n(unsigned begin, uint32 pix, unsigned duration) FasterPixelMethod
        {
            if(pix == most) return;
            unsigned n_hits = 0;
            while(duration--)
                if(begin++ % LoopingLogLength == offs)
                    ++n_hits;
            result.set_n(pix, n_hits);
        }
        inline uint32 get() const FasterPixelMethod
        {
            uint32 res = result.get();
            return (res != DefaultPixel) ? res : most;
        }
    };

    struct LoopingLastSlave
    {
        uint32       result;
        uint32       most;
        unsigned     offs;
        LoopingLastSlave(unsigned timer, uint32 m)
            : result(m), most(m), offs(timer % LoopingLogLength) { }
        void set_n(unsigned begin, uint32 pix, unsigned duration) FasterPixelMethod
        {
            if(pix == most) return;
            while(duration--)
                if(begin++ % LoopingLogLength == offs)
                    { result = pix; break; }
        }
        inline uint32 get() const FasterPixelMethod
        {
            return result;
        }
    };

    inline uint32 GetActionAvg(unsigned=0) const FastPixelMethod
    {
        return GetTimerAggregate<ActionAvgSlave<AveragePixel> > ();
    }
    inline uint32 GetLoopingAvg(unsigned timer) const FastPixelMethod
    {
        return GetLoopingAggregate<LoopingAvgSlave<AveragePixel> >(timer);
    }
    inline uint32 GetLoopingLog(unsigned timer) const FastPixelMethod
    {
        return GetLoopingAggregate<LoopingLastSlave>(timer);
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
    inline uint32 GetTinyAverage(unsigned=0) const FastPixelMethod
    {
        return GetAggregate<TinyAveragePixel> ();
    }
    inline uint32 GetSolid(unsigned=0) const FastPixelMethod
    {
        return GetAggregate<SolidPixel> ();
    }
    inline uint32 GetLast(unsigned=0) const FastPixelMethod
    {
        return history.empty() ? DefaultPixel : history.rbegin()->second;
    }
    inline uint32 GetFirst(unsigned=0) const FastPixelMethod
    {
        return history.empty() ? DefaultPixel : history.begin()->second;
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
                return GetChangeLogBackground();
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
        MapType<unsigned, uint32>::const_iterator i = ubound(timer);

        /* Pre-begin value: Use background */
        if(i == history.begin())
        {
            return GetChangeLogBackground(-1);
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
            return GetChangeLogBackground(+1);
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
        MapType<unsigned, uint32>::const_iterator i = ubound(timer);

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

private:
    MapType<unsigned, uint32>::const_iterator ubound(unsigned timer) const
    {
        if(history.empty()) return history.end();
        // Returns an iterator pointing to first element > timer, or end().
        return history.upper_bound_interp(timer);
        /* ^ Use the interpolative search for better swap behavior */
    }

public:
/////////
    static const unsigned SizePenalty = 32;
};
