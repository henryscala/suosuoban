#include "geom.h"
#include <cmath>
#include <algorithm>
#include <cassert>
using namespace std;




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

QRectF calcEncloseRect(QPointF point, qreal radius)
{
    QPointF topleft(point.x() - radius, point.y()-radius);
    QPointF bottomRight(point.x()+radius,point.y()+radius);
    return QRectF(topleft,bottomRight);
}

QRectF calcEncloseRect(const QList<QPointF> &points)
{
    qreal minx=INFINITY;
    qreal miny=INFINITY;
    qreal maxx=-INFINITY;
    qreal maxy=-INFINITY;

    for (int i=0;i<points.size();i++){
        if (points[i].x()<minx){
            minx = points[i].x();
        }
        if (points[i].x()>maxx){
            maxx = points[i].x();
        }
        if (points[i].y()<miny){
            miny = points[i].y();
        }
        if (points[i].y()>maxy){
            maxy = points[i].y();
        }
    }

    return QRectF(QPointF(minx,miny),QPointF(maxx,maxy));
}

void fillCircle(QPointF point, qreal radius, Grid<unsigned char>& gridBoard){
    QRectF rect=calcEncloseRect(point,radius);
    int x,y;
    for (x=round(rect.left());x<rect.right();x++){
        for (y=round(rect.top());y<rect.bottom();y++){
            QPointF p(x,y);
            if (abs(p-point)<=radius){
                unsigned char curValue = gridBoard.getValue(x,y);
                if (curValue<=0) {
                    curValue = 1;
                } else if (curValue >= 255){
                    curValue = 255;
                } else {
                    curValue ++;
                }
                gridBoard.setValue(x,y,curValue);
            }
        }
    }
}

qreal abs(QPointF p){
    return sqrt(p.x() * p.x() + p.y() * p.y());
}

qreal dist(QPointF p1, QPointF p2){
    QPointF p = p1 - p2;
    return abs(p);
}

QPointF normalize(QPointF p){

    qreal a = abs(p);
    if(a<=EPSILON){
        return p;
    }
    return QPointF(p.x()/a,p.y()/a);
}


bool floatEqual(qreal r1, qreal r2)
{
    return abs(r1-r2) < EPSILON;
}

QPointF complexMultiply(QPointF p1, QPointF p2)
{
    QPointF res;
    //real part
    res.setX(p1.x()*p2.x()-p1.y()*p2.y());
    //image part
    res.setY(p1.x()*p2.y()+p2.x()*p1.y());
    return res;
}

qreal positiveAngle(QPointF v) {
    v = normalize(v);
    qreal cosV = v.x();
    qreal sinV = v.y();
    if (cosV > 0 && sinV > 0) {//first quadrant
        return acos(cosV);
    }
    if (cosV < 0 && sinV > 0) {//second quadrant
        return acos(cosV);
    }
    if (cosV < 0 && sinV < 0) {//third quadrant
        return PI + acos(-cosV);
    }
    if (cosV > 0 && sinV < 0) {//fourth quadrant
        return PI + PI - acos(cosV);
    }
    if (cosV == 0 && sinV != 0) {
        if (sinV > 0) {
            return PI / 2;
        } else {
            return PI * 3 / 2;
        }
    }
    if (cosV != 0 && sinV == 0) {
        if (cosV > 0) {
            return 0;
        } else {
            return PI;
        }
    }

    return 0; //cosV && sinV both == 0
}

void toPolygon(const QRectF &r, /*out*/ PolyLine& polygon)
{
    polygon << r.topLeft()
            << QPointF(r.bottomRight().x(),r.topLeft().y())
            << r.bottomRight()
            << QPointF(r.topLeft().x(),r.bottomRight().y()) ;
}

QPointF calcTopRightPoint(const QList<QPointF> &points, /*out*/ int& index){
    assert(points.size() > 0);
    QList<QPointF> minYPoints;
    QList<int> indices;
    int i;
    for (i=0;i<points.size();i++){
        if (minYPoints.size()<=0){
            minYPoints.append( points[i] );
            indices.append(i);
        } else {
            QPointF firstPoint = minYPoints[0];
            if (floatEqual(firstPoint.y(), points[i].y())){
                minYPoints.append(points[i]);
                indices.append(i);
            } else if (firstPoint.y()>points[i].y()){
                minYPoints.clear();
                minYPoints.append(points[i]);
                indices.clear();
                indices.append(i);
            }//else do nothing
        }

    }

    QPointF maxXPoint=minYPoints[0];
    int maxPointIndex=indices[0];
    for (i=1;i<minYPoints.size();i++){
        if (minYPoints[i].x() > maxXPoint.x()){
            maxXPoint = minYPoints[i];
            maxPointIndex=indices[i];
        }

    }
    index = maxPointIndex;
    return maxXPoint;
}

void convexHull(const QList<QPointF> &points, /*out*/ QList<QPointF> &hullPoints) {
    if (points.size()<=0){
        return;
    }
    int pivotIndex;
    QPointF pivot = calcTopRightPoint(points,pivotIndex);
    QList<QPointF> pointsCopy = points;
    pointsCopy.removeAt(pivotIndex);

    if (pointsCopy.size() <= 0) {
        return ;
    }

    PointAngleCompare angleLessThan(pivot);


    std::sort(pointsCopy.begin(),pointsCopy.end(),angleLessThan);

    hullPoints << pivot << pointsCopy[0];

    for (int i = 1; i < pointsCopy.size(); ) {
        if (hullPoints.size() < 2) {
            hullPoints << pointsCopy[i];
            i++;
            continue;
        }

        QPointF top = hullPoints.last();
        QPointF prevTop = hullPoints[hullPoints.size()-1-1];
        qreal angle = positiveAngle(pointsCopy[i] - top);
        qreal anglePrev = positiveAngle(top - prevTop);

        if (angle > anglePrev) {//right turn in html5 canvas coordinates
            hullPoints << pointsCopy[i];
            i++;
        } else if (angle < anglePrev) {
            hullPoints.removeLast();
            //next turn without increasing i;
        } else {//angle==anglePrev

            qreal distanceCurr = dist(pointsCopy[i],pivot);
            qreal distancePrev = dist(top,pivot);

            if (distanceCurr > distancePrev) {
                hullPoints.removeLast();
                hullPoints << pointsCopy[i];
            }//else discard the current point
            i++;

        }

    }

    return;
}


void enlargePolygon(const PolyLine &polyLine, /* out */ PolyLine &largePolyLine ) {
    if (polyLine.size() <= 0){
        return;
    }

    if (polyLine.size() == 1) {
        largePolyLine << polyLine[0];
        return;
    }

    if (polyLine.size() == 2) {
        largePolyLine << polyLine[0] << polyLine[1];
        return;
    }

    for (int i=0; i<polyLine.size(); i++){
        QPointF currPoint = polyLine[i];
        QPointF prevPoint = polyLine[prevIndex(polyLine,i)];
        QPointF nextPoint = polyLine[nextIndex(polyLine,i)];
        //rotate 90 degree
        QPointF rotateVector(0,-1);
        qreal padding = Config::instance()->contourPadding();
        QPointF perpendicularNext = currPoint + normalize( complexMultiply(nextPoint - currPoint,rotateVector) ) * padding;
        QPointF perpendicularPrev = currPoint + normalize( complexMultiply(currPoint - prevPoint,rotateVector) ) * padding;
        QPointF midPoint = (perpendicularNext + perpendicularPrev) * 0.5;
        largePolyLine << midPoint;
    }

}

QRectF enlargeRect(QRectF r)
{
    qreal padding = Config::instance()->contourPadding();
    QPointF unit(padding,padding);
    QPointF topLeft = r.topLeft() - unit;
    QPointF bottomRight = r.bottomRight() + unit;
    return QRectF(topLeft, bottomRight);
}

void calcContourPolygon(const QList<QPointF> &points, PolyLine &polyLine)
{
    assert(points.size()>0);

    //special handling for too less points, so that it can also get a meaningful contour
    if (points.size()<=2) {
        QRectF enclosedRect = calcEncloseRect(points);
        QRectF largeRect = enlargeRect(enclosedRect);
        toPolygon(largeRect, polyLine);
        return;
    }

    PolyLine hull;
    convexHull(points, hull);

    enlargePolygon(hull,polyLine);
}

void getAllPoints(const PolyLineCluster &cluster, QList<QPointF> &points)
{
    for (int o=0;o<cluster.size();o++){
        PolyLine* line = cluster[o];
        for (int i=0;i<line->size();i++){
            points << line[i];
        }
    }
}


