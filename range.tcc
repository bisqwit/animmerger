#include <algorithm> // for std::max, std::min
#include "range.hh"

/* map::lower_bound(k) = find the first element whose key >= k */
/* map::upper_bound(k) = find the first element whose key > k */

template<typename Key, typename Valueholder, typename Allocator>
void rangecollection<Key,Valueholder,Allocator>::erase(const Key& lo, const Key& up)
{
    typename Cont::iterator next_thing = data.lower_bound(up);
    if(next_thing != data.end() && next_thing->first == up)
    {
        /* If our "up" is already defined */
        if(next_thing->second.is_nil())
        {
            /* Erase it, if it's the same as us */
            data.erase(next_thing);
        }
    }
    else if(next_thing == data.begin())
    {
        /* If the world begins after our end, do nothing */
    }
    else
    {
        --next_thing;
        if(!next_thing->second.is_nil())
        {
            /* We need this node at our "up" */
            data.insert(std::make_pair(up, next_thing->second));
        }
    }

    /*
     -  Erase all elements that are left inside our range
    */
    for(typename Cont::iterator j,i = data.lower_bound(lo);
        i != data.end() && i->first < up;
        i=j)
    {
        j=i; ++j;
        data.erase(i);
    }

    /*
     -  Find what was going on before <lo>
        If it does not exist or was different than this,
          Insert the new value at <lo>
    */
    typename Cont::iterator prev_thing = data.lower_bound(lo);
    if(prev_thing == data.begin())
    {
        /* If nothing was going on, we do nothing */
    }
    else
    {
        /* Start us with a nil if the previous node wasn't nil */
        --prev_thing;
        if(!prev_thing->second.is_nil())
        {
            data.insert(std::make_pair(lo, Valueholder()));
        }
    }
}

template<typename Key, typename Valueholder, typename Allocator>
void rangecollection<Key,Valueholder,Allocator>::erase_before(const Key& lo)
{
    if(!empty())
    {
        const_iterator b = begin();
        if(b->first < lo) erase(b->first, lo);
    }
}

template<typename Key, typename Valueholder, typename Allocator>
void rangecollection<Key,Valueholder,Allocator>::erase_after(const Key& hi)
{
    if(!empty())
    {
        typename Cont::const_reverse_iterator b = data.rbegin();
        if(b->first > hi) erase(hi, b->first);
    }
}

template<typename Key, typename Valueholder, typename Allocator>
  template<typename Valuetype>
void rangecollection<Key,Valueholder,Allocator>::set(const Key& lo, const Key& up, const Valuetype& val)
{
    Valueholder newvalue(val);

    /*
     -  Find what is supposed to be continuing after this block
        If nothing was going on before the end, add nil at <up>
        If was different than the new value, add it at <up>
    */
    typename Cont::iterator next_thing = data.lower_bound(up);
    if(next_thing != data.end() && next_thing->first == up)
    {
        /* If our "up" is already defined */
        if(next_thing->second == newvalue)
        {
            /* Erase it, if it's the same as us */
            data.erase(next_thing);
        }
    }
    else if(next_thing == data.begin())
    {
        /* We need a nil node at our "up" */
        data.insert(std::make_pair(up, Valueholder()));
    }
    else
    {
        --next_thing;
        if(next_thing->second != newvalue)
        {
            /* We need this node at our "up" */
            data.insert(std::make_pair(up, next_thing->second));
        }
    }

    /*
     -  Erase all elements that are left inside our range
    */
    for(typename Cont::iterator j,i = data.lower_bound(lo);
        i != data.end() && i->first < up;
        i=j)
    {
        j=i; ++j;
        data.erase(i);
    }

    /*
     -  Find what was going on before <lo>
        If it does not exist or was different than this,
          Insert the new value at <lo>
    */
    typename Cont::iterator prev_thing = data.lower_bound(lo);
    if(prev_thing == data.begin())
    {
        data.insert(std::make_pair(lo, newvalue));
    }
    else
    {
        --prev_thing;
        if(prev_thing->second != newvalue)
        {
            data.insert(std::make_pair(lo, newvalue));
        }
    }
}

template<typename Key, typename Valueholder, typename Allocator>
void rangecollection<Key,Valueholder,Allocator>::flip(const Key& floor, const Key& ceil)
{
    const_iterator i = lower_bound(floor);
    if(i == end() || i->first != floor)
    {
        ///// UNDER CONSTRUCTION
    }
}

template<typename Key, typename Valueholder, typename Allocator>
const typename rangecollection<Key,Valueholder,Allocator>::const_iterator
    rangecollection<Key,Valueholder,Allocator>::find(const Key& v) const
{
    typename Cont::const_iterator tmp = data.lower_bound(v);
    if(tmp == data.end())
    {
        return tmp;
        // return end(); -^ the same thing
    }
    if(tmp->first > v)
    {
        // This range begins after the given key, so get the previous one
        if(tmp == data.begin()) return end();
        --tmp;
    }
    if(tmp->second.is_nil()) return end();
    return tmp;
}

template<typename Key>
rangetype<Key> rangetype<Key>::intersect(const rangetype<Key>& b) const
{
    rangetype<Key> result;
    result.lower = std::max(lower, b.lower);
    result.upper = std::min(upper, b.upper);
    if(result.upper < result.lower) result.upper = result.lower;
    return result;
}
