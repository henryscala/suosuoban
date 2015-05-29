#include "canvaswidget.h"
#include "ui_canvaswidget.h"
#include <QtWidgets>
#include <iostream>
using namespace std;

CanvasWidget::CanvasWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanvasWidget),
    isMouseDown(false)
{
    ui->setupUi(this);

    currPath = 0;
    currCluster = new PolyLineCluster;

}

CanvasWidget::~CanvasWidget()
{
    delete ui;

    delete currCluster;
}

void CanvasWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);


    painter.drawRect(10,10,this->geometry().width()-50,this->geometry().height()-50);
    painter.drawLine(0,0,this->geometry().width(),this->geometry().height());
    painter.drawLine(this->geometry().width(),0,0,this->geometry().height());

    PolyLineCluster::NodeType* pPolyLineNode = currCluster->head;
    cout <<"currCluster->size " << currCluster->size << endl;
    for (int i=0;currCluster->size;i++){
        PolyLine::NodeType* pVec = pPolyLineNode->value.head;
        cout <<"pPolyLineNode->value.size " << pPolyLineNode->value.size << endl;
        for (int j=0;j<pPolyLineNode->value.size-1;j++){
            painter.drawLine(pVec->value.x,pVec->value.y,pVec->next->value.x,pVec->next->value.y);
            pVec=pVec->next;
            break;
        }
        break;
        pPolyLineNode=pPolyLineNode->next;
    }

}

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    isMouseDown = true;

    cout << "mouse press" << endl;
    if (currPath!=0){
        currPath->removeAndFreeAll();
    } else {
        currPath = new PolyLine;
    }

}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{

    if (!isMouseDown) {
        return;
    }

    PolyLine::NodeType *node = new PolyLine::NodeType;
    node->value.x=event->pos().x();
    node->value.y=event->pos().y();
    cout << "mouse move " << node->value.x << " " << node->value.y << endl;
    currPath->pushBack(node);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }
    cout << "mouse release" << endl;

    if (currPath!=0 ){
        if (currPath->size>=2) {
            PolyLineCluster::NodeType *nodePolyLine = new PolyLineCluster::NodeType;
            for (int i=0;i<currPath->size;i++){
                nodePolyLine->value.pushBack(currPath->head);
                currPath->remove(currPath->head);
            }
            currCluster->pushBack(nodePolyLine);

        } else {

        }
        delete currPath;
        currPath = 0;
    }

    isMouseDown = false;
}
