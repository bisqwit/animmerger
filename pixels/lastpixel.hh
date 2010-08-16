class LastPixel
{
    uint32 pix;
public:
    LastPixel() : pix(DefaultPixel)
    {
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
    static const unsigned SizePenalty = 0;
    static const unsigned Components =
        (1ul << impl_Last);
};
