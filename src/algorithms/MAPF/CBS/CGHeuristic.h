#ifndef PO_MAPF_CGHEURISTIC_H
#define PO_MAPF_CGHEURISTIC_H


#include "../Conflicts.h"

class CGHeuristic {
private:
    int findMinCover(std::vector<std::vector<int>> &graph, int m);
    bool isCover(std::vector<std::vector<int>> &graph, int k, int m);
public:
    int count(Conflicts &conflicts);
};


#endif //PO_MAPF_CGHEURISTIC_H
