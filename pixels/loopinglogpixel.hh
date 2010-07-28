//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"

#include <vector>
#include <algorithm>

#include "vectype.hh"

class LoopingLogPixel
{
    MostUsedPixel most_used;

    VecType<LastPixel> history;
public:
    LoopingLogPixel(): history(LoopingLogLength)
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

        unsigned offs = CurrentTimer % LoopingLogLength;
        if(history[offs] == DefaultPixel || p != most_used)
            history[offs].set(p);
    }
    operator uint32() const
    {
        unsigned offs = CurrentTimer % LoopingLogLength;
        uint32 result = history[offs];//.value_ignore(most_used);
        if(result == DefaultPixel) return most_used;
        return result;
    }

    void Compress()
    {
        most_used.Compress();
        //for(unsigned a=0; a<LoopingLogLength; ++a)
        //    history[a].CompressButIgnore(most_used);
    }
};
