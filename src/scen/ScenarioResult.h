//
// Created by Gleb.Oborin on 10/20/21.
//

#ifndef PO_MAPF_SCENARIORESULT_H
#define PO_MAPF_SCENARIORESULT_H

#include <vector>
#include "AgentResult.h"

class ScenarioResult {
public:
    bool isCorrect { false };
    double answer { 0 };
    std::vector<AgentResult> agents;
    ScenarioResult() = default;
    explicit ScenarioResult(std::vector<AgentResult> &a) : agents(a), isCorrect(true) {
        for (auto &agent: agents) {
            answer += agent.answer;
        }
    }
};


#endif //PO_MAPF_SCENARIORESULT_H
