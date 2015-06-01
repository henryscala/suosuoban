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

void QMyPathItem::setSelfPath(){
    QPainterPath path;
    if (points.count() > 0){
        path.moveTo(points[0]);
    }
    for (int i=1;i<points.count();i++){
        path.lineTo(points[i]);
    }
    this->setPath(path);
}
