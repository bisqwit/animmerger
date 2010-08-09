struct PixelMethod1: private LastPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
    }
    static uint32 (PixelMethod1::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod1::*const PixelMethod1::methods[NPixelMethods])(unsigned)const =
{
    &PixelMethod1::GetDummy,
    &LastPixel::GetLast,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
    &PixelMethod1::GetDummy,
};
struct PixelMethod4: private AveragePixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        AveragePixel::set(timer,v);
    }
    static uint32 (PixelMethod4::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod4::*const PixelMethod4::methods[NPixelMethods])(unsigned)const =
{
    &AveragePixel::GetAverage,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
    &PixelMethod4::GetDummy,
};
struct PixelMethod5: private LastPixel,private AveragePixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixelMethod5::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod5::*const PixelMethod5::methods[NPixelMethods])(unsigned)const =
{
    &AveragePixel::GetAverage,
    &LastPixel::GetLast,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
    &PixelMethod5::GetDummy,
};
struct PixelMethod2: private FirstPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
    }
    static uint32 (PixelMethod2::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod2::*const PixelMethod2::methods[NPixelMethods])(unsigned)const =
{
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &FirstPixel::GetFirst,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
    &PixelMethod2::GetDummy,
};
struct PixelMethod6: private FirstPixel,private AveragePixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
        AveragePixel::set(timer,v);
    }
    static uint32 (PixelMethod6::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod6::*const PixelMethod6::methods[NPixelMethods])(unsigned)const =
{
    &AveragePixel::GetAverage,
    &PixelMethod6::GetDummy,
    &FirstPixel::GetFirst,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
    &PixelMethod6::GetDummy,
};
struct PixelMethod3: private LastPixel,private FirstPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        FirstPixel::set(timer,v);
    }
    static uint32 (PixelMethod3::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod3::*const PixelMethod3::methods[NPixelMethods])(unsigned)const =
{
    &PixelMethod3::GetDummy,
    &LastPixel::GetLast,
    &FirstPixel::GetFirst,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
    &PixelMethod3::GetDummy,
};
struct PixelMethod7: private LastPixel,private FirstPixel,private AveragePixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
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
    &AveragePixel::GetAverage,
    &LastPixel::GetLast,
    &FirstPixel::GetFirst,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
    &PixelMethod7::GetDummy,
};
struct PixelMethod8: private MostUsedPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        MostUsedPixel::set(timer,v);
    }
    static uint32 (PixelMethod8::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod8::*const PixelMethod8::methods[NPixelMethods])(unsigned)const =
{
    &MostUsedPixel::GetAverage,
    &PixelMethod8::GetDummy,
    &PixelMethod8::GetDummy,
    &MostUsedPixel::GetMostUsed,
    &MostUsedPixel::GetLeastUsed,
    &MostUsedPixel::GetActionAvg,
    &PixelMethod8::GetDummy,
    &PixelMethod8::GetDummy,
    &PixelMethod8::GetDummy,
    &PixelMethod8::GetDummy,
    &PixelMethod8::GetDummy,
    &PixelMethod8::GetDummy,
};
struct PixelMethod10: private ChangeLogPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        ChangeLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod10::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod10::*const PixelMethod10::methods[NPixelMethods])(unsigned)const =
{
    &ChangeLogPixel::GetAverage,
    &ChangeLogPixel::GetLast,
    &ChangeLogPixel::GetFirst,
    &ChangeLogPixel::GetMostUsed,
    &ChangeLogPixel::GetLeastUsed,
    &ChangeLogPixel::GetActionAvg,
    &ChangeLogPixel::GetChangeLog,
    &PixelMethod10::GetDummy,
    &ChangeLogPixel::GetLoopingAvg,
    &ChangeLogPixel::GetLoopingLast,
    &ChangeLogPixel::GetLastNMost,
    &ChangeLogPixel::GetFirstNMost,
};
struct PixelMethod20: private LoopingLogPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod20::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod20::*const PixelMethod20::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetAverage,
    &PixelMethod20::GetDummy,
    &PixelMethod20::GetDummy,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &PixelMethod20::GetDummy,
    &LoopingLogPixel::GetLoopingLog,
    &PixelMethod20::GetDummy,
    &PixelMethod20::GetDummy,
    &PixelMethod20::GetDummy,
    &PixelMethod20::GetDummy,
};
struct PixelMethod21: private LastPixel,private LoopingLogPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        LastPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod21::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod21::*const PixelMethod21::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetAverage,
    &LastPixel::GetLast,
    &PixelMethod21::GetDummy,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &PixelMethod21::GetDummy,
    &LoopingLogPixel::GetLoopingLog,
    &PixelMethod21::GetDummy,
    &PixelMethod21::GetDummy,
    &PixelMethod21::GetDummy,
    &PixelMethod21::GetDummy,
};
struct PixelMethod22: private FirstPixel,private LoopingLogPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        FirstPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod22::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod22::*const PixelMethod22::methods[NPixelMethods])(unsigned)const =
{
    &LoopingLogPixel::GetAverage,
    &PixelMethod22::GetDummy,
    &FirstPixel::GetFirst,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &PixelMethod22::GetDummy,
    &LoopingLogPixel::GetLoopingLog,
    &PixelMethod22::GetDummy,
    &PixelMethod22::GetDummy,
    &PixelMethod22::GetDummy,
    &PixelMethod22::GetDummy,
};
struct PixelMethod23: private LastPixel,private FirstPixel,private LoopingLogPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
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
    &LoopingLogPixel::GetAverage,
    &LastPixel::GetLast,
    &FirstPixel::GetFirst,
    &LoopingLogPixel::GetMostUsed,
    &LoopingLogPixel::GetLeastUsed,
    &LoopingLogPixel::GetActionAvg,
    &PixelMethod23::GetDummy,
    &LoopingLogPixel::GetLoopingLog,
    &PixelMethod23::GetDummy,
    &PixelMethod23::GetDummy,
    &PixelMethod23::GetDummy,
    &PixelMethod23::GetDummy,
};
struct PixelMethod30: private ChangeLogPixel,private LoopingLogPixel
{
    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }
    inline void set(unsigned timer, uint32 v) FastPixelMethod
    {
        ChangeLogPixel::set(timer,v);
        LoopingLogPixel::set(timer,v);
    }
    static uint32 (PixelMethod30::*const methods[NPixelMethods])(unsigned)const;
};
uint32 (PixelMethod30::*const PixelMethod30::methods[NPixelMethods])(unsigned)const =
{
    &ChangeLogPixel::GetAverage,
    &ChangeLogPixel::GetLast,
    &ChangeLogPixel::GetFirst,
    &ChangeLogPixel::GetMostUsed,
    &ChangeLogPixel::GetLeastUsed,
    &ChangeLogPixel::GetActionAvg,
    &ChangeLogPixel::GetChangeLog,
    &LoopingLogPixel::GetLoopingLog,
    &ChangeLogPixel::GetLoopingAvg,
    &ChangeLogPixel::GetLoopingLast,
    &ChangeLogPixel::GetLastNMost,
    &ChangeLogPixel::GetFirstNMost,
};
const FactoryType Factories[] =
{
/*0	*/
{ FactoryMethods<PixelMethod1 >::Construct,
  FactoryMethods<PixelMethod1 >::Copy,
  FactoryMethods<PixelMethod1 >::Assign },
/*1	*/
{ FactoryMethods<PixelMethod4 >::Construct,
  FactoryMethods<PixelMethod4 >::Copy,
  FactoryMethods<PixelMethod4 >::Assign },
/*2	*/
{ FactoryMethods<PixelMethod5 >::Construct,
  FactoryMethods<PixelMethod5 >::Copy,
  FactoryMethods<PixelMethod5 >::Assign },
/*3	*/
{ FactoryMethods<PixelMethod2 >::Construct,
  FactoryMethods<PixelMethod2 >::Copy,
  FactoryMethods<PixelMethod2 >::Assign },
/*4	*/
{ FactoryMethods<PixelMethod6 >::Construct,
  FactoryMethods<PixelMethod6 >::Copy,
  FactoryMethods<PixelMethod6 >::Assign },
/*5	*/
{ FactoryMethods<PixelMethod3 >::Construct,
  FactoryMethods<PixelMethod3 >::Copy,
  FactoryMethods<PixelMethod3 >::Assign },
/*6	*/
{ FactoryMethods<PixelMethod7 >::Construct,
  FactoryMethods<PixelMethod7 >::Copy,
  FactoryMethods<PixelMethod7 >::Assign },
/*7	*/
{ FactoryMethods<PixelMethod8 >::Construct,
  FactoryMethods<PixelMethod8 >::Copy,
  FactoryMethods<PixelMethod8 >::Assign },
/*8	*/
{ FactoryMethods<PixelMethod10 >::Construct,
  FactoryMethods<PixelMethod10 >::Copy,
  FactoryMethods<PixelMethod10 >::Assign },
/*9	*/
{ FactoryMethods<PixelMethod20 >::Construct,
  FactoryMethods<PixelMethod20 >::Copy,
  FactoryMethods<PixelMethod20 >::Assign },
/*10	*/
{ FactoryMethods<PixelMethod21 >::Construct,
  FactoryMethods<PixelMethod21 >::Copy,
  FactoryMethods<PixelMethod21 >::Assign },
/*11	*/
{ FactoryMethods<PixelMethod22 >::Construct,
  FactoryMethods<PixelMethod22 >::Copy,
  FactoryMethods<PixelMethod22 >::Assign },
/*12	*/
{ FactoryMethods<PixelMethod23 >::Construct,
  FactoryMethods<PixelMethod23 >::Copy,
  FactoryMethods<PixelMethod23 >::Assign },
/*13	*/
{ FactoryMethods<PixelMethod30 >::Construct,
  FactoryMethods<PixelMethod30 >::Copy,
  FactoryMethods<PixelMethod30 >::Assign },
};
const unsigned char FactoryIndex[1ul << NPixelMethods] =
{ 0,1,0,2,3,4,5,6,7,7,8,8,8,8,8,8,7,7,8,8,8,8,8,8,7,7,8,8,8,8,8,8,
7,7,8,8,8,8,8,8,7,7,8,8,8,8,8,8,7,7,8,8,8,8,8,8,7,7,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
9,9,10,10,11,11,12,12,9,9,10,10,11,11,12,12,9,9,10,10,11,11,12,12,9,9,10,10,11,11,12,12,
9,9,10,10,11,11,12,12,9,9,10,10,11,11,12,12,9,9,10,10,11,11,12,12,9,9,10,10,11,11,12,12,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
 };
