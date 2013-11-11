#ifndef bqtRangeMapHH
#define bqtRangeMapHH

#include "range.hh"

/***************
 *
 * The idea of a rangemap is to have a map<> that allows to
 * have large consequent blocks of repetitive content.
 *
 * Implemented using changepoints.
 *
 * i.e., for a rangemap that has 0..5=A, 6..9=B, 12..15=C
 * you would have a map with: 0=A, 6=B, 10=nil, 12=C, 16=nil
 */
template<typename Key, typename Value, typename Allocator = std::allocator<std::pair<Key,Value> > >
class rangemap
{
    class Valueholder
    {
        Value val;
        bool nil;
    public:
        Valueholder(): val(), nil(true) {}
        Valueholder(const Value& v): val(v), nil(false) {}
        void set(const Value& v) { val=v; nil=false; }
        void clear() { val=Value(); nil=true; }
        bool is_nil() const { return nil; }
        bool operator==(const Valueholder& b) const { return nil==b.nil && val==b.val; }
        bool operator!=(const Valueholder& b) const { return !operator==(b); }
        const Value& get_value() const { return val; }
    };
    typedef rangecollection<Key, Valueholder, Allocator> Cont;
    Cont data;

public:
    struct const_iterator: public rangetype<Key>
    {
        const const_iterator* operator-> () const { return this; }
        typename Cont::const_iterator i;
        Value value;
    public:
        const_iterator(const Cont& c): data(c) { }

        bool operator==(const const_iterator& b) const { return i == b.i; }
        bool operator!=(const const_iterator& b) const { return !operator==(b); }
        const_iterator& operator++ ();
        const_iterator& operator-- ();

    private:
        const Cont& data;
        void Reconstruct();
        friend class rangemap;
    };
private:
    const const_iterator ConstructIterator(typename Cont::const_iterator i) const;

public:
    rangemap() : data() {}

    /* Erase everything between the given range */
    void erase(const Key& lo, const Key& up) { data.erase(lo, up); }

    /* Erase a single value */
    void erase(const Key& lo) { data.erase(lo, lo+1); }

    /* Modify the given range to have the given value */
    void set(const Key& lo, const Key& up, const Value& v) { data.set(lo, up, v); }

    /* Adds the given value to each item in the given range */
    void offset(const Key& lo, const Key& up, long offset, bool delete_when_zero = true)
        { data.offset(lo, up, offset, delete_when_zero); }

    void set(const Key& pos, const Value& v) { set(pos, pos+1, v); }

    /* Calculates the intersection between this map and the other.
     * Values are copied from this map, not the other.
     */
    rangemap intersect(const rangemap& b) const;

    /* Find the range that has this value */
    const_iterator find(const Key& v) const { return ConstructIterator(data.find(v)); }

    /* Find subranges at least minlen long */
    typedef enum { First, Smallest, Largest } allocationstrategy;

    rangetype<Key> find_set_subrange
        (const Key& lo, const Key& up, size_t minlen,
         allocationstrategy strategy = First) const;

    rangetype<Key> find_unset_subrange
        (const Key& lo, const Key& up, size_t minlen,
         allocationstrategy strategy = First) const;

    /* Standard functions */
    const_iterator begin() const { return ConstructIterator(data.begin()); }
    const_iterator end() const { return ConstructIterator(data.end()); }
    const_iterator lower_bound(const Key& v) const { return ConstructIterator(data.lower_bound(v)); }
    const_iterator upper_bound(const Key& v) const { return ConstructIterator(data.upper_bound(v)); }
    unsigned size() const { return data.size(); }
    bool empty() const { return data.empty(); }
    void clear() { data.clear(); }

    bool operator==(const rangemap& b) const { return data == b.data; }
    bool operator!=(const rangemap& b) const { return !operator==(b); }

    // default copy cons. and assign-op. are fine
};

#include "rangemap.tcc"

#endif
