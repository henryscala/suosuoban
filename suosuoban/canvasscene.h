#ifndef CANVASSCENE_H
#define CANVASSCENE_H

#include <QtWidgets>
#include "qmypathitem.h"

#include "geom.h"

typedef QList<QMyPathItem*> PathCluster;
typedef QList<PathCluster*> PathClusters;

enum CanvasMode{
    MODE_DRAW,
    MODE_CLUSTER,
    MODE_ERASE,
    MODE_MAX
};

#define CLUSTER_IDX 0

class CanvasScene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CanvasScene(QObject *parent);
    ~CanvasScene();
public slots:
    void canvasModeChange();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

    void calcContour();
    void addPathItem(QMyPathItem* pathItem);
    void getAllPoints(PathCluster &cluster, /* out */ QList<QPointF>& points);
    qreal dist(const PolyLine& polyLine, const PathCluster& cluster);
private:
    QMyPathItem* currPathItem;
    PathClusters pathClusters;
    QList<QMyPathItem*> contourItems;
    QList<int> selectedClusterIndices;

    bool isMouseDown;
    CanvasMode canvasMode;

};



#endif // CANVASSCENE_H
