template<unsigned BUFFER_SIZE = 16>
class MostUsedWithinPixel
{
    // Read the first BUFFER_SIZE amount of pixels into this buffer.
    // The last item is reused as a place for the current most common pixel.
    unsigned pixels[BUFFER_SIZE];

    // Index to the current unused place in the buffer:
    unsigned pixelIndex;

 public:
    MostUsedWithinPixel(): pixelIndex(0)
    {
        // By default the color of the current most common pixel is black:
        pixels[BUFFER_SIZE-1] = 0;
    }

    void set(unsigned r, unsigned g, unsigned b)
    {
        // If we have already read BUFFER_SIZE pixels, ignore the rest:
        if(pixelIndex == BUFFER_SIZE) return;
        const unsigned newPixel = (r<<16) | (g<<8) | b;
        set_p(newPixel);
    }

    void set(uint32 p)
    {
        if(pixelIndex == BUFFER_SIZE) return;
        set_p(p);
    }

    void set_p(const unsigned newPixel)
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
