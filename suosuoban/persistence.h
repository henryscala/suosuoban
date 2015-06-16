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
    bool gotoPrev();
    bool gotoNext();
    const PolyLineOp* getOp();

}

#endif // PERSISTENCE_H
