#ifndef GEOM_H
#define GEOM_H

#include "vec2.h"
#include "cyclelist.h"

typedef Vec2<double> VecDouble;
typedef Vec2<int> VecInt;
typedef CycleList< Vec2<double> > PolyLine;
typedef CycleList<PolyLine*> PolyLineCluster;


VecInt vecDouble2vecInt(VecDouble);
VecDouble vecInt2vecDouble(VecInt);

#endif // GEOM_H
