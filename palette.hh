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
    unsigned luma;

    LabAndLuma() {}
    LabAndLuma(uint32 rgb);
    LabAndLuma(int r,int g,int b);
};

struct Palette
{
    void SetHardcoded(unsigned n, ...);

    void Analyze(); // Assigns luma to each element.

    size_t Size() const { return Data.size(); }
    size_t NumCombinations() const { return Combinations.size(); }

    uint32 GetColor(unsigned index) const { return Data[index].rgb; }
    unsigned GetLuma(unsigned index) const { return Data[index].meta.luma; }
    const LabItem& GetLAB(unsigned index) const { return Data[index].meta.lab; }
    const LabAndLuma& GetMeta(unsigned index) const { return Data[index].meta; }

    uint32 GetCombinationColor(unsigned index) const { return Combinations[index].combination.rgb; }
    unsigned GetCombinationLuma(unsigned index) const { return Combinations[index].combination.meta.luma; }
    const LabItem& GetCombinationLAB(unsigned index) const { return Combinations[index].combination.meta.lab; }
    const LabAndLuma& GetCombinationMeta(unsigned index) const { return Combinations[index].combination.meta; }

    Palette GetSlice(unsigned offset, unsigned count) const;
    void AddPaletteRGB(uint32 p);
public:
    struct DataItem
    {
        uint32      rgb;   // The rgb to be saved to a file, and input to LAB
        LabAndLuma  meta;
        double      r,g,b; // Gamma corrected 

        DataItem() : rgb(0), meta(), r(0),g(0),b(0) { }
        DataItem(uint32 v);
        DataItem(uint32 v, double R,double G,double B);
    };
    struct PaletteItem: public DataItem
    {
        PaletteItem()         : DataItem()  {}
        PaletteItem(uint32 v) : DataItem(v) { }
    };
    std::vector<PaletteItem> Data;
    struct Combination
    {
        std::vector<unsigned> indexlist;
        DataItem combination;

        Combination(const std::vector<unsigned>& i, uint32 v)
            : indexlist(i), combination(v) { }
        Combination(const std::vector<unsigned>& i, uint32 v, double r,double g,double b)
            : indexlist(i), combination(v, r,g,b) { }
    };
    std::vector<Combination> Combinations;

    KDTree<unsigned> DataTree;
    KDTree<unsigned> CombinationTree;

    std::pair<unsigned,double>
        FindClosestDataIndex(int r,int g,int b, const LabAndLuma& meta) const;
    std::pair<unsigned,double>
        FindClosestCombinationIndex(int r,int g,int b, const LabAndLuma& meta) const;
};

typedef std::vector<unsigned short> MixingPlan;

MixingPlan FindBestMixingPlan(int r,int g,int b, const Palette& Palette);

struct HistogramType: public std::map<uint32, unsigned, std::less<uint32>, FSBAllocator<int> >
{
};

void ReduceHistogram(HistogramType& Histogram);
Palette MakePalette(const HistogramType& hist, unsigned MaxColors);

std::vector<unsigned> CreateDispersedDitheringMatrix();
std::vector<unsigned> CreateTemporalDitheringMatrix();

double ColorCompare(int r1,int g1,int b1, // 0..255
                    const LabAndLuma&,
                    int r2,int g2,int b2, // 0..255
                    const LabAndLuma& );

void SetColorCompareFormula(const std::string& expr);

#endif
