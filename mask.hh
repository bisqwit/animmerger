#include "settype.hh"
#include "vectype.hh"
#include "types.hh"

struct AlphaRange
{
    unsigned x1,y1, width,height;
    SetType<unsigned> colors;
};

enum MaskMethod
{
    MaskHole,
    MaskCircularBlur,
    MaskPattern,
    MaskBlack
};

extern VecType<AlphaRange> alpha_ranges;
extern MaskMethod maskmethod;

void MaskImage(VecType<uint32>& pixels, unsigned sx, unsigned sy);
