#ifndef GEOM_H
#define GEOM_H





#include <QtWidgets>
#include "vec2.h"
#include "grid.h"

#define INVINITY 0x7FFFFFFF
#define EPSILON 0.000001
#define PI 3.14159265

typedef Vec2<double> VecDouble;
typedef Vec2<int> VecInt;

typedef QList<QPointF> PolyLine;
typedef QList<PolyLine*> PolyLineCluster;



/*return equivalent positive angle
* acos return values [0,pi], while asin return values [-PI/2,PI/2], but I want [0,2PI]
*/
qreal positiveAngle(QPointF v);

class PointAngleCompare {
private:
    QPointF point;
public:

    PointAngleCompare(const QPointF &point){
        this->point = point;

    }

    bool operator() (const QPointF &p1, const QPointF &p2){


        qreal angle1 = positiveAngle(p1 - point);
        qreal angle2 = positiveAngle(p2 - point);

        if (angle1 >= angle2) {
            return false;
        }
        return true;
    }
} ;







QPointF vecDouble2qpointf(VecDouble);
VecDouble qpointf2vecDouble(QPointF);
VecInt vecDouble2vecInt(VecDouble);
VecDouble vecInt2vecDouble(VecInt);


void fillCircle(QPointF point, qreal radius, Grid<unsigned char>& gridBoard);
QRectF calcEncloseRect(QPointF point, qreal radius);
QRectF calcEncloseRect(const QList<QPointF> &points);

qreal abs(QPointF p);
qreal dist(QPointF p1, QPointF p2);
QPointF normalize(QPointF p);

//whether two real numbers are equal
bool floatEqual(qreal r1, qreal r2);



QPointF calcTopRightPoint(const QList<QPointF> &points, /*out*/ int& index);
/*given a list of points, calculate the convex hull of the points*/
void convexHull(const QList<QPointF> &points, /*out*/ QList<QPointF> &hullPoints);

#endif // GEOM_H
