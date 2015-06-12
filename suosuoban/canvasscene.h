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

    void canvasModeChange(int mode);
public slots:

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void keyPressEvent ( QKeyEvent * event ) Q_DECL_OVERRIDE;
    void keyReleaseEvent ( QKeyEvent * event ) Q_DECL_OVERRIDE;

    PathCluster* clonePathCluster(PathCluster *cluster);
    QMyPathItem* clonePathItem(QMyPathItem* item);
    void clearPathClusters(PathClusters& clusters);
    void calcContour();
    void addPathItem(PathClusters& clusters,QMyPathItem* pathItem);
    void removePathItem(PathClusters& clusters,QMyPathItem* pathItem);
    void getAllPathItems(QList< QMyPathItem* > &items);
    void rebuildPathClusters();
    void getAllPoints(PathCluster &cluster, /* out */ QList<QPointF>& points);
    qreal dist(const PolyLine& polyLine, const PathCluster& cluster);
    void excludeClustersFarAway(const PathClusters& clusters,const QMyPathItem &pathItem,/*out*/ PathClusters& subset);
private:
    QMyPathItem* currPathItem;
    PathClusters pathClusters;
    QList<QMyPathItem*> contourItems;
    QList<int> selectedClusterIndices;
    PathClusters movingClusters;

    bool isMouseDown;
    bool isModKeyDown;
    CanvasMode canvasMode;

};



#endif // CANVASSCENE_H
