#include "pixel.hh"
#include "pixels.hh"
#include "openmp.hh"
#include "alloc/FSBAllocator.hh"

#include <iostream>
#include <iomanip>

#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <string>


/* Create pixel method name list */
#define MakePixName(o,f,name) #name,
static const char* const PixelMethodNames[NPixelMethods] =
{
     DefinePixelMethods(MakePixName)
};
#undef MakePixName

#define MakeMethod(name) { #name, name::Traits, sizeof(name) },
static const struct Method
{
    const char*   name;
    unsigned long traits;
    unsigned long size;
} Methods[] =
{
    DefinePixelClasses(MakeMethod)
};
#undef MakeMethod
static const unsigned nmethods = sizeof(Methods) / sizeof(*Methods);

static const unsigned long ncombinations = 1ul << NPixelMethods;
static const unsigned long nmethod_combinations = 1ul << nmethods;

static std::map<std::string, unsigned> NameTable;
static std::vector<unsigned> NameList;

static std::ostringstream Creations;
static std::ostringstream Classes;
static std::ostringstream NameDeclarations;
static std::ostringstream NameDeclarationArray;
static unsigned n_factories = 0;

static void PutResult(unsigned long input, unsigned long combination)
{
    static std::set<unsigned long> DoneCombinations;

    std::ostringstream CombinationName;
    CombinationName << "PixelMethod" << std::hex << combination;

    if(DoneCombinations.find(combination) == DoneCombinations.end())
    {
        ++n_factories;
        DoneCombinations.insert(combination);
        std::string name = CombinationName.str();

        Classes
            << "struct " << name << ": private DummyPixel";
        for(unsigned n=0; n<nmethods; ++n)
            if(combination & (1 << n))
                Classes << ", private " << Methods[n].name;
        Classes
            << "\n"
               "{\n";
        /*for(unsigned p=0; p<NPixelMethods; ++p)
        {
            bool used=false;
            for(unsigned n=0; n<nmethods; ++n)
                if((combination & (1 << n))
                && (Methods[n].traits & (1 << p)))
                {
                    Classes << "    using " << Methods[n].name << "::Get" << PixelMethodNames[p] << ";\n";
                    used=true;
                    break;
                }
        }*/
        Classes <<
            "    inline void set(unsigned timer, uint32 v) FastPixelMethod\n"
            "    {\n";
        for(unsigned n=0; n<nmethods; ++n)
            if(combination & (1 << n))
            {
                Classes << "        " << Methods[n].name << "::set(timer,v);\n";
            }
        Classes <<
            "    }\n"
            "    static uint32 (" << name << "::*const methods[NPixelMethods])(unsigned)const;\n"
            "};\n"
            "uint32 (" << name << "::*const " << name << "::methods[NPixelMethods])(unsigned)const =\n"
            "{\n";
        for(unsigned p=0; p<NPixelMethods; ++p)
        {
            bool used=false;
            for(unsigned n=0; n<nmethods; ++n)
                if((combination & (1 << n))
                && (Methods[n].traits & (1 << p)))
                {
                    Classes << "    &" << Methods[n].name << "::Get" << PixelMethodNames[p] << ",\n";
                    used=true;
                    break;
                }
            if(!used)
                Classes << "    &DummyPixel::GetDummy,\n";
        }
        Classes <<
            "};\n";
    }

    std::string namestr = CombinationName.str();

    if(NameTable.find(namestr) == NameTable.end())
    {
        unsigned nameid = NameTable.size();
        NameTable[namestr] = nameid;
        NameList.push_back(nameid);

        NameDeclarations
        << "/*" << nameid << "\t*/\n"
           "{ FactoryMethods<" << namestr << ">::Construct,\n"
           "  FactoryMethods<" << namestr << ">::Copy,\n"
           "  FactoryMethods<" << namestr << ">::Assign },\n";
    }
    else
    {
        unsigned nameid = NameTable[namestr];
        NameList.push_back(nameid);
    }
}

static unsigned PopCount(unsigned n)
{
    static const unsigned char popcount16[] =
    { 0,1,1,2, 1,2,2,3, 1,2,2,3, 2,3,3,4 };
    return popcount16[n&15] + popcount16[(n>>4)&15]
         + popcount16[(n>>8)&15] + popcount16[(n>>12)&15]
         + popcount16[(n>>16)&15];
}
static bool OkBitmask(unsigned n, unsigned maxbits)
{
    unsigned p = PopCount(n);
    if(p <= 3) return true;
    if(n) while(!(n & 1)) n >>= 1;
    return n < (1u << maxbits);
}

void CreateFactoryIndex(std::vector<bool> done)
{
    const unsigned n = NameList.size();

    for(;;)
    {
        unsigned remain = 0, min_remain=n, max_remain=0;
        for(size_t a=0; a<n; ++a)
            if(!done[a])
                { if(a < min_remain) min_remain = a;
                  if(a > max_remain) max_remain = a;
                  ++remain;
                }
        if(!remain) break;

        /*if(max_remain - min_remain + 1 <= 32)
        {
            std::cout << "static const unsigned char table[] = {\n";
            for(unsigned a=min_remain;  a<=max_remain; ++a)
            {
                if(a != min_remain) std::cout << ",";
                if((a-min_remain)==32) std::cout << "\n";
                std::cout << NameList[a];
            }
            std::cout << "};\n";
            std::cout << "return table[index - " << min_remain << "];\n";
            break;
        }*/
        int best_add=0;
        unsigned best_mask=0;
        double best_score=0;
        unsigned best_coverage=0;
        unsigned best_miscoverage=0;
      #ifdef _OPENMP
        MutexType BestLock;
      #endif

        #define AddFromShift(i)  (i>0 ? (1 << (i-1)) : (i<0 ? -(1 << (-i-1)) : 0))

      if(1)
      {
        //#pragma omp parallel for schedule(dynamic)
        for(unsigned mask=1; mask<=max_remain; ++mask)
        {
            if(OkBitmask(mask, 4))
            for(int addshift=-4; addshift<=4; ++addshift)
            {
              {
                const int add = AddFromShift(addshift);

                typedef std::map<unsigned, unsigned, std::less<unsigned>, FSBAllocator<int> > r1;
                typedef std::map<unsigned, r1, std::less<unsigned>, FSBAllocator<int> > r2;
                r2 result_counts;
                for(unsigned a=0; a<n; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + add) & mask;
                    const unsigned value = NameList[a];
                    ++result_counts[index][value];
                }
                std::map<unsigned, unsigned, std::less<unsigned>, FSBAllocator<int> > results;
                for(r2::iterator i = result_counts.begin(); i != result_counts.end(); ++i)
                {
                    unsigned mostcommon=0, mostvalue=0;
                    for(r1::iterator j = i->second.begin(); j != i->second.end(); ++j)
                        if(j->second > mostcommon)
                            { mostcommon = j->second; mostvalue = j->first; }
                    results[i->first] = mostvalue;
                    //std::cout << "/* " << i->first << ": " << mostvalue << "*/\n";
                }
                int coverage = 0, miscoverage = 0;
                unsigned miscoverage_min=n, miscoverage_max=0;
                std::set<unsigned, std::less<unsigned>, FSBAllocator<int> > cases;
                for(unsigned a=0; a<n; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + add) & mask;
                    const unsigned value = NameList[a];
                    ++coverage;
                    if(results[index] != value)
                    {
                        ++miscoverage;
                        if(a < miscoverage_min) miscoverage_min = a;
                        if(a > miscoverage_max) miscoverage_max = a;
                    }
                    else
                        cases.insert(index);
                }
                unsigned n_cases = cases.size();
                if(!n_cases) goto nextmask;
                if(miscoverage_min != n)
                    miscoverage = miscoverage_max - miscoverage_min + 1;

                //if(miscoverage_max-miscoverage_min+1 > ) goto nextmask;
                if(miscoverage >= coverage/2) goto nextmask;

                //double score = coverage / (double)n_cases - (miscoverage+31)/32;
                double score = coverage / (double)n_cases;
                score -= miscoverage/8.0;// / (double)(n_cases);

                if(add == 0) score += 0.02;
                if(add  > 0) score += 0.01;

              #ifdef _OPENMP
                ScopedLock lck(BestLock);
              #endif
                #pragma omp flush(best_score,best_add,best_mask,best_coverage,best_miscoverage)
                if(score > best_score)
                {
                    best_score = score;
                    best_add  = add;
                    best_mask = mask;
                    best_coverage = coverage;
                    best_miscoverage = miscoverage;
                }
                #pragma omp flush(best_score,best_add,best_mask,best_coverage,best_miscoverage)
              }
            nextmask:;
            }
        }
      }

        std::cout << "/* of remaining " << remain
                  << " [" << min_remain << ".." << max_remain << "]"
                  << ", coverage " << best_coverage
                  << ", miscoverage " << best_miscoverage
                  << ", best_score " << best_score
                  << ", config=(add:" << best_add << ",mask:" << best_mask << ")"
                  << " */\n" << std::flush;

        if(best_score > 2.05)
        {
            typedef std::map<unsigned, unsigned, std::less<unsigned>, FSBAllocator<int> > r1;
            typedef std::map<unsigned, r1, std::less<unsigned>, FSBAllocator<int> > r2;
            r2 result_counts;
            for(unsigned a=0; a<n; ++a)
            {
                if(done[a]) continue;
                const unsigned index = (a + best_add) & best_mask;
                const unsigned value = NameList[a];
                ++result_counts[index][value];
            }
            std::map<unsigned, unsigned, std::less<unsigned>, FSBAllocator<int> > results;
            for(r2::iterator i = result_counts.begin(); i != result_counts.end(); ++i)
            {
                unsigned mostcommon=0, mostvalue=0;
                for(r1::iterator j = i->second.begin(); j != i->second.end(); ++j)
                    if(j->second > mostcommon)
                        { mostcommon = j->second; mostvalue = j->first; }
                results[i->first] = mostvalue;
                //std::cout << "/* " << i->first << ": " << mostvalue << "*/\n";
            }

            if(best_miscoverage > 0)
            {
                std::vector<bool> child_simulated_done=done;
                unsigned miscoverage_min=n, miscoverage_max=0;
                for(unsigned a=0; a<n; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + best_add) & best_mask;
                    const unsigned value = NameList[a];
                    if(results[index] != value)
                    {
                        if(a < miscoverage_min) miscoverage_min = a;
                        if(a > miscoverage_max) miscoverage_max = a;
                    }
                }
                for(unsigned a=0; a<n; ++a)
                {
                    if(a < miscoverage_min || a > miscoverage_max)
                        child_simulated_done[a] = true;
                    else
                        done[a] = true;
                }
                if(miscoverage_min == 0)
                    std::cout << "        if(index < " << (miscoverage_max+1) << ") {\n";
                else
                    std::cout << "        if(index >= " << miscoverage_min << " && index <= " << miscoverage_max << ") {\n";
                CreateFactoryIndex(child_simulated_done);
                std::cout << "        } /* " << miscoverage_min << ".." << miscoverage_max << " */\n";
                //std::cout << "/* Done recursion */\n";
            }

            unsigned mask_downshift = 0;
            while(  ((best_mask >> mask_downshift)&1) == 0)
                ++mask_downshift;

            std::cout <<
                "        switch( ((index + " << best_add << ") >> " << mask_downshift << ") & " << (best_mask >> mask_downshift) << ")\n"
                "        {\n";

            for(std::map<unsigned, unsigned>::const_iterator
                i = results.begin(); i != results.end();
                ++i)
            {
                if(i->second == ~0u) continue;
                if(i->first
                !=  ((i->first >> mask_downshift) << mask_downshift))
                {
                    std::cout << "    /* ERROR */\n";
                }
                unsigned caseval = i->first >> mask_downshift;
                std::cout
                    << "            case " << caseval
                    << ": return &FactoryList[" << i->second << "];\n";
                for(unsigned a=0; a<n; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + best_add) & best_mask;
                    const unsigned value = NameList[a];
                    if(index == i->first)
                    {
                        done[a] = true;
                        if(value != i->second)
                        {
                            std::cout << "    /* ERROR */\n";
                        }
                    }
                }
            }
            std::cout <<
                "        }\n" << std::flush;
        }
        else
        {
            std::cout <<
                "          static const unsigned char table[" << (max_remain-min_remain+1) << "] = {";
            for(unsigned a=min_remain;  a<=max_remain; ++a)
            {
                if(a != min_remain) std::cout << ",";
                if((a-min_remain)%32 == 0) std::cout << "\n          ";
                std::cout << std::setw(2);
                std::cout << NameList[a];
            }
            std::cout <<
                "};\n"
                "          return &FactoryList[table[index - " << min_remain << "]];\n";
            break;
        }
    }
}

void CreateFactoryIndex()
{
    const unsigned n = NameList.size();
    std::vector<bool> done(n, false);

    CreateFactoryIndex(done);
}

int main()
{
    Classes <<
        "struct DummyPixel\n"
        "{\n"
        "    uint32 GetDummy(unsigned=0) const FasterPixelMethod { return 0; }\n"
        "};\n"
        "\n";

    for(unsigned long a=0; a<ncombinations; ++a)
    {
        unsigned long best_m    = 0;
        unsigned long best_size = 0;
        for(unsigned long m=0; m<nmethod_combinations; ++m)
        {
            unsigned long traits = 0;
            unsigned long size   = 0;
            for(unsigned n=0; n<nmethods; ++n)
                if(m & (1 << n))
                {
                    traits |= Methods[n].traits;
                    size   += Methods[n].size;
                }
            if((traits & a) == a)
            {
                if(best_size == 0 || size < best_size)
                {
                    best_size = size;
                    best_m    = m;
                }
            }
        }
        PutResult(a, best_m);
    }
    std::cout <<
        "const class Factories\n"
        "{\n"
        "    static const FactoryType FactoryList[" << n_factories << "];\n"
        "public:\n"
        "    const FactoryType* operator[] (unsigned index) const\n"
        "    {\n";
    CreateFactoryIndex();
    std::cout <<
        "        return 0;\n"
        "    }\n"
        "} Factories = {};\n"
        "\n";
    std::cout << Classes.str();
    std::cout <<
        "\n"
        "const FactoryType Factories::FactoryList[] =\n"
        "{\n"
        << NameDeclarations.str()
     << "};\n";
}
