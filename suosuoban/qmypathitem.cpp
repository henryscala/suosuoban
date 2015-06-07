#include "qmypathitem.h"
#include <QPainterPath>
#include "geom.h"

QMyPathItem::QMyPathItem()
{

}

QMyPathItem::~QMyPathItem()
{
    points.clear();
}

void QMyPathItem::addPoint(QPointF p){
    points.append(p);
}

void QMyPathItem::setSelfPath(bool close, bool smooth){
    QPainterPath path;
    int i;

    if (points.size() <= 0){
        return;
    }
    path.moveTo(points[0]);

    if (!smooth ){
        for (i=1;i<points.size();i++){
            path.lineTo(points[i]);
        }
    } else {
        QList<QPair<QPointF,QPointF> > listOfPair;
        calcControlPointsOfPolygon(points, listOfPair);
        for (i=1;i<points.size();i++){
            path.cubicTo(listOfPair[i].first,listOfPair[i].second, points[i]);
        }
    }


    if (close){

        path.closeSubpath();
    }

    this->setPath(path);
}
