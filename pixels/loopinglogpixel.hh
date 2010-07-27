//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"

#include <vector>
#include <algorithm>
template<unsigned LOOP_LENGTH = 90>
class LoopingLogPixel
{
    MostUsedPixel most_used;

    LastPixel history[LOOP_LENGTH];
public:
    LoopingLogPixel()
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        if(CurrentTimer == 0)
        {
            // Ignore first frame. It's gray.
            return;
        }
        most_used.set_p(p);

        unsigned offs = CurrentTimer % LOOP_LENGTH;
        if(history[offs] == DefaultPixel || p != most_used)
            history[offs].set_p(p);
    }
    operator uint32() const
    {
        unsigned offs = CurrentTimer % LOOP_LENGTH;
        uint32 result = history[offs];//.value_ignore(most_used);
        if(result == DefaultPixel) return most_used;
        return result;
    }

    void Compress()
    {
        most_used.Compress();
        //for(unsigned a=0; a<LOOP_LENGTH; ++a)
        //    history[a].CompressButIgnore(most_used);
    }
};
