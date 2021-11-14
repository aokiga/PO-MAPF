//
// Created by Gleb.Oborin on 10/31/21.
//

#include "PPAStar.h"
#include "PathPlanningWithConstraints.h"
#include <algorithm>
#include <set>
#include <map>

typedef std::pair<int, int> state;


ScenarioResult PPAStar::run(Map &g, Scenario &scen) {
    std::set<state> constraints;
    std::vector<std::pair<int, int>> schedule(scen.agents.size());
    for (int i = 0; i < scen.agents.size(); ++i) {
        schedule[i] = std::make_pair(this->prior[i], i);
    }
    std::sort(schedule.begin(), schedule.end());
    std::reverse(schedule.begin(), schedule.end());
    std::vector<AgentResult> res(scen.agents.size());
    for (auto &x : schedule) {
        int agentNum = x.second;
        AgentResult agentResult = PathPlanningWithConstraints::computePath(g, scen.agents[agentNum], constraints);
        if (!agentResult.isFound) {
            return ScenarioResult();
        }
        //std::cerr << x.second << ' ' << agentResult.isFound << '\n';
        res[agentNum] = agentResult;
        int timer = 0;
        for (auto &y: agentResult.path) {
            //std::cerr << "! " << timer << ' ' << g.cellToInt(y) << '\n';
            constraints.insert(std::make_pair(timer, g.cellToInt(y)));
            timer++;
        }
    }
    return ScenarioResult(res);
}
