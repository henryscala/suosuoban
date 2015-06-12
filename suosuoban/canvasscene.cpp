#include "canvasscene.h"

#include <cassert>
#include <iostream>
#include "geom.h"
#include "config.h"

using namespace std;

CanvasScene::CanvasScene(QObject *parent)
        :QGraphicsScene(parent)
{
    currPathItem = NULL;
    isMouseDown = false;
    isModKeyDown = false;
    canvasMode = MODE_DRAW;


}

CanvasScene::~CanvasScene()
{
    //
    delete currPathItem;

    //release resources in pathClusters

    clearPathClusters(pathClusters);
    clearPathClusters(movingClusters);
}

void CanvasScene::clearPathClusters(PathClusters& clusters){
    for (int i=0;i<clusters.size();i++){
        PathCluster* cluster = clusters[i];
        for (int k=0;k<cluster->size();k++){
            QMyPathItem* pathItem = (*cluster)[k];
            if(pathItem){
                delete pathItem;
            }
        }
        if (cluster){
            delete cluster;
        }
    }
    clusters.clear();
}

void CanvasScene::canvasModeChange(int mode)
{


    canvasMode=(CanvasMode)mode;
    cout << "canvasMode " << canvasMode << endl;


    selectedClusterIndices.clear();
    calcContour();
}



void CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton){
        return;
    }

    isMouseDown = true;
    QPointF point = mouseEvent->scenePos();


    if (!currPathItem){
        delete currPathItem;
    }
    currPathItem = new QMyPathItem();
    this->addItem(currPathItem);
    QPen defaultPen ;
    defaultPen.setWidth(Config::instance()->penWidth());
    currPathItem->setPen(defaultPen);
    currPathItem->addPoint(point);

    if (canvasMode == MODE_DRAW){

    } else if (canvasMode == MODE_ERASE) {

    } else if (canvasMode == MODE_CLUSTER){
        int seledtedIdx = -1;
        QMyPathItem* clusterItem = NULL;
        for (int i=0;i<contourItems.size();i++){
            clusterItem = contourItems[i];
            if (pointInPolygon(point,clusterItem->points)){
                seledtedIdx = i;
                cout << "selected idx " << seledtedIdx << endl;
                break;
            }
        }

        if (seledtedIdx >= 0){
            if (isModKeyDown){
                if (!selectedClusterIndices.contains(seledtedIdx)){
                    selectedClusterIndices << seledtedIdx;
                }
            } else if (selectedClusterIndices.contains(seledtedIdx)){
                //it is likely to be the start of moving cluster click, don't add/clear
            } else {
                selectedClusterIndices.clear();
                selectedClusterIndices << seledtedIdx;
            }
        } else {
            this->selectedClusterIndices.clear();
        }
    }



    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CanvasScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (!isMouseDown) {
        return;
    }

    QPointF currPosF = mouseEvent->scenePos();

    QPointF lastPosF = currPathItem->points.last();

    //try to minimize number of points way 1(step)
    qreal gap = ::dist(currPosF,lastPosF);
    if (gap < Config::instance()->minGap() ){
        return;
    }

    if (currPathItem->points.size() > 0) {
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
    currPathItem->setSelfPath(false,false);

    if (canvasMode == MODE_DRAW){       

    } else if (canvasMode == MODE_ERASE) {
        if (currPathItem && currPathItem->points.size() >=2){
            PathClusters closeClusters;
            excludeClustersFarAway(pathClusters, *currPathItem, closeClusters);
            for (int i=0; i<closeClusters.size();i++){
                PathCluster* cluster = closeClusters[i];
                int clusterSize = cluster->size();//copy it because cluster may be deleted in the loop
                for (int k=clusterSize-1; k>=0; k--){//changing it from 0 to size is likely to cause crash because cluster may be modified in the loop, check further infuture
                    QMyPathItem* item = (*cluster)[k];
                    if (isPathIntersect(item->points, currPathItem->points)){
                        removePathItem(pathClusters,item);
                    }
                }
            }
        }

    } else if (canvasMode == MODE_CLUSTER ) {
        if (selectedClusterIndices.size() && currPathItem && currPathItem->points.size()>=2){
            clearPathClusters(movingClusters);

            QPointF deviation = currPosF - currPathItem->points[0];
            for (int i=0;i<selectedClusterIndices.size();i++){
                PathCluster* cluster = pathClusters[selectedClusterIndices[i]];
                cluster = clonePathCluster(cluster);
                movingClusters << cluster;

                for (int k=0; k<cluster->size(); k++){
                    QMyPathItem* item = (*cluster)[k];
                    move(item->points, deviation);
                    item->setSelfPath(false, false);
                }
            }

        }

    }


    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }

    QPointF currPosF = mouseEvent->scenePos();


    if (canvasMode == MODE_DRAW){
        if (currPathItem){
            if (currPathItem->points.size() > 1){
                addPathItem(pathClusters,currPathItem);
                currPathItem = NULL;
            }
        }

    } else if (canvasMode == MODE_ERASE) {

    } else if (canvasMode == MODE_CLUSTER) {
        //at least two points
        if (selectedClusterIndices.size() && currPathItem && currPathItem->points.size() > 1){
            QPointF startPos = currPathItem->points.first();
            QPointF deviation = currPosF - startPos;
            for (int i=0; i<selectedClusterIndices.size(); i++){
                PathCluster* cluster = pathClusters[selectedClusterIndices[i]];


                //copy
                if (isModKeyDown){
                    PathCluster copyCluster = *cluster;//make a copy because cluster may change in the loop
                    for (int k=0;k<copyCluster.size();k++){
                        QMyPathItem* pathItem = copyCluster[k];
                        QMyPathItem* newPathItem = clonePathItem(pathItem);
                        move(newPathItem->points, deviation);
                        newPathItem->setSelfPath(false,false);
                        addPathItem(pathClusters,newPathItem);
                    }
                } else {//move

                    for (int k=0; k<cluster->size();k++){
                        QMyPathItem* pathItem = (*cluster)[k];
                        move(pathItem->points, deviation);
                        pathItem->setSelfPath(false,false);

                    }
                    rebuildPathClusters();
                }
            }
            selectedClusterIndices.clear();
        }

        clearPathClusters(movingClusters);

    }

    if (currPathItem){
        delete currPathItem;
        currPathItem = NULL;
    }

    isMouseDown = false;

    calcContour();

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CanvasScene::keyPressEvent ( QKeyEvent * event )
{

    if (event->key() == Qt::Key_Control
            || event->key() == Qt::Key_Alt
            || event->key() ==Qt::Key_Shift)
    {
        isModKeyDown = true;
        cout << "key press " << event->key() << endl;
    }
}

void CanvasScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control
            || event->key() == Qt::Key_Alt
            || event->key() ==Qt::Key_Shift)
    {
        isModKeyDown = false;
        cout << "key release " << event->key() << endl ;
    }

}

PathCluster *CanvasScene::clonePathCluster(PathCluster *cluster)
{
    PathCluster* newCluster = new PathCluster();
    for (int i=0;i<cluster->size();i++){
        (*newCluster) << clonePathItem((*cluster)[i]);
    }
    return newCluster;
}

QMyPathItem *CanvasScene::clonePathItem(QMyPathItem *item)
{
    QMyPathItem* newItem = new QMyPathItem();
    newItem->setPen(item->pen());
    newItem->setBrush(item->brush());
    newItem->points = item->points;//copy

    this->addItem(newItem);
    return newItem;
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
        contourItem->setData(CLUSTER_IDX,i);

        for (int k=0;k<hullPoints.size();k++){
            contourItem->points.append(hullPoints[k]);
        }

        contourItems << contourItem;

        if (canvasMode==MODE_CLUSTER){
            contourItem->setSelfPath(true,false);
        } else {
            contourItem->setSelfPath(true,true);
        }

        QPen penNone = QPen(Qt::NoPen);
        QPen penSolid = QPen(Qt::SolidLine);
        QColor color=Config::instance()->clusterColor();
        QBrush brush(color);
        contourItem->setBrush(brush);

        if (selectedClusterIndices.contains(i)){
            contourItem->setPen(penSolid);
        } else {
            contourItem->setPen(penNone);
        }

        this->addItem(contourItem);
    }



}

void CanvasScene::addPathItem(PathClusters& clusters,QMyPathItem *pathItem)
{
    int i;
    PathCluster* cluster;
    PathClusters pathClustersSubset;

    excludeClustersFarAway(clusters,*pathItem,pathClustersSubset);

    PathClusters closeClusters;
    for (i=0;i<pathClustersSubset.size();i++){
        cluster = pathClustersSubset[i];
        qreal distance = dist(pathItem->points,*cluster);
        if (distance < Config::instance()->contourPadding()){
            closeClusters << cluster;
        }
    }

    PathCluster* newCluster = new PathCluster;
    clusters << newCluster;
    newCluster->append(pathItem);

    //merge close enough clusters to one
    for (i=0; i< closeClusters.size();i++){
        cluster = closeClusters[i];
        for (int k=0;k<cluster->size();k++){
            QMyPathItem *aPathItem = (*cluster)[k];
            newCluster->append(aPathItem);
        }
        clusters.removeOne(cluster);
        delete cluster;
    }

    cout << " cluster number is " << clusters.size() << endl;
}

void CanvasScene::removePathItem(PathClusters &clusters, QMyPathItem *pathItem)
{
    PathCluster* cluster = NULL;
    for (int i=0; i<clusters.size(); i++){
        if (clusters[i]->contains(pathItem)){
            cluster = clusters[i];
            break;
        }
    }
    assert(cluster);
    cluster->removeOne(pathItem);
    delete pathItem;
    if (cluster->size() <=0){
        clusters.removeOne(cluster);
        delete cluster;
        cluster = NULL;
    } else if (cluster->size() >=2){//check whether need to divide the cluster
        PathClusters subClusters;
        QMyPathItem *item;
        for (int i=0; i<cluster->size();i++){
            item = (*cluster)[i];
            addPathItem(subClusters,item);
        }
        if (subClusters.size() > 1){
            clusters.removeOne(cluster);
            delete cluster;
            cluster = NULL;
            for (int i=0; i< subClusters.size(); i++){
                clusters << subClusters[i];
            }

        } else {
            for (int i=0; i<subClusters.size(); i++) {
                delete subClusters[i];
            }
            subClusters.clear();
        }
    }


}

void CanvasScene::getAllPathItems(QList<QMyPathItem *> &items)
{
    for (int i=0; i<pathClusters.size();i++){
        for (int k=0;k<pathClusters[i]->size();k++){
            items << (*(pathClusters[i]))[k];
        }
    }
}

void CanvasScene::rebuildPathClusters()
{
    QList<QMyPathItem *> items;
    getAllPathItems(items);
    for (int i=0; i<pathClusters.size(); i++){
        delete pathClusters[i];
    }
    pathClusters.clear();
    for (int i=0; i<items.size(); i++){
        addPathItem(pathClusters,items[i]);
    }
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
    qreal mindist = MYINFINITY;

    for (int i=0; i<cluster.size(); i++){
        QMyPathItem* pathItem=cluster[i];
        qreal distance = ::dist(pathItem->points,polyLine);
        if (mindist>distance){
            mindist = distance;
        }
    }
    return mindist;
}

void CanvasScene::excludeClustersFarAway(const PathClusters &clusters, const QMyPathItem &pathItem, PathClusters &subset)
{
    /* RAII -- resource acquisition is initialization */
    for (int i=0; i< clusters.size(); i++){
        PathCluster* cluster = clusters[i];
        QList<QPointF> pointsInCluster;
        getAllPoints(*cluster, pointsInCluster);
        PolyLine clusterContour;
        calcContourPolygon(pointsInCluster, clusterContour);
        QRectF clusterContourRect = calcEncloseRect(clusterContour);
        QRectF pathRect = calcEncloseRect(pathItem.points);
        if (clusterContourRect.intersects(pathRect)){
            subset << cluster;
        }
    }


}
