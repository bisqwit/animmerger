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
    uint32 get(unsigned timer) const FastPixelMethod
    {
        unsigned offs = timer % LoopingLogLength;
        uint32 result = history[offs].get();//.value_ignore(most_used);
        if(result == DefaultPixel) return GetMostUsed();
        return result;
    }
    inline uint32 GetMostUsed() const FasterPixelMethod
    {
        return most_used.get();
    }

    inline uint32 GetAverage() const FasterPixelMethod
    {
        return most_used.GetAverage();
    }

    inline void Compress()
    {
        most_used.Compress();
        //for(unsigned a=0; a<LoopingLogLength; ++a)
        //    history[a].CompressButIgnore(most_used);
    }
};

template<>
class TwoPixels<LoopingLogPixel, MostUsedPixel>: private LoopingLogPixel
{
public:
    using LoopingLogPixel::set;
    using LoopingLogPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned)       const FasterPixelMethod { return GetMostUsed(); }
};

template<>
class TwoPixels<LoopingLogPixel, AveragePixel>: private LoopingLogPixel
{
public:
    using LoopingLogPixel::set;
    using LoopingLogPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned)       const FasterPixelMethod { return GetAverage(); }
};


template<>
class TwoPixels<MostUsedPixel, LoopingLogPixel>
    : public SwapTwoPixels<LoopingLogPixel, MostUsedPixel>
{
};

template<>
class TwoPixels<AveragePixel, LoopingLogPixel>
    : public SwapTwoPixels<LoopingLogPixel, AveragePixel>
{
};
