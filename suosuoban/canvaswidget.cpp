#include "canvaswidget.h"
#include "ui_canvaswidget.h"
#include <QtWidgets>
#include <iostream>
#include <cassert>
#include "config.h"
using namespace std;

CanvasWidget::CanvasWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanvasWidget),
    isMouseDown(false)
{
    ui->setupUi(this);

    currPath = NULL;
    currCluster = new PolyLineCluster;

}

CanvasWidget::~CanvasWidget()
{
    delete ui;

    //deleting null pointer is ok
    delete currPath;
    delete currCluster;

}

void CanvasWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);//notice: cannot user painter outside paintEvent

    painter.drawRect(10,10,this->geometry().width()-50,this->geometry().height()-50);
    painter.drawLine(0,0,this->geometry().width(),this->geometry().height());
    painter.drawLine(this->geometry().width(),0,0,this->geometry().height());

    PolyLineCluster::NodeType* pPolyLineNode = currCluster->head;
    cout <<"currCluster->size " << currCluster->size << endl;
    for (int i=0;i< currCluster->size;i++){
        PolyLine::NodeType* pVec = pPolyLineNode->value->head;
        cout <<"pPolyLineNode->value.size " << pPolyLineNode->value->size << endl;
        for (int j=0;j<pPolyLineNode->value->size-1;j++){
            painter.drawLine(pVec->value.x,pVec->value.y,pVec->next->value.x,pVec->next->value.y);
            pVec=pVec->next;

        }

        pPolyLineNode=pPolyLineNode->next;
    }

}

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    isMouseDown = true;

    VecDouble currPos;
    currPos.x = event->pos().x();
    currPos.y = event->pos().y();
    cout << "mouse press" << currPos.x << " , " <<currPos.y  <<endl;
    if (currPath){
        //ignore half way decent path
        while (currPath->size > 0){
            PolyLine::NodeType* node= currPath->head;
            currPath->remove(node);
            delete node;
        }
    } else {
        currPath = new PolyLine;
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{

    if (!isMouseDown) {
        return;
    }

    assert(currPath);

    VecDouble currPos;
    currPos.x = event->pos().x();
    currPos.y = event->pos().y();

    VecDouble lastPos;
    bool lastPosSet = false;

    if (currPath->size > 0 ) {
        lastPos = currPath->peekBack()->value;
        lastPosSet = true;
        double gap = (lastPos - currPos).abs();
        if (gap < Config::instance()->minGap() ){
            return;
        }
    }


    PolyLine::NodeType *node = new PolyLine::NodeType;
    node->value = currPos;

    //cout << "mouse move " << node->value.x << " " << node->value.y << endl;
    currPath->pushBack(node);

    if(lastPosSet) {
        update();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    VecDouble currPos;
    currPos.x = event->pos().x();
    currPos.y = event->pos().y();
    cout << "mouse release" << currPos.x << " , " <<currPos.y  <<endl;


    if (currPath!=0 ){
        //a valid path must have GE 2 points
        if (currPath->size >= 2) {
            PolyLineCluster::NodeType *nodePolyLine = new PolyLineCluster::NodeType;
            nodePolyLine->value = currPath; //currPath's owner moved
            currCluster->pushBack(nodePolyLine);
            currPath = NULL;

        } else {
            while(currPath->size > 0){
                PolyLine::NodeType* node = currPath->head;
                currPath->remove(node);
                delete node;
            }

        }

    }

    isMouseDown = false;
}
