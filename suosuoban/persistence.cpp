#include "persistence.h"
#include "config.h"

#include <QList>
static QList<history::PolyLineOp> sPolyLineOpList;
static int index = -1;

static void removeHeadOp(){

    QList<history::PolyLineOp>::iterator it = sPolyLineOpList.begin();
    for (int i=0;i<it->polyLineCluster.size();i++){
        PolyLine* plHead = it->polyLineCluster.at(i);
        delete plHead;
    }
    it->polyLineCluster.clear();
    sPolyLineOpList.pop_front();

}

static void removeTailOp(){

    history::PolyLineOp &op = sPolyLineOpList.back();
    for (int i=0;i<op.polyLineCluster.size();i++){
        PolyLine* plHead = op.polyLineCluster.at(i);
        delete plHead;
    }
    op.polyLineCluster.clear();
    sPolyLineOpList.pop_back();

}

static void discardLaterHistory(){
    //goto prev history happens, remove items from then on
    if (sPolyLineOpList.size() >0 && index<sPolyLineOpList.size()-1){
        while(sPolyLineOpList.size() > index){
            removeTailOp();
        }
    }
}

void history::addPolyLine(PolyLine &pl)
{
    discardLaterHistory();

    history::PolyLineOp op;
    op.opType = history::ADD;
    PolyLine* plCopy = new PolyLine;
    *plCopy = pl;
    op.polyLineCluster << plCopy;
    sPolyLineOpList << op;

    if (sPolyLineOpList.size() > Config::instance()->numHistory()){
        removeHeadOp();
    }

    index = sPolyLineOpList.size() - 1;
}


void history::delPolyLine(PolyLine &pl)
{
    discardLaterHistory();

    history::PolyLineOp op;
    op.opType = history::DEL;
    PolyLine* plCopy = new PolyLine;
    *plCopy = pl;
    op.polyLineCluster << plCopy;
    sPolyLineOpList << op;

    if (sPolyLineOpList.size() > Config::instance()->numHistory()){
        removeHeadOp();
    }

    index = sPolyLineOpList.size() - 1;
}


void history::delPolyLineCluster(PolyLineCluster &plc)
{
    discardLaterHistory();

    history::PolyLineOp op;
    op.opType = history::DEL_CLUSTER;
    op.polyLineCluster = plc;
    sPolyLineOpList << op;

    if (sPolyLineOpList.size() > Config::instance()->numHistory()){
        removeHeadOp();
    }

    index = sPolyLineOpList.size() - 1;
}


bool history::gotoPrev()
{
    index --;
    if (index<0){
        index = 0;
        return false;
    }
    return true;
}


bool history::gotoNext()
{
    index ++;
    if (index>=sPolyLineOpList.size()){
        index = sPolyLineOpList.size() -1;
        return false;
    }
    return true;
}


const history::PolyLineOp *history::getOp()
{
    if (sPolyLineOpList.size()<=0){
        return NULL;
    }
    return &sPolyLineOpList.at(index);
}
