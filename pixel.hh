#ifndef bqtTileTrackerPixelHH
#define bqtTileTrackerPixelHH

typedef unsigned int uint32;
typedef unsigned char uint8 ;
typedef unsigned short uint16;
typedef unsigned long long uint64;

static const uint32 DefaultPixel = 0xFF404041u;

extern unsigned LoopingLogLength;

extern unsigned CurrentTimer;
/* Timer value for set() and get methods in ChangeLog and LoopingLog methods */

#include "pixels/mostusedpixel.hh"

#include "pixels/averagepixel.hh"
#include "pixels/lastpixel.hh"
#include "pixels/mostusedwithinpixel.hh"
#include "pixels/changelogpixel.hh"
#include "pixels/loopinglogpixel.hh"

extern enum PixelMethod
{
    pm_AveragePixel,
    pm_LastPixel,
    pm_MostUsedPixel,
    pm_MostUsed16Pixel,
    pm_ChangeLogPixel,
    pm_LoopingLogPixel
} method;

struct UncertainPixel
{
public:
    static bool is_changelog()
    {
        return method == pm_ChangeLogPixel;
    }
    static bool is_loopinglog()
    {
        return method == pm_LoopingLogPixel;
    }
    static bool is_animated()
    {
        return is_changelog() || is_loopinglog();
    }
    static unsigned GetLoopLength()
    {
        if(!is_loopinglog()) return 0;
        return LoopingLogLength;
    }
};

struct Array256x256of_Base
{
    virtual ~Array256x256of_Base() { }
    virtual uint32 GetPixel(unsigned index) const = 0;
    virtual uint32 GetMostUsed(unsigned index) const = 0;
    virtual void set(unsigned index, uint32 p) = 0;
    virtual void Compress() = 0;
};
template<typename T>
struct Array256x256of: public Array256x256of_Base
{
public:
    T data[256*256];
public:
    Array256x256of() { }
    virtual ~Array256x256of() { }
public:
    virtual uint32 GetPixel(unsigned index) const
    {
        return data[index].get_pixel();
    }
    virtual uint32 GetMostUsed(unsigned index) const
    {
        return data[index].get_mostused();
    }
    virtual void set(unsigned index, uint32 p)
    {
        data[index].set(p);
    }
    virtual void Compress()
    {
        for(unsigned a=0; a<256*256; ++a)
            data[a].Compress();
    }
};

class UncertainPixelVector256x256
{
    class Get256x256pixelFactory
    {
        struct factoryBase
        {
            typedef Array256x256of_Base ObjT;
            virtual ObjT* Construct() const = 0;
            virtual ObjT* CopyConstruct(const ObjT& b) const = 0;
            virtual void CopyAssign(ObjT*& ptr, const ObjT& b) const = 0;
        };
        template<typename T>
        static inline const factoryBase* factory()
        {
            static struct : public factoryBase
            {
                typedef Array256x256of_Base ObjT;
                typedef Array256x256of<T> ResT;
                virtual ObjT* Construct() const
                    { return new ResT; }
                virtual ObjT* CopyConstruct(const ObjT& b) const
                    { return new ResT ( (const ResT&) b ) ; }
                virtual void CopyAssign(ObjT*& ptr, const ObjT& b) const
                    { if(ptr) *ptr = (const ResT&) b;
                      else ptr = new ResT( (const ResT&) b ); }
            } local;
            return &local;
        }
    public:
        const factoryBase* operator-> () const
        {
            static struct factories: public MapType<PixelMethod, const factoryBase*>
            {
                factories()
                {
                    insert(value_type(pm_AveragePixel,   factory<AveragePixelAndMostUsedPixel>()));
                    insert(value_type(pm_LastPixel,      factory<LastPixelAndMostUsedPixel>()));
                    insert(value_type(pm_MostUsedPixel,  factory<MostUsedPixelAndMostUsedPixel>()));
                    insert(value_type(pm_MostUsed16Pixel,factory<MostUsedWithinAndMostUsedPixel<16> >()));
                    insert(value_type(pm_ChangeLogPixel, factory<ChangeLogPixelAndMostUsedPixel>()));
                    insert(value_type(pm_LoopingLogPixel,factory<LoopingLogPixelAndMostUsedPixel>()));
                }
            } Factories;
            return Factories.find(method)->second;
        }
    };
public: 
    // By default, the vector is uninitialized (empty)
    UncertainPixelVector256x256() : data(0)
    {
    }
    // Init: Resize the vector to 256x256 elements
    void init()
    {
        /* Construct the type of object determined by the global enum "method" */
        if(!data) data = Get256x256pixelFactory()->Construct();
    }
    // Copy constructor
    UncertainPixelVector256x256(const UncertainPixelVector256x256& b) : data(0)
    {
        if(b.data) data = Get256x256pixelFactory()->CopyConstruct(*b.data);
    }
    // Assign operator (create copy of the other vector)
    UncertainPixelVector256x256& operator= (const UncertainPixelVector256x256& b)
    {
        if(!b.data) { delete data; data=0; }
        else Get256x256pixelFactory()->CopyAssign(data, *b.data);
        return *this;
    }
    // Destructor: Deallocate vector of alloated
    ~UncertainPixelVector256x256()
    {
        delete data;
    }
    // Visit: Run user-given functor, give reference to the actual vector data as param
    template<typename F>
    inline void Visit(F func)
    {
        if(data) func(*data);
    }
    template<typename F>
    inline void Visit(F func) const
    {
        if(data) func(*data);
    }
    
    // Run Compress() method on each vector element
    inline void Compress()
    {
        if(data) data->Compress();
    }
    
    // Test whether vector is empty (uninitialized)
    bool empty() const { return !data; }

private:
    Array256x256of_Base* data;
};

#endif
