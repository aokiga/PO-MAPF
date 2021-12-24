#ifndef PO_MAPF_VERTEXCONFLICT_H
#define PO_MAPF_VERTEXCONFLICT_H

#include <vector>

class VertexConflict {
public:
    int a1, a2, v, time;
    VertexConflict() = default;
    VertexConflict(int _a1, int _a2, int _v, int _time) : a1(_a1), a2(_a2), v(_v), time(_time) {}
};

class EdgeConflict {
public:
    int a1, a2, v, to, time;
    EdgeConflict() = default;
    EdgeConflict(int _a1, int _a2, int _v, int _to, int _time) : a1(_a1), a2(_a2), v(_v), to(_to), time(_time) {}
};

typedef std::vector<VertexConflict> VertexConflicts;
typedef std::vector<EdgeConflict> EdgesConflicts;
typedef std::pair<VertexConflicts, EdgesConflicts> Conflicts;

#endif //PO_MAPF_VERTEXCONFLICT_H
