template<typename Base=DummyPixel>
class TinyAveragePixel: public Base
{
    unsigned result, n;
public:
    TinyAveragePixel() : result(DefaultPixel), n(0)
    {
    }
    void set(uint32 p, unsigned timer=0) FasterPixelMethod
    {
        Base::set(p, timer);
        set_rgb((p>>16)&0xFF, (p>>8)&0xFF, p&0xFF);
    }
    void set_n(uint32 p, unsigned count) FasterPixelMethod
    {
        set_n_rgb((p>>16)&0xFF, (p>>8)&0xFF, p&0xFF, count);
    }
    void set_rgb(unsigned R,unsigned G,unsigned B, unsigned=0) FastPixelMethod
    {
        set_n_rgb(R,G,B, 1);
    }
    void set_n_rgb(unsigned R,unsigned G,unsigned B, unsigned count) FastPixelMethod
    {
        unsigned r = (result>>16)&0xFF, g = (result>>8)&0xFF, b = (result&0xFF);
        r = (r*n+R*count) / (n+count);
        g = (g*n+G*count) / (n+count);
        b = (b*n+B*count) / (n+count);
        n += count;
        result = (r<<16) | (g<<8) | (b<<0);
    }

    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetTinyAverage();
    }

    inline uint32 GetTinyAverage(unsigned=0) const FasterPixelMethod
    {
        return result;
    }
/////////
    static const unsigned long Traits =
        Base::Traits
      | (1ul << pm_TinyAveragePixel);
    static const unsigned SizePenalty = Base::SizePenalty + 0;
};
