/* SolidPixel records a background; if, however,
 * in the next frame, something changes over the
 * background, it is assumed that either the new
 * frame or the old frame had an actor at that
 * location. Just in case, both are cleared
 * (so here, the background is cleared).
 */
class SolidPixel
{
    uint32 pix, best_pix;
    unsigned short pix_confidence, best_confidence;
public:
    SolidPixel() : pix(DefaultPixel), best_pix(DefaultPixel),
                   pix_confidence(0), best_confidence(0)
    {
    }
    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        if(pix == DefaultPixel)
        {
            pix = p;
            pix_confidence = 0;
        }
        else if(p == pix)
        {
            if(pix_confidence) ++pix_confidence;
            else pix_confidence = 2;
        }
        else
        {
            if(pix != DefaultPixel && pix_confidence > best_confidence)
            {
                best_confidence = pix_confidence;
                best_pix        = pix;
            }
            pix_confidence = 0;
            pix = p;
        }
    }
    void set_n(uint32 p, unsigned count) FasterPixelMethod
    {
        switch(count)
        {
            case 0: return;
            case 1: set(p); return;
            default:
                if(pix == DefaultPixel)
                {
                    pix = p;
                    pix_confidence = count;
                }
                else if(p == pix)
                {
                    if(pix_confidence) pix_confidence += count;
                    else pix_confidence += 1 + count;
                }
                else
                {
                    if(pix != DefaultPixel && pix_confidence > best_confidence)
                    {
                        best_confidence = pix_confidence;
                        best_pix        = pix;
                    }
                    pix_confidence = count;
                    pix = p;
                }
        }
    }
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetSolid();
    }
    inline uint32 GetSolid(unsigned=0) const FasterPixelMethod
    {
        return (best_confidence > pix_confidence) ? best_pix : pix;
    }

/////////
    static const unsigned SizePenalty = 0;
};
