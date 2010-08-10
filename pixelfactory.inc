const class Factories
{
    static const FactoryType FactoryList[14];
public:
    const FactoryType* operator[] (unsigned index) const
    {
/* of remaining 4096 [0..4095], coverage 4096, miscoverage 128, best_score 2032.02, config=(add:0,mask:1) */
        if(index < 128) {
/* of remaining 128 [0..127], coverage 128, miscoverage 15, best_score 14.145, config=(add:0,mask:7) */
        if(index < 15) {
/* of remaining 15 [0..14], coverage 0, miscoverage 0, best_score 0, config=(add:0,mask:0) */
          static const unsigned char table[15] = {
           0, 1, 2, 3, 0, 4, 5, 6, 7, 1, 8, 3, 9, 4,10};
          return &FactoryList[table[index - 0]];
        } /* 0..14 */
        switch( ((index + 0) >> 0) & 7)
        {
            case 0: return &FactoryList[11];
            case 1: return &FactoryList[1];
            case 2: return &FactoryList[12];
            case 3: return &FactoryList[3];
            case 4: return &FactoryList[12];
            case 5: return &FactoryList[4];
            case 6: return &FactoryList[12];
            case 7: return &FactoryList[6];
        }
        } /* 0..127 */
        switch( ((index + 0) >> 0) & 1)
        {
            case 0: return &FactoryList[12];
            case 1: return &FactoryList[13];
        }
        return 0;
    }
} Factories = {};

struct DummyPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
};

struct PixelMethod1: private DummyPixel, private LastPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
    }
    static uint32 (PixelMethod1::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod1::*const PixelMethod1::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
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
};
struct PixelMethod20: private DummyPixel, private LoopingLogPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod20::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod20::*const PixelMethod20::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetLoopingLog,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
struct PixelMethod2: private DummyPixel, private FirstPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
    }
    static uint32 (PixelMethod2::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod2::*const PixelMethod2::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
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
};
struct PixelMethod22: private DummyPixel, private FirstPixel, private LoopingLogPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod22::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod22::*const PixelMethod22::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetLoopingLog,
    &FirstPixel::GetFirst,
    &DummyPixel::GetDummy,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
struct PixelMethod21: private DummyPixel, private LastPixel, private LoopingLogPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod21::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod21::*const PixelMethod21::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetLoopingLog,
    &DummyPixel::GetDummy,
    &LastPixel::GetLast,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
struct PixelMethod3: private DummyPixel, private LastPixel, private FirstPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        FirstPixel::set(timer,v);
    }
    static uint32 (PixelMethod3::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod3::*const PixelMethod3::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
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
};
struct PixelMethod23: private DummyPixel, private LastPixel, private FirstPixel, private LoopingLogPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        FirstPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod23::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod23::*const PixelMethod23::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetLoopingLog,
    &FirstPixel::GetFirst,
    &LastPixel::GetLast,
    &LoopingLogPixel::GetAverage,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
struct PixelMethod4: private DummyPixel, private AveragePixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        AveragePixel::set(timer,v);
    }
    static uint32 (PixelMethod4::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod4::*const PixelMethod4::methods[NPixelMethods])(unsigned)const =
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
struct PixelMethod6: private DummyPixel, private FirstPixel, private AveragePixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixelMethod6::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod6::*const PixelMethod6::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &FirstPixel::GetFirst,
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
struct PixelMethod5: private DummyPixel, private LastPixel, private AveragePixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixelMethod5::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod5::*const PixelMethod5::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &LastPixel::GetLast,
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
struct PixelMethod7: private DummyPixel, private LastPixel, private FirstPixel, private AveragePixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        FirstPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixelMethod7::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod7::*const PixelMethod7::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &FirstPixel::GetFirst,
    &LastPixel::GetLast,
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
struct PixelMethod8: private DummyPixel, private MostUsedPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        MostUsedPixel::set(timer,v);
    }
    static uint32 (PixelMethod8::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod8::*const PixelMethod8::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &MostUsedPixel::GetAverage,
    &MostUsedPixel::GetMostUsed,
    &MostUsedPixel::GetLeastUsed,
    &MostUsedPixel::GetActionAvg,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
    &DummyPixel::GetDummy,
};
struct PixelMethod10: private DummyPixel, private ChangeLogPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        ChangeLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod10::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod10::*const PixelMethod10::methods[NPixelMethods])(unsigned)const =
{
    &DummyPixel::GetDummy,
    &ChangeLogPixel::GetFirst,
    &ChangeLogPixel::GetLast,
    &ChangeLogPixel::GetAverage,
    &ChangeLogPixel::GetMostUsed,
    &ChangeLogPixel::GetLeastUsed,
    &ChangeLogPixel::GetActionAvg,
    &ChangeLogPixel::GetChangeLog,
    &ChangeLogPixel::GetLoopingAvg,
    &ChangeLogPixel::GetLoopingLast,
    &ChangeLogPixel::GetFirstNMost,
    &ChangeLogPixel::GetLastNMost,
};
struct PixelMethod30: private DummyPixel, private ChangeLogPixel, private LoopingLogPixel
{
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        ChangeLogPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod30::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod30::*const PixelMethod30::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetLoopingLog,
    &ChangeLogPixel::GetFirst,
    &ChangeLogPixel::GetLast,
    &ChangeLogPixel::GetAverage,
    &ChangeLogPixel::GetMostUsed,
    &ChangeLogPixel::GetLeastUsed,
    &ChangeLogPixel::GetActionAvg,
    &ChangeLogPixel::GetChangeLog,
    &ChangeLogPixel::GetLoopingAvg,
    &ChangeLogPixel::GetLoopingLast,
    &ChangeLogPixel::GetFirstNMost,
    &ChangeLogPixel::GetLastNMost,
};

const FactoryType Factories::FactoryList[] =
{
/*0	*/
{ FactoryMethods<PixelMethod1>::Construct,
  FactoryMethods<PixelMethod1>::Copy,
  FactoryMethods<PixelMethod1>::Assign },
/*1	*/
{ FactoryMethods<PixelMethod20>::Construct,
  FactoryMethods<PixelMethod20>::Copy,
  FactoryMethods<PixelMethod20>::Assign },
/*2	*/
{ FactoryMethods<PixelMethod2>::Construct,
  FactoryMethods<PixelMethod2>::Copy,
  FactoryMethods<PixelMethod2>::Assign },
/*3	*/
{ FactoryMethods<PixelMethod22>::Construct,
  FactoryMethods<PixelMethod22>::Copy,
  FactoryMethods<PixelMethod22>::Assign },
/*4	*/
{ FactoryMethods<PixelMethod21>::Construct,
  FactoryMethods<PixelMethod21>::Copy,
  FactoryMethods<PixelMethod21>::Assign },
/*5	*/
{ FactoryMethods<PixelMethod3>::Construct,
  FactoryMethods<PixelMethod3>::Copy,
  FactoryMethods<PixelMethod3>::Assign },
/*6	*/
{ FactoryMethods<PixelMethod23>::Construct,
  FactoryMethods<PixelMethod23>::Copy,
  FactoryMethods<PixelMethod23>::Assign },
/*7	*/
{ FactoryMethods<PixelMethod4>::Construct,
  FactoryMethods<PixelMethod4>::Copy,
  FactoryMethods<PixelMethod4>::Assign },
/*8	*/
{ FactoryMethods<PixelMethod6>::Construct,
  FactoryMethods<PixelMethod6>::Copy,
  FactoryMethods<PixelMethod6>::Assign },
/*9	*/
{ FactoryMethods<PixelMethod5>::Construct,
  FactoryMethods<PixelMethod5>::Copy,
  FactoryMethods<PixelMethod5>::Assign },
/*10	*/
{ FactoryMethods<PixelMethod7>::Construct,
  FactoryMethods<PixelMethod7>::Copy,
  FactoryMethods<PixelMethod7>::Assign },
/*11	*/
{ FactoryMethods<PixelMethod8>::Construct,
  FactoryMethods<PixelMethod8>::Copy,
  FactoryMethods<PixelMethod8>::Assign },
/*12	*/
{ FactoryMethods<PixelMethod10>::Construct,
  FactoryMethods<PixelMethod10>::Copy,
  FactoryMethods<PixelMethod10>::Assign },
/*13	*/
{ FactoryMethods<PixelMethod30>::Construct,
  FactoryMethods<PixelMethod30>::Copy,
  FactoryMethods<PixelMethod30>::Assign },
};
