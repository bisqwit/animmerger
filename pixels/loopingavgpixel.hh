//#include "pixels/changelogpixel.hh"

class LoopingAvgPixel: private ChangeLogPixel
{
public:
    using ChangeLogPixel::set;
    using ChangeLogPixel::GetActionAvg;
    using ChangeLogPixel::GetMostUsed;
    using ChangeLogPixel::GetAverage;
    using ChangeLogPixel::GetLast;
    using ChangeLogPixel::Compress;

    inline uint32 get(unsigned timer) const FasterPixelMethod
    {
        return ChangeLogPixel::GetLoopingAvg(timer);
    }
    inline uint32 GetChangeLog(unsigned timer) const FasterPixelMethod
    {
        return ChangeLogPixel::get(timer);
    }
};

template<>
class TwoPixels<LoopingAvgPixel, ChangeLogPixel>: private LoopingAvgPixel
{
public:
    using LoopingAvgPixel::set;
    using LoopingAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned timer) const FasterPixelMethod { return GetChangeLog(timer); }
};

template<>
class TwoPixels<LoopingAvgPixel, MostUsedPixel>: private LoopingAvgPixel
{
public:
    using LoopingAvgPixel::set;
    using LoopingAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned=0)     const FasterPixelMethod { return GetMostUsed(); }
};

template<>
class TwoPixels<LoopingAvgPixel, AveragePixel>: private LoopingAvgPixel
{
public:
    using LoopingAvgPixel::set;
    using LoopingAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned=0)     const FasterPixelMethod { return GetAverage(); }
};

template<>
class TwoPixels<LoopingAvgPixel, LastPixel>: private LoopingAvgPixel
{
public:
    using LoopingAvgPixel::set;
    using LoopingAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned=0)     const FasterPixelMethod { return GetLast(); }
};


template<>
class TwoPixels<ChangeLogPixel, LoopingAvgPixel>
    : public SwapTwoPixels<LoopingAvgPixel, ChangeLogPixel>
{
};

template<>
class TwoPixels<MostUsedPixel, LoopingAvgPixel>
    : public SwapTwoPixels<LoopingAvgPixel, MostUsedPixel>
{
};

template<>
class TwoPixels<AveragePixel, LoopingAvgPixel>
    : public SwapTwoPixels<LoopingAvgPixel, AveragePixel>
{
};

template<>
class TwoPixels<LastPixel, LoopingAvgPixel>
    : public SwapTwoPixels<LoopingAvgPixel, LastPixel>
{
};
