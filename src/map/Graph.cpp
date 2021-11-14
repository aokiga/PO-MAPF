//
// Created by Gleb.Oborin on 10/20/21.
//

#include "Graph.h"
#include <string>


std::vector<int> Graph::get_neighbors(int v) {
    return g[v].neighbors;
}
