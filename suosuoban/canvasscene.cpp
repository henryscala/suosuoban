#include "canvasscene.h"

#include <cassert>

#include <cmath>
#include "geom.h"
#include "config.h"
#include "persistence.h"


using namespace std;

CanvasScene::CanvasScene(QObject *parent)
        :QGraphicsScene(parent)
{
    currPathItem = NULL;
    isMouseDown = false;
    isModKeyDown = false;
    isShowCluster = true;
    canvasMode = MODE_DRAW;

    QBrush brush(Config::instance()->backColor());
    this->setBackgroundBrush(brush);
}

CanvasScene::~CanvasScene()
{
    //
    delete currPathItem;

    //release resources in pathClusters

    clearPathClusters(pathClusters);
    clearPathClusters(movingClusters);
}

bool CanvasScene::saveFile(QString fileName)
{
    QList<QMyPathItem *> items;
    getAllPathItems(items);
    ScenePersistence marshal;
    return marshal.saveToFile(fileName,items);

}

bool CanvasScene::readFile(QString fileName)
{
    QList<QMyPathItem *> items;
    ScenePersistence unmarshal;
    unmarshal.loadFromFile(fileName,items);
    clearPathClusters(pathClusters);

    QListIterator<QMyPathItem*> it(items);
    while (it.hasNext()){
        QMyPathItem* item = it.next();
        this->addItem(item);
        item->setSelfPath(false,false);
        addPathItem(pathClusters,item);
    }
    calcContour();
    return true;
}

void CanvasScene::clearPathClusters(PathClusters& clusters){
    for (int i=0;i<clusters.size();i++){
        PathCluster* cluster = clusters[i];

        if (cluster){
            clearPathCluster(&cluster);
        }
    }
    clusters.clear();
}

void CanvasScene::clearPathCluster(PathCluster **cluster)
{
    for (int k=0;k<(*cluster)->size();k++){
        QMyPathItem* pathItem = (**cluster)[k];
        if(pathItem){
            delete pathItem;
        }
    }

    delete *cluster;
    *cluster = NULL;
}

void CanvasScene::canvasModeChange(CanvasMode mode)
{


    canvasMode=mode;



    selectedClusterIndices.clear();
    calcContour();
}

void CanvasScene::canvasColorChange(CanvasColorType colorType, QColor color)
{
    QBrush brush(color);
    switch (colorType){
    case COLOR_TYPE_PEN:
        Config::instance()->penColor(color);
        break;
    case COLOR_TYPE_BACK:
        Config::instance()->backColor(color);

        this->setBackgroundBrush(brush);
        break;
    case COLOR_TYPE_CLUSTER:
        Config::instance()->clusterColor(color);
        calcContour();
        break;
    default:
        assert(false);
    }
}

void CanvasScene::setShowCluster(bool show)
{
    isShowCluster = show;
    calcContour();
}

void CanvasScene::selectAll()
{
    selectedClusterIndices.clear();

    for (int i=0;i<pathClusters.size();i++){
        selectedClusterIndices << i;
    }

    calcContour();
}

void CanvasScene::delCluster()
{
    if (!isShowCluster){
        return;
    }

    if (canvasMode != MODE_CLUSTER){
        return;
    }

    if (selectedClusterIndices.size() <= 0) {
        return;
    }

    std::sort(selectedClusterIndices.begin(),selectedClusterIndices.end());
    for (int i=selectedClusterIndices.size()-1; i>=0; i--){
        int idx = selectedClusterIndices[i];
        PathCluster* cluster = pathClusters[idx];
        pathClusters.removeAt(idx);

        //undo/redo handling
        PolyLineCluster plCluster;
        pathClusterToPolylineCluster(*cluster,plCluster);
        history::delPolyLineCluster(plCluster);

        clearPathCluster(&cluster);
    }
    selectedClusterIndices.clear();
    calcContour();
}

void CanvasScene::undoRedo(const history::PolyLineOp &op, bool isUndo)
{
    PolyLine* pl;
    QMyPathItem* item;
    const PolyLineCluster& cluster=op.polyLineCluster;

#define del_line  { \
    pl=op.polyLineCluster[0]; \
    item=findPathItem(*pl); \
    if(item){ \
        removePathItem(pathClusters,item); \
        removeEmptyCluster(); \
    } \
}

#define add_line  { \
    pl=op.polyLineCluster[0]; \
    item=createPathItem(); \
    item->points = *pl; \
    item->setSelfPath(false,false); \
    addPathItem(pathClusters,item); \
}

#define add_cluster { \
    for (int i=0;i<cluster.size();i++){ \
        pl=cluster.at(i); \
        item=createPathItem(); \
        item->points = *pl; \
        item->setSelfPath(false,false); \
        addPathItem(pathClusters,item); \
    } \
    rebuildPathClusters(); \
}

#define del_cluster { \
    for (int i=0;i<cluster.size();i++){ \
        pl=cluster.at(i); \
        item=findPathItem(*pl); \
        if (item){ \
            removePathItem(pathClusters,item); \
        } \
    } \
    removeEmptyCluster(); \
    rebuildPathClusters(); \
}

#define first_or_second( execFirst, firstFunc, secondFunc ) { \
    if ( (execFirst) ){ \
        firstFunc; \
    } else { \
        secondFunc; \
    } \
}

    switch(op.opType){
    case history::ADD_LINE:
        first_or_second(isUndo,del_line,add_line);
        break;
    case history::DEL_LINE:
        first_or_second(!isUndo,del_line,add_line);
        break;
    case history::ADD_CLUSTER:
        first_or_second(isUndo,del_cluster,add_cluster);
        break;
    case history::DEL_CLUSTER:
        first_or_second(!isUndo,del_cluster,add_cluster);
        break;
    }

    calcContour();
}

void CanvasScene::configChange()
{
    rebuildPathClusters();
    calcContour();

    //reset pen width
    QList<QMyPathItem *> items;
    getAllPathItems(items);
    for (int i=0;i<items.size();i++){
        QMyPathItem *item = items[i];
        QPen pen = item->pen();
        pen.setWidth(Config::instance()->penWidth());
        item->setPen(pen);
    }
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
    currPathItem = createPathItem();
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
        if (currPathItem && currPathItem->points.size() >=2 ){
            PathClusters closeClusters;
            excludeClustersFarAway(pathClusters, *currPathItem, closeClusters);

            for (int i=0; i<closeClusters.size();i++){
                PathCluster* cluster = closeClusters[i];

                for (int k=cluster->size()-1; k >=0 ; k--){
                    QMyPathItem* item = (*cluster)[k];
                    if (isPathIntersect(item->points, currPathItem->points)){
                        history::delPolyLine(item->points);
                        removePathItem(pathClusters,item);
                    }
                }
            }

            removeEmptyCluster();

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
                history::addPolyLine(currPathItem->points);//for undo/redo
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
            PathCluster *orphanCluster = new PathCluster; //it is not in pathClusters now
            for (int i=0; i<selectedClusterIndices.size(); i++){
                PathCluster* cluster = pathClusters[selectedClusterIndices[i]];


                //copy
                if (isModKeyDown){

                    for (int k=0;k<cluster->size();k++){
                        QMyPathItem* pathItem = (*cluster)[k];
                        QMyPathItem* newPathItem = clonePathItem(pathItem);
                        move(newPathItem->points, deviation);
                        newPathItem->setSelfPath(false,false);
                        //temporarily add the item to orphanCluster, later everything will be rebuilt
                        //in this way clusters in pathClusters keeps stable to avoid crash
                        *orphanCluster << newPathItem;
                    }

                    //undo redo handling
                    PolyLineCluster plc;
                    pathClusterToPolylineCluster(*orphanCluster,plc);
                    history::addPolyLineCluster(plc);

                } else {//move
                    //move action is like first delete and later add
                    //undo redo handling
                    PolyLineCluster plcOld;
                    pathClusterToPolylineCluster(*cluster,plcOld);
                    history::delPolyLineCluster(plcOld);

                    for (int k=0; k<cluster->size();k++){
                        QMyPathItem* pathItem = (*cluster)[k];
                        move(pathItem->points, deviation);
                        pathItem->setSelfPath(false,false);

                    }

                    //undo redo handling
                    PolyLineCluster plcNew;
                    pathClusterToPolylineCluster(*cluster,plcNew);
                    history::addPolyLineCluster(plcNew);
                }
            }

            pathClusters << orphanCluster;
            rebuildPathClusters();
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

    emit sceneChanged();

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void CanvasScene::keyPressEvent ( QKeyEvent * event )
{

    if (event->key() == Qt::Key_Control
            || event->key() == Qt::Key_Alt
            || event->key() ==Qt::Key_Shift)
    {
        isModKeyDown = true;

    }
}

void CanvasScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Control
            || event->key() == Qt::Key_Alt
            || event->key() ==Qt::Key_Shift)
    {
        isModKeyDown = false;

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

    if(!isShowCluster){
        return;
    }

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

        if (canvasMode==MODE_CLUSTER){
            contourItem->setSelfPath(true,false);
        } else {
            contourItem->setSelfPath(true,true);
        }

        QPen penNone = QPen(Qt::NoPen);
        QPen penSolid = QPen(Qt::SolidLine);
        penSolid.setColor(Config::instance()->penColor());
        QColor brushColor=Config::instance()->clusterColor();
        QBrush brush(brushColor);
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
        //postpone remove because I don't want to invalidate old clusters referenced outside the function
        cluster->clear();//make it size 0

    } else if (cluster->size() >=2){//check whether need to divide the cluster
        PathClusters subClusters;
        QMyPathItem *item;
        for (int i=0; i<cluster->size();i++){
            item = (*cluster)[i];
            addPathItem(subClusters,item);
        }
        if (subClusters.size() > 1){
            //postpone remove because I don't want to invalidate old clusters referenced outside the function
            cluster->clear();//make it size 0

            for (int i=0; i< subClusters.size(); i++){
                clusters << subClusters[i];
            }

        } else if (subClusters.size() == 1) {
            delete subClusters[0];
        } else {
            assert(false);
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

void CanvasScene::removeEmptyCluster()
{

    //remove empty cluster
    for (int i=pathClusters.size()-1; i>=0; i--){
        PathCluster* cluster = pathClusters[i];
        if (cluster->size() <= 0){
            pathClusters.removeAt(i);
            delete cluster;
        }
    }
}

QMyPathItem *CanvasScene::findPathItem(const PolyLine &pl)
{
    QMyPathItem *item = NULL;
    for (int i=0; i<pathClusters.size();i++){
        for (int k=0;k<pathClusters[i]->size();k++){
            item=(*(pathClusters[i]))[k];
            if (item->points.size() != pl.size()){
                continue;
            }
            int t;
            for (t=0;t<pl.size();t++){
                QPointF pa = pl[t];
                QPointF pb = item->points[t];
                //pa != pb
                if (!(closeToZero(pa.x()-pb.x()) && closeToZero(pa.y()-pb.y()))){
                    break;
                }
            }
            if (t>=pl.size()){
                return item;
            }
        }
    }
    return NULL;
}

QMyPathItem *CanvasScene::createPathItem()
{
    QMyPathItem* item = new QMyPathItem(Config::instance()->penWidth(),
                                        Config::instance()->penColor(),
                                        this);

    return item;
}


void pathClusterToPolylineCluster(const PathCluster &pc, PolyLineCluster &plc)
{
    for (int k=0; k<pc.size();k++){
        PolyLine *polyLine = new PolyLine();
        *polyLine = pc[k]->points;
        plc << polyLine;
    }
}
