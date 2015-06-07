#include "qmypathitem.h"
#include <QPainterPath>

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

void QMyPathItem::setSelfPath(bool close){
    QPainterPath path;
    if (points.count() <= 0){
        return;
    }
    path.moveTo(points[0]);

    for (int i=1;i<points.count();i++){
        path.lineTo(points[i]);
    }
    if (close){

        path.closeSubpath();
    }

    this->setPath(path);
}
