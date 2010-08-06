#include <vector>
#include <list>

#include "alloc/FSBAllocator.hh"
#include "openmp.hh"
#include "sprites.hh"

const unsigned Minimum_Sprite_Width  = 4;
const unsigned Minimum_Sprite_Height = 4;

bool DifferingSection::operator< (const DifferingSection& b) const
{
    if(x1 != b.x1) return x1 < b.x1;
    if(y1 != b.y1) return y1 < b.y1;
    if(wid != b.wid) return wid < b.wid;
    return hei != b.hei;
}

bool DifferingSection::operator== (const DifferingSection& b) const
{
    return x1==b.x1 && y1==b.y1 && wid==b.wid && hei==b.hei;
}

bool DifferingSection::Overlaps(const DifferingSection& b) const
{
    // Exclusive lower-right coordinates
    const unsigned x2  =   x1  +  wid, y2  =   y1+  hei;
    const unsigned bx2 = b.x1  +b.wid, by2 = b.y1+b.hei;
    return   x1 < bx2 &&   y1 < by2
        && b.x1 <  x2 && b.y1 <  y2;
}

namespace
{
    class Partitioner
    {
        typedef std::pair<unsigned, unsigned> work;
        std::list<work, FSBAllocator<work> > works;
    public:
        Partitioner(unsigned begin, unsigned count)
            : works(1, work(begin,count))
        {
        }

        int Partition()
        {
            if(works.empty()) return -1;

            work p = works.front(); works.pop_front();
            unsigned begin  = p.first, count = p.second;
            unsigned middle = count/2;

            if(!count) return -1;

            if(middle > 0) works.push_back( work(begin, middle) );
            unsigned remain = count-(middle+1);
            if(remain > 0) works.push_back( work(begin+middle+1, remain) );

            return begin+middle;
        }
    };

    typedef std::vector<bool> DiffVector;

    void FindDifferencesRecursive(
        DifferencesOnFrame& result,
        const DiffVector& pixeldiff,
        unsigned width, unsigned height,
        unsigned x1,unsigned y1, unsigned sect_width,unsigned sect_height)
    {
        #define Check_Line(x,y, span,incr, whatif) \
        do { bool empty = true; \
             unsigned p1=(y)*width+(x), p2=p1+span; \
             for(unsigned p=p1; p<p2; p+=incr) \
                 if(pixeldiff[p]) \
                     { empty = false; break; } \
             if(empty) { whatif; } \
           } while(0)
        #define Check_Vertical(x, whatif) Check_Line(x,y1, sect_height*width,sect_width, whatif)
        #define Check_Horizontal(y, whatif) Check_Line(x1,y, sect_width,1, whatif)

        /* Reduce the edges as much as possible */
        for(;;)
        {
            if(sect_height > sect_width)
            {
                if(sect_height > 0)
                {
                  Check_Horizontal(y1, sect_height-=1; y1+=1; goto Recheck;);
                  if(sect_height > 1)
                    Check_Horizontal(y1+sect_height-1, sect_height-=1; goto Recheck;);
                }
                if(sect_width > 0)
                {
                  Check_Vertical(x1, sect_width-=1; x1+=1; goto Recheck;);
                  if(sect_width > 1)
                    Check_Vertical(x1+sect_width-1, sect_width-=1; goto Recheck;);
                }
            }
            else
            {
                if(sect_width > 0)
                {
                  Check_Vertical(x1, sect_width-=1; x1+=1; goto Recheck;);
                  if(sect_width > 1)
                    Check_Vertical(x1+sect_width-1, sect_width-=1; goto Recheck;);
                }
                if(sect_height > 0)
                {
                  Check_Horizontal(y1, sect_height-=1; y1+=1; goto Recheck;);
                  if(sect_height > 1)
                    Check_Horizontal(y1+sect_height-1, sect_height-=1; goto Recheck;);
                }
            }
            break; // edges were not reducible
        Recheck:;
        }

        /* Find out whether there is a line we can use to
         * divide the field in two either horizontally
         * or vertically. If there is none, then we have
         * found the sprite.
         * Use a binary partitioning scheme to find the line.
         * i.e. instead of
         *  x|xxxxx  xx|xxxx  xxx|xxx  xxxx|xx  xxxxx|x
         *  x|xxxxx  xx|xxxx  xxx|xxx  xxxx|xx  xxxxx|x
         *
         * Try
         *  xxx|xxx  xx|xxxx  xxxx|xx  x|xxxxx  xxxxx|x
         *  xxx|xxx  xx|xxxx  xxxx|xx  x|xxxxx  xxxxx|x
         *
         * This gets largest partitions first.
         */
        #define CheckAllVertical() \
        do { Partitioner worker(x1+Minimum_Sprite_Width, sect_width-Minimum_Sprite_Width); \
            for(;;) \
            { \
                part_x = worker.Partition(); if(part_x < 0) break; \
                Check_Vertical(part_x, goto DoHorizPartition;); \
            } } while(0)
        #define CheckAllHorizontal() \
        do { Partitioner worker(y1+Minimum_Sprite_Height, sect_height-Minimum_Sprite_Height); \
            for(;;) \
            { \
                part_y = worker.Partition(); if(part_y < 0) break; \
                Check_Horizontal(part_y, goto DoVertPartition;); \
            } } while(0)

        int part_x=0, part_y=0;
        if(sect_width > sect_height)
        {
            if(sect_width >= Minimum_Sprite_Width)   CheckAllVertical();
            if(sect_height >= Minimum_Sprite_Height) CheckAllHorizontal();
        }
        else
        {
            if(sect_height >= Minimum_Sprite_Height) CheckAllHorizontal();
            if(sect_width >= Minimum_Sprite_Width)   CheckAllVertical();
        }
        goto WasUndivisible;
    DoHorizPartition:
      {
        // Create pointers for the references, because OpenMP firstprivate
        // directive (which copies the local var to thread's local context)
        // cannot copy references; it can only copy pointers.
        DifferencesOnFrame* r = &result;
        const DiffVector* p = &pixeldiff;
      #pragma omp task
        FindDifferencesRecursive(*r,*p,width,height,
            x1,y1, (part_x-x1), sect_height);
      #pragma omp task
        FindDifferencesRecursive(*r,*p,width,height,
            part_x+1,y1, (sect_width-(part_x+1)), sect_height);
        return;
      }
    DoVertPartition:
      {
        DifferencesOnFrame* r = &result;
        const DiffVector* p = &pixeldiff;
      #pragma omp task
        FindDifferencesRecursive(*r,*p,width,height,
            x1,y1, sect_width, (part_y-y1));
      #pragma omp task
        FindDifferencesRecursive(*r,*p,width,height,
            x1,part_y+1, sect_width, (sect_height-(part_y+1)));
        return;
      }
        #undef CheckAllHorizontal
        #undef CheckAllVertical
        #undef Check_Horizontal
        #undef Check_Vertical
        #undef Check_Line
    WasUndivisible:
        // It was undivisible.
        if(sect_width > 0 && sect_height > 0)
        {
            DifferingSection section = { x1,y1, sect_width,sect_height };
        #ifdef _OPENMP
            static MutexType partition_lock;
            ScopedLock lck(partition_lock);
        #endif
            result.push_back(section);
        }
    }
}

DifferencesOnFrame FindDifferences
    (const VecType<uint32>& background,
     const VecType<uint32>& screen,
     unsigned width)
{
    const unsigned npixels = background.size();
    const unsigned height = npixels / width;
    DiffVector pixeldiff(false, npixels);

    #pragma omp parallel for schedule(static,width)
    for(unsigned a=0; a<npixels; ++a)
        pixeldiff[a] = background[a] != screen[a];

    DifferencesOnFrame result;

    #pragma omp parallel
    {
    #pragma omp single
    {
    FindDifferencesRecursive(result, pixeldiff, width,height,
            0,0,
            width,height);
    }}
    return result;
}

/* In any average animation, sometimes actors collide.
 * They shoot projectiles, in which case one actor splits into two.
 * They pass each others, in which case two actors become one, or
 * one becomes two, or both.
 * The job of this function is to find out which DifferingSections
 * refer to the same actor, and to ignore those DifferingSections
 * that are ambiguous.
 * Note: Sometimes actors move on the screen at such intervals
 * that the bounding boxes in adjacent frames are disconnected.
 *
 * TODO: Later phase:
 * There is also the possibility that an actor is partially visible
 * (emerging from screen edges or from behind an object); one should
 * reconstruct the actor in those cases.
 *
 * TODO: Later phase:
 * An actor may also sometimes become partially transparent. This
 * may happen if it shares background colors. One should attempt
 * to merge the individual frames by checking if one pose
 * can be overlayed over another pose without conflicts.
 */
void SpriteLore::FindDistinctActors()
{
}
