#ifndef CANVASSCENE_H
#define CANVASSCENE_H

#include <QtWidgets>
#include "qmypathitem.h"

#include "geom.h"

class CanvasScene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CanvasScene(QObject *parent);
    ~CanvasScene();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    QMyPathItem* currPathItem;
    QList<QMyPathItem*> allPathItems;
    bool isMouseDown;
};

#endif // CANVASSCENE_H
