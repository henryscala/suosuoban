#ifndef HISTORY_H
#define HISTORY_H


#include "geom.h"

namespace history {
    enum PolyLineOpTypes{
        ADD_LINE,DEL_LINE,ADD_CLUSTER,DEL_CLUSTER
    };

    struct PolyLineOp{
        PolyLineOpTypes opType;
        PolyLineCluster polyLineCluster;
    };

    void addPolyLine(PolyLine& pl);
    void delPolyLine(PolyLine& pl);
    void delPolyLineCluster(PolyLineCluster& plc);
    void addPolyLineCluster(PolyLineCluster& plc);
    bool isUndoable();
    bool isRedoable();
    const PolyLineOp& undo();
    const PolyLineOp& redo();

}

#endif // HISTORY_H
