#ifndef bqtAnimMergerUntreeTypeHH
#define bqtAnimMergerUntreeTypeHH

#include "vectype.hh"

template<typename T, bool Multiple,
         typename key_method>
class UntreeType: private VecType<T>
{
    typedef VecType<T> rep;
public:
    using typename rep::value_type;
    using typename rep::pointer;
    using typename rep::const_pointer;
    using typename rep::reference;
    using typename rep::const_reference;
    using typename rep::size_type;

public:
    UntreeType() { }

    using typename rep::iterator;
    using typename rep::const_iterator;
    using typename rep::reverse_iterator;
    using typename rep::const_reverse_iterator;
    using rep::begin;
    using rep::end;
    using rep::rbegin;
    using rep::rend;
    using rep::clear;
    using rep::empty;
    using rep::size;
    using rep::insert;
    using rep::erase;

    template<typename Kt>
    typename rep::iterator lower_bound(Kt key)
    {
        return lower_bound_template(key, begin(), end());
    }

    template<typename Kt>
    typename rep::const_iterator lower_bound(Kt key) const
    {
        return lower_bound_template(key, begin(), end());
    }

    template<typename Kt>
    typename rep::iterator upper_bound(Kt key)
    {
        return upper_bound_template(key, begin(), end());
    }

    template<typename Kt>
    typename rep::const_iterator upper_bound(Kt key) const
    {
        return upper_bound_template(key, begin(), end());
    }

    template<typename Kt>
    typename rep::iterator find(Kt key)
    {
        return find_template(key, lower_bound(key), end());
    }

    template<typename Kt>
    typename rep::const_iterator find(Kt key) const
    {
        return find_template(key, lower_bound(key), end());
    }

    void insert(const T& val)
    {
        typename rep::iterator i = lower_bound( key_method()(val) );
        if(!Multiple && (i == end() || !( key_method()(val) == key_method()(*i))))
            insert(i, val);
    }
    template<typename It>
    void insert(It first, It last)
    {
        while(first != last) insert(*first++);
    }

private:
    template<typename Kt, typename It>
    static It lower_bound_template(Kt key, It first, It last)
    {
        typename rep::size_type limit;
      #ifdef __GNUC__
        while(__builtin_expect((limit=last-first) > 0, 1))
      #else
        while(                 (limit=last-first) > 0    )
      #endif
        {
            It middle = first + (limit>>1);
            if(key_method()(*middle) < key)
                first = middle+1;
            else
                last = middle;
        }
        return first;
    }

    template<typename Kt, typename It>
    static It upper_bound_template(Kt key, It first, It last)
    {
        typename rep::size_type limit;
      #ifdef __GNUC__
        while(__builtin_expect((limit=last-first) > 0, 1))
      #else
        while(                 (limit=last-first) > 0    )
      #endif
        {
            It middle = first + (limit>>1);
            if(key < key_method()(*middle))
                last = middle;
            else
                first = middle+1;
        }
        return first;
    }

    template<typename Kt, typename It>
    static It find_template(Kt key, It i, It last)
    {
        return i == last || !(key==(key_method()(*i))) ? last : i;
    }
};

#endif
