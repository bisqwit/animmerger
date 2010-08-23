/* SolidPixel records a background; if, however,
 * in the next frame, something changes over the
 * background, it is assumed that either the new
 * frame or the old frame had an actor at that
 * location. Just in case, both are cleared
 * (so here, the background is cleared).
 */
class SolidPixel
{
    uint32 pix;
public:
    SolidPixel() : pix(DefaultPixel)
    {
    }
    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        if(pix == DefaultPixel) pix = p;
        else if(p != pix) pix = DefaultPixel;
    }
    void set_n(uint32 p, unsigned count) FasterPixelMethod
    {
        if(count > 1) pix = p;
        else set(p);
    }
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return pix;
    }
    inline uint32 GetSolid(unsigned=0) const FasterPixelMethod
    {
        return pix;
    }

/////////
    static const unsigned long Traits =
        (1ul << pm_SolidPixel);
    static const unsigned SizePenalty = 0;
    static const unsigned Components =
        (1ul << impl_Solid);
};
