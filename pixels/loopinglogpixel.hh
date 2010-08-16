//#include "pixels/mostusedpixel.hh"
//#include "pixels/lastpixel.hh"

#include "vectype.hh"

/*
The difference between LoopingLog, and the LoopingLast method in ChangeLog
is that this samples MostUsed at *every* set() call and compares to
that *current* value, whereas loopingLast compares to the MostUsed
value _after_ all animation has been created.
The latter is more reliable. LoopingLog has been preserved for posterity.
*/


/*
LoopingLog requires an instance of MostUsedPixel to work.
However, MostUsedPixel can be provided by MostUsedPixel or ChangeLogPixel.
When both ChangeLogPixel and LoopingLogPixel are required,
it makes more sense to use ChangeLogPixel instead of MostUsedPixel.
Hence, complicated template trickery here.

The Components feature is required to prevent compiler from
spamming the console with warning messages when it tries to
use MostUsedPixel<LoopingLogPixel>. With Components, we can
explicitly avoid overlapping feature sets in the inheritance
chain.
*/


template<typename Slave>
class LoopingLogPixelBase: public Slave
{
    LastPixel* history;
public:
    LoopingLogPixelBase() : history(new LastPixel [LoopingLogLength])
    {
    }
    ~LoopingLogPixelBase()
    {
        delete[] history;
    }
    LoopingLogPixelBase(const LoopingLogPixelBase<Slave>& b)
        : history(new LastPixel[LoopingLogLength])
    {
        for(unsigned a=0; a<LoopingLogLength; ++a)
            history[a] = b.history[a];
    }
    LoopingLogPixelBase& operator=(const LoopingLogPixelBase<Slave>& b)
    {
        for(unsigned a=0; a<LoopingLogLength; ++a)
            history[a] = b.history[a];
        return *this;
    }

    void set(uint32 p, unsigned timer) FastPixelMethod
    {
        Slave::set(p, timer);

        unsigned offs = timer % LoopingLogLength;
        if(history[offs].get() == DefaultPixel || p != Slave::GetMostUsed())
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
        if(result == DefaultPixel) return Slave::GetMostUsed();
        return result;
    }
/////////
    static const unsigned long Traits =
        (1ul << pm_LoopingLogPixel)
      | Slave::Traits;
    static const unsigned SizePenalty =
        Slave::SizePenalty + 8;
};

struct LoopingLogAndMostUsedPixel: public LoopingLogPixelBase<MostUsedPixel>
{
    static const unsigned Components =
        (1ul << impl_LoopingLogAndMostUsed) | MostUsedPixel::Components;
};
struct LoopingLogAndChangeLogPixel: public LoopingLogPixelBase<ChangeLogPixel>
{
    static const unsigned Components =
        (1ul << impl_LoopingLogAndChangeLog) | ChangeLogPixel::Components;
};
