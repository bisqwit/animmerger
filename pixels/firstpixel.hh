template<typename Base=DummyPixel>
class FirstPixel: public Base
{
    uint32 pix;
public:
    FirstPixel() : pix(DefaultPixel)
    {
    }
    void set(uint32 p, unsigned timer=0) FasterPixelMethod
    {
        Base::set(p,timer);
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
    static const unsigned long Traits =
        Base::Traits
      | (1ul << pm_FirstPixel);
    static const unsigned SizePenalty = Base::SizePenalty + 0;
};
