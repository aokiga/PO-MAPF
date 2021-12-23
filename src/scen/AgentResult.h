#include <vector>
#include "../map/Map.h"
#include "../algorithms/MAPF/CBS/BDD.h"

#ifndef PO_MAPF_AGENTRESULT_H
#define PO_MAPF_AGENTRESULT_H

class AgentResult {
public:
    bool isFound { false };
    int answer { 0 };
    std::vector<Cell> path { std::vector<Cell>(0) };

    AgentResult() = default;
    explicit AgentResult(std::vector<Cell> &_path, int answer) : isFound(true), path(_path), answer(answer) {}

    BDD bdd = BDD();
};


#endif //PO_MAPF_AGENTRESULT_H
