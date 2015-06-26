#ifndef CANVASSCENE_H
#define CANVASSCENE_H

#include <QtWidgets>
#include "qmypathitem.h"
#include "history.h"

#include "geom.h"

typedef QList<QMyPathItem*> PathCluster;
typedef QList<PathCluster*> PathClusters;



enum CanvasMode{
    MODE_DRAW,
    MODE_CLUSTER,
    MODE_ERASE,
    MODE_MAX
};

enum CanvasColorType{
    COLOR_TYPE_PEN,
    COLOR_TYPE_BACK,
    COLOR_TYPE_CLUSTER,
    COLOR_TYPE_MAX
};

void pathClusterToPolylineCluster(const PathCluster& pc,PolyLineCluster& plc);

class CanvasScene: public QGraphicsScene
{
    Q_OBJECT
public:

    explicit CanvasScene(QObject *parent);
    ~CanvasScene();

    bool saveFile(QString fileName);
    bool readFile(QString fileName);
    void canvasModeChange(CanvasMode mode);
    void canvasColorChange(CanvasColorType colorType, QColor color);
    void setShowCluster(bool show);
    void selectAll();
    void delCluster();
    void undoRedo(const history::PolyLineOp &op,bool isUndo);
    void configChange();

    QList<int> selectedClusterIndices;
    PathClusters pathClusters;
public slots:

signals:
    void sceneChanged();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    void keyPressEvent ( QKeyEvent * event ) Q_DECL_OVERRIDE;
    void keyReleaseEvent ( QKeyEvent * event ) Q_DECL_OVERRIDE;

    PathCluster* clonePathCluster(PathCluster *cluster);
    QMyPathItem* clonePathItem(QMyPathItem* item);
    void clearPathClusters(PathClusters& clusters);
    void clearPathCluster(PathCluster** cluster);
    void calcContour();
    void addPathItem(PathClusters& clusters,QMyPathItem* pathItem);
    void removePathItem(PathClusters& clusters,QMyPathItem* pathItem);
    void getAllPathItems(QList< QMyPathItem* > &items);
    void rebuildPathClusters();
    void getAllPoints(PathCluster &cluster, /* out */ QList<QPointF>& points);
    qreal dist(const PolyLine& polyLine, const PathCluster& cluster);
    void excludeClustersFarAway(const PathClusters& clusters,const QMyPathItem &pathItem,/*out*/ PathClusters& subset);
    void removeEmptyCluster();

    QMyPathItem* findPathItem(const PolyLine &pl);
    QMyPathItem* createPathItem();
private:
    QMyPathItem* currPathItem;

    QList<QMyPathItem*> contourItems;

    PathClusters movingClusters;

    bool isMouseDown;
    bool isModKeyDown;
    bool isShowCluster;
    CanvasMode canvasMode;

};



#endif // CANVASSCENE_H
