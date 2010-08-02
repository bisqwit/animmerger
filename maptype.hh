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
    typedef typename rep::reverse_iterator reverse_iterator;
    typedef typename rep::const_reverse_iterator const_reverse_iterator;

    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    const_reverse_iterator rbegin() const { return data.rbegin(); }
    const_reverse_iterator rend() const { return data.rend(); }
    reverse_iterator rbegin() { return data.rbegin(); }
    reverse_iterator rend() { return data.rend(); }

    void clear()
    {
        data.~rep();          // std::_Destroy(&data);
        new(&data) rep();     // std::_Construct(&data);
    }
    bool empty() const
    {
        return data.empty();
    }

    void insert(iterator i, const value_type& val)
    {
        data.insert(i, val);
    }

    iterator lower_bound(K key)
    {
        iterator first = begin(), last = end();
        size_t limit;
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

    const_iterator lower_bound(K key) const
    {
        const_iterator first = begin(), last = end();
        size_t limit;
        #ifdef __GNUC__
        while(__builtin_expect((limit=last-first) > 0, 1))
        #else
        while(                 (limit=last-first) > 0    )
        #endif
        {
            const_iterator middle = first + (limit>>1);
            if(middle->first < key)
                first = middle+1;
            else
                last = middle;
        }
        return first;
    }

    const_iterator upper_bound(K key) const
    {
        const_iterator first = begin(), last = end();
        size_t limit;
        #ifdef __GNUC__
        while(__builtin_expect((limit=last-first) > 0, 1))
        #else
        while(                 (limit=last-first) > 0    )
        #endif
        {
            const_iterator middle = first + (limit>>1);
            if(key < middle->first)
                last = middle;
            else
                first = middle+1;
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
