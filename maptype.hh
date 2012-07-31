#ifndef bqtAnimMergerMapTypeHH
#define bqtAnimMergerMapTypeHH

#include <utility>
#include "binaryheap.hh"

template<typename K,typename V>
struct MapKeyMethod
{
    inline const K& operator() (const std::pair<K,V>& p) { return p.first; }
};

template<typename K,typename V, bool Multiple>
class MapBaseType: public
    BinaryHeapType<std::pair<K,V>, Multiple, MapKeyMethod<K,V> >
{
public:
    typedef K key_type;
    typedef V mapped_type;

    template<typename Kt>
    V& operator[] (Kt key)
    {
        typename BinaryHeapType<std::pair<K,V>, Multiple, MapKeyMethod<K,V> >::
        iterator i = this->lower_bound(key);
        if(i == this->end() || !(key==i->first))
        {
            i = this->insert(i,
                typename BinaryHeapType<std::pair<K,V>, Multiple, MapKeyMethod<K,V> >::
                    value_type(key, V()));
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
