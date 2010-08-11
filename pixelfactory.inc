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
    /*  0 */ &FactoryMethods<PixelMethodImplComb<2>::result>::data /* 4(+0) bytes: FirstPixel */,
    /*  1 */ &FactoryMethods<PixelMethodImplComb<1>::result>::data /* 4(+0) bytes: LastPixel */,
    /*  2 */ &FactoryMethods<PixelMethodImplComb<3>::result>::data /* 8(+0) bytes: LastPixel+FirstPixel */,
    /*  3 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data /* 32(+24) bytes: LoopingLogPixel */,
    /*  4 */ &FactoryMethods<PixelMethodImplComb<34>::result>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /*  5 */ &FactoryMethods<PixelMethodImplComb<33>::result>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /*  6 */ &FactoryMethods<PixelMethodImplComb<35>::result>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /*  7 */ &FactoryMethods<PixelMethodImplComb<8>::result>::data /* 16(+0) bytes: AveragePixel */,
    /*  8 */ &FactoryMethods<PixelMethodImplComb<10>::result>::data /* 20(+0) bytes: FirstPixel+AveragePixel */,
    /*  9 */ &FactoryMethods<PixelMethodImplComb<9>::result>::data /* 20(+0) bytes: LastPixel+AveragePixel */,
    /* 10 */ &FactoryMethods<PixelMethodImplComb<11>::result>::data /* 24(+0) bytes: LastPixel+FirstPixel+AveragePixel */,
    /* 11 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data /* 32(+24) bytes: LoopingLogPixel */,
    /* 12 */ &FactoryMethods<PixelMethodImplComb<34>::result>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /* 13 */ &FactoryMethods<PixelMethodImplComb<33>::result>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /* 14 */ &FactoryMethods<PixelMethodImplComb<35>::result>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /* 15 */ &FactoryMethods<PixelMethodImplComb<4>::result>::data /* 8(+0) bytes: TinyAveragePixel */,
    /* 16 */ &FactoryMethods<PixelMethodImplComb<6>::result>::data /* 12(+0) bytes: FirstPixel+TinyAveragePixel */,
    /* 17 */ &FactoryMethods<PixelMethodImplComb<5>::result>::data /* 12(+0) bytes: LastPixel+TinyAveragePixel */,
    /* 18 */ &FactoryMethods<PixelMethodImplComb<7>::result>::data /* 16(+0) bytes: LastPixel+FirstPixel+TinyAveragePixel */,
    /* 19 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data /* 32(+24) bytes: LoopingLogPixel */,
    /* 20 */ &FactoryMethods<PixelMethodImplComb<34>::result>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /* 21 */ &FactoryMethods<PixelMethodImplComb<33>::result>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /* 22 */ &FactoryMethods<PixelMethodImplComb<35>::result>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /* 23 */ &FactoryMethods<PixelMethodImplComb<12>::result>::data /* 24(+0) bytes: TinyAveragePixel+AveragePixel */,
    /* 24 */ &FactoryMethods<PixelMethodImplComb<14>::result>::data /* 28(+0) bytes: FirstPixel+TinyAveragePixel+AveragePixel */,
    /* 25 */ &FactoryMethods<PixelMethodImplComb<13>::result>::data /* 28(+0) bytes: LastPixel+TinyAveragePixel+AveragePixel */,
    /* 26 */ &FactoryMethods<PixelMethodImplComb<15>::result>::data /* 32(+0) bytes: LastPixel+FirstPixel+TinyAveragePixel+AveragePixel */,
    /* 27 */ &FactoryMethods<PixelMethodImplComb<16>::result>::data /* 16(+16) bytes: MostUsedPixel */,
    /* 28 */ &FactoryMethods<PixelMethodImplComb<18>::result>::data /* 24(+16) bytes: FirstPixel+MostUsedPixel */,
    /* 29 */ &FactoryMethods<PixelMethodImplComb<17>::result>::data /* 24(+16) bytes: LastPixel+MostUsedPixel */,
    /* 30 */ &FactoryMethods<PixelMethodImplComb<19>::result>::data /* 24(+16) bytes: LastPixel+FirstPixel+MostUsedPixel */,
    /* 31 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data /* 32(+24) bytes: LoopingLogPixel */,
    /* 32 */ &FactoryMethods<PixelMethodImplComb<34>::result>::data /* 40(+24) bytes: FirstPixel+LoopingLogPixel */,
    /* 33 */ &FactoryMethods<PixelMethodImplComb<33>::result>::data /* 40(+24) bytes: LastPixel+LoopingLogPixel */,
    /* 34 */ &FactoryMethods<PixelMethodImplComb<35>::result>::data /* 40(+24) bytes: LastPixel+FirstPixel+LoopingLogPixel */,
    /* 35 */ &FactoryMethods<PixelMethodImplComb<64>::result>::data /* 16(+24) bytes: ChangeLogPixel */,
    /* 36 */ &FactoryMethods<PixelMethodImplComb<96>::result>::data /* 40(+48) bytes: LoopingLogPixel+ChangeLogPixel */,
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
