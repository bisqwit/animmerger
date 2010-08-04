class LastPixel
{
    uint32 pix;
public:
    LastPixel() : pix(DefaultPixel)
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        set(((R) << 16) + ((G) << 8) + B);
    }
    void set(uint32 p)
    {
        pix = p;
    }
    inline operator uint32() const { return pix; }
    inline void Compress()
    {
    }
};
