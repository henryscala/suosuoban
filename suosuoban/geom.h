#ifndef GEOM_H
#define GEOM_H

#include "vec2.h"
#include "cyclelist.h"
#include <QLine>
#include <QLineF>
#include <QPoint>
#include <QPointF>

typedef Vec2<double> VecDouble;
typedef Vec2<int> VecInt;

typedef CycleList<QPointF> QMyPolyLine;
typedef CycleList<QMyPolyLine*> QMyPolyLineCluster;


typedef CycleList< Vec2<double> > PolyLine;
typedef CycleList<PolyLine*> PolyLineCluster;

QPointF vecDouble2qpointf(VecDouble);
VecDouble qpointf2vecDouble(QPointF);
VecInt vecDouble2vecInt(VecDouble);
VecDouble vecInt2vecDouble(VecInt);



#endif // GEOM_H
