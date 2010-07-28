#include "maptype.hh"

class MostUsedPixel
{
    typedef MapType<uint32, unsigned short> vmap;
    vmap values;
    unsigned pix;
    unsigned short max;
    //bool final;
public:
    MostUsedPixel() : pix(DefaultPixel),max(0) //, final(false)
    {
    }

    void set(unsigned R,unsigned G,unsigned B)
    {
        //if(final) return;
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set(p);
    }
    void set(uint32 p)
    {
        unsigned short v = ++values[p];
        if(v > max) { max = v; pix = p; }
        //if(v > 300) { final=true; values.clear(); }
    }
    uint32 value_ignore(uint32 ignore) const
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
    }
    operator uint32() const { return pix; }
    void Compress()
    {
        values.clear();
    }
    void CompressButIgnore(uint32 ignore)
    {
        pix = value_ignore(ignore);
        values.clear();
    }
};
