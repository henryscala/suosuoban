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
    allPathItems.clear();
}



void CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    isMouseDown = true;
    cout << "mouse down" << endl;

    if (!currPathItem){
        delete currPathItem;
    }
    currPathItem = new QMyPathItem();
    this->addItem(currPathItem);
    QPen defaultPen ;
    defaultPen.setWidth(Config::instance()->defaultPenWidth());
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
    VecDouble currPos = qpointf2vecDouble(currPosF);



    if (currPathItem->points.size() > 0) {
        QPointF lastPosF = currPathItem->points.last();
        VecDouble lastPos = qpointf2vecDouble(lastPosF);
        double gap = (lastPos - currPos).abs();
        if (gap < Config::instance()->minGap() ){
            return;
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
            allPathItems.append(currPathItem);
        } else {
            delete currPathItem;
        }
    }
    currPathItem = NULL;
    isMouseDown = false;


    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

