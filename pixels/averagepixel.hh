class AveragePixel
{
    unsigned r,g,b;
    unsigned n;
public:
    AveragePixel() : r(0),g(0),b(0),n(0)
    {
    }
    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        set_rgb((p>>16)&0xFF, (p>>8)&0xFF, p&0xFF);
    }
    void set_n(uint32 p, unsigned count) FasterPixelMethod
    {
        set_n_rgb((p>>16)&0xFF, (p>>8)&0xFF, p&0xFF, count);
    }
    void set_rgb(unsigned R,unsigned G,unsigned B, unsigned=0) FastPixelMethod
    {
        r+=R; g+=G; b+=B;
        ++n;
    }
    void set_n_rgb(unsigned R,unsigned G,unsigned B, unsigned count) FastPixelMethod
    {
        r+=R*count; g+=G*count; b+=B*count;
        n += count;
    }

    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetAverage();
    }

    uint32 GetAverage(unsigned=0) const FastPixelMethod
    {
        return n
            ? (((r/n) << 16) + ((g/n) << 8) + (b/n))
            : DefaultPixel;
    }


/////////
    static const unsigned long Traits =
        (1ul << pm_AveragePixel);
    static const unsigned SizePenalty = 0;

    static const unsigned Components =
        (1ul << impl_Average);
};
