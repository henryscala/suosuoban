#ifndef CANVASSCENE_H
#define CANVASSCENE_H

#include <QtWidgets>
#include "qmypathitem.h"

#include "geom.h"

typedef QList<QMyPathItem*> PathCluster;
typedef QList<PathCluster*> PathClusters;

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

    void calcContour();
    void addPathItem(QMyPathItem* pathItem);
private:
    QMyPathItem* currPathItem;
    PathClusters pathClusters;

    bool isMouseDown;

};



#endif // CANVASSCENE_H
