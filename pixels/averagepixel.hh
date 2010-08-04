class AveragePixel
{
    unsigned r,g,b;
    unsigned n;
public:
    AveragePixel() : r(0),g(0),b(0),n(0)
    {
    }
    void set(uint32 p)
    {
        set((p>>16)&0xFF, (p>>8)&0xFF, p&0xFF);
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        r+=R; g+=G; b+=B;
        ++n;
    }
    operator uint32() const
    {
        return n
            ? (((r/n) << 16) + ((g/n) << 8) + (b/n))
            : DefaultPixel;
    }
    void Compress()
    {
    }
};

class AveragePixelAndMostUsedPixel
{
public:
    AveragePixel  pixel;
private:
    MostUsedPixel most_used;
public:
    inline void set(uint32 p)
    {
        pixel.set(p);
        most_used.set(p);
    }
    inline uint32 get_pixel() const { return pixel; }
    inline uint32 get_mostused() const { return most_used; }
    inline void Compress() { pixel.Compress(); most_used.Compress(); }
};
