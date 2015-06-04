#ifndef GEOM_H
#define GEOM_H

#include "vec2.h"
#include "cyclelist.h"
#include <QLine>
#include <QLineF>
#include <QPoint>
#include <QPointF>
#include "grid.h"
#include <QtWidgets>

#define INVINITY 0x7FFFFFFF
#define EPSILON 0.000001
#define PI 3.14159265

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


void fillCircle(QPointF point, qreal radius, Grid<unsigned char>& gridBoard);
QRectF calcEncloseRect(QPointF point, qreal radius);
QRectF calcEncloseRect(const QList<QPointF> &points);

qreal abs(QPointF p);
qreal distance(QPointF p1, QPointF p2);
QPointF normalize(QPointF p);

/*return equivalent positive angle
* acos return values [0,pi], while asin return values [-PI/2,PI/2], but I want [0,2PI]
*/
qreal positiveAngle(QPointF v);

#endif // GEOM_H
