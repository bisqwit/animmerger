class FirstPixel
{
    uint32 pix;
public:
    FirstPixel() : pix(DefaultPixel)
    {
    }
    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        if(pix == DefaultPixel) pix = p;
    }
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return pix;
    }
    inline uint32 GetFirst(unsigned=0) const FasterPixelMethod
    {
        return pix;
    }

/////////
    static const unsigned SizePenalty = 0;
};
