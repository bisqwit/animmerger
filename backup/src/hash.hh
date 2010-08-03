#ifndef bqtHashHH
#define bqtHashHH

/* Set to 0 if you have compilation problems
 * with hash_set or hash_map
 */
#define USE_HASH 1


#if USE_HASH

#include <string>

#include <tr1/unordered_map>
#include <tr1/unordered_set>
using namespace std::tr1;

using std::basic_string;

struct BitSwapHashFon
{
  size_t operator() (unsigned n) const
  {
    unsigned rot = n&31;
    return (n << rot) | (n >> (32-rot));
  }
};

namespace std {
namespace tr1
{
  template<typename T>
  struct hash<basic_string<T> >
  {
    size_t operator() (const basic_string<T> &s) const
    {
        unsigned h=0;
        for(unsigned a=0,b=s.size(); a<b; ++a)
        {
            unsigned c = s[a];
            c=h^c;
            h^=(c*707106);
        }
        return h;
    }
  };
  template<> struct hash<std::wstring>
  {
    size_t operator() (const std::wstring &s) const
    {
        unsigned h=0;
        for(unsigned a=0,b=s.size(); a<b; ++a)
        {
            unsigned c = s[a];
            c=h^c;
            h^=(c*707106);
        }
        return h;
    }
  };
  template<> struct hash<wchar_t>
  {
    size_t operator() (wchar_t n) const
    {
        /* Since values of n<128 are the most common,
         * values of n<256 the second common
         * and big values of n are rare, we rotate some
         * bits to make the distribution more even.
         * Multiplying n by 33818641 (a prime near 2^32/127) scales
         * the numbers nicely to fit the whole range and keeps the
         * distribution about even.
         */
        return (n * 33818641UL);
    }
  };
} }

#else

#include <map>
#include <set>

#define hash_map std::map
#define hash_set std::set

#endif // USE_HASH

#endif // bqtHashHH
