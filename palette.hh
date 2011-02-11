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
    return std::pow(x, DitherGamma);
}
inline double GammaUncorrect(double x)
{
    return std::pow(x, 1.0 / DitherGamma);
}

struct XYZitem { double X,Y,Z;      };
struct LabItem { double L,a,b, C,h; };
LabItem XYZtoLAB(const XYZitem& xyz);
LabItem RGBtoLAB(int r,int g,int b);
struct LabAndLuma
{
    LabItem lab;
    int     luma;
    uint32  rgb;     // The rgb to be saved to a file, and input to LAB
    double  r,g,b,a; // gamma corrected

    LabAndLuma() {}
    LabAndLuma(uint32 val);
    LabAndLuma(int R,int G,int B, int A=0);
    LabAndLuma(uint32 val, double R,double G,double B,double A);
};

struct Palette
{
    void SetHardcoded(unsigned n, ...);

    void Analyze(); // Assigns luma to each element.

    size_t Size() const { return Data.size(); }
    size_t NumCombinations() const { return Combinations.size(); }

    uint32 GetColor(unsigned index) const { return Data[index].rgb; }
    int    GetLuma(unsigned index) const { return Data[index].luma; }
    const LabAndLuma& GetMeta(unsigned index) const { return Data[index]; }

    uint32 GetCombinationColor(unsigned index) const { return Combinations[index].combination.rgb; }
    int    GetCombinationLuma(unsigned index) const { return Combinations[index].combination.luma; }
    const LabAndLuma& GetCombinationMeta(unsigned index) const { return Combinations[index].combination; }

    Palette GetSlice(unsigned offset, unsigned count) const;
    void AddPaletteRGB(uint32 p);
public:
    struct PaletteItem: public LabAndLuma
    {
        PaletteItem()         : LabAndLuma()  {}
        PaletteItem(uint32 v) : LabAndLuma(v) { }
    };
    std::vector<PaletteItem> Data;
    struct Combination
    {
        std::vector<unsigned> indexlist;
        LabAndLuma combination;

        Combination(const std::vector<unsigned>& i, uint32 v)
            : indexlist(i), combination(v) { }
        Combination(const std::vector<unsigned>& i, uint32 v,
                    double r,double g,double b,double a)
            : indexlist(i), combination(v, r,g,b,a) { }
    };
    std::vector<Combination> Combinations;

    KDTree<unsigned,4> CombinationTree;

    std::pair<unsigned,double>
        FindClosestCombinationIndex(const LabAndLuma& meta) const;
};

typedef std::vector<unsigned short> MixingPlan;

MixingPlan FindBestMixingPlan(const LabAndLuma& input,  const Palette& Palette);

struct HistogramType
    : public std::map<uint32, unsigned, std::less<uint32>, FSBAllocator<int> >
{
};

void ReduceHistogram(HistogramType& Histogram);
Palette MakePalette(const HistogramType& hist, unsigned MaxColors);

std::vector<unsigned> CreateDispersedDitheringMatrix();
std::vector<unsigned> CreateTemporalDitheringMatrix();

double ColorCompare(const LabAndLuma&, const LabAndLuma& );

void SetColorCompareFormula(const std::string& expr);

#endif
