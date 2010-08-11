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

#include <cstring>


/* Create pixel method name list */
#define MakePixName(o,f,name) #name,
static const char* const PixelMethodNames[NPixelMethods] =
{
     DefinePixelMethods(MakePixName)
};
#undef MakePixName

static const unsigned long nmethod_combinations = 1ul << (0 +
#define CountImpl(id,name) +1
DefinePixelClasses(CountImpl)
#undef CountImpl
);

static const unsigned long ncombinations = 1ul << NPixelMethods;

static unsigned Solution[ncombinations];

static std::ostringstream FactoryIndex, FactoryTable;

template<unsigned CombinationIndex>
struct AllCombinations
{
    typedef typename PixelMethodImplComb<CombinationIndex>::result Obj;
    static unsigned FindSize(unsigned s)
    {
        if(s == CombinationIndex) return sizeof(Obj);
        return AllCombinations<CombinationIndex-1>::FindSize(s);
    }
    static unsigned FindSizePenalty(unsigned s)
    {
        if(s == CombinationIndex) return Obj::SizePenalty;
        return AllCombinations<CombinationIndex-1>::FindSizePenalty(s);
    }
    static unsigned long FindTraits(unsigned s)
    {
        if(s == CombinationIndex) return Obj::Traits;
        return AllCombinations<CombinationIndex-1>::FindTraits(s);
    }
    static const char* FindName(unsigned s)
    {
        if(s == CombinationIndex) return PixelMethodImplName<Obj>::getname();
        return AllCombinations<CombinationIndex-1>::FindName(s);
    }
};
template<>
struct AllCombinations<0>
{
    static inline unsigned FindSize(unsigned) { return 0; }
    static inline unsigned FindSizePenalty(unsigned) { return 0; }
    static inline unsigned long FindTraits(unsigned) { return 0; }
    static inline const char* FindName(unsigned) { return 0; }
};
typedef AllCombinations<nmethod_combinations-1> CombinationLore;

static std::string GetFactoryName(unsigned long combination)
{
    unsigned size        = CombinationLore::FindSize(combination);
    unsigned sizepenalty = CombinationLore::FindSizePenalty(combination);
    const char* name = CombinationLore::FindName(combination);

    std::ostringstream tmp;
    tmp << "FactoryMethods<PixelMethodImplComb<" << combination << ">::result>::data"
           " /* " << size << "(+" << sizepenalty << ") bytes: " << name << " */";
    return tmp.str();
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

static std::vector<std::string> FactoryTableData;
static unsigned FindTableOffset(const std::vector<std::string>& names)
{
    unsigned begin=0;
    for(; begin < FactoryTableData.size(); ++begin)
    {
        bool match=true, partial_match=false;
        for(unsigned n=0; n<names.size(); ++n)
        {
            if(begin+n >= FactoryTableData.size())
            {
                partial_match=true;
                break;
            }
            if(FactoryTableData[begin+n] != names[n])
            {
                match=false;
                break;
            }
        }
        if(partial_match) break;
        if(match) return begin;
    }
    for(unsigned n=0; n<names.size(); ++n)
        if(begin+n >= FactoryTableData.size())
            FactoryTableData.push_back(names[n]);
    return begin;
}

static void CreateFactoryTable()
{
    FactoryTable <<
        "    static const FactoryType* const table[" << FactoryTableData.size() << "] =\n"
        "    {\n";
    for(unsigned a=0; a<FactoryTableData.size(); ++a)
    {
        FactoryTable
        <<  "    "
            "/* " << std::setw(2) << a << " */ "
            "&" << FactoryTableData[a] << ",\n";
    }
    FactoryTable <<
        "    };\n";
}

static void CreateFactoryIndex(bool done[ncombinations])
{
    for(;;)
    {
        unsigned remain = 0, min_remain=ncombinations, max_remain=0;
        for(unsigned a=0; a<ncombinations; ++a)
            if(!done[a])
                { if(a < min_remain) min_remain = a;
                  if(a > max_remain) max_remain = a;
                  ++remain;
                }
        if(!remain) break;

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
        #pragma omp parallel for schedule(dynamic)
        for(unsigned mask=1; mask<=max_remain; ++mask)
        {
            if(OkBitmask(mask, 4))
            for(int addshift=-13; addshift<=13; ++addshift)
            {
              {
                const int add = AddFromShift(addshift);

                typedef std::map<unsigned, unsigned, std::less<unsigned>, FSBAllocator<int> > r1;
                typedef std::map<unsigned, r1, std::less<unsigned>, FSBAllocator<int> > r2;
                r2 result_counts;
                for(unsigned a=0; a<ncombinations; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + add) & mask;
                    const unsigned value = Solution[a];
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
                    //FactoryIndex << "/* " << i->first << ": " << mostvalue << "*/\n";
                }
                int coverage = 0, miscoverage = 0;
                unsigned miscoverage_min=ncombinations, miscoverage_max=0;
                std::set<unsigned, std::less<unsigned>, FSBAllocator<int> > cases;
                for(unsigned a=0; a<ncombinations; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + add) & mask;
                    const unsigned value = Solution[a];
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
                if(miscoverage_min != ncombinations)
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

        FactoryIndex << "/* of remaining " << remain
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
            for(unsigned a=0; a<ncombinations; ++a)
            {
                if(done[a]) continue;
                const unsigned index = (a + best_add) & best_mask;
                const unsigned value = Solution[a];
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
                //FactoryIndex << "/* " << i->first << ": " << mostvalue << "*/\n";
            }

            if(best_miscoverage > 0)
            {
                bool child_simulated_done[ncombinations];
                std::memcpy(child_simulated_done, done, sizeof(done));

                unsigned miscoverage_min=ncombinations, miscoverage_max=0;
                for(unsigned a=0; a<ncombinations; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + best_add) & best_mask;
                    const unsigned value = Solution[a];
                    if(results[index] != value)
                    {
                        if(a < miscoverage_min) miscoverage_min = a;
                        if(a > miscoverage_max) miscoverage_max = a;
                    }
                }
                for(unsigned a=0; a<ncombinations; ++a)
                {
                    if(a < miscoverage_min || a > miscoverage_max)
                        child_simulated_done[a] = true;
                    else
                        done[a] = true;
                }
                if(miscoverage_min == 0)
                    FactoryIndex << "    if(featuremask < " << (miscoverage_max+1) << ") {\n";
                else
                    FactoryIndex << "    if(featuremask >= " << miscoverage_min << " && featuremask <= " << miscoverage_max << ") {\n";
                CreateFactoryIndex(child_simulated_done);
                FactoryIndex << "    } /* " << miscoverage_min << ".." << miscoverage_max << " */\n";
                //FactoryIndex << "/* Done recursion */\n";
            }

            unsigned mask_downshift = 0;
            while(  ((best_mask >> mask_downshift)&1) == 0)
                ++mask_downshift;

            std::ostringstream expr;
            expr << "((featuremask + " << best_add << ") >> " << mask_downshift << ") & " << (best_mask >> mask_downshift);

            std::map<unsigned, std::string> SwitchOptions;

            for(std::map<unsigned, unsigned>::const_iterator
                i = results.begin(); i != results.end();
                ++i)
            {
                if(i->second == ~0u) continue;
                if(i->first
                !=  ((i->first >> mask_downshift) << mask_downshift))
                {
                    FactoryIndex << "    /* ERROR 1 */\n";
                }
                unsigned caseval = i->first >> mask_downshift;

                SwitchOptions[caseval] = GetFactoryName(i->second);

                for(unsigned a=0; a<ncombinations; ++a)
                {
                    if(done[a]) continue;
                    const unsigned index = (a + best_add) & best_mask;
                    const unsigned value = Solution[a];
                    if(index == i->first)
                    {
                        done[a] = true;
                        if(value != i->second)
                        {
                            FactoryIndex << "    /* ERROR 2 */\n";
                        }
                    }
                }
            }

            if(SwitchOptions.size() == (best_mask >> mask_downshift)+1
            && (SwitchOptions.rbegin()->first - SwitchOptions.begin()->first + 1)
                 == SwitchOptions.size()) // An uninterrupted array
            {
                std::vector<std::string> table;
                for(std::map<unsigned, std::string>::const_iterator
                    i = SwitchOptions.begin();
                    i != SwitchOptions.end();
                    ++i)
                {
                    table.push_back(i->second);
                }
                unsigned begin_offset = FindTableOffset(table);
                FactoryIndex <<
                    "    return table[(" << expr.str() << ") + " << begin_offset << "];\n";
            }
            else
            {
                FactoryIndex <<
                    "    switch( " << expr.str() << " )\n"
                    "    {\n";
                for(std::map<unsigned, std::string>::const_iterator
                    i = SwitchOptions.begin();
                    i != SwitchOptions.end();
                    ++i)
                {
                    FactoryIndex
                        << "        case " << i->first
                        << ": return &" << i->second << ";\n";
                }
                FactoryIndex <<
                    "    }\n" << std::flush;
            }
        }
        else
        {
            if(min_remain == 0)
            {
                min_remain = 1; // Skip #0
                // Because it never happens that no method is wanted at all.
            }

            best_mask = 0xFFFF;
            std::vector<std::string> table;
            for(unsigned a=min_remain;  a<=max_remain; ++a)
            {
                table.push_back( GetFactoryName( Solution[a] ) );
                if((a-min_remain) >= best_mask) break;
            }
            unsigned begin_offset = FindTableOffset(table);

            FactoryIndex <<
                "    return table[(featuremask - " << min_remain << ") + " << begin_offset << "];\n";
            break;
        }
    }
}

static void CreateFactoryIndex()
{
    bool done [ ncombinations ] = { false };
    CreateFactoryIndex(done);
}

int main()
{
    /* For each combination of pixel methods (trait bitmasks) */
    for(unsigned long a=0; a<ncombinations; ++a)
    {
        unsigned long best_m      = 0;
        unsigned long best_size   = 0;
        unsigned long best_traits = 0;
        /* Go through each combination of pixel implementations */
        for(unsigned long m=0; m<nmethod_combinations; ++m)
        {
            unsigned long traits = CombinationLore::FindTraits(m);
            unsigned long size   = CombinationLore::FindSize(m)
                                 + CombinationLore::FindSizePenalty(m);

            /* If this combination implements all the desired methods */
            if((traits & a) == a)
            {
                /* Pick that combination that takes least space */
                if(best_size == 0
                || (size < best_size)
                /*|| (size == best_size &&
                       (PopCount(m) < PopCount(best_m)
                    || (PopCount(m) == PopCount(best_m) &&
                          PopCount(traits) < PopCount(best_traits) ))*/
                  )
                {
                    best_size   = size;
                    best_m      = m;
                    best_traits = traits;
                }
            }
        }

        /* The combination has been chosen */
        Solution[a] = best_m;
    }

    /* Create minimal C++ program that accesses that array */
    /* Otherwise, we need a 8192-pointer array, which is not that nice */
    CreateFactoryIndex();
    CreateFactoryTable();

    std::cout
    <<  "/* FindFactory() returns a FactoryType for the smallest pixel\n"
        " * type that implements all features indicated in @featuremask.\n"
        " * It does so by the means of an algorithm generated\n"
        " * by the makepixels program provided in animmerger\n"
        " * source code package.\n"
        " * @featuremask is a bitmask of requested PixelMethods.\n"
        " * Range: 0 < @featuremask < " << ncombinations << "\n"
        " * where " << ncombinations << " = 2**NPixelMethods\n"
        " */\n"
        "const FactoryType* FindFactory(unsigned featuremask)\n"
        "{\n"
    <<  FactoryTable.str()
    <<  FactoryIndex.str()
    <<  "    return 0;\n"
        "}\n";
}
