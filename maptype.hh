#ifndef bqtAnimMergerMapTypeHH
#define bqtAnimMergerMapTypeHH

#include <utility>
#include "untreetype.hh"

template<typename K,typename V>
struct MapKeyMethod
{
    inline const K& operator() (const std::pair<K,V>& p) { return p.first; }
};

template<typename K,typename V, bool Multiple>
class MapBaseType: public
    UntreeType<std::pair<K,V>, Multiple, MapKeyMethod<K,V> >
{
public:
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<K,V> value_type;

    template<typename Kt>
    V& operator[] (Kt key)
    {
        typename UntreeType<std::pair<K,V>, Multiple, MapKeyMethod<K,V> >::
        iterator i = lower_bound(key);
        if(i == this->end() || !(key==i->first))
        {
            i = insert(i, value_type(key, V()));
        }
        return i->second;
    }

};

template<typename K,typename V>
class MapType: public MapBaseType<K,V,false>
{
};
template<typename K,typename V>
class MultiMapType: public MapBaseType<K,V,true>
{
};

#endif
