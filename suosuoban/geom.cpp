#include "geom.h"

VecInt vecDouble2vecInt(VecDouble vd){
    VecInt res;
    res.x = (int)vd.x;
    res.y = (int)vd.y;
    return res;
}

VecDouble vecInt2vecDouble(VecInt vi){
    VecDouble res;
    res.x = (double)vi.x;
    res.y = (double)vi.y;
    return res;
}
