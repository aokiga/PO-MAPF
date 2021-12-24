#ifndef PO_MAPF_MDD_H
#define PO_MAPF_MDD_H

#include <map>

class MDD {
public:
    std::map<int, int> vertexLevelCounter;
    std::map<int, int> edgesLevelCounter;

    MDD() = default;
};


#endif //PO_MAPF_MDD_H
