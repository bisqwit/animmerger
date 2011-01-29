#ifndef bqtTileTrackerPaletteHH
#define bqtTileTrackerPaletteHH

#include <map>
#include <string>

#include "types.hh"
#include "alloc/FSBAllocator.hh"

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

struct Palette
{
    void SetHardcoded(unsigned n, ...);

    void Analyze(); // Assigns luma to each element.

    size_t Size() const { return Data.size(); }
    size_t NumCombinations() const { return Combinations.size(); }

    uint32 GetColor(unsigned index) const { return Data[index].rgb; }
    unsigned GetLuma(unsigned index) const { return Data[index].luma; }

    uint32 GetCombinationColor(unsigned index) const { return Combinations[index].combination.rgb; }
    unsigned GetCombinationLuma(unsigned index) const { return Combinations[index].combination.luma; }

    Palette GetSlice(unsigned offset, unsigned count) const;
public:
    struct DataItem
    {
        uint32   rgb;
        unsigned luma;

        DataItem() : rgb(0),luma(0) { }
        DataItem(uint32 v) :
            rgb(v),
            luma(((v>>16)&0xFF)*299u
                + ((v>>8)&0xFF)*587u
                + ( v    &0xFF)*114u) { }

        void SplitRGB(unsigned& r, unsigned& g, unsigned& b) const
            { r = (rgb >> 16); g = (rgb >> 8) & 0xFF; b = rgb & 0xFF; }
    };
    std::vector<DataItem> Data;
    struct Combination
    {
        unsigned indexcount;
        unsigned indexlist[4];
        DataItem combination;

        Combination(unsigned i0,unsigned i1,                         uint32 v)
            : indexcount(2), combination(v)
            { indexlist[0] = i0; indexlist[1] = i1; }
        Combination(unsigned i0,unsigned i1,unsigned i2,             uint32 v)
            : indexcount(3), combination(v)
            { indexlist[0] = i0; indexlist[1] = i1; indexlist[2] = i2; }
        Combination(unsigned i0,unsigned i1,unsigned i2,unsigned i3, uint32 v)
            : indexcount(4), combination(v)
            { indexlist[0] = i0; indexlist[1] = i1; indexlist[2] = i2; indexlist[3] = i3; }
    };
    std::vector<Combination> Combinations;
};

typedef std::vector<unsigned short> PalettePair;

PalettePair FindBestPalettePair(int r,int g,int b, const Palette& Palette);

typedef std::map<uint32, unsigned, std::less<uint32>, FSBAllocator<int> >
    HistogramType;

void ReduceHistogram(HistogramType& Histogram);
Palette MakePalette(const HistogramType& hist, unsigned MaxColors);

std::vector<unsigned> CreateDispersedDitheringMatrix();
std::vector<unsigned> CreateTemporalDitheringMatrix();

#endif
