//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"

#include "vectype.hh"

template<typename Base=DummyPixel>
class LoopingLogPixel: public Base
{
    MostUsedPixel<> most_used;

    VecType<LastPixel<> > history;
public:
    LoopingLogPixel(): history(LoopingLogLength)
    {
    }
    void set(uint32 p, unsigned timer) FastPixelMethod
    {
        Base::set(p,timer);
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
        return most_used.GetMostUsed();
    }
    inline uint32 GetLeastUsed(unsigned=0) const FasterPixelMethod
    {
        return most_used.GetLeastUsed();
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
        Base::Traits
      | (1ul << pm_LoopingLogPixel)
      | MostUsedPixel<>::Traits;
};
