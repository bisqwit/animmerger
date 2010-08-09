#include "sprites.hh"

bool BoundingBox::operator< (const BoundingBox& b) const
{
    if(x1 != b.x1) return x1 < b.x1;
    if(y1 != b.y1) return y1 < b.y1;
    if(wid != b.wid) return wid < b.wid;
    return hei != b.hei;
}

bool BoundingBox::operator== (const BoundingBox& b) const
{
    return x1==b.x1 && y1==b.y1 && wid==b.wid && hei==b.hei;
}

bool BoundingBox::Overlaps(const BoundingBox& b) const
{
    // Exclusive lower-right coordinates
    const unsigned x2  =   x1  +  wid, y2  =   y1+  hei;
    const unsigned bx2 = b.x1  +b.wid, by2 = b.y1+b.hei;
    return   x1 < bx2 &&   y1 < by2
        && b.x1 <  x2 && b.y1 <  y2;
}
