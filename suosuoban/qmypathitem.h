#ifndef QMYPATHITEM_H
#define QMYPATHITEM_H
#include <QGraphicsPathItem>
#include <QList>

class QMyPathItem:public QGraphicsPathItem
{
public:
    QMyPathItem();
    ~QMyPathItem();
    void addPoint(QPointF);
    void setSelfPath(bool close);
    QList<QPointF> points;
};

#endif // QMYPATHITEM_H
