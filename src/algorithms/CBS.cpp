//
// Created by Gleb.Oborin on 11/4/21.
//

#include "CBS.h"
#include "PathPlanningWithConstraints.h"
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>

typedef std::pair<int, int> state;

ScenarioResult CBS::lowLevelSearch(Map &g, Scenario &scen, std::vector<std::set<state>> &constraints) {
    std::vector<std::pair<int, int>> schedule(scen.agents.size());
    for (int i = 0; i < scen.agents.size(); ++i) {
        schedule[i] = std::make_pair(this->prior[i], i);
    }
    std::sort(schedule.begin(), schedule.end());
    std::reverse(schedule.begin(), schedule.end());
    std::vector<AgentResult> res(scen.agents.size());
    for (auto &x: schedule) {
        int agentNum = x.second;
        AgentResult agentResult = PathPlanningWithConstraints::computePath(g, scen.agents[agentNum], constraints[agentNum]);
        if (!agentResult.isFound) {
            return ScenarioResult();
        }
        res[agentNum] = agentResult;
    }
    return ScenarioResult(res);
}

ScenarioResult CBS::run(Map &g, Scenario &scen) {
    std::vector<AgentResult> res(scen.agents.size());
    std::vector<std::vector<std::set<state>>> treeConstraints;
    std::vector<ScenarioResult> treeSolutions;
    std::set<std::pair<double, int>> open;
    treeConstraints.push_back(std::vector<std::set<state>>(scen.agents.size()));
    treeSolutions.push_back(lowLevelSearch(g, scen, treeConstraints[0]));
    if (!treeSolutions[0].isCorrect) {
        return treeSolutions[0];
    }
    open.insert({treeSolutions[0].answer, 0});
    while (!open.empty()) {
        auto vx = (*open.begin());
        open.erase(open.begin());
        int node = vx.second;
        int maxt = 0;
        for (auto &agent: treeSolutions[node].agents) {
            maxt = std::max(maxt, int(agent.path.size()));
        }
        bool conflictFlag = false;
        int c1 = -1, c2 = -1, time = -1, conflictPos = -1;
        for (int t = 0; t < maxt; ++t) {
            std::unordered_map<int, int> x;
            for (int i = 0; i < treeSolutions[node].agents.size(); ++i) {
                AgentResult agent = treeSolutions[node].agents[i];
                int pos;
                if (t < agent.path.size()) {
                    pos = g.cellToInt(agent.path[t]);
                } else {
                    pos = g.cellToInt(agent.path.back());
                }
                if (x.count(pos)) {
                    c1 = x[pos];
                    conflictPos = pos;
                    c2 = i;
                    time = t;
                    conflictFlag = true;
                    break;
                }
                x[pos] = i;
            }
            if (conflictFlag) break;
        }
        if (!conflictFlag) {
            return treeSolutions[node];
        }
        std::vector<int> conflicting = {c1, c2};
        for (auto agentNum : conflicting) {
            int currNode = treeConstraints.size();
            treeConstraints.push_back(treeConstraints[node]);
            treeConstraints[currNode][agentNum].insert({time, conflictPos});
            treeSolutions.push_back(lowLevelSearch(g, scen, treeConstraints[currNode]));
            if (treeSolutions[currNode].isCorrect) {
                open.insert({treeSolutions[currNode].answer, currNode});
            }
        }
    }
    return ScenarioResult();
}