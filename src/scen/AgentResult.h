//
// Created by Gleb.Oborin on 10/20/21.
//

#include <vector>
#include "../map/Map.h"

#ifndef PO_MAPF_AGENTRESULT_H
#define PO_MAPF_AGENTRESULT_H

class AgentResult {
public:
    bool isFound { false };
    double answer { 0 }, trueAnswer { 0 };
    std::vector<Cell> path { std::vector<Cell>(0) };

    AgentResult() = default;
    AgentResult(double trueAnswer) : trueAnswer(trueAnswer) {}
    explicit AgentResult(std::vector<Cell> &_path, double answer, double trueAnswer) : isFound(true), path(_path), answer(answer), trueAnswer(trueAnswer) {}
};


#endif //PO_MAPF_AGENTRESULT_H
