#include "PPAStar.h"
#include "PPAStarParam.h"
#include <algorithm>
#include <set>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

ScenarioResult PPAStar::run() {
    std::vector<int> prior = param->getPrior(scenario);

    VertexConstraints vertexConstraints;
    EdgeConstraints edgeConstraints;
    std::vector<std::pair<int, int>> schedule(scenario.agents.size());
    for (int i = 0; i < scenario.agents.size(); ++i) {
        schedule[i] = std::make_pair(prior[i], i);
    }
    std::sort(schedule.begin(), schedule.end());
    std::reverse(schedule.begin(), schedule.end());
    std::vector<AgentResult> res(scenario.agents.size());
    std::unordered_map<int, int> blockedV;

    double time_begin = clock();

    for (auto &x : schedule) {
        double currtime = clock();
        double runtime = (currtime - time_begin) / CLOCKS_PER_SEC;
        if (runtime > timeLimit) {
            return ScenarioResult();
        }
        int agentNum = x.second;
        AgentResult agentResult = computePath(scenario.agents[agentNum], vertexConstraints,
                                                                           edgeConstraints, blockedV);
        if (!agentResult.isFound) {
            return ScenarioResult();
        }
        //std::cerr << x.second << ' ' << agentResult.isFound << '\n';
        res[agentNum] = agentResult;
        int timer = 0;
        for (int i = 0; i < agentResult.path.size(); ++i) {
            auto curV = agentResult.path[i];
            int v = map.cellToInt(curV);
            vertexConstraints.insert(std::make_pair(timer, v));
            if (i + 1 < agentResult.path.size()) {
                auto curTo = agentResult.path[i + 1];
                int to = map.cellToInt(curTo);
                edgeConstraints.insert(std::make_tuple(timer, to, v));
            }
            timer++;
        }
        blockedV[map.cellToInt(agentResult.path.back())] = timer;
    }
    return ScenarioResult(res);
}


AgentResult PPAStar::computePath(Agent &a, VertexConstraints &vertexConstraints,
                                 EdgeConstraints &edgeConstraints,
                                                     std::unordered_map<int, int> &blockedV) {
    int start = map.cellToInt(a.start);
    int end = map.cellToInt(a.end);
    std::set<std::pair<VertexInfo, VertexState>> s;
    VertexState startState = VertexState(0, start);
    s.insert({VertexInfo(0, map.h(start, end)), startState});
    std::unordered_map<VertexState, VertexInfo, hash_VertexState> dist;
    std::unordered_map<VertexState, VertexState, hash_VertexState> p;
    dist[startState] = VertexInfo(0, map.h(start, end));
    bool foundPath = false;
    VertexState endState;
    while (!s.empty()) {
        auto vx = (*s.begin());
        s.erase(s.begin());
        VertexState vState = vx.second;
        int time = vx.second.first;
        int v = vx.second.second;
        if (v == end) {
            foundPath = true;
            endState = vState;
            break;
        }
        for (auto &to : map.g[v].neighbors) {
            VertexState toState = {time + 1, to};
            if (vertexConstraints.count(toState) > 0) continue;
            if (edgeConstraints.count({time, v, to}) > 0) continue;
            if (blockedV.count(v) > 0 && time >= blockedV[v]) continue;
            if (dist.count(toState) == 0 || dist[toState].g > dist[vState].g + 1) {
                VertexInfo nInfo = VertexInfo(dist[vState].g + 1, map.h(v, to));
                s.erase({ dist[toState], toState } );
                dist[toState] = nInfo;
                p[toState] = vState;
                s.insert({ dist[toState], toState});
            }
        }
    }
    if (!foundPath) {
        return AgentResult();
    }
    int answer = dist[endState].g;
    std::vector<Cell> path;
    VertexState cur = endState;
    while (cur != startState) {
        path.push_back(map.intToCell(cur.second));
        cur = p[cur];
    }
    path.push_back(map.intToCell(start));
    std::reverse(path.begin(), path.end());
    return AgentResult(path, answer);
}