//#include "pixels/mostusedpixel.hh"

/* undef or define. */
static const bool CHANGELOG_GUESS_OUTSIDES = true;

#include <vector>
#include <algorithm>

class ChangeLogPixel
{
    struct historyitem
    {
        unsigned moment;
        uint32 pixel;
    public:
        historyitem() { }
        
        // Because std::upper_bound doesn't allow searching by types
        // other than what the container has, here are utility functions
        // to handle this container as if it only contained the "moment" value.
        // Conveniently, it allows also the insertion of the "first moment" pixel.
        historyitem(unsigned time) : moment(time) { }
        bool operator< (const historyitem& b) const { return moment < b.moment; }
    };
    
    std::vector<historyitem> history;
    uint32 lastpixel;
    unsigned lastmoment;

    bool FirstMomentIsVague;    // Used when CHANGELOG_GUESS_OUTSIDES
    MostUsedPixel first_moment; // Used when CHANGELOG_GUESS_OUTSIDES

#if 0
    MapType<uint32, unsigned short> minvalues;
#endif
public:
    ChangeLogPixel() : lastpixel(DefaultPixel), lastmoment(0)
        , FirstMomentIsVague(true)
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
        
#if 0
        ++minvalues[p];
#endif

        // Store the value into the history.
        // (But only if it's different than previous value.)
        if(lastpixel != p || history.empty())
        {
            lastpixel = p;
            
            if(CurrentTimer == 0)
            {
                // The value of the pixel at first moment is precisely known.
                FirstMomentIsVague = false;
            }
            else if(history.empty())
            {
                // The value of the pixel at the first moment is not precisely known.
                if(CHANGELOG_GUESS_OUTSIDES)
                {
                    FirstMomentIsVague = true;
                    history.push_back(historyitem(0));
                }
            }
            
            historyitem item;
            item.pixel  = p;
            item.moment = CurrentTimer;
            
            history.push_back(item);
        }
        if(CHANGELOG_GUESS_OUTSIDES
        && FirstMomentIsVague)
        {
            UpdateFirstMoment(p);
        }
        lastmoment = CurrentTimer;
    }
    
    operator uint32() const
    {
#if 0
        /* Return the rarest of the last N values. */
        unsigned tmpmin = lastmoment;
        unsigned retval = Find(0);
        for(MapType<uint32, unsigned short>::const_iterator
               i = minvalues.begin();
               i != minvalues.end();
               ++i)
        {
            if(i->second <= tmpmin) { tmpmin = i->second; retval = i->first; }
        }
        return retval;
#else
        return Find(CurrentTimer);
#endif
    }

    void Compress()
    {
        if(CHANGELOG_GUESS_OUTSIDES)
        {
            first_moment.Compress();
        }
    }

private:
    uint32 Find(unsigned time) const
    {
        // Find the pixel value that was present at the given time.
        /*
          map::lower_bound: 
            Returns an iterator pointing to first element >= key, or end().
          map::upper_bound:
            Returns an iterator pointing to the first element > key, or end().
          
          What we want is an iterator pointing
            to the last element that is <= key.
         */
        std::vector<historyitem>::const_iterator
            i = std::upper_bound(history.begin(), history.end(), historyitem(time));

        /* Pre-begin value: reasonable default */
        if(i == history.begin())
        {
            return CHANGELOG_GUESS_OUTSIDES
                ? (history.empty() ? lastpixel : history[0].pixel)
                : DefaultPixel;
        }

#if 1
        /* Post-end value: Special handling */
        if(i == history.end())
        {
            /* If the last value is somewhat behind the chosen moment,
             * take the reasonable default instead */
            if(lastmoment+0 < time)
            {
                return CHANGELOG_GUESS_OUTSIDES
                    ? (history.empty() ? lastpixel : history[0].pixel)
                    : DefaultPixel;
            }
        }
#endif
        
        /* Anything else. Take the value. */
        --i;
        return i->pixel;
    }

private:    
    void UpdateFirstMoment(uint32 p) // For CHANGELOG_GUESS_OUTSIDES only
    {
        // Updates the value of the pixel at the first moment to the value
        // that has appeared the most.
        first_moment.set(p);
        history[0].pixel = first_moment;
    }
};
