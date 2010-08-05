#include "maptype.hh"

class MostUsedPixel
{
    typedef MapType<uint32, unsigned short> vmap;
    vmap values;
    //uint32   pix;       // current result
    //unsigned short max; // maximum occurrence count
    //bool final;
public:
    MostUsedPixel() : values()/*,pix(DefaultPixel),max(0)*/ //, final(false)
    {
    }

    void set(unsigned R,unsigned G,unsigned B, unsigned=0) FasterPixelMethod
    {
        //if(final) return;
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set(p);
    }
    void set(uint32 p, unsigned=0) FastPixelMethod
    {
        vmap::iterator i = values.lower_bound(p);
        if(i == values.end() || i->first != p)
            values.insert(i, vmap::value_type(p,1));
        else
            i->second += 1;
        /*unsigned short v = ++values[p];*/
        //if(v > max) { max = v; pix = p; }
    }
    /*uint32 value_ignore(uint32 ignore) const
    {
        if(pix != ignore) return pix;
        unsigned max2 = 0;
        uint32 result = DefaultPixel;
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
        {
            if(i->first != ignore)
            {
                unsigned v = i->second;
                if(v > max2) { max2 = v; result = i->first; }
            }
        }
        return result;
    }*/
    uint32 get(unsigned=0) const FastPixelMethod
    {
        std::pair<uint32,unsigned short> result(DefaultPixel,0);
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
            if(i->second > result.second)
                result = *i;
        return result.first;
        //return pix;
    }

    uint32 GetAverage() const FastPixelMethod
    {
        if(values.empty())
            return DefaultPixel;

        unsigned r=0, g=0, b=0, n=0;
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
        {
            unsigned count = i->second;
            unsigned pix = i->first;
            unsigned R = (pix>>16)&0xFF, G=(pix>>8)&0xFF, B=(pix&0xFF);
            r += R*count;
            g += G*count;
            b += B*count;
            n += count;
        }
        return ((r/n) << 16) + ((g/n) << 8) + (b/n);
    }

    void Compress()
    {
        std::pair<uint32,unsigned short> result(DefaultPixel,0);
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
            if(i->second > result.second)
                result = *i;
        values.clear();
        values.insert(values.end(), result);
    }
    /*void CompressButIgnore(uint32 ignore)
    {
        pix = value_ignore(ignore);
        values.clear();
    }*/
};

template<>
class TwoPixels<MostUsedPixel, AveragePixel>: private MostUsedPixel
{
public:
    using MostUsedPixel::set;
    using MostUsedPixel::Compress;
    inline uint32 get_pixel1(unsigned timer) const FasterPixelMethod { return get(timer); }
    inline uint32 get_pixel2(unsigned)       const FasterPixelMethod { return GetAverage(); }
};

template<>
class TwoPixels<AveragePixel, MostUsedPixel>
    : public SwapTwoPixels<MostUsedPixel, AveragePixel>
{
};
