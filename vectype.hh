#ifndef bqtAnimMergerVecTypeHH
#define bqtAnimMergerVecTypeHH

#if 1

#include <vector>
#define VecType std::vector

#else


#include <algorithm>

template<typename T>
class VecType
{
public:
    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T& reference;
    typedef const T& const_reference;
    
public:
    VecType() : data(0),len(0) { }
    ~VecType() { if(len) delete[] data; }
    
    void reserve(unsigned) { } /* ignore */
    
    VecType(const VecType& b) : len(b.len)
    {
        if(len)
        {
            data = new T[len];
            std::copy(b.begin(),b.end(), data);
        }
    }
    VecType& operator= (const VecType& b)
    {
        if(&b == this) return *this;
        if(len) { delete[] data; if(!b.len) { len=0; data=0; return *this; } }
        len = b.len;
        data = new T[len];
        std::copy(b.begin(),b.end(), data);
        return *this;
    }    
public:
    reference operator[] (unsigned ind) { return data[ind]; }
    iterator begin() { return data; }
    iterator end() { return data+len; }
    const_iterator begin() const { return data; }
    const_iterator end() const { return data+len; }
    unsigned size() const { return len; }
    iterator insert(iterator pos, T value)
    {
        if(!len) { data = new T[1]; *data = value; return data; }
        
        T* newdata = new T[len+1];
        unsigned diff = pos-data;
        std::copy(data,pos, newdata);
        newdata[diff] = value;
        std::copy(pos, data+len, newdata+diff+1);
        delete[] data;
        data = newdata;
        ++len;
        return newdata + diff;
    }
private:
    T* data;
    unsigned len;
};
#endif

#endif
