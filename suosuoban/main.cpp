#include "mainwindow.h"
#include <QApplication>

#include <cassert>
#include "geom.h"

void testAll();

int main(int argc, char *argv[])
{
    //testing
    testAll();

    //running
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}




void testPolyLine(){
    PolyLine polyLine;
    PolyLine::NodeType* node;
    assert(polyLine.size==0);
    assert(polyLine.head==NULL);

    VecDouble vec;
    vec.x = 1;
    vec.y = 1;

    for (int i=0;i<3;i++){

        node = new PolyLine::NodeType;
        vec.x = i;
        vec.y = i;
        node->value = vec;
        polyLine.pushBack(node);
    }

    assert(polyLine.size==3);
    assert(polyLine.head);

    node = polyLine.head;
    for (int i=0;i<3;i++){
        node = node->next;
    }

    assert(polyLine.head==node);


    while(polyLine.size>0){
        node = polyLine.head;
        polyLine.remove(node);
        delete node;
    }
    assert(polyLine.size==0);
    assert(polyLine.head==NULL);
}

void testAll(){
    testPolyLine();
}
