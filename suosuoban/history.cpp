#include "history.h"



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

//line operation
static void lineOp(PolyLine &pl,history::PolyLineOpTypes type){
    clearList(redoList);

    history::PolyLineOp op;
    op.opType = type;
    PolyLine* plCopy = new PolyLine;
    *plCopy = pl;
    op.polyLineCluster << plCopy;
    undoList << op;

    if (undoList.size() > Config::instance()->numHistory()){
        removeHeadOp(undoList);
    }
}

static void clusterOp(PolyLineCluster& plc,history::PolyLineOpTypes type){
    clearList(redoList);

    history::PolyLineOp op;
    op.opType = type;
    op.polyLineCluster = plc;
    undoList << op;

    if (undoList.size() > Config::instance()->numHistory()){
        removeHeadOp(undoList);
    }
}

void history::addPolyLine(PolyLine &pl)
{
    lineOp(pl,history::ADD_LINE);
}


void history::delPolyLine(PolyLine &pl)
{
    lineOp(pl,history::DEL_LINE);

}

void history::addPolyLineCluster(PolyLineCluster& plc){
    clusterOp(plc,history::ADD_CLUSTER);
}



void history::delPolyLineCluster(PolyLineCluster &plc)
{
    clusterOp(plc,history::DEL_CLUSTER);
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
