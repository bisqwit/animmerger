class TinyAveragePixel
{
    unsigned result, n;
public:
    TinyAveragePixel() : result(0), n(0)
    {
        // "result" stores the average as such:
        //    R in 10 bits
        //    G in 12 bits
        //    B in 10 bits
    }
    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        //     10987654321098765432109876543210
        // in: 00000000RRRRRRRRGGGGGGGGBBBBBBBB
        // out:                        BBBBBBBBbb
        //                     GGGGGGGGgggg
        //             RRRRRRRRrr
        set_n_widergb( (p&0xFF0000u) >> 14,
                       (p&0xFF00u) >> 4,
                       (p&0xFFu)<<2, 1);
    }
    void set_n(uint32 p, unsigned count) FasterPixelMethod
    {
        set_n_widergb( (p&0xFF0000u) >> 14,
                       (p&0xFF00u) >> 4,
                       (p&0xFFu)<<2, count);
    }
    void set_n_widergb(unsigned R,unsigned G,unsigned B, unsigned count) FastPixelMethod
    {
        unsigned r = (result>>22)&0x3FF, g = (result>>10)&0xFFF, b = (result&0x3FF);
        r = (r*n + R*count) / (n+count);
        g = (g*n + G*count) / (n+count);
        b = (b*n + B*count) / (n+count);
        n += count;
        result = (r<<22) | (g<<10) | (b<<0);
    }

    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetTinyAverage();
    }

    inline uint32 GetTinyAverage(unsigned=0) const FasterPixelMethod
    {
        if(!n) return DefaultPixel;
        //     7   6   5   4   3   2   1   0
        //     32103210321032103210321032103210
        //     10987654321098765432109876543210
        // in: RRRRRRRRrrGGGGGGGGggggBBBBBBBBbb
        //out: 00000000RRRRRRRRGGGGGGGGBBBBBBBB
        //                           ^^ = B rshift amount (2)
        //               ^^^^^^ = G rshift amount (6)
        //     ^^^^^^^^ = R rshift amount (8)
      /*
        union
        {
            unsigned char bytes[4];
            uint32 result;
        } dummy;
        dummy.result = result;
        dummy.bytes[0] = result>>2;
        dummy.bytes[1] = result>>14;
        dummy.bytes[2] = result>>24;
        dummy.bytes[3] = 0;
        return dummy.result;
      */
        unsigned char B=result>>2, G=result>>(6+8), R=result>>(8+16);
        return (R<<16) | (G<<8) | B;
        /*
        unsigned res = result >> 2;
        res = (res   &0xFFFFFu) | ((res   >>2) & 0x3FC00000u);
        res = (res   &0xFFu)    | ((res   >>4) & 0xFFFF00u);
        return res;
        */
        return ((result>>2) & 0x0000FFu) // b: drop 2 bits
             | ((result>>6) & 0x00FF00u) // g: drop 4 bits (2+4=6)
             | ((result>>8) & 0xFFFF00u) // r: drop 2 bits (2+4+2=8)
        ;
    }

/////////
    static const unsigned long Traits =
        (1ul << pm_TinyAveragePixel);
    static const unsigned SizePenalty = 0;

    static const unsigned Components =
        (1ul << impl_TinyAverage);
};
