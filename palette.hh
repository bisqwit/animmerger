#ifndef bqtTileTrackerPaletteHH
#define bqtTileTrackerPaletteHH

#include <map>
#include <string>
#include <cmath>

#include "types.hh"
#include "alloc/FSBAllocator.hh"

#include "kdtree.hh"

#define DefinePaletteMethods(callback) \
    callback(m,MedianCut) \
    callback(d,Diversity) \
    callback(b,BlendDiversity) \
    callback(g,Merging) \
    callback(o,Octree) \
    callback(q,NeuQuant)

#define MakeEnum(o,name) quant_##name,
enum PaletteMethod { DefinePaletteMethods(MakeEnum) };
#undef MakeEnum

struct PaletteMethodItem
{
    PaletteMethod method;
    unsigned      size;
    std::string   filename;
};
extern std::vector<PaletteMethodItem> PaletteReductionMethod;

extern double DitherErrorFactor; // 0 .. 1
extern unsigned DitherMatrixWidth;
extern unsigned DitherMatrixHeight;
extern unsigned TemporalDitherSize; // 1 = no temporal dithering
extern bool     TemporalDitherMSB;  // Use MSB rather than LSB for temporal dithering
extern unsigned DitherColorListSize;
extern double   DitherCombinationContrast;
extern double   DitherGamma;
extern enum ColorCompareMethod
{
    Compare_RGB,
    Compare_CIE76_DeltaE,
    Compare_CIE94_DeltaE,
    Compare_CMC_lc,
    Compare_BFD_lc,
    Compare_CIEDE2000_DeltaE,
    Compare_fparser
} ColorComparing;
extern enum DitheringMethod
{
    Dither_KnollYliluoma,
    Dither_Yliluoma1,
    Dither_Yliluoma2,
    Dither_Yliluoma3
} Dithering;
extern enum DiffusionMethod
{
    Diffusion_None,
    Diffusion_FloydSteinberg,
    Diffusion_JarvisJudiceNinke,
    Diffusion_Stucki,
    Diffusion_Burkes,
    Diffusion_Sierra3,
    Diffusion_Sierra2,
    Diffusion_Sierra24A,
    Diffusion_StevensonArce,
    Diffusion_Atkinson
} Diffusion;

inline double GammaCorrect(double x)
{
    //return std::pow(x, DitherGamma);
    if(DitherGamma == 1.0) return x;
    if(DitherGamma == 2.0) return x*x;
    if(DitherGamma == 1.5) return x * std::sqrt(x);
    return std::exp(std::log(x) * DitherGamma);
}
inline double GammaUncorrect(double x)
{
    //return std::pow(x, 1.0 / DitherGamma);
    if(DitherGamma == 1.0) return x;
    if(DitherGamma == 2.0) return std::sqrt(x);
    return std::exp(std::log(x) / DitherGamma);
}

struct ColorInfo
{
    // 32-bit ARGB, as it is saved/loaded with a file:
    union
    {
        uint32 rgb;
      #ifdef IS_BIG_ENDIAN
        struct { unsigned char A,R,G,B; };
      #else
        struct { unsigned char B,G,R,A; };
      #endif
    };
    // Luma in 0..255000 range:
    int     luma;
    // ARGB, gamma corrected, all values in 0..1 range:
    struct
    {
        double  r,g,b,a;
    } gammac;
    // CIE XYZ value
    struct
    {
        double  X,Y,Z;
    } xyz;
    // CIE L*a*b* value
    struct
    {
        double  L,a,b, C,h;
    } lab;
public:
    // Dummy constructor
    ColorInfo() {}
    // Constructor with 32-bit ARGB value
    ColorInfo(uint32 val);
    // Constructor with 32-bit ARGB value in separate fields
    ColorInfo(int rr,int gg,int bb, int aa=0);
    // Constructor with 32-bit ARGB value + gamma-corrected values
    // Use this constructor if you already have the gamma-corrected
    // values, so they will not need to be recalculated.
    ColorInfo(uint32 val, double rr,double gg,double bb,double aa);
private:
    template<bool HaveLuma, bool HaveGamma, bool HaveXYZ, bool HaveLAB>
    inline void MakeNeeded();
};

struct Palette
{
    void SetHardcoded(unsigned n, ...);

    void Analyze(); // Assigns luma to each element.

    size_t Size() const { return Data.size(); }
    size_t NumCombinations() const { return Combinations.size(); }

    uint32 GetColor(unsigned index) const { return Data[index].rgb; }
    int    GetLuma(unsigned index) const { return Data[index].luma; }
    const ColorInfo& GetMeta(unsigned index) const { return Data[index]; }

    uint32 GetCombinationColor(unsigned index) const { return Combinations[index].combination.rgb; }
    int    GetCombinationLuma(unsigned index) const { return Combinations[index].combination.luma; }
    const ColorInfo& GetCombinationMeta(unsigned index) const { return Combinations[index].combination; }

    Palette GetSlice(unsigned offset, unsigned count) const;
    void AddPaletteRGB(uint32 p);
public:
    struct PaletteItem: public ColorInfo
    {
        PaletteItem()         : ColorInfo()  {}
        PaletteItem(uint32 v) : ColorInfo(v) { }
    };
    std::vector<PaletteItem> Data;
    struct Combination
    {
        std::vector<unsigned> indexlist;
        ColorInfo combination;

        Combination(const std::vector<unsigned>& i, uint32 v)
            : indexlist(i), combination(v) { }
        Combination(const std::vector<unsigned>& i, uint32 v,
                    double r,double g,double b,double a)
            : indexlist(i), combination(v, r,g,b,a) { }
    };
    std::vector<Combination> Combinations;

    KDTree<unsigned,4> CombinationTree;

    std::pair<unsigned,double>
        FindClosestCombinationIndex(const ColorInfo& meta) const;
};

typedef std::vector<unsigned short> MixingPlan;

MixingPlan FindBestMixingPlan(const ColorInfo& input,  const Palette& Palette);

struct HistogramType
    : public std::map<uint32, unsigned, std::less<uint32>, FSBAllocator<int> >
{
};

void ReduceHistogram(HistogramType& Histogram);
Palette MakePalette(const HistogramType& hist, unsigned MaxColors);

std::vector<unsigned> CreateDispersedDitheringMatrix();
std::vector<unsigned> CreateTemporalDitheringMatrix();

double ColorCompare(const ColorInfo&, const ColorInfo& );

void SetColorCompareFormula(const std::string& expr);

#endif
