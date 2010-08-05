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
