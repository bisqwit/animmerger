#ifndef bqtTileTrackerPixelHH
#define bqtTileTrackerPixelHH

#include "types.hh"

static const uint32 DefaultPixel = 0xFF404041u;

extern unsigned LoopingLogLength;

extern unsigned CurrentTimer;
/* Timer value for Set() and get methods in ChangeLog and LoopingLog methods */

extern enum PixelMethod
{
    pm_AveragePixel,
    pm_LastPixel,
    pm_MostUsedPixel,
    pm_MostUsed16Pixel,
    pm_ChangeLogPixel,
    pm_LoopingLogPixel
} pixelmethod;
extern bool NeedsMostUsedPixel;

/* A vector of 256x256 pixels. */
struct Array256x256of_Base
{
    virtual ~Array256x256of_Base() { }
    virtual uint32 GetPixel(unsigned index) const = 0;
    virtual uint32 GetMostUsed(unsigned index) const = 0;
    virtual void Set(unsigned index, uint32 p) = 0;
    virtual void Compress() = 0;
};
class UncertainPixelVector256x256
{
public:
    // By default, the vector is uninitialized (empty)
    UncertainPixelVector256x256() : data(0)
    {
    }

    // Init: Resize the vector to 256x256 elements
    void init();

    // Copy constructor
    UncertainPixelVector256x256(const UncertainPixelVector256x256& b);

    // Assign operator (create copy of the other vector)
    UncertainPixelVector256x256& operator= (const UncertainPixelVector256x256& b);

    // Destructor: Deallocate vector of alloated
    ~UncertainPixelVector256x256()
    {
        delete data;
    }

    // Visit: Run user-given functor, give reference to the actual vector data as param
    template<typename F>
    inline void Visit(F func)
    {
        if(data) func(*data);
    }

    template<typename F>
    inline void Visit(F func) const
    {
        if(data) func(*data);
    }

    // Run Compress() method on each vector element
    inline void Compress()
    {
        if(data) data->Compress();
    }

    // Test whether vector is empty (uninitialized)
    bool empty() const { return !data; }

private:
    Array256x256of_Base* data;
};

#endif
