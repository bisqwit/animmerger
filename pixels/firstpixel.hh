class FirstPixel
{
    uint32 pix;
public:
    FirstPixel() : pix(DefaultPixel)
    {
    }
    void set(unsigned R,unsigned G,unsigned B, unsigned=0) FasterPixelMethod
    {
        set(((R) << 16) + ((G) << 8) + B);
    }
    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        if(pix == DefaultPixel) pix = p;
    }
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return pix;
    }
};
