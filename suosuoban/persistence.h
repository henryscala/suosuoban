#ifndef PERSISTENCE_H
#define PERSISTENCE_H
#include "geom.h"

namespace history {
    enum PolyLineOpTypes{
        ADD,DEL,DEL_CLUSTER
    };

    struct PolyLineOp{
        PolyLineOpTypes opType;
        PolyLineCluster polyLineCluster;
    };

    void addPolyLine(PolyLine& pl);
    void delPolyLine(PolyLine& pl);
    void delPolyLineCluster(PolyLineCluster& plc);
    bool isUndoable();
    bool isRedoable();
    const PolyLineOp& undo();
    const PolyLineOp& redo();

}

#endif // PERSISTENCE_H
