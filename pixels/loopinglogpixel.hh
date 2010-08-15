//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"

#include "vectype.hh"

class LoopingLogPixel
{
    MostUsedPixel most_used;
    LastPixel* history;
public:
    LoopingLogPixel()
        : history(new LastPixel [LoopingLogLength])
    {
    }
    ~LoopingLogPixel()
    {
        delete[] history;
    }
    LoopingLogPixel(const LoopingLogPixel& b)
        : history(new LastPixel[LoopingLogLength])
    {
        for(unsigned a=0; a<LoopingLogLength; ++a)
            history[a] = b.history[a];
    }
    LoopingLogPixel& operator=(const LoopingLogPixel& b)
    {
        for(unsigned a=0; a<LoopingLogLength; ++a)
            history[a] = b.history[a];
        return *this;
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

    inline uint32 GetTinyAverage(unsigned=0) const FasterPixelMethod
    {
        return most_used.GetTinyAverage();
    }

    inline uint32 GetActionAvg(unsigned=0) const FasterPixelMethod
    {
        return most_used.GetActionAvg();
    }

/////////
    static const unsigned long Traits =
        (1ul << pm_LoopingLogPixel)
      | MostUsedPixel::Traits;
    static const unsigned SizePenalty =
        MostUsedPixel::SizePenalty + 8;
};

