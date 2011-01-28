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

/* Of the given palette, find the two color indices
 * that most closely resemble the given rgb color.
 * The return value is a "double" value that indicates
 * the degree of mixing between entry1 and entry2
 * that results in most perfect match for the given color.
 * That is, in the ideal world,
 * inputrgb = entry1.rgb + result * (entry2.rgb - entry1.rgb)
 */
struct PalettePair
{
    unsigned short entry1, entry2;
    float result;
};
PalettePair FindBestPalettePair(int r,int g,int b, const uint32* Palette, unsigned PaletteSize);

typedef std::map<uint32, unsigned, std::less<uint32>, FSBAllocator<int> >
    HistogramType;

void ReduceHistogram(HistogramType& Histogram);
unsigned MakePalette(uint32* Palette, const HistogramType& hist, unsigned MaxColors);
void SortPalette(uint32* Palette, unsigned PaletteSize);

#endif
