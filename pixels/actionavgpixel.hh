//#include "pixels/changelogpixel.hh"

class ActionAvgPixel: private ChangeLogPixel
{
public:
    using ChangeLogPixel::set;
    using ChangeLogPixel::GetLoopingAvg;
    using ChangeLogPixel::GetMostUsed;
    using ChangeLogPixel::GetAverage;
    using ChangeLogPixel::GetLast;
    using ChangeLogPixel::Compress;

    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return ChangeLogPixel::GetActionAvg();
    }
    inline uint32 GetChangeLog(unsigned timer) const FasterPixelMethod
    {
        return ChangeLogPixel::get(timer);
    }
};

template<>
class TwoPixels<ActionAvgPixel, ChangeLogPixel>: public ActionAvgPixel
{
public:
    using ActionAvgPixel::set;
    using ActionAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned=0)     const FasterPixelMethod { return get(); }
    inline uint32 get_pixel2(unsigned timer) const FasterPixelMethod { return GetChangeLog(timer); }
};

template<>
class TwoPixels<ActionAvgPixel, MostUsedPixel>: public ActionAvgPixel
{
public:
    using ActionAvgPixel::set;
    using ActionAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned=0) const FasterPixelMethod { return get(); }
    inline uint32 get_pixel2(unsigned=0) const FasterPixelMethod { return GetMostUsed(); }
};

template<>
class TwoPixels<ActionAvgPixel, LastPixel>: public ActionAvgPixel
{
public:
    using ActionAvgPixel::set;
    using ActionAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned=0) const FasterPixelMethod { return get(); }
    inline uint32 get_pixel2(unsigned=0) const FasterPixelMethod { return GetLast(); }
};

template<>
class TwoPixels<ActionAvgPixel, AveragePixel>: public ActionAvgPixel
{
public:
    using ActionAvgPixel::set;
    using ActionAvgPixel::Compress;
    inline uint32 get_pixel1(unsigned=0) const FasterPixelMethod { return get(); }
    inline uint32 get_pixel2(unsigned=0) const FasterPixelMethod { return GetAverage(); }
};

template<>
class TwoPixels<ChangeLogPixel, ActionAvgPixel>
    : public SwapTwoPixels<ActionAvgPixel, ChangeLogPixel>
{
};

template<>
class TwoPixels<MostUsedPixel, ActionAvgPixel>
    : public SwapTwoPixels<ActionAvgPixel, MostUsedPixel>
{
};

template<>
class TwoPixels<LastPixel, ActionAvgPixel>
    : public SwapTwoPixels<ActionAvgPixel, LastPixel>
{
};

template<>
class TwoPixels<AveragePixel, ActionAvgPixel>
    : public SwapTwoPixels<ActionAvgPixel, AveragePixel>
{
};

template<>
class TwoPixels<LoopingAvgPixel, ActionAvgPixel>: private ChangeLogPixel
{
public:
    using ChangeLogPixel::set;
    using ChangeLogPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return GetLoopingAvg(timer); }
    inline uint32 get_pixel2(unsigned=0)     const FasterPixelMethod { return GetActionAvg(); }
};

template<>
class TwoPixels<ActionAvgPixel, LoopingAvgPixel>
    : public SwapTwoPixels<LoopingAvgPixel, ActionAvgPixel>
{
};
