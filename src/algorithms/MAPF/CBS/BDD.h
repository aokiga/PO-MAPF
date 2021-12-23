#ifndef PO_MAPF_BDD_H
#define PO_MAPF_BDD_H

#include <map>

class BDD {
public:
    std::map<int, int> vertexLevelCounter;
    std::map<int, int> edgesLevelCounter;

    BDD() = default;
};


#endif //PO_MAPF_BDD_H
