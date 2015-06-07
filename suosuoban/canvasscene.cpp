#include "canvasscene.h"

#include <iostream>
#include "geom.h"
#include "config.h"
using namespace std;

CanvasScene::CanvasScene(QObject *parent)
        :QGraphicsScene(parent)
{
    currPathItem = NULL;
    isMouseDown = false;



}

CanvasScene::~CanvasScene()
{
    //
    delete currPathItem;

    //release resources in pathClusters
    PathCluster* cluster;
    QMyPathItem* pathItem;
    int i,k;
    for (i=0;i<pathClusters.size();i++){
        cluster = pathClusters[i];
        for (k=0;k<cluster->size();k++){
            pathItem = (*cluster)[k];
            delete pathItem;
        }
        delete cluster;
    }
    pathClusters.clear();

}



void CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton){
        return;
    }



    isMouseDown = true;


    if (!currPathItem){
        delete currPathItem;
    }
    currPathItem = new QMyPathItem();
    this->addItem(currPathItem);
    QPen defaultPen ;
    defaultPen.setWidth(Config::instance()->penWidth());
    currPathItem->setPen(defaultPen);


    currPathItem->addPoint(mouseEvent->scenePos());



    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CanvasScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!isMouseDown) {
        return;
    }

    QPointF currPosF = mouseEvent->scenePos();




    if (currPathItem->points.size() > 0) {
        QPointF lastPosF = currPathItem->points.last();

        //try to minimize number of points way 1(step)
        qreal gap = ::dist(currPosF,lastPosF);
        if (gap < Config::instance()->minGap() ){
            return;
        }

        //try to minimize number of points way 2(angle)
        if (currPathItem->points.size()>=2){
            QPointF prevLastPosF = currPathItem->points[currPathItem->points.size()-1-1];
            qreal anglePrev = positiveAngle(lastPosF-prevLastPosF);

            qreal angleNow =  positiveAngle(currPosF-lastPosF);

            if (abs(anglePrev-angleNow) <= Config::instance()->minAngle() * PI / 180 ){
                currPathItem->points.removeLast();
            }
        }
    }


    currPathItem->addPoint(currPosF);
    currPathItem->setSelfPath(false);


    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }



    if (currPathItem){
        if (currPathItem->points.size() > 1){
            addPathItem(currPathItem);
        } else {
            delete currPathItem;
        }
    }
    currPathItem = NULL;
    isMouseDown = false;

    calcContour();

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CanvasScene::calcContour()
{
    int i;
    QMyPathItem* contourItem;
    //remove old ones
    for (i=0;i<contourItems.size();i++){
        contourItem = contourItems[i];
        delete contourItem;
    }
    contourItems.clear();

    //generate new ones
    for (i=0; i < pathClusters.size(); i++){


        PathCluster* cluster = pathClusters[i];


        QList<QPointF> allPoints;
        getAllPoints(*cluster,allPoints);

        PolyLine hullPoints;
        calcContourPolygon(allPoints,hullPoints);

        contourItem = new QMyPathItem;

        for (int k=0;k<hullPoints.size();k++){
            contourItem->points.append(hullPoints[k]);
        }

        contourItems << contourItem;

        contourItem->setSelfPath(true);

        QPen pen = QPen(Qt::NoPen);
        QColor color=Config::instance()->clusterColor();
        QBrush brush(color);
        contourItem->setBrush(brush);
        contourItem->setPen(pen);

        this->addItem(contourItem);
    }

}

void CanvasScene::addPathItem(QMyPathItem *pathItem)
{
    int i;
    PathCluster* cluster;
    PathClusters pathClustersSubset;
    PathClusters closeClusters;

    /* excluding faraway clusters */
    for (i=0; i< pathClusters.size(); i++){
        cluster = pathClusters[i];
        QList<QPointF> pointsInCluster;
        getAllPoints(*cluster, pointsInCluster);
        PolyLine clusterContour;
        calcContourPolygon(pointsInCluster, clusterContour);
        QRectF clusterContourRect = calcEncloseRect(clusterContour);
        QRectF pathRect = calcEncloseRect(pathItem->points);
        if (clusterContourRect.intersects(pathRect)){
            pathClustersSubset << cluster;
        }
    }



    for (i=0;i<pathClustersSubset.size();i++){
        cluster = pathClustersSubset[i];
        qreal distance = dist(pathItem->points,*cluster);
        if (distance < Config::instance()->contourPadding()){
            closeClusters << cluster;
        }
    }

    PathCluster* newCluster = new PathCluster;
    pathClusters << newCluster;
    newCluster->append(pathItem);

    //merge close enough clusters to one
    for (i=0; i< closeClusters.size();i++){
        cluster = closeClusters[i];
        for (int k=0;k<cluster->size();k++){
            QMyPathItem *aPathItem = (*cluster)[k];
            newCluster->append(aPathItem);
        }
        pathClusters.removeOne(cluster);
    }

    cout << "total cluster number is " << pathClusters.size() << endl;
}

void CanvasScene::getAllPoints(PathCluster &cluster, QList<QPointF> &points)
{
    for (int i=0;i<cluster.size();i++){
        QMyPathItem* pathItem = cluster[i];
        for (int k=0;k<pathItem->points.size();k++){
            points << pathItem->points[k];
        }
    }
}

qreal CanvasScene::dist(const PolyLine &polyLine, const PathCluster &cluster)
{
    qreal mindist = INFINITY;

    for (int i=0; i<cluster.size(); i++){
        QMyPathItem* pathItem=cluster[i];
        qreal distance = ::dist(pathItem->points,polyLine);
        if (mindist>distance){
            mindist = distance;
        }
    }
    return mindist;
}
