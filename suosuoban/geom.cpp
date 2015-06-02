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

qreal distance(QPointF p1, QPointF p2){
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
