#include "persistence.h"
#include "config.h"

#include <QList>
static QList<history::PolyLineOp> undoList;
static QList<history::PolyLineOp> redoList;


static void removeHeadOp(QList<history::PolyLineOp>& list){

    QList<history::PolyLineOp>::iterator it = list.begin();
    for (int i=0;i<it->polyLineCluster.size();i++){
        PolyLine* plHead = it->polyLineCluster.at(i);
        delete plHead;
    }
    it->polyLineCluster.clear();
    list.pop_front();

}

static void removeTailOp(QList<history::PolyLineOp>& list){

    history::PolyLineOp &op = list.back();
    for (int i=0;i<op.polyLineCluster.size();i++){
        PolyLine* plHead = op.polyLineCluster.at(i);
        delete plHead;
    }
    op.polyLineCluster.clear();
    list.pop_back();

}

static void clearList(QList<history::PolyLineOp>& list){
    while(list.size() > 0){
        removeTailOp(list);
    }

}

void history::addPolyLine(PolyLine &pl)
{
    clearList(redoList);

    history::PolyLineOp op;
    op.opType = history::ADD;
    PolyLine* plCopy = new PolyLine;
    *plCopy = pl;
    op.polyLineCluster << plCopy;
    undoList << op;

    if (undoList.size() > Config::instance()->numHistory()){
        removeHeadOp(undoList);
    }


}


void history::delPolyLine(PolyLine &pl)
{
    clearList(redoList);

    history::PolyLineOp op;
    op.opType = history::DEL;
    PolyLine* plCopy = new PolyLine;
    *plCopy = pl;
    op.polyLineCluster << plCopy;
    undoList << op;

    if (undoList.size() > Config::instance()->numHistory()){
        removeHeadOp(undoList);
    }


}


void history::delPolyLineCluster(PolyLineCluster &plc)
{
    clearList(redoList);

    history::PolyLineOp op;
    op.opType = history::DEL_CLUSTER;
    op.polyLineCluster = plc;
    undoList << op;

    if (undoList.size() > Config::instance()->numHistory()){
        removeHeadOp(undoList);
    }


}


bool history::isUndoable()
{
    return undoList.size() > 0;
}


bool history::isRedoable()
{
    return redoList.size() > 0;
}


const history::PolyLineOp &history::undo()
{
    history::PolyLineOp op = undoList.back();
    undoList.pop_back();
    redoList << op;
    return redoList.back();
}

const history::PolyLineOp &history::redo()
{
    history::PolyLineOp op = redoList.back();
    redoList.pop_back();
    undoList << op;
    return undoList.back();
}
