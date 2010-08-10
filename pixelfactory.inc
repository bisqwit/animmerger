struct DummyPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
};

struct PixCombination_LastPixel: private DummyPixel, private LastPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
    }
    static uint32 (PixCombination_LastPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LastPixel::*const PixCombination_LastPixel::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &LastPixel::GetLast,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_LastPixel::name[] = "PixCombination_LastPixel";
struct PixCombination_FirstPixel: private DummyPixel, private FirstPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
    }
    static uint32 (PixCombination_FirstPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_FirstPixel::*const PixCombination_FirstPixel::methods[NPixelMethods])(unsigned)const =
{
    &FirstPixel::GetFirst,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_FirstPixel::name[] = "PixCombination_FirstPixel";
struct PixCombination_LastPixel_FirstPixel: private DummyPixel, private LastPixel, private FirstPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        FirstPixel::set(timer,v);
    }
    static uint32 (PixCombination_LastPixel_FirstPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LastPixel_FirstPixel::*const PixCombination_LastPixel_FirstPixel::methods[NPixelMethods])(unsigned)const =
{
    &FirstPixel::GetFirst,
    &LastPixel::GetLast,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_LastPixel_FirstPixel::name[] = "PixCombination_LastPixel_FirstPixel";
struct PixCombination_LoopingLogPixel: private DummyPixel, private LoopingLogPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixCombination_LoopingLogPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LoopingLogPixel::*const PixCombination_LoopingLogPixel::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &LoopingLogPixel::GetLoopingLog,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetActionAvg,
    &LoopingLogPixel::GetLeastUsed,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_LoopingLogPixel::name[] = "PixCombination_LoopingLogPixel";
struct PixCombination_FirstPixel_LoopingLogPixel: private DummyPixel, private FirstPixel, private LoopingLogPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixCombination_FirstPixel_LoopingLogPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_FirstPixel_LoopingLogPixel::*const PixCombination_FirstPixel_LoopingLogPixel::methods[NPixelMethods])(unsigned)const =
{
    &FirstPixel::GetFirst,
    &DummyPixel::GetDummy,
    &LoopingLogPixel::GetLoopingLog,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetActionAvg,
    &LoopingLogPixel::GetLeastUsed,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_FirstPixel_LoopingLogPixel::name[] = "PixCombination_FirstPixel_LoopingLogPixel";
struct PixCombination_LastPixel_LoopingLogPixel: private DummyPixel, private LastPixel, private LoopingLogPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixCombination_LastPixel_LoopingLogPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LastPixel_LoopingLogPixel::*const PixCombination_LastPixel_LoopingLogPixel::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &LastPixel::GetLast,
    &LoopingLogPixel::GetLoopingLog,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetActionAvg,
    &LoopingLogPixel::GetLeastUsed,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_LastPixel_LoopingLogPixel::name[] = "PixCombination_LastPixel_LoopingLogPixel";
struct PixCombination_LastPixel_FirstPixel_LoopingLogPixel: private DummyPixel, private LastPixel, private FirstPixel, private LoopingLogPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        FirstPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixCombination_LastPixel_FirstPixel_LoopingLogPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LastPixel_FirstPixel_LoopingLogPixel::*const PixCombination_LastPixel_FirstPixel_LoopingLogPixel::methods[NPixelMethods])(unsigned)const =
{
    &FirstPixel::GetFirst,
    &LastPixel::GetLast,
    &LoopingLogPixel::GetLoopingLog,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetActionAvg,
    &LoopingLogPixel::GetLeastUsed,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_LastPixel_FirstPixel_LoopingLogPixel::name[] = "PixCombination_LastPixel_FirstPixel_LoopingLogPixel";
struct PixCombination_AveragePixel: private DummyPixel, private AveragePixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        AveragePixel::set(timer,v);
    }
    static uint32 (PixCombination_AveragePixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_AveragePixel::*const PixCombination_AveragePixel::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &AveragePixel::GetAverage,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_AveragePixel::name[] = "PixCombination_AveragePixel";
struct PixCombination_FirstPixel_AveragePixel: private DummyPixel, private FirstPixel, private AveragePixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixCombination_FirstPixel_AveragePixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_FirstPixel_AveragePixel::*const PixCombination_FirstPixel_AveragePixel::methods[NPixelMethods])(unsigned)const =
{
    &FirstPixel::GetFirst,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &AveragePixel::GetAverage,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_FirstPixel_AveragePixel::name[] = "PixCombination_FirstPixel_AveragePixel";
struct PixCombination_LastPixel_AveragePixel: private DummyPixel, private LastPixel, private AveragePixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixCombination_LastPixel_AveragePixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LastPixel_AveragePixel::*const PixCombination_LastPixel_AveragePixel::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &LastPixel::GetLast,
    &DummyPixel::GetDummy,
    &AveragePixel::GetAverage,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_LastPixel_AveragePixel::name[] = "PixCombination_LastPixel_AveragePixel";
struct PixCombination_LastPixel_FirstPixel_AveragePixel: private DummyPixel, private LastPixel, private FirstPixel, private AveragePixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        FirstPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixCombination_LastPixel_FirstPixel_AveragePixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LastPixel_FirstPixel_AveragePixel::*const PixCombination_LastPixel_FirstPixel_AveragePixel::methods[NPixelMethods])(unsigned)const =
{
    &FirstPixel::GetFirst,
    &LastPixel::GetLast,
    &DummyPixel::GetDummy,
    &AveragePixel::GetAverage,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_LastPixel_FirstPixel_AveragePixel::name[] = "PixCombination_LastPixel_FirstPixel_AveragePixel";
struct PixCombination_MostUsedPixel: private DummyPixel, private MostUsedPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        MostUsedPixel::set(timer,v);
    }
    static uint32 (PixCombination_MostUsedPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_MostUsedPixel::*const PixCombination_MostUsedPixel::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &MostUsedPixel::GetAverage,
    &MostUsedPixel::GetMostUsed,
    &MostUsedPixel::GetActionAvg,
    &MostUsedPixel::GetLeastUsed,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
const char PixCombination_MostUsedPixel::name[] = "PixCombination_MostUsedPixel";
struct PixCombination_ChangeLogPixel: private DummyPixel, private ChangeLogPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        ChangeLogPixel::set(timer,v);
    }
    static uint32 (PixCombination_ChangeLogPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_ChangeLogPixel::*const PixCombination_ChangeLogPixel::methods[NPixelMethods])(unsigned)const =
{
    &ChangeLogPixel::GetFirst,
    &ChangeLogPixel::GetLast,
    &DummyPixel::GetDummy,
    &ChangeLogPixel::GetAverage,
    &ChangeLogPixel::GetMostUsed,
    &ChangeLogPixel::GetActionAvg,
    &ChangeLogPixel::GetLeastUsed,
    &ChangeLogPixel::GetChangeLog,
    &ChangeLogPixel::GetLoopingAvg,
    &ChangeLogPixel::GetLoopingLast,
    &ChangeLogPixel::GetFirstNMost,
    &ChangeLogPixel::GetLastNMost,
};
const char PixCombination_ChangeLogPixel::name[] = "PixCombination_ChangeLogPixel";
struct PixCombination_LoopingLogPixel_ChangeLogPixel: private DummyPixel, private LoopingLogPixel, private ChangeLogPixel
{
    static const char name[];
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LoopingLogPixel::set(timer,v);
        ChangeLogPixel::set(timer,v);
    }
    static uint32 (PixCombination_LoopingLogPixel_ChangeLogPixel::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixCombination_LoopingLogPixel_ChangeLogPixel::*const PixCombination_LoopingLogPixel_ChangeLogPixel::methods[NPixelMethods])(unsigned)const =
{
    &ChangeLogPixel::GetFirst,
    &ChangeLogPixel::GetLast,
    &LoopingLogPixel::GetLoopingLog,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetActionAvg,
    &LoopingLogPixel::GetLeastUsed,
    &ChangeLogPixel::GetChangeLog,
    &ChangeLogPixel::GetLoopingAvg,
    &ChangeLogPixel::GetLoopingLast,
    &ChangeLogPixel::GetFirstNMost,
    &ChangeLogPixel::GetLastNMost,
};
const char PixCombination_LoopingLogPixel_ChangeLogPixel::name[] = "PixCombination_LoopingLogPixel_ChangeLogPixel";

const FactoryType* FindFactory(unsigned index)
{
    static const FactoryType* const table[21] =
    {
    /*  0 */ &FactoryMethods<PixCombination_FirstPixel>::data,
    /*  1 */ &FactoryMethods<PixCombination_LastPixel>::data,
    /*  2 */ &FactoryMethods<PixCombination_LastPixel_FirstPixel>::data,
    /*  3 */ &FactoryMethods<PixCombination_LoopingLogPixel>::data,
    /*  4 */ &FactoryMethods<PixCombination_FirstPixel_LoopingLogPixel>::data,
    /*  5 */ &FactoryMethods<PixCombination_LastPixel_LoopingLogPixel>::data,
    /*  6 */ &FactoryMethods<PixCombination_LastPixel_FirstPixel_LoopingLogPixel>::data,
    /*  7 */ &FactoryMethods<PixCombination_AveragePixel>::data,
    /*  8 */ &FactoryMethods<PixCombination_FirstPixel_AveragePixel>::data,
    /*  9 */ &FactoryMethods<PixCombination_LastPixel_AveragePixel>::data,
    /* 10 */ &FactoryMethods<PixCombination_LastPixel_FirstPixel_AveragePixel>::data,
    /* 11 */ &FactoryMethods<PixCombination_MostUsedPixel>::data,
    /* 12 */ &FactoryMethods<PixCombination_ChangeLogPixel>::data,
    /* 13 */ &FactoryMethods<PixCombination_ChangeLogPixel>::data,
    /* 14 */ &FactoryMethods<PixCombination_ChangeLogPixel>::data,
    /* 15 */ &FactoryMethods<PixCombination_LoopingLogPixel>::data,
    /* 16 */ &FactoryMethods<PixCombination_FirstPixel_LoopingLogPixel>::data,
    /* 17 */ &FactoryMethods<PixCombination_LastPixel_LoopingLogPixel>::data,
    /* 18 */ &FactoryMethods<PixCombination_LastPixel_FirstPixel_LoopingLogPixel>::data,
    /* 19 */ &FactoryMethods<PixCombination_ChangeLogPixel>::data,
    /* 20 */ &FactoryMethods<PixCombination_LoopingLogPixel_ChangeLogPixel>::data,
    };
/* of remaining 4096 [0..4095], coverage 4096, miscoverage 128, best_score 2032.02, config=(add:0,mask:4) */
    if(index < 128) {
/* of remaining 128 [0..127], coverage 128, miscoverage 12, best_score 14.52, config=(add:0,mask:7) */
    if(index < 12) {
/* of remaining 12 [0..11], coverage 12, miscoverage 4, best_score 1.02, config=(add:0,mask:7) */
    return table[(index - 1) + 0];
    } /* 0..11 */
    return table[(((index + 0) >> 0) & 7) + 11];
    } /* 0..127 */
    return table[(((index + 0) >> 2) & 1) + 19];
    return 0;
}

