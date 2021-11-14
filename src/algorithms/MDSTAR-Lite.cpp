//
// Created by Gleb.Oborin on 10/20/21.
//

#include "MDSTAR_Lite.h"
#include "../map/Map.h"
#include <vector>
#include <set>
#include <iostream>

std::vector<int> A_Star(Map &g, int start, int end) {
    std::set<std::pair<double, int>> a;
    std::vector<int> p(g.size, -1);
    std::vector<double> d(g.size, 100000000);
    d[start] = 0;
    a.insert(std::make_pair(0 + g.h(start, end), start));
    while (!a.empty()) {
        auto vv = *(a.begin());
        a.erase(a.begin());
        int v = vv.second;
        if (v == end) break;
        //std::cerr << g.g[v].neighbors.size() << '\n';
        for (auto to : g.g[v].neighbors) {
            if (g.g[to].isBlocked) continue;
            if (1 + d[v] < d[to]) {
                double nh = g.h(to, end);
                a.erase(std::make_pair(d[to] + nh, to));
                d[to] = d[v] + 1;
                p[to] = v;
                a.insert(std::make_pair(d[to] + nh, to));
            }
        }
    }
    std::vector<int> ans;
    int cur = end;
    if (p[cur] == -1) return {};
    while (cur != start) {
        ans.push_back(cur);
        cur = p[cur];
    }
    std::reverse(ans.begin(), ans.end());
    return ans;
}

ScenarioResult MDSTAR_Lite::run(Map &g, Scenario &scen) {
    int n = scen.agents.size();
    std::vector<AgentResult> results(n);
    for (int i = 0; i < n; ++i) {
        results[i].trueAnswer = scen.agents[i].answer;
        results[i].path.push_back(scen.agents[i].start);
        int curv = g.cellToInt(scen.agents[i].start);
        g.g[curv].isBlocked = true;
    }
    int agentsInProgress = n;
    while (agentsInProgress > 0) {
        for (int agentNum = 0; agentNum < n; ++agentNum) {
            if (results[agentNum].path.back() == scen.agents[agentNum].end) continue;

            int curv = g.cellToInt(results[agentNum].path.back());
            int endv = g.cellToInt(scen.agents[agentNum].end);

            std::vector<int> path = A_Star(g, curv, endv);
            if (path.empty()) {
                results[agentNum].path.push_back(results[agentNum].path.back());
                continue;
            }
            int nextv = path[0];
            g.g[curv].isBlocked = false;
            g.g[nextv].isBlocked = true;
            results[agentNum].path.push_back(g.intToCell(nextv));
            //std::cerr << "Agent " << agentNum << " next move:" <<  results[agentNum].path.back().first << ' ' << results[agentNum].path.back().second << '\n';
            results[agentNum].answer++;
            if (results[agentNum].path.back() == scen.agents[agentNum].end) {
                agentsInProgress--;
                results[agentNum].isFound = true;
            }
        }
    }
    return ScenarioResult(results);
}
