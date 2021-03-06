#ifndef bqtAnimMergerVecTypeHH
#define bqtAnimMergerVecTypeHH

#if 0

#include <vector>
#define VecType std::vector

#else

#include <algorithm>
#include <iterator>

template<typename T, typename SizeType = unsigned>
class VecType
{
public:
    typedef T value_type;
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef SizeType size_type;

public:
    VecType() : data(0),len(0),cap(0) { }
    ~VecType() { clear(); if(cap) getalloc().deallocate(data,cap); }

    VecType(size_type length) : data(0),len(0),cap(0)
    {
        resize(length);
    }
    template<typename K>
    VecType(size_type length, K value) : data(0),len(0),cap(0)
    {
        resize(length, value);
    }
    VecType(const VecType& b) : data(0), len(b.len), cap(b.len)
    {
        if(len)
        {
            data = getalloc().allocate(len);
            copy_construct(&data[0], &b.data[0], len);
        }
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    VecType(VecType&& b): data(b.data), len(b.len), cap(b.cap)
    {
        b.data = 0;
        b.len  = 0;
        b.cap  = 0;
    }
#endif
    VecType& operator= (const VecType& b)
    {
        if(&b == this) return *this;
        if(len < b.len)
        {
            reserve(b.len);
            copy_assign(&data[0], &b.data[0], len);
            copy_construct(&data[len], &b.data[len], b.len-len);
        }
        else if(len > b.len)
        {
            destroy(&data[b.len], len-b.len);
            copy_assign(&data[0], &b.data[0], b.len);
        }
        len = b.len;
        return *this;
    }
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    VecType& operator= (VecType&& b)
    {
        if(&b != this) swap(b);
        return *this;
    }
#endif

    template<typename It>
    void assign(It first, It last)
    {
        size_type newlen = (size_type) std::distance(first, last);
        if(cap < newlen)
        {
            destroy(&data[0], len);
            getalloc().deallocate(data, cap);
            data = getalloc().allocate(cap = newlen);
            copy_construct(&data[0], first, newlen);
        }
        else if(len < newlen)
            copy_construct(&data[len],
                copy_assign(&data[0], first, len),
                newlen-len);
        else // len >= newlen
        {
            copy_assign(&data[0], first, newlen);
            destroy(&data[newlen], len-newlen);
        }
        len = newlen;
    }

public:
    reference operator[] (size_type ind) { return data[ind]; }
    const_reference operator[] (size_type ind) const { return data[ind]; }
    iterator begin() { return data; }
    iterator end() { return data+len; }
    reference front() { return *begin(); }
    reference back() { return *rbegin(); }
    const_iterator begin() const { return data; }
    const_iterator end() const { return data+len; }
    const_reference front() const { return *begin(); }
    const_reference back() const { return *rbegin(); }
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    reverse_iterator rbegin() { return reverse_iterator( end() ); }
    reverse_iterator rend()   { return reverse_iterator( begin() ); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator( end() ); }
    const_reverse_iterator rend()   const { return const_reverse_iterator( begin() ); }

    template<typename K>
    void push_back(K value)
    {
        //insert(end(), value);
        if(len >= cap) reserve(cap ? cap*2 : default_size());
        new(&data[len++]) T ( value );
    }

    template<typename K>
    iterator insert(iterator pos, K value)
    {
        size_type ins_pos = pos - begin();
        if(len < cap)
        {
            if(ins_pos == len)
                new(&data[ins_pos]) T( value );
            else
            {
                move_construct(&data[len], &data[len-1], 1);
                move_assign_backwards(&data[ins_pos+1], &data[ins_pos], len-ins_pos);
                data[ins_pos] = value;
            }
            ++len;
            return data+ins_pos;
        }
        size_type newcap = cap ? cap*2 : default_size();
        if(ins_pos == len)
        {
            reserve(newcap);
            new(&data[ins_pos]) T( value );
            ++len;
            return data+ins_pos;
        }
        T* newdata = getalloc().allocate(newcap);
        move_construct(&newdata[0], &data[0], ins_pos);
        new(&newdata[ins_pos]) T( value );
        move_construct(&newdata[ins_pos+1], &data[ins_pos], len-ins_pos);
        destroy(&data[0], len);
        getalloc().deallocate(data, cap);
        ++len;
        data = newdata;
        cap  = newcap;
        return data+ins_pos;
    }

#ifdef __GXX_EXPERIMENTAL_CXX0X__
    template<typename K>
    iterator insert_move(iterator pos, K&& value)
    {
        size_type ins_pos = pos - begin();
        if(len < cap)
        {
            if(ins_pos == len)
                new(&data[ins_pos]) T( std::move(value) );
            else
            {
                move_construct(&data[len], &data[len-1], 1);
                move_assign_backwards(&data[ins_pos+1], &data[ins_pos], len-ins_pos);
                data[ins_pos] = std::move(value);
            }
            ++len;
            return data+ins_pos;
        }
        size_type newcap = cap ? cap*2 : default_size();
        if(ins_pos == len)
        {
            reserve(newcap);
            new(&data[ins_pos]) T( std::move(value) );
            ++len;
            return data+ins_pos;
        }
        T* newdata = getalloc().allocate(newcap);
        move_construct(&newdata[0], &data[0], ins_pos);
        new(&newdata[ins_pos]) T( std::move(value) );
        move_construct(&newdata[ins_pos+1], &data[ins_pos], len-ins_pos);
        destroy(&data[0], len);
        getalloc().deallocate(data, cap);
        ++len;
        data = newdata;
        cap  = newcap;
        return data+ins_pos;
    }
#endif

    template<typename It>
    void insert(iterator pos, It first, It last)
    {
        size_type ins_pos = pos - begin();
        size_type count   = (size_type) std::distance(first, last);

        if(len+count <= cap)
        {
            if(ins_pos == len)
                copy_construct(&data[ins_pos], first, count);
            else
            {
                /*******
                 *      012345678
                 *           ^NM
                 *      ins_pos = 5, count = 2
                 *      len = 9, new_length = 11
                 *      tail_length = 4 ("5678")
                 *      tail_source_pos = 5
                 *      tail_target_pos = 7
                 *      num_tail_bytes_after_current_area = min(tail_length, count)
                 */
                //unsigned new_length = len + count;
                unsigned tail_length = len - ins_pos;
                unsigned tail_source_pos = ins_pos;
                unsigned tail_target_pos = ins_pos + count;
                unsigned num_tail_bytes_after_current_area =
                    count < tail_length ? count : tail_length;
                unsigned num_tail_bytes_to_movecopy =
                    tail_length - num_tail_bytes_after_current_area;
                move_construct(&data[tail_target_pos + num_tail_bytes_to_movecopy],
                               &data[tail_source_pos+num_tail_bytes_to_movecopy],
                               num_tail_bytes_after_current_area);
                if(num_tail_bytes_to_movecopy > 0)
                {
                    move_assign_backwards(
                        &data[ins_pos+count],
                        &data[ins_pos],
                        num_tail_bytes_to_movecopy);
                }
                unsigned num_piece_bytes_after_current_area =
                    (ins_pos + count) > len ? (ins_pos+count)-len : 0;
                unsigned num_piece_bytes_to_copy =
                    count - num_piece_bytes_after_current_area;
                if(num_piece_bytes_to_copy > 0)
                {
                    copy_assign(
                        &data[ins_pos],
                        first,
                        num_piece_bytes_to_copy);
                }
                if(num_piece_bytes_after_current_area > 0)
                {
                    copy_construct(
                        &data[len],
                        first+num_piece_bytes_to_copy,
                        num_piece_bytes_after_current_area);
                }
            }
            len += count;
            return;
        }
        size_type newcap = (cap+count)*2;
        if(ins_pos == len)
        {
            reserve(newcap);
            copy_construct(&data[ins_pos], first, count);
            len += count;
            return;
        }
        T* newdata = getalloc().allocate(newcap);
        move_construct(&newdata[0], &data[0], ins_pos);
        copy_construct(&newdata[ins_pos], first, count);
        move_construct(&newdata[ins_pos+count], &data[ins_pos], len-ins_pos);
        destroy(&data[0], len);
        getalloc().deallocate(data, cap);
        len += count;
        data = newdata;
        cap  = newcap;
    }

    void erase(iterator pos)
    {
        size_type del_pos = pos - begin();
        move_assign(&data[del_pos], &data[del_pos+1], len-del_pos-1);
        destroy(&data[--len], 1);
    }
    void erase(iterator first, iterator last)
    {
        size_type del_pos = first - begin();
        size_type count   = last - first;
        if(!count) return;
        move_assign(&data[del_pos], &data[del_pos+count], len-del_pos-count);
        destroy(&data[len-count], count);
        len -= count;
    }

    void reserve(size_type newcap)
    {
        if(cap < newcap)
        {
            T* newdata = getalloc().allocate(newcap);
            move_construct(&newdata[0], &data[0], len);
            destroy(&data[0], len);
            getalloc().deallocate(data, cap);
            data = newdata;
            cap  = newcap;
        }
    }

    void pop_back()
    {
        destroy(&data[--len], 1);
    }

    void resize(size_type newlen)
    {
        if(newlen < len)
        {
            destroy(&data[newlen], len-newlen);
            len = newlen;
        }
        else if(newlen == len)
            return;
        else if(newlen <= cap) // newlen > len, too.
        {
            construct(&data[len], newlen-len);
            len = newlen;
        }
        else // newlen > cap, and newlen > len.
        {
            // reallocation required
            size_type newcap = newlen;
            T* newdata = getalloc().allocate(newcap);
            move_construct(&newdata[0], &data[0], len);
            destroy(&data[0], len);
            getalloc().deallocate(data, cap);
            construct(&newdata[len], newlen-len);
            data = newdata;
            len  = newlen;
            cap  = newcap;
        }
    }

    template<typename K>
    void resize(size_type newlen, K value)
    {
        if(newlen < len)
        {
            destroy(&data[newlen], len-newlen);
            len = newlen;
        }
        else if(newlen == len)
            return;
        else if(newlen <= cap) // newlen > len, too.
        {
            construct(&data[len], newlen-len, value);
            len = newlen;
        }
        else // newlen > cap, and newlen > len.
        {
            // reallocation required
            size_type newcap = newlen;
            T* newdata = getalloc().allocate(newcap);
            move_construct(&newdata[0], &data[0], len);
            destroy(&data[0], len);
            getalloc().deallocate(data, cap);
            construct(&newdata[len], newlen-len, value);
            data = newdata;
            len  = newlen;
            cap  = newcap;
        }
    }

    void swap(VecType<T>& b)
    {
        std::swap(data, b.data);
        std::swap(len,  b.len);
        std::swap(cap,  b.cap);
    }

    bool empty() const { return len==0; }
    void clear()
    {
        if(!cap) return;
        destroy(&data[0], len);
        len = 0;
        getalloc().deallocate(data, cap);
        data = 0;
        cap = 0;
    }
    size_type size()     const { return len; }
    size_type capacity() const { return cap; }

private:
    static inline void construct(T* target, size_type count)
    {
        for(size_type a=0; a<count; ++a)
            new(&target[a]) T();
    }
    template<typename K>
    static inline void construct(T* target, size_type count, K param)
    {
        for(size_type a=0; a<count; ++a)
            new(&target[a]) T(param);
    }
    static inline void destroy(T* target, size_type count)
    {
        for(size_type a=count; a-- > 0; )
            target[a].~T();
    }
    static inline void move_assign(T* target, T* source, size_type count)
    {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        for(size_type a=0; a<count; ++a)
            target[a] = std::move(source[a]);
#else
        copy_assign(target, source, count);
#endif
    }
    static inline void move_assign_backwards(T* target, T* source, size_type count)
    {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        for(size_type a=count; a-- > 0; )
            target[a] = std::move(source[a]);
#else
        copy_assign_backwards(target, source, count);
#endif
    }
    static inline void move_construct(T* target, T* source, size_type count)
    {
#ifdef __GXX_EXPERIMENTAL_CXX0X__
        for(size_type a=0; a<count; ++a)
            new(&target[a]) T( std::move(source[a]) );
#else
        copy_construct(target, source, count);
#endif
    }
    template<typename It>
    static inline It copy_assign(T* target, It source, size_type count)
    {
        for(size_type a=0; a<count; ++a)
            target[a] = *source++;
        return source;
    }
    template<typename It>
    static inline It copy_assign_backwards(T* target, It source, size_type count)
    {
        for(size_type a=count; a-- > 0; )
            target[a] = source[a];
        return source;
    }
    template<typename It>
    static inline It copy_construct(T* target, It source, size_type count)
    {
        for(size_type a=0; a<count; ++a)
            new(&target[a]) T( *source++ );
        return source;
    }

    static size_t default_size()
    {
        return 1; //sizeof(T) < 16 ? 2 : (sizeof(T) < 256 ? 2 : 1);
    }

    static std::allocator<T>& getalloc()
    {
        static std::allocator<T> alloc;
        return alloc;
    }

private:
    T* data;
    size_type len, cap;
    //__gnu_cxx::__mt_alloc<T> alloc;
};
#endif

#endif
