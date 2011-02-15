#ifndef bqtTileTrackerHistogramHH
#define bqtTileTrackerHistogramHH

#include <map>
#include "alloc/FSBAllocator.hh"

#include "types.hh"

struct HistogramType
    : public std::map<uint32, unsigned, std::less<uint32>, FSBAllocator<int> >
{
};

void ReduceHistogram(HistogramType& Histogram);

struct Palette MakePalette(const HistogramType& hist, unsigned MaxColors);

#endif
