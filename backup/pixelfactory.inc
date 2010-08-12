/* FindFactory() returns a FactoryType for the smallest pixel
 * type that implements all features indicated in @featuremask.
 * It does so by the means of an algorithm generated
 * by the makepixels program provided in animmerger
 * source code package.
 * @featuremask is a bitmask of requested PixelMethods.
 * Range: 0 < @featuremask < 8192
 * where 8192 = 2**NPixelMethods
 */
const FactoryType* FindFactory(unsigned featuremask)
{
    static const FactoryType* const table[37] =
    {
    /*  0 */ &PixelImplFactory<2>::data /* 4(+0) bytes: FirstPixel */,
    /*  1 */ &PixelImplFactory<1>::data /* 4(+0) bytes: LastPixel */,
    /*  2 */ &PixelImplFactory<3>::data /* 8(+0) bytes: LastPixel+FirstPixel */,
    /*  3 */ &PixelImplFactory<32>::data /* 32(+24) bytes: LoopingLogPixel */,
    /*  4 */ &PixelImplFactory<34>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /*  5 */ &PixelImplFactory<33>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /*  6 */ &PixelImplFactory<35>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /*  7 */ &PixelImplFactory<8>::data /* 16(+0) bytes: AveragePixel */,
    /*  8 */ &PixelImplFactory<10>::data /* 20(+0) bytes: FirstPixel+AveragePixel */,
    /*  9 */ &PixelImplFactory<9>::data /* 20(+0) bytes: LastPixel+AveragePixel */,
    /* 10 */ &PixelImplFactory<11>::data /* 24(+0) bytes: LastPixel+FirstPixel+AveragePixel */,
    /* 11 */ &PixelImplFactory<32>::data /* 32(+24) bytes: LoopingLogPixel */,
    /* 12 */ &PixelImplFactory<34>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /* 13 */ &PixelImplFactory<33>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /* 14 */ &PixelImplFactory<35>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /* 15 */ &PixelImplFactory<4>::data /* 8(+0) bytes: TinyAveragePixel */,
    /* 16 */ &PixelImplFactory<6>::data /* 12(+0) bytes: FirstPixel+TinyAveragePixel */,
    /* 17 */ &PixelImplFactory<5>::data /* 12(+0) bytes: LastPixel+TinyAveragePixel */,
    /* 18 */ &PixelImplFactory<7>::data /* 16(+0) bytes: LastPixel+FirstPixel+TinyAveragePixel */,
    /* 19 */ &PixelImplFactory<32>::data /* 32(+24) bytes: LoopingLogPixel */,
    /* 20 */ &PixelImplFactory<34>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /* 21 */ &PixelImplFactory<33>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /* 22 */ &PixelImplFactory<35>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /* 23 */ &PixelImplFactory<12>::data /* 24(+0) bytes: TinyAveragePixel+AveragePixel */,
    /* 24 */ &PixelImplFactory<14>::data /* 28(+0) bytes: FirstPixel+TinyAveragePixel+AveragePixel */,
    /* 25 */ &PixelImplFactory<13>::data /* 28(+0) bytes: LastPixel+TinyAveragePixel+AveragePixel */,
    /* 26 */ &PixelImplFactory<15>::data /* 32(+0) bytes: LastPixel+FirstPixel+TinyAveragePixel+AveragePixel */,
    /* 27 */ &PixelImplFactory<16>::data /* 16(+16) bytes: MostUsedPixel */,
    /* 28 */ &PixelImplFactory<18>::data /* 24(+16) bytes: FirstPixel+MostUsedPixel */,
    /* 29 */ &PixelImplFactory<17>::data /* 24(+16) bytes: LastPixel+MostUsedPixel */,
    /* 30 */ &PixelImplFactory<19>::data /* 24(+16) bytes: LastPixel+FirstPixel+MostUsedPixel */,
    /* 31 */ &PixelImplFactory<32>::data /* 32(+24) bytes: LoopingLogPixel */,
    /* 32 */ &PixelImplFactory<34>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /* 33 */ &PixelImplFactory<33>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /* 34 */ &PixelImplFactory<35>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /* 35 */ &PixelImplFactory<64>::data /* 16(+24) bytes: ChangeLogPixel */,
    /* 36 */ &PixelImplFactory<96>::data /* 40(+48) bytes: LoopingLogPixel+ChangeLogPixel */,
    };
/* of remaining 8192 [0..8191], coverage 8192, miscoverage 256, best_score 4064.02, config=(add:0,mask:4) */
    if(featuremask < 256) {
/* of remaining 256 [0..255], coverage 256, miscoverage 28, best_score 28.52, config=(add:0,mask:7) */
    if(featuremask < 28) {
/* of remaining 28 [0..27], coverage 28, miscoverage 12, best_score 0.27, config=(add:0,mask:15) */
    return table[(featuremask - 1) + 0];
    } /* 0..27 */
    return table[(((featuremask + 0) >> 0) & 7) + 27];
    } /* 0..255 */
    return table[(((featuremask + 0) >> 2) & 1) + 35];
    return 0;
}
