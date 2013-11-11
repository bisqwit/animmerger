#include "rangemap.hh"


template<typename Key, typename Value, typename Allocator>
const typename rangemap<Key,Value,Allocator>::const_iterator
    rangemap<Key,Value,Allocator>::ConstructIterator(typename Cont::const_iterator i) const
{
    const_iterator tmp(data);
    while(i != data.end() && i->second.is_nil()) ++i;
    tmp.i = i;
    tmp.Reconstruct();
    return tmp;
}
template<typename Key, typename Value, typename Allocator>
void rangemap<Key,Value,Allocator>::const_iterator::Reconstruct()
{
    if(i != data.end())
    {
        rangetype<Key>::lower = i->first;
        typename Cont::const_iterator j = i;
        if(++j != data.end())
            rangetype<Key>::upper = j->first;
        else
            rangetype<Key>::upper = rangetype<Key>::lower;

        if(i->second.is_nil())
        {
            fprintf(stderr, "rangemap: internal error\n");
        }
        value = i->second.get_value();
    }
}
template<typename Key, typename Value, typename Allocator>
typename rangemap<Key,Value,Allocator>::const_iterator& rangemap<Key,Value,Allocator>::const_iterator::operator++ ()
{
    /* The last node before end() is always nil. */
    while(i != data.end())
    {
        ++i;
        if(!i->second.is_nil())break;
    }
    Reconstruct();
    return *this;
}
template<typename Key, typename Value, typename Allocator>
typename rangemap<Key,Value,Allocator>::const_iterator& rangemap<Key,Value,Allocator>::const_iterator::operator-- ()
{
    /* The first node can not be nil. */
    while(i != data.begin())
    {
        --i;
        if(!i->second.is_nil())break;
    }
    Reconstruct();
    return *this;
}


template<typename Key,typename Value,typename Allocator>
rangemap<Key,Value,Allocator> rangemap<Key,Value,Allocator>::intersect(const rangemap<Key,Value,Allocator>& b) const
{
    rangemap<Key,Value,Allocator> result;
    const_iterator ai = begin();
    const_iterator bi = b.begin();

    for(;;)
    {
        if(ai == end()) break;
        if(bi == b.end()) break;

        if(ai->upper <= bi->lower) { ++ai; continue; }
        if(bi->upper <= ai->lower) { ++bi; continue; }

        rangetype<Key> intersection = ai->intersect(bi->range);
        
        if(!intersection.empty())
            result.set(intersection.lower, intersection.upper, ai->value);

        if(ai->upper < bi->upper)         // A is smaller
            ++ai;
        else if(ai->upper == bi->upper)   // equal
            { ++ai; ++bi; }
        else                              // B is smaller
            ++bi;
    }
    return result;
}


template<typename Key,typename Value,typename Allocator>
rangetype<Key> rangemap<Key,Value,Allocator>::find_set_subrange
    (const Key& lo, const Key& up, size_t minlen,
     allocationstrategy strategy) const
{
    rangemap<Key,Value,Allocator> tmp;
    tmp.set(lo,up);
    rangemap<Key,Value,Allocator> clipped_by = tmp.intersect(*this);

    rangetype<Key> retval; bool first=true; size_t prevsize=0;
    for(const_iterator i = clipped_by.begin(); i != clipped_by.end(); ++i)
        if(i.upper - i.lower >= minlen)
            switch(strategy)
            {
                case First:
                    retval = i;
                    return retval;
                case Smallest:
                    if(first) { first=false; prevsize=i.upper-i.lower; retval=i; }
                    else if(i.upper-i.lower < prevsize) { prevsize=i.upper-i.lower; retval=i; }
                    break;
                case Largest:
                    if(first) { first=false; prevsize=i.upper-i.lower; retval=i; }
                    else if(i.upper-i.lower > prevsize) { prevsize=i.upper-i.lower; retval=i; }
                    break;
            }
    return retval;
}

template<typename Key,typename Value,typename Allocator>
rangetype<Key> rangemap<Key,Value,Allocator>::find_unset_subrange
    (const Key& lo, const Key& up, size_t minlen, allocationstrategy strategy) const
{
    rangemap<Key,Value,Allocator> tmp;
    tmp.set(lo,up);
    rangemap<Key,Value,Allocator> clipped_by = tmp.intersect(*this);
    for(const_iterator i = clipped_by.begin(); i != clipped_by.end(); ++i)
        tmp.erase(i.lower, i.upper);
    // Now tmp is the negated intersection

    rangetype<Key> retval; bool first=true; size_t prevsize=0;
    for(const_iterator i = tmp.begin(); i != tmp.end(); ++i)
        if(i.upper - i.lower >= minlen)
            switch(strategy)
            {
                case First:
                    retval = i.range;
                    return retval;
                case Smallest:
                    if(first) { first=false; prevsize=i.upper-i.lower; retval=i.range; }
                    else if(i.upper-i.lower < prevsize) { prevsize=i.upper-i.lower; retval=i.range; }
                    break;
                case Largest:
                    if(first) { first=false; prevsize=i.upper-i.lower; retval=i.range; }
                    else if(i.upper-i.lower > prevsize) { prevsize=i.upper-i.lower; retval=i.range; }
                    break;
            }
    return retval;
}
