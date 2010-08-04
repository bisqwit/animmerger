class LastPixel
{
    unsigned pix;
public:
    LastPixel() : pix(DefaultPixel)
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        set(((R) << 16) + ((G) << 8) + B);
    }
    void set(uint32 p)
    {
        pix = p;
    }
    operator uint32() const { return pix; }
    void Compress()
    {
    }
};


class LastPixelAndMostUsedPixel
{
public:
    LastPixel  pixel;
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
