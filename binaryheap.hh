#ifndef bqtAnimMergerBinaryHeapTypeHH
#define bqtAnimMergerBinaryHeapTypeHH

#include "vectype.hh"

template<typename T, bool Multiple,
         typename key_method>
class BinaryHeapType: private VecType<T>
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
    BinaryHeapType() { }

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
    using rep::front;
    using rep::back;

    using rep::reserve; // extension

    #define make_boundfun(name, type, c) \
       template<typename Kt> \
       type name##_bound(Kt key) c\
           { return name##_bound_template(key, begin(), end()); } \
       template<typename Kt> \
       type name##_bound(Kt key, type b, type e) c \
           { return name##_bound_template(key, b, e); } \
       \
       template<typename Kt> \
       type name##_bound_interp(Kt key) c\
           { return name##_bound_template_interp(key, begin(), end()); } \
       template<typename Kt> \
       type name##_bound_interp(Kt key, type b, type e) c \
           { return name##_bound_template_interp(key, b, e); } \
       \
       template<typename Kt> \
       type name##_bound_guess(Kt key, type guess) c \
           { return name##_bound_template_guess(key, guess, begin(), end()); } \
       template<typename Kt> \
       type name##_bound_guess(Kt key, type guess, type b, type e) c \
           { return name##_bound_template_guess(key, guess, b, e); }
    #define make_versions(type, c) \
        make_boundfun(lower, type, c) \
        make_boundfun(upper, type, c) \
        \
        template<typename Kt> \
        type find(Kt key) c \
            { return find_template(key, lower_bound(key), end()); } \
        template<typename Kt> \
        type find(Kt key, type b, type e) c \
            { return find_template(key, lower_bound(key,b,e), end()); } \
        \
        template<typename Kt> \
        type find_interp(Kt key) c \
            { return find_template(key, lower_bound_intep(key), end()); } \
        template<typename Kt> \
        type find_interp(Kt key, type b, type e) c \
            { return find_template(key, lower_bound_intep(key,b,e), end()); } \
        \
        template<typename Kt> \
        type find_guess(Kt key, type guess) c \
            { return find_template(key, lower_bound_guess(key, guess), end()); } \
        template<typename Kt> \
        type find_guess(Kt key, type guess, type b, type e) c \
            { return find_template(key, lower_bound_guess(key,guess,b,e), end()); }
    make_versions(typename rep::iterator, )
    make_versions(typename rep::const_iterator, const)
    #undef make_versions
    #undef make_boundfun

    void insert(const T& val)
    {
        typename rep::iterator i = lower_bound( key_method()(val) );
        if(!Multiple && (i == this->end() || !( key_method()(val) == key_method()(*i))))
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
    static It lower_bound_template_interp(Kt key, It first, It last)
    {
        typename rep::size_type limit;
      #ifdef __GNUC__
        while(__builtin_expect((limit=last-first) > 0, 1))
      #else
        while(                 (limit=last-first) > 0    )
      #endif
        {
            It tmp( last ); --tmp;
            const Kt& first_value = key_method()(*first);
            const Kt& last_value  = key_method()(*tmp);
            It middle (
                (key <= first_value) ? first
              : (key >= last_value)  ? tmp
              : (first + (key - first_value) * (last-first) / (last_value-first_value+1)) );

            if(key_method()(*middle) < key)
                first = middle+1;
            else
                last = middle;
        }
        return first;
    }

    template<typename Kt, typename It>
    static It upper_bound_template_interp(Kt key, It first, It last)
    {
        typename rep::size_type limit;
      #ifdef __GNUC__
        while(__builtin_expect((limit=last-first) > 0, 1))
      #else
        while(                 (limit=last-first) > 0    )
      #endif
        {
            It tmp( last ); --tmp;
            const Kt& first_value = key_method()(*first);
            const Kt& last_value  = key_method()(*tmp);
            It middle (
                (key <= first_value) ? first
              : (key >= last_value)  ? tmp
              : (first + (key - first_value) * (last-first) / (last_value-first_value+1)) );

            if(key < key_method()(*middle))
                last = middle;
            else
                first = middle+1;
        }
        return first;
    }

    template<typename Kt, typename It>
    static It lower_bound_template_guess(Kt key, It first, It last, It guess)
    {
      #ifdef __GNUC__
        while(__builtin_expect(first < last), 1)
      #else
        while(first < last)
      #endif
        {
            It middle = guess;
            if(key_method()(*middle) < key)
                first = middle + 1;
            else
                last = middle;
            guess = first + ((last-first) >> 1);
        }
        return first;
    }

    template<typename Kt, typename It>
    static It upper_bound_template_guess(Kt key, It first, It last, It guess)
    {
      #ifdef __GNUC__
        while(__builtin_expect(first < last, 1))
      #else
        while(first < last)
      #endif
        {
            It middle = guess;
            if(key < key_method()(*middle))
                last = middle;
            else
                first = middle + 1;
            guess = first + ((last-first) >> 1);
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
