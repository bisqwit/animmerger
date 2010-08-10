#define MakePixelCombination(alias,type) \
    typedef type alias; \
    template<> \
    const char PixelAccessMethodName<alias>::name[] = #type

MakePixelCombination(t1, LastPixel<>);
MakePixelCombination(t2, FirstPixel<>);
MakePixelCombination(t3, LastPixel<FirstPixel<> >);
MakePixelCombination(t16, LoopingLogPixel<>);
MakePixelCombination(t18, FirstPixel<LoopingLogPixel<> >);
MakePixelCombination(t17, LastPixel<LoopingLogPixel<> >);
MakePixelCombination(t19, LastPixel<FirstPixel<LoopingLogPixel<> > >);
MakePixelCombination(t4, AveragePixel<>);
MakePixelCombination(t6, FirstPixel<AveragePixel<> >);
MakePixelCombination(t5, LastPixel<AveragePixel<> >);
MakePixelCombination(t7, LastPixel<FirstPixel<AveragePixel<> > >);
MakePixelCombination(t8, MostUsedPixel<>);
MakePixelCombination(t32, ChangeLogPixel<>);
MakePixelCombination(t48, LoopingLogPixel<ChangeLogPixel<> >);

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
    /*  0 */ &FactoryMethods<t2>::data,
    /*  1 */ &FactoryMethods<t1>::data,
    /*  2 */ &FactoryMethods<t3>::data,
    /*  3 */ &FactoryMethods<t16>::data,
    /*  4 */ &FactoryMethods<t18>::data,
    /*  5 */ &FactoryMethods<t17>::data,
    /*  6 */ &FactoryMethods<t19>::data,
    /*  7 */ &FactoryMethods<t4>::data,
    /*  8 */ &FactoryMethods<t6>::data,
    /*  9 */ &FactoryMethods<t5>::data,
    /* 10 */ &FactoryMethods<t7>::data,
    /* 11 */ &FactoryMethods<t8>::data,
    /* 12 */ &FactoryMethods<t32>::data,
    /* 13 */ &FactoryMethods<t32>::data,
    /* 14 */ &FactoryMethods<t32>::data,
    /* 15 */ &FactoryMethods<t16>::data,
    /* 16 */ &FactoryMethods<t18>::data,
    /* 17 */ &FactoryMethods<t17>::data,
    /* 18 */ &FactoryMethods<t19>::data,
    /* 19 */ &FactoryMethods<t32>::data,
    /* 20 */ &FactoryMethods<t48>::data,
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
