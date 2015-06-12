#ifndef GEOM_H
#define GEOM_H

#include <QtWidgets>
#include "vec2.h"
#include "grid.h"
#include "config.h"

#define MYINFINITY 0x7FFFFFFF
#define EPSILON 0.000001
#define PI 3.14159265

/*the four enums are used to specify relations of points and lines */
#define COLINEAR_IN  0 //colinear and on the line segment
#define COLINEAR_OUT  2  //colinear and not on the line segment
#define RIGHT  1
#define LEFT  -1

typedef Vec2<double> VecDouble;
typedef Vec2<int> VecInt;

typedef QList<QPointF> PolyLine;
typedef QList<PolyLine*> PolyLineCluster;


struct LineSegment {
    LineSegment(){}
    LineSegment(QPointF s, QPointF e):start(s),end(e){

    }

    QPointF start;
    QPointF end;
};

/* return equivalent positive angle
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


/* look the list as a cycle list, get the previous index*/
template <typename T>
int prevIndex(const QList<T>& list, int currIndex){
    currIndex --;
    if (currIndex<0){
        currIndex = list.size() - 1;
    }
    return currIndex;
}

/* look the list as a cycle list, get the next index*/
template <typename T>
int nextIndex(const QList<T>& list, int currIndex){
    currIndex ++;
    if (currIndex>=list.size()){
        currIndex = 0;
    }
    return currIndex;
}

qreal minElem(const QList<qreal> &list);

QPointF vecDouble2qpointf(VecDouble);
VecDouble qpointf2vecDouble(QPointF);
VecInt vecDouble2vecInt(VecDouble);
VecDouble vecInt2vecDouble(VecInt);


void fillCircle(QPointF point, qreal radius, Grid<unsigned char>& gridBoard);
QRectF calcEncloseRect(QPointF point, qreal radius);
QRectF calcEncloseRect(const QList<QPointF> &points);



qreal abs(QPointF p);
QPointF normalize(QPointF p);
qreal dist(QPointF p1, QPointF p2);
qreal dist(const LineSegment& ls1, const LineSegment& ls2);
qreal dist(const PolyLine &path1, const PolyLine &path2);

/* simulate the multiplication of two complex numbers */
QPointF complexMultiply(QPointF p1, QPointF p2);

/* calculate the cross product of two vectors, it is related to sin(theta) between the two vectors */
qreal crossProduct(QPointF p1, QPointF p2);

//whether two real numbers are equal
bool floatEqual(qreal r1, qreal r2);


QPointF calcTopRightPoint(const QList<QPointF> &points, /*out*/ int& index);

/* given a list of points, calculate the convex hull of the points */
void convexHull(const QList<QPointF> &points, /*out*/ QList<QPointF> &hullPoints);

/* enlarge polygon, so that the new polygon contains the old polygon.
 * for every vertex, get two vectors(passing the vertex) perpendicular to the neighbour lines,
 * get the middle point of the two vectors, regard it as the new vertex of the new polygon.
 */
void enlargePolygon(const PolyLine &polyLine, /* out */ PolyLine &largePolyLine ) ;

/* make the rect largger */
QRectF enlargeRect(QRectF r);

/* get all points in the cluster */
void getAllPoints(const PolyLineCluster& cluster,/*out*/ QList<QPointF> &points);

/* rect to polygon */
void toPolygon(const QRectF &r, /*out*/ PolyLine& polygon);


void calcContourPolygon(const QList<QPointF> &points, /*out*/ PolyLine& polyLine);

void calcControlPointsOfPolygon(const PolyLine& polygon,/*out*/ QList<QPair<QPointF,QPointF> > &pointPairs);

/* determine whether p3 is on the right of the line constructed by p1 and p2 */
int onTheRight(QPointF p1, QPointF p2, QPointF p3);

bool closeToZero(qreal num);

bool isLineSegmentIntersect(LineSegment ls1, LineSegment ls2);
/*return true's precondition is that path should have at least two points
 *
 */
bool isPathIntersect(const PolyLine& path1, const PolyLine& path2);

/*polygon area with sign*/
qreal polygonArea(const PolyLine &polygon);

/* in  canvas coordinate*/
bool isPolygonClockwise(const PolyLine &polygon);

bool pointInPolygon(QPointF point, const PolyLine& polygon);

/* move points according to point */
void move(QList<QPointF> &points, QPointF point);

#endif // GEOM_H
