#ifndef bqtAnimMergerUntreeTypeHH
#define bqtAnimMergerUntreeTypeHH

#include "vectype.hh"

template<typename T, bool Multiple,
         typename key_method>
class UntreeType
{
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
    typedef VecType<value_type> rep;
    rep data;

public:
    UntreeType() { }

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
            if(key_method()(*middle) < key)
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
            if(key_method()(*middle) < key)
                first = middle+1;
            else
                last = middle;
        }
        return first;
    }

    template<typename Kt>
    iterator upper_bound(Kt key)
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
            if(key < key_method()(*middle))
                last = middle;
            else
                first = middle+1;
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
            if(key < key_method()(*middle))
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
        return i == end() || !(key==(key_method()(*i))) ? end() : i;
    }

    template<typename Kt>
    const_iterator find(Kt key) const
    {
        const_iterator i = lower_bound(key);
        return i == end() || !(key==(key_method()(*i))) ? end() : i;
    }

    iterator insert(iterator i, const value_type& val)
    {
        return data.insert(i, val);
    }
    void insert(const value_type& val)
    {
        iterator i = lower_bound( key_method()(val) );
        if(!Multiple && (i == end() || !( key_method()(val) == key_method()(*i))))
            data.insert(i, val);
    }
    void erase(iterator i)
    {
        data.erase(i);
    }
};

#endif
