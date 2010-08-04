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

    void set(unsigned R,unsigned G,unsigned B)
    {
        //if(final) return;
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set(p);
    }
    void set(uint32 p)
    {
        /*unsigned short v =*/ ++values[p];
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
    operator uint32() const
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

class MostUsedPixelAndMostUsedPixel
{
public:
    MostUsedPixel  pixel;
public:
    inline void set(uint32 p)
    {
        pixel.set(p);
    }
    inline uint32 get_pixel() const    { return pixel; }
    inline uint32 get_mostused() const { return pixel; }
    inline void Compress() { pixel.Compress(); }
};
