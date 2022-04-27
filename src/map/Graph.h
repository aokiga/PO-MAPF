#include <vector>
#include <string>
#include "Node.h"

#ifndef PO_MAPF_GRAPH_H
#define PO_MAPF_GRAPH_H

class Graph {
public:
    int size;
    std::vector<Node> g;

    virtual std::vector<int> get_neighbors(int v) = 0;
};


#endif //PO_MAPF_GRAPH_H
