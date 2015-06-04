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
    cout << "mouse down" << endl;

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
        qreal gap = dist(currPosF,lastPosF);
        if (gap < Config::instance()->minGap() ){
            return;
        }

        //try to minimize number of points way 2(angle)
        if (currPathItem->points.size()>=2){
            QPointF prevLastPosF = currPathItem->points[currPathItem->points.size()-1-1];
            qreal anglePrev = positiveAngle(lastPosF-prevLastPosF);
            cout << "anglePrev" << anglePrev << endl;
            qreal angleNow =  positiveAngle(currPosF-lastPosF);
            cout << "angleNow" << angleNow << endl;
            if (abs(anglePrev-angleNow) <= Config::instance()->minAngle() * PI / 180 ){
                currPathItem->points.removeLast();
            }
        }
    }


    currPathItem->addPoint(currPosF);
    currPathItem->setSelfPath();


    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;



    if (currPathItem){
        if (currPathItem->points.size() >1){
            addPathItem(currPathItem);
        } else {
            delete currPathItem;
        }
    }
    currPathItem = NULL;
    isMouseDown = false;

    calcContour();//for testing

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CanvasScene::calcContour()
{
    if (pathClusters.size()<=0) return;

    PathCluster* cluster = pathClusters[0];
    QMyPathItem* pathItem;
    int i,k;
    QList<QPointF> allPoints;
    for (i=0;i<cluster->size();i++){
        pathItem = (*cluster)[i];
        for (k=0;k<pathItem->points.size();k++){
            allPoints << pathItem->points[k];
        }
    }

    QList<QPointF> hullPoints;
    QPolygonF polygon;
    convexHull(allPoints,hullPoints);
    for (i=0;i<hullPoints.size();i++){
        polygon.append(hullPoints[i]);
    }


    this->addPolygon(polygon);

}

void CanvasScene::addPathItem(QMyPathItem *pathItem)
{
    PathCluster* cluster;
    if (pathClusters.size()<=0){
        cluster = new PathCluster;
        pathClusters.append(cluster);
    } else {
        cluster = pathClusters[0];
    }


    cluster->append(currPathItem);
}
