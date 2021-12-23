#ifndef PO_MAPF_VERTEXSTATE_H
#define PO_MAPF_VERTEXSTATE_H

#include <tuple>

typedef std::pair<int, int> VertexState;
typedef std::tuple<int, int, int> EdgeState;

class VertexInfo {
public:
    int g{}, h{}, f{};

    VertexInfo() = default;

    VertexInfo(int _g, int _h) : g(_g), h(_h) {
        f = g + h;
    }
};

bool operator < (VertexInfo a, VertexInfo b);

#endif //PO_MAPF_VERTEXSTATE_H
