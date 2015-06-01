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

QPointF vecDouble2qpointf(VecDouble v){
    QPointF q;
    q.setX(v.x);
    q.setY(v.y);
    return q;
}

VecDouble qpointf2vecDouble(QPointF q){
    VecDouble v;
    v.x = q.x();
    v.y = q.y();
    return v;
}




