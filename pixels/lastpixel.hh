template<typename Base=DummyPixel>
class LastPixel: public Base
{
    uint32 pix;
public:
    LastPixel() : pix(DefaultPixel)
    {
    }
    void set(uint32 p, unsigned timer=0) FasterPixelMethod
    {
        Base::set(p,timer);
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
        Base::Traits
      | (1ul << pm_LastPixel);
    static const unsigned SizePenalty = Base::SizePenalty + 0;
};
