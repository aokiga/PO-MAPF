#ifndef PO_MAPF_PPASTAR_H
#define PO_MAPF_PPASTAR_H


#include "../MAPFAlgorithm.h"
#include "../../../scen/ScenarioResult.h"
#include "../../../scen/Scenario.h"
#include "../MAPFParam.h"
#include "PPAStarParam.h"
#include "../VertexState.h"
#include <vector>
#include <set>
#include <unordered_map>

class PPAStar: public MAPFAlgorithm {
private:
    Map map;
    Scenario scenario;
    PPAStarParam* param;

    AgentResult computePath(Agent &a, std::set<VertexState> &vertexConstraints, std::set<EdgeState> &edgeConstraints,
                std::unordered_map<int, int> &blockedV);
public:
    ScenarioResult run() override;

    double timeLimit;

    PPAStar(Map &g, Scenario &scen, MAPFParam* paramMAPF, double _tl = -1): map(g), scenario(scen), param(nullptr), timeLimit(_tl) {
        param = dynamic_cast<PPAStarParam*>(paramMAPF);
    }
};


#endif //PO_MAPF_PPASTAR_H
