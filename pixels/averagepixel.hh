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
        set((p>>16)&0xFF, (p>>8)&0xFF, p&0xFF);
    }
    void set(unsigned R,unsigned G,unsigned B, unsigned=0) FastPixelMethod
    {
        r+=R; g+=G; b+=B;
        ++n;
    }
    uint32 get(unsigned=0) const FastPixelMethod
    {
        return n
            ? (((r/n) << 16) + ((g/n) << 8) + (b/n))
            : DefaultPixel;
    }
    void Compress()
    {
    }
};
