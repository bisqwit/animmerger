//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"

#include "vectype.hh"

class LoopingLogPixel
{
    MostUsedPixel most_used;

    VecType<LastPixel> history;
public:
    LoopingLogPixel(): history(LoopingLogLength)
    {
    }
    void set(unsigned R,unsigned G,unsigned B, unsigned timer) FasterPixelMethod
    {
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set(p, timer);
    }
    void set(uint32 p, unsigned timer) FastPixelMethod
    {
        most_used.set(p);

        unsigned offs = timer % LoopingLogLength;
        if(history[offs].get() == DefaultPixel || p != GetMostUsed())
            history[offs].set(p);
    }

    inline uint32 get(unsigned timer) const FasterPixelMethod
    {
        return GetLoopingLog(timer);
    }

    uint32 GetLoopingLog(unsigned timer) const FastPixelMethod
    {
        unsigned offs = timer % LoopingLogLength;
        uint32 result = history[offs].get();//.value_ignore(most_used);
        if(result == DefaultPixel) return GetMostUsed();
        return result;
    }

    inline uint32 GetMostUsed(unsigned=0) const FasterPixelMethod
    {
        return most_used.get();
    }

    inline uint32 GetAverage(unsigned=0) const FasterPixelMethod
    {
        return most_used.GetAverage();
    }

    inline uint32 GetActionAvg(unsigned=0) const FasterPixelMethod
    {
        return most_used.GetActionAvg();
    }
/////////
    static const unsigned long Traits =
        (1ul << pm_LoopingLogPixel)
      | (1ul << pm_MostUsedPixel)
      | (1ul << pm_AveragePixel)
      | (1ul << pm_ActionAvgPixel);
};
