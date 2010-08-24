namespace AvgYUV
{
    static const double Umax = 0.436;
    static const double Vmax = 0.615;
    static const double Wr   = 0.299;
    static const double Wb   = 0.114;
    static const double Wg   = 1.0 - Wr - Wb;
}
class AveragePixel
{
    union
    {
        struct { unsigned r,g,b; };
        struct { float    y,u,v; };
    };
    unsigned n;
public:
    AveragePixel() : n(0)
    {
        if(AveragesInYUV)
            y=u=v=0.f;
        else
            r=g=b=0u;
    }
    inline void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        set_n(p, 1);
    }
    void set_n(uint32 p, unsigned count) FasterPixelMethod
    {
        set_n_rgb((p>>16)&0xFF, (p>>8)&0xFF, p&0xFF, count);
    }
    inline void set_rgb(unsigned R,unsigned G,unsigned B, unsigned=0) FasterPixelMethod
    {
        set_n_rgb(R,G,B, 1);
    }
    void set_n_rgb(unsigned R,unsigned G,unsigned B, unsigned count) FastPixelMethod
    {
        if(AveragesInYUV)
        {
            using namespace AvgYUV;
            double Y = R*(Wr/255.0)
                     + G*(Wg/255.0)
                     + B*(Wb/255.0);
            double U = (Umax/(1-(Wb))) * (B/255.0-Y);
            double V = (Vmax/(1-(Wr))) * (R/255.0-Y);
            double c = count;
            y += Y*c;
            u += U*c;
            v += V*c;
        }
        else
        {
            r+=R*count; g+=G*count; b+=B*count;
        }
        n += count;
    }

    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return GetAverage();
    }

    uint32 GetAverage(unsigned=0) const FastPixelMethod
    {
        if(!n) return DefaultPixel;
        if(AveragesInYUV)
        {
            using namespace AvgYUV;
            double invN = 1.0 / n;
            double Y = double(y)*invN, U = double(u)*invN, V = double(v)*invN;
            int r=0,g=0,b=0;
            for(int run=0; run<1000; ++run)
            {
                r = (int)( /*0.5 +*/ 255.0*(Y + V * (   (1-Wr)/(Vmax   ))));
                g = (int)( /*0.5 +*/ 255.0*(Y - U * (Wb*(1-Wb)/(Umax*Wg))
                                              - V * (Wr*(1-Wr)/(Vmax*Wg))
                         )             );
                b = (int)( /*0.5 +*/ 255.0*(Y + U * (   (1-Wb)/(Umax   ))));
                if(r>=0 && r<=255
                && g>=0 && g<=255
                && b>=0 && b<=255)
                {
                    return (((r) << 16) + ((g) << 8) + (b));
                }
                /* In case of overflow, reduce chroma but keep luma unchanged */
                U *= 0.99;
                V *= 0.99;
            }
            if(r<0) r=0; if(r>255) r=255;
            if(g<0) g=0; if(g>255) g=255;
            if(b<0) b=0; if(b>255) b=255;
            return (((r) << 16) + ((g) << 8) + (b));
        }
        else
        {
            return ((((r+n/2)/n) << 16) + (((g+n/2)/n) << 8) + ((b+n/2)/n));
        }
    }

/////////
    static const unsigned long Traits =
        (1ul << pm_AveragePixel);
    static const unsigned SizePenalty = 0;

    // These properties are made conditional on pixel.cc's inclusion,
    // because AveragePixel is also used by main.cc and canvas.cc
  #ifdef DefinePixelImpls
    static const unsigned Components = (1ul << impl_Average);
  #else
    static const unsigned Components = 0;
  #endif
};
