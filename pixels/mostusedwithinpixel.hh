template<unsigned BUFFER_SIZE = 16>
class MostUsedWithinPixel
{
    // Read the first BUFFER_SIZE amount of pixels into this buffer.
    // The last item is reused as a place for the current most common pixel.
    uint32 pixels[BUFFER_SIZE];

    // Index to the current unused place in the buffer:
    unsigned pixelIndex;

 public:
    MostUsedWithinPixel(): pixelIndex(0)
    {
        // By default the color of the current most common pixel is black:
        pixels[BUFFER_SIZE-1] = DefaultPixel;
    }

    void set(unsigned r, unsigned g, unsigned b, unsigned=0) FastPixelMethod
    {
        // If we have already read BUFFER_SIZE pixels, ignore the rest:
        if(pixelIndex == BUFFER_SIZE) return;
        const unsigned newPixel = (r<<16) | (g<<8) | b;
        set_p(newPixel);
    }

    void set(uint32 p, unsigned=0) FasterPixelMethod
    {
        if(pixelIndex == BUFFER_SIZE) return;
        set_p(p);
    }

    void set_p(const uint32 newPixel) FastPixelMethod
    {
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
        unsigned currentMaxCount = 0;
        uint32 mostUsedPixel = 0;
        for(i = 0; i < pixelIndex;)
        {
            uint32 currentPixel = pixels[i];
            unsigned count = 1;
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
    
    inline uint32 get(unsigned=0) const FasterPixelMethod
    {
        return pixels[BUFFER_SIZE-1];
    }

    void Compress()
    {
    }
};
