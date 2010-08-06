#include "maptype.hh"

class MostUsedPixel
{
    typedef MapType<uint32, unsigned short> vmap;
    vmap values;
public:
    MostUsedPixel() : values()
    {
    }

    void set(unsigned R,unsigned G,unsigned B, unsigned=0) FasterPixelMethod
    {
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set(p);
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
};

/*** MOSTUSED VARIANTS ***/

struct ActionAvgPixel: public MostUsedPixel
{
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetActionAvg();
    }
};
struct LeastUsedPixel: public MostUsedPixel
{
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetLeastUsed();
    }
};

/*
MostUsed defines these:

    GetMostUsed
    GetLeastUsed   (UNIQUE)
    GetActionAvg   (NOT UNIQUE)
    GetAverage     (EMULATED, NOT UNIQUE)
*/

DefineBasePair(MostUsedPixel, MostUsed,Average)
DefineBasePair(MostUsedPixel, MostUsed,LeastUsed)
DefineBasePair(MostUsedPixel, MostUsed,ActionAvg)

DefineBasePair(MostUsedPixel, ActionAvg,Average)
DefineBasePair(MostUsedPixel, ActionAvg,LeastUsed)

DefineBasePair(MostUsedPixel, LeastUsed,Average)
