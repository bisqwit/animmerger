#ifndef bqtAnimMergerMapTypeHH
#define bqtAnimMergerMapTypeHH

#include <utility>
#include "vectype.hh"

template<typename K,typename V>
class MapType
{
public:
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<K,V> value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
    typedef VecType<value_type> rep;
    rep data;

public:
    MapType() { }

    typedef typename rep::iterator iterator;
    typedef typename rep::const_iterator const_iterator;

    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }

    void clear()
    {
        data.~rep();          // std::_Destroy(&data);
        new(&data) rep();     // std::_Construct(&data);
    }

    iterator lower_bound(K key)
    {
        iterator first = begin(), last = end();
        unsigned limit;
        #ifdef __GNUC__
        while(__builtin_expect((limit=last-first) > 0, 1))
        #else
        while(                 (limit=last-first) > 0    )
        #endif
        {
            iterator middle = first + (limit>>1);
            if(middle->first < key)
                first = middle+1;
            else
                last = middle;
        }
        return first;
    }
    V& operator[] (K key)
    {
        iterator i = lower_bound(key);
        if(i == end() || i->first > key)
        {
            i = data.insert(i, value_type(key, V()));
        }
        return i->second;
    }
};

#endif
