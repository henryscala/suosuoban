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


bool closeToZero(qreal num){
    num=abs(num);
    if (num < EPSILON){
        return true;
    }
    return false;
}

qreal crossProduct(QPointF p1, QPointF p2){
    return p1.x()*p2.y() - p2.x()*p1.y();
}

int onTheRight(QPointF p1, QPointF p2, QPointF p3)
{
    QPointF v1 = p2 - p1;
    QPointF v2 = p3 - p1;
    //spedial handling if p1 and p2 nearly overlap or the same point
    if(closeToZero( abs(v1))){
        if(closeToZero(abs(v2))) {
            return COLINEAR_IN;
        } else {
            return RIGHT;
        }
    }
    qreal product = crossProduct(v1,v2);
    if (product > 0) {
        return RIGHT;
    }
    if (product < 0) {
        return LEFT;
    }

    //colinear handling
    qreal dist1 = abs(p3 - p1);
    qreal dist2 = abs(p3 - p2);
    qreal dist3 = abs(p1 - p2);
    if (closeToZero(dist1 + dist2 - dist3)) {
        return COLINEAR_IN;
    }
    return COLINEAR_OUT;
}

bool isLineSegmentIntersect(LineSegment ls1, LineSegment ls2) {
    int r2start = onTheRight(ls1.start, ls1.end, ls2.start);
    int r2end = onTheRight(ls1.start, ls1.end, ls2.end);
    int r1start = onTheRight(ls2.start, ls2.end, ls1.start);
    int r1end = onTheRight(ls2.start, ls2.end, ls1.end);
    if (r2start == COLINEAR_IN || r2end == COLINEAR_IN || r1start == COLINEAR_IN || r1end == COLINEAR_IN) {
        return true;
    }
    //this may have logic problems
    if (r2start == COLINEAR_OUT || r2end == COLINEAR_OUT || r1start == COLINEAR_OUT || r1end == COLINEAR_OUT) {
        return false;
    }
    if ((r2start * r2end < 0) && (r1start * r1end < 0)) {//on the opposite  side
        return true;
    }
    return false;
}



qreal dist(const LineSegment &ls1, const LineSegment &ls2)
{
    if (isLineSegmentIntersect(ls1,ls2)){
        return 0.0;
    }

    qreal ls1Len = abs(ls1.start - ls1.end);

    //area of triangle divide length of one side is height
    //height from ls2.start to ls1
    qreal height1 = abs( crossProduct(ls2.start - ls1.start, ls2.start - ls1.end) / ls1Len / 2 );
    //height from ls2.end to ls1
    qreal height2 = abs ( crossProduct(ls1.start - ls2.end, ls1.end - ls2.end) / ls1Len / 2 );

    qreal height3 = abs (ls1.start-ls2.start);

    qreal height4 = abs (ls1.start-ls2.end);

    qreal height5 = abs (ls1.end-ls2.start);

    qreal height6 = abs (ls1.end-ls2.end);

    QList<qreal> heights;
    heights << height1<< height2<<height3 << height4<< height5<<height6;
    return minElem(heights);
}

qreal minElem(const QList<qreal> &list){
    qreal theMin = INFINITY;
    for(int i=0; i<list.size(); i++){
        if (list[i]<theMin){
            theMin = list[i];
        }
    }
    return theMin;
}


bool isPathIntersect(const PolyLine &path1, const PolyLine &path2)
{
    if (path1.size() <=1 || path2.size() <=1 ){
        return false;
    }
    for (int i=1;i<path1.size();i++){
        LineSegment ls1(path1[prevIndex(path1,i)], path1[i]);
        for (int k=1; k<path2.size(); k++){
            LineSegment ls2(path2[prevIndex(path2,k)], path2[k]);
            if (isLineSegmentIntersect(ls1,ls2)){
                return true;
            }
        }
    }
    return false;

}


qreal dist(const PolyLine &path1, const PolyLine &path2)
{
    qreal mindist = INFINITY;
    if (isPathIntersect(path1, path2)) {
        return 0.0;
    }

    for (int i=1;i<path1.size();i++){
        LineSegment ls1(path1[prevIndex(path1,i)], path1[i]);
        for (int k=1;k<path2.size();k++){
            LineSegment ls2(path2[prevIndex(path2,k)], path2[k]);
            qreal distance = dist(ls1,ls2);
            if (distance<mindist){
                mindist = distance;
            }
        }
    }

    return mindist;
}


void calcControlPointsOfPolygon(const PolyLine &polygon, QList<QPair<QPointF, QPointF> > &pointPairs)
{
    for (int i=0; i<polygon.size(); i++){
        QPointF curr = polygon[i];
        QPointF prev = polygon[prevIndex(polygon,i)];
        QPointF next = polygon[nextIndex(polygon,i)];
        QPointF midPointNext = (curr + next) * 0.5;
        QPointF midPointPrev = (prev + curr) * 0.5;

        qreal nextSegmentLen = abs(next - curr);
        qreal prevSegmentLen = abs(curr - prev);
        qreal totalLen = nextSegmentLen + prevSegmentLen;

        QPointF pivotPointBetweenMidPoint = midPointPrev + (midPointNext - midPointPrev) * (prevSegmentLen / totalLen);
        QPointF movingVector = curr - pivotPointBetweenMidPoint;
        QPointF controlPointPrev = midPointPrev + movingVector;
        QPointF controlPointNext = midPointNext + movingVector;
        pointPairs << QPair<QPointF,QPointF>(controlPointPrev,controlPointNext);
    }
}


qreal polygonArea(const PolyLine &polygon) {
    qreal area = 0.0;
    for (int i=0; i<polygon.size(); i++){
        QPointF curr = polygon[i];
        QPointF next = polygon[ nextIndex(polygon,i)];
        area += crossProduct(curr,next);
    }

    return area / 2;
}


bool isPolygonClockwise(const PolyLine& polygon) {
    if (polygonArea(polygon) >= 0) {
        return true;
    }
    return false;
}

bool pointInPolygon(QPointF point, const PolyLine &polygon)
{
    bool isClockwise = isPolygonClockwise(polygon);
    PolyLine newPolygon;
    if (isClockwise){
        newPolygon = polygon;
    } else {
        //reverse
        for (int k=0;k<polygon.size();k++){
            newPolygon.push_front(polygon[k]);
        }
    }
    for (int i=0; i<newPolygon.size(); i++){
        QPointF curr = newPolygon[i];
        QPointF next = newPolygon[ nextIndex(newPolygon,i)];
        int result = onTheRight(curr,next,point);
        if (result == COLINEAR_OUT){
            return false;
        }

        if (result == LEFT){
            return false;
        }
    }
    return true;
}
