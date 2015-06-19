#ifndef QMYPATHITEM_H
#define QMYPATHITEM_H
#include <QGraphicsPathItem>
#include <QList>
#include <QColor>
#include <QGraphicsScene>

class QMyPathItem:public QGraphicsPathItem
{
public:

    QMyPathItem(int penWidth,QColor penColor,QGraphicsScene* parent);
    QMyPathItem();
    ~QMyPathItem();
    void addPoint(QPointF);
    void setSelfPath(bool close,bool smooth);
    QList<QPointF> points;
};

#endif // QMYPATHITEM_H
