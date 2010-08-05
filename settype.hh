#ifndef bqtAnimMergerSetTypeHH
#define bqtAnimMergerSetTypeHH

#include "untreetype.hh"

struct SetKeyMethod
{
    template<typename K>
    inline const K& operator() (const K& k) { return k; }
};

template<typename K, bool Multiple>
class SetBaseType: public
    UntreeType<K, Multiple, SetKeyMethod>
{
};

template<typename K>
class SetType: public SetBaseType<K,false>
{
};
template<typename K>
class MultiSetType: public SetBaseType<K,true>
{
};

#endif
