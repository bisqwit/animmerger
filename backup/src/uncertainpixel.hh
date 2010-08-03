typedef unsigned int uint32 ;
typedef unsigned char uint8 ;
typedef unsigned short uint16;

#define PIXEL_METHOD_AVERAGE     0
#define PIXEL_METHOD_MOST_USED   1
#define PIXEL_METHOD_MOST_USED16 2
#define PIXEL_METHOD_CHANGELOG   3
#define PIXEL_METHOD_LOOPINGLOG  4
#define PIXEL_METHOD_LAST        5

#define PIXEL_METHOD    PIXEL_METHOD_CHANGELOG
//#define PIXEL_METHOD    PIXEL_METHOD_MOST_USED16
//#define PIXEL_METHOD    PIXEL_METHOD_AVERAGE
//#define PIXEL_METHOD    PIXEL_METHOD_LOOPINGLOG

#if PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
static const unsigned LoopLength = 90;
#endif

/* undef or define. */
#define CHANGELOG_GUESS_OUTSIDES

static const uint32 DefaultPixel = 0x404041;

#include <utility>
#include <algorithm>
#include <vector>

#if 1
#define VecType std::vector
#else
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

template<typename K,typename V>
class MapType
{
public:
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<K,V> value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
private:
    typedef VecType<value_type> rep;
    rep data;
    
public:
    MapType() { }
    
    typedef typename rep::iterator iterator;
    typedef typename rep::const_iterator const_iterator;
    
    const_iterator begin() const { return data.begin(); }
    const_iterator end() const { return data.end(); }
    iterator begin() { return data.begin(); }
    iterator end() { return data.end(); }
    
    void clear()
    {
        std::_Destroy(&data);
        std::_Construct(&data);
    }
    
    iterator lower_bound(K key)
    {
        iterator first = begin(), last = end();
        unsigned limit;
        while(__builtin_expect((limit=last-first) > 0, 1))
        {
            iterator middle = first + (limit>>1);
            if(middle->first < key)
                first = middle+1;
            else
                last = middle;
        }
        return first;        
    }
    V& operator[] (K key)
    {
        iterator i = lower_bound(key);
        if(i == end() || i->first > key)
        {
            i = data.insert(i, value_type(key, V()));
        }
        return i->second;
    }
};

class MostUsedPixel
{
    typedef MapType<uint32, unsigned short> vmap;
    vmap values;
    unsigned pix;
    unsigned short max;
    //bool final;
public:
    MostUsedPixel() : pix(DefaultPixel),max(0) //, final(false)
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        //if(final) return;
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        set_p(p);
    }
    void set_p(uint32 p)
    {
        unsigned short v = ++values[p];
        if(v > max) { max = v; pix = p; }
        //if(v > 300) { final=true; values.clear(); }
    }
    uint32 value_ignore(uint32 ignore) const
    {
        if(pix != ignore) return pix;
        unsigned max2 = 0;
        uint32 result = DefaultPixel;
        for(vmap::const_iterator i = values.begin(); i != values.end(); ++i)
        {
            if(i->first != ignore)
            {
                unsigned v = i->second;
                if(v > max2) { max2 = v; result = i->first; }
            }
        }
        return result;
    }
    operator uint32() const { return pix; }
    void Compress()
    {
        values.clear();
    }
    void CompressButIgnore(uint32 ignore)
    {
        pix = value_ignore(ignore);
        values.clear();
    }
};

class AveragePixel
{
    unsigned r,g,b;
    unsigned n;
    unsigned pix;
public:
    AveragePixel() : r(0),g(0),b(0),n(0), pix(DefaultPixel)
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        r+=R; g+=G; b+=B;
        ++n;
        
        pix = (((r/n) << 16) + ((g/n) << 8) + (b/n));
    }
    operator uint32() const { return pix; }
    void Compress()
    {
    }
};

class LastPixel
{
    unsigned pix;
public:
    LastPixel() : pix(DefaultPixel)
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        set_p(((R) << 16) + ((G) << 8) + B);
    }
    void set_p(uint32 p)
    {
        pix = p;
    }
    operator uint32() const { return pix; }
    void Compress()
    {
    }
};

#if PIXEL_METHOD==PIXEL_METHOD_AVERAGE
typedef AveragePixel UncertainPixel;
#endif

#if PIXEL_METHOD==PIXEL_METHOD_MOST_USED
typedef MostUsedPixel UncertainPixel;
#endif

#if PIXEL_METHOD==PIXEL_METHOD_MOST_USED16
class UncertainPixel
{
    static const unsigned BUFFER_SIZE = 16;

    // Read the first BUFFER_SIZE amount of pixels into this buffer.
    // The last item is reused as a place for the current most common pixel.
    unsigned pixels[BUFFER_SIZE];

    // Index to the current unused place in the buffer:
    unsigned pixelIndex;

 public:
    UncertainPixel(): pixelIndex(0)
    {
        // By default the color of the current most common pixel is black:
        pixels[BUFFER_SIZE-1] = 0;
    }

    void set(unsigned r, unsigned g, unsigned b)
    {
        // If we have already read BUFFER_SIZE pixels, ignore the rest:
        if(pixelIndex == BUFFER_SIZE) return;

        const unsigned newPixel = (r<<16) | (g<<8) | b;

        // Insert the new pixel into its sorted place:
        unsigned i = pixelIndex;
        while(i > 0 && pixels[i-1] > newPixel)
        {
            pixels[i] = pixels[i-1];
            --i;
        }
        pixels[i] = newPixel;
        ++pixelIndex;

        // Count the most used pixel and put it to end of the buffer:
        unsigned currentMaxCount = 0, mostUsedPixel = 0;
        for(i = 0; i < pixelIndex;)
        {
            unsigned currentPixel = pixels[i], count = 1;
            while(++i < pixelIndex && pixels[i] == currentPixel)
                ++count;
            if(count > currentMaxCount)
            {
                mostUsedPixel = currentPixel;
                currentMaxCount = count;
            }
        }
        pixels[BUFFER_SIZE-1] = mostUsedPixel;
    }

    operator uint32() const
    {
        return pixels[BUFFER_SIZE-1];
    }

    void Compress()
    {
    }
};
#endif

#if PIXEL_METHOD==PIXEL_METHOD_CHANGELOG
#include <vector>
#include <algorithm>
extern unsigned CurrentTimer;
class UncertainPixel
{
    struct historyitem
    {
        unsigned moment;
        unsigned pixel;
    public:
        historyitem() { }
        
        // Because std::upper_bound doesn't allow searching by types
        // other than what the container has, here are utility functions
        // to handle this container as if it only contained the "moment" value.
        // Conveniently, it allows also the insertion of the "first moment" pixel.
        historyitem(unsigned time) : moment(time) { }
        bool operator< (const historyitem& b) const { return moment < b.moment; }
    };
    
    std::vector<historyitem> history;
    unsigned lastpixel;
    unsigned lastmoment;

#ifdef CHANGELOG_GUESS_OUTSIDES
    bool FirstMomentIsVague;
    MostUsedPixel first_moment;
#endif

#if 0
    MapType<uint32, unsigned short> minvalues;
#endif
public:
    UncertainPixel() : lastpixel(DefaultPixel), lastmoment(0)
#ifdef CHANGELOG_GUESS_OUTSIDES
        , FirstMomentIsVague(true)
#endif
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        unsigned p = (((R) << 16) + ((G) << 8) + (B));
        
        if(CurrentTimer == 0)
        {
            // Ignore first frame. It's gray.
            return;
        }
        
#if 0
        ++minvalues[p];
#endif

        // Store the value into the history.
        // (But only if it's different than previous value.)
        if(lastpixel != p || history.empty())
        {
            lastpixel = p;
            
            if(CurrentTimer == 0)
            {
                // The value of the pixel at first moment is precisely known.
#ifdef CHANGELOG_GUESS_OUTSIDES
                FirstMomentIsVague = false;
#endif
            }
            else if(history.empty())
            {
                // The value of the pixel at the first moment is not precisely known.
#ifdef CHANGELOG_GUESS_OUTSIDES
                FirstMomentIsVague = true;
                history.push_back(historyitem(0));
#endif
            }
            
            historyitem item;
            item.pixel  = p;
            item.moment = CurrentTimer;
            
            history.push_back(item);
        }
#ifdef CHANGELOG_GUESS_OUTSIDES
        if(FirstMomentIsVague) UpdateFirstMoment(p);
#endif
        lastmoment = CurrentTimer;
    }
    
    operator uint32() const
    {
#if 0
        /* Return the rarest of the last N values. */
        unsigned tmpmin = lastmoment;
        unsigned retval = Find(0);
        for(MapType<uint32, unsigned short>::const_iterator
               i = minvalues.begin();
               i != minvalues.end();
               ++i)
        {
            if(i->second <= tmpmin) { tmpmin = i->second; retval = i->first; }
        }
        return retval;
#else
        return Find(CurrentTimer);
#endif
    }

    void Compress()
    {
#ifdef CHANGELOG_GUESS_OUTSIDES
        first_moment.Compress();
#endif
    }

private:
    uint32 Find(unsigned time) const
    {
        // Find the pixel value that was present at the given time.
        /*
          map::lower_bound: 
            Returns an iterator pointing to first element >= key, or end().
          map::upper_bound:
            Returns an iterator pointing to the first element > key, or end().
          
          What we want is an iterator pointing
            to the last element that is <= key.
         */
        std::vector<historyitem>::const_iterator
            i = std::upper_bound(history.begin(), history.end(), historyitem(time));

        /* Pre-begin value: reasonable default */
        if(i == history.begin())
        {
#ifdef CHANGELOG_GUESS_OUTSIDES
            return lastpixel;
#else
            return DefaultPixel;
#endif
        }

#if 1
        /* Post-end value: Special handling */
        if(i == history.end())
        {
            /* If the last value is somewhat behind the chosen moment,
             * take the reasonable default instead */
            if(lastmoment+0 < time)
            {
#ifdef CHANGELOG_GUESS_OUTSIDES
                return history[0].pixel;
#else
                return DefaultPixel;
#endif
            }
        }
#endif
        
        /* Anything else. Take the value. */
        --i;
        return i->pixel;
    }

private:    
#ifdef CHANGELOG_GUESS_OUTSIDES
    void UpdateFirstMoment(uint32 p)
    {
        // Updates the value of the pixel at the first moment to the value
        // that has appeared the most.
        first_moment.set_p(p);
        history[0].pixel = first_moment;
    }
#endif
};
#endif

#if PIXEL_METHOD==PIXEL_METHOD_LOOPINGLOG
#include <vector>
#include <algorithm>
extern unsigned CurrentTimer;
class UncertainPixel
{
    MostUsedPixel most_used;

    LastPixel history[LoopLength];
public:
    UncertainPixel()
    {
    }
    void set(unsigned R,unsigned G,unsigned B)
    {
        uint32 p = (((R) << 16) + ((G) << 8) + (B));
        if(CurrentTimer == 0)
        {
            // Ignore first frame. It's gray.
            return;
        }
        most_used.set_p(p);

        unsigned offs = CurrentTimer % LoopLength;
        if(history[offs] == DefaultPixel || p != most_used)
            history[offs].set_p(p);
    }
    operator uint32() const
    {
        unsigned offs = CurrentTimer % LoopLength;
        uint32 result = history[offs];//.value_ignore(most_used);
        if(result == DefaultPixel) return most_used;
        return result;
    }

    void Compress()
    {
        most_used.Compress();
        //for(unsigned a=0; a<LoopLength; ++a)
        //    history[a].CompressButIgnore(most_used);
    }
};
#endif

#if PIXEL_METHOD==PIXEL_METHOD_LAST
typedef LastPixel UncertainPixel;
#endif
