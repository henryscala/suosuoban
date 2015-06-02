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

    gridBlackLevel = NULL;

}

CanvasScene::~CanvasScene()
{
    delete gridBlackLevel;
    allPathItems.clear();
}



void CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton){
        return;
    }

    //lazy init because the size of sceneRect is unknown when CanvasScene is created.
    //consider dynamically change gridBlackLevel according to resizeEvent in future.
    if(!gridBlackLevel){
        QRectF selfRect = this->sceneRect();
        gridBlackLevel = new Grid<unsigned char>(selfRect.width(),selfRect.height());
        int x;
        int y;
        for (x=0;x<gridBlackLevel->xsize;x++){
            for (y=0;y<gridBlackLevel->ysize;y++){
                gridBlackLevel->setValue(x,y,0);
            }
        }
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

        qreal gap = abs(currPosF-lastPosF);
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

void CanvasScene::calcContour()
{
    int i;
    for (i=0;i<allPathItems.size();i++){
        QMyPathItem* pathItem = allPathItems[i];
        int k;
        for (k=0;k<pathItem->points.size();k++){
            QPointF point = pathItem->points[k];
            fillCircle(point,Config::instance()->contourPadding(),*gridBlackLevel);
        }
    }
}
