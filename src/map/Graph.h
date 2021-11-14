//
// Created by Gleb.Oborin on 10/20/21.
//

#include <vector>
#include <string>
#include "Node.h"

#ifndef PO_MAPF_GRAPH_H
#define PO_MAPF_GRAPH_H

class Graph {
public:
    int size;
    std::vector<Node> g;

    std::vector<int> get_neighbors(int v);
};


#endif //PO_MAPF_GRAPH_H
