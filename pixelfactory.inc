/* FindFactory() returns a FactoryType for the smallest pixel
 * type that implements all features indicated in @featuremask.
 * It does so by the means of an algorithm generated
 * by the makepixels program provided in animmerger
 * source code package.
 * @featuremask is a bitmask of requested PixelMethods.
 * Range: 0 < @featuremask < 4096
 * where 4096 = 2**NPixelMethods
 */
const FactoryType* FindFactory(unsigned featuremask)
{
    static const FactoryType* const table[21] =
    {
    /*  0 */ &FactoryMethods<PixelMethodImplComb<2>::result>::data,
    /*  1 */ &FactoryMethods<PixelMethodImplComb<1>::result>::data,
    /*  2 */ &FactoryMethods<PixelMethodImplComb<3>::result>::data,
    /*  3 */ &FactoryMethods<PixelMethodImplComb<16>::result>::data,
    /*  4 */ &FactoryMethods<PixelMethodImplComb<18>::result>::data,
    /*  5 */ &FactoryMethods<PixelMethodImplComb<17>::result>::data,
    /*  6 */ &FactoryMethods<PixelMethodImplComb<19>::result>::data,
    /*  7 */ &FactoryMethods<PixelMethodImplComb<4>::result>::data,
    /*  8 */ &FactoryMethods<PixelMethodImplComb<6>::result>::data,
    /*  9 */ &FactoryMethods<PixelMethodImplComb<5>::result>::data,
    /* 10 */ &FactoryMethods<PixelMethodImplComb<7>::result>::data,
    /* 11 */ &FactoryMethods<PixelMethodImplComb<8>::result>::data,
    /* 12 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data,
    /* 13 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data,
    /* 14 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data,
    /* 15 */ &FactoryMethods<PixelMethodImplComb<16>::result>::data,
    /* 16 */ &FactoryMethods<PixelMethodImplComb<18>::result>::data,
    /* 17 */ &FactoryMethods<PixelMethodImplComb<17>::result>::data,
    /* 18 */ &FactoryMethods<PixelMethodImplComb<19>::result>::data,
    /* 19 */ &FactoryMethods<PixelMethodImplComb<32>::result>::data,
    /* 20 */ &FactoryMethods<PixelMethodImplComb<48>::result>::data,
    };
/* of remaining 4096 [0..4095], coverage 4096, miscoverage 128, best_score 2032.02, config=(add:0,mask:4) */
    if(featuremask < 128) {
/* of remaining 128 [0..127], coverage 128, miscoverage 12, best_score 14.52, config=(add:0,mask:7) */
    if(featuremask < 12) {
/* of remaining 12 [0..11], coverage 12, miscoverage 4, best_score 1.02, config=(add:0,mask:7) */
    return table[(featuremask - 1) + 0];
    } /* 0..11 */
    return table[(((featuremask + 0) >> 0) & 7) + 11];
    } /* 0..127 */
    return table[(((featuremask + 0) >> 2) & 1) + 19];
    return 0;
}
