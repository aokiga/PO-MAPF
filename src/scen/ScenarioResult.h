#ifndef PO_MAPF_SCENARIORESULT_H
#define PO_MAPF_SCENARIORESULT_H

#include <vector>
#include "AgentResult.h"

class ScenarioResult {
public:
    bool isCorrect { false };
    int answer { 0 };
    int CTNodes { 0 };
    double runtime { 0 };
    std::vector<AgentResult> agents;
    ScenarioResult() = default;
    explicit ScenarioResult(std::vector<AgentResult> &a, int nodes = 0, double _runtime = 0) : agents(a), isCorrect(true), CTNodes(nodes), runtime(_runtime) {
        for (auto &agent: agents) {
            answer += agent.answer;
        }
    }
};


#endif //PO_MAPF_SCENARIORESULT_H
