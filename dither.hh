#ifndef bqtTileTrackerDitherHH
#define bqtTileTrackerDitherHH

#include <vector>

extern double DitherErrorFactor; // 0 .. 1
extern unsigned DitherMatrixWidth;
extern unsigned DitherMatrixHeight;
extern unsigned TemporalDitherSize; // 1 = no temporal dithering
extern bool     TemporalDitherMSB;  // Use MSB rather than LSB for temporal dithering
extern unsigned DitherColorListSize;
extern double   DitherCombinationContrast;
extern unsigned DitherCombinationRecursionLimit;
extern unsigned DitherCombinationChangesLimit;
extern bool     DitherCombinationAllowSame;

extern enum DitheringMethod
{
    Dither_Yliluoma1Iterative,
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

std::vector<unsigned> CreateDispersedDitheringMatrix();
std::vector<unsigned> CreateTemporalDitheringMatrix();

struct SectionDithering
{
    // For every section of this size (0=maximum),
    unsigned width, height;
    // This many distinct colors can be used from the current palette.
    unsigned n_colors;

    // However, for every section of this size (0=maximum),
    unsigned width2, height2;
    // only this many distinct sub-palettes can be used. 0 if infinite.
    unsigned combination_limit;
};
extern std::vector<SectionDithering> DitheringSections;

#endif
