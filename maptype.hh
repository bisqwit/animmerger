#ifndef bqtAnimMergerMapTypeHH
#define bqtAnimMergerMapTypeHH

#include <utility>
#include "vectype.hh"

template<typename K,typename V, bool Multiple>
class MapBaseType
{
public:
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<K, V> value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
    typedef VecType<value_type> rep;
    rep data;

public:
    MapBaseType() { }

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
        data.~rep();
        new(&data) rep();
    }
    bool empty() const
    {
        return data.empty();
    }

    template<typename Kt>
    iterator lower_bound(Kt key)
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

    template<typename Kt>
    const_iterator lower_bound(Kt key) const
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

    template<typename Kt>
    const_iterator upper_bound(Kt key) const
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

    template<typename Kt>
    iterator find(Kt key)
    {
        iterator i = lower_bound(key);
        return i == end() || !(key==i->first) ? end() : i;
    }

    template<typename Kt>
    const_iterator find(Kt key) const
    {
        iterator i = lower_bound(key);
        return i == end() || !(key==i->first) ? end() : i;
    }

    template<typename Kt>
    V& operator[] (Kt key)
    {
        iterator i = lower_bound(key);
        if(i == end() || !(key==i->first))
        {
            i = data.insert(i, value_type(key, V()));
        }
        return i->second;
    }

    void insert(iterator i, const value_type& val)
    {
        data.insert(i, val);
    }
    void insert(const value_type& val)
    {
        iterator i = lower_bound(val.first);
        if(!Multiple && (i == end() || !(val.first==i->first)))
            data.insert(i, val);
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
