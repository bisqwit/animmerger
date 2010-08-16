#include "maptype.hh"

class MostUsedPixel
{
    typedef MapType<uint32, unsigned short> vmap;
    vmap values;
public:
    MostUsedPixel() : values()
    {
    }

    void set(uint32 p, unsigned=0) FastPixelMethod
    {
        vmap::iterator i = values.lower_bound(p);
        if(i == values.end() || i->first != p)
            values.insert(i, vmap::value_type(p,1));
        else
            i->second += 1;
    }
    void set_n(uint32 p, unsigned count) FastPixelMethod
    {
        vmap::iterator i = values.lower_bound(p);
        if(i == values.end() || i->first != p)
            values.insert(i, vmap::value_type(p, count));
        else
            i->second += count;
    }

    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetMostUsed();
    }

    uint32 GetMostUsed(unsigned=0) const FastPixelMethod
    {
        std::pair<uint32,unsigned short> result(DefaultPixel,0);
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
            if(i->second > result.second)
                result = *i;
        return result.first;
    }

    uint32 GetLeastUsed(unsigned=0) const FastPixelMethod
    {
        std::pair<uint32,unsigned short> result(DefaultPixel,~(unsigned short)0);
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
            if(i->second < result.second)
                result = *i;
        return result.first;
    }

    uint32 GetAverage(unsigned=0) const FastPixelMethod
    {
        AveragePixel result;
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
            result.set_n(i->first, i->second);
        return result.get();
    }

    uint32 GetTinyAverage(unsigned=0) const FastPixelMethod
    {
        TinyAveragePixel result;
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
            result.set_n(i->first, i->second);
        return result.get();
    }

    uint32 GetActionAvg(unsigned=0) const FastPixelMethod
    {
        const uint32 most = GetMostUsed();
        AveragePixel result;
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
            result.set_n(i->first, i->first!=most ? i->second : 1);
        uint32 res = result.get();
        if(res == DefaultPixel) return most;
        return res;
    }
/////////
    static const unsigned long Traits =
        (1ul << pm_MostUsedPixel)
      | (1ul << pm_LeastUsedPixel)
      | (1ul << pm_AveragePixel)
      | (1ul << pm_TinyAveragePixel)
      | (1ul << pm_ActionAvgPixel);
    static const unsigned SizePenalty = 16;

    static const unsigned Components =
        (1ul << impl_MostUsed);
};

// These variants are needed by ChangeLog to simplify templates
/*struct ActionAvgPixel: public MostUsedPixel
{
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return MostUsedPixel::GetActionAvg();
    }
};*/
struct LeastUsedPixel: public MostUsedPixel
{
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return MostUsedPixel::GetLeastUsed();
    }
};
