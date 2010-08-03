#ifndef bqtAnimMergerSetTypeHH
#define bqtAnimMergerSetTypeHH

#include "vectype.hh"

template<typename K, bool Multiple>
class SetBaseType
{
public:
    typedef K value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
    typedef VecType<value_type> rep;
    rep data;

public:
    SetBaseType() { }

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

    typename rep::size_type size() const { return data.size(); }

    void insert(iterator i, const value_type& val)
    {
        data.insert(i, val);
    }
    void insert(const value_type& val)
    {
        iterator i = lower_bound(val);
        if(!Multiple && i != end() && *i == val) return;
        data.insert(i, val);
    }
    const_iterator find(const value_type& val) const
    {
        const_iterator i = lower_bound(val);
        if(i != end() && !(*i == val)) return end();
        return i;
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
            if(*middle < key)
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
            if(*middle < key)
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
            if(key < *middle)
                last = middle;
            else
                first = middle+1;
        }
        return first;
    }
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
