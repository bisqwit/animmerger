class LastPixel
{
    uint32 pix;
public:
    LastPixel() : pix(DefaultPixel)
    {
    }
    void set(unsigned R,unsigned G,unsigned B, unsigned=0) FasterPixelMethod
    {
        set(((R) << 16) + ((G) << 8) + B);
    }
    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        pix = p;
    }

    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return pix;
    }
    inline uint32 GetLast(unsigned=0) const FasterPixelMethod
    {
        return pix;
    }
/////////
    static const unsigned long Traits =
        (1ul << pm_LastPixel);
};
