//
// Created by Gleb.Oborin on 11/4/21.
//

#include "PathPlanningWithConstraints.h"

#include <map>
#include <set>

typedef std::pair<int, int> state;

AgentResult PathPlanningWithConstraints::computePath(Map &g, Agent &a, std::set<state> &constraints) {
    int start = g.cellToInt(a.start);
    int end = g.cellToInt(a.end);
    //std::cerr << a.start.first << ' ' << a.start.second << ' ' << a.end.first << ' ' << a.end.second << '\n';
    std::set<std::pair<int, state>> s;
    state startState = state(0, start);
    s.insert({0 + g.h(start, end), startState});
    std::map<state, double> dist;
    std::map<state, state> p;
    dist[startState] = 0;
    bool foundPath = false;
    state endState;
    while (!s.empty()) {
        auto vx = (*s.begin());
        s.erase(s.begin());
        state vState = vx.second;
        int time = vx.second.first;
        int v = vx.second.second;
        if (v == end) {
            foundPath = true;
            endState = vState;
            break;
        }
        for (auto &to : g.g[v].neighbors) {
            state toState = {time + 1, to};
            if (constraints.count(toState) > 0) continue;
            if (dist.count(toState) == 0 || dist[toState] > dist[vState] + 1) {
                s.erase({ dist[toState] + g.h(v, to), toState } );
                dist[toState] = dist[vState] + 1;
                p[toState] = vState;
                s.insert({ dist[toState] + g.h(v, to), toState});
            }
        }
    }
    if (!foundPath) {
        return AgentResult(a.answer);
    }
    std::vector<Cell> path;
    state cur = endState;
    while (cur != startState) {
        path.push_back(g.intToCell(cur.second));
        cur = p[cur];
    }
    path.push_back(g.intToCell(start));
    std::reverse(path.begin(), path.end());
    return AgentResult(path, endState.first, a.answer);
}