#include "../Conflicts.h"
#include "../Constraints.h"
#include <map>
#include <unordered_map>
#include <vector>
#include "../../../scen/Scenario.h"
#include "../../../scen/ScenarioResult.h"
#include "../MAPFParam.h"
#include "CBS.h"
#include "CGHeuristic.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

std::mt19937 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

ScenarioResult CBS::run() {
    std::vector<AgentResult> res(scenario.agents.size());

    treeVertexConstraints.push_back(std::vector<VertexConstraints>(scenario.agents.size()));
    treeEdgeConstraints.push_back(std::vector<EdgeConstraints>(scenario.agents.size()));
    treeLandmarks.push_back(std::vector<std::set<VertexState>>(scenario.agents.size()));
    treeAllLandmarks.push_back(std::unordered_map<VertexState, int, hash_VertexState>());
    treeTimeEndBlocked.push_back(std::vector<std::set<int>>(scenario.agents.size()));

    for (int i = 0; i < scenario.agents.size(); ++i) {
        treeLandmarks[0][i].insert(std::make_pair(0, map.cellToInt(scenario.agents[i].start)));
        treeAllLandmarks[0][VertexState(0, map.cellToInt(scenario.agents[i].start))] = i;
    }

    auto time_begin = std::chrono::steady_clock::now();
    std::vector<int> emptyVector = {};
    treeSolutions.push_back(lowLevelSearch(0, emptyVector));
    parent.push_back(-1);
    if (!treeSolutions[0].isCorrect) {
        auto time_end = std::chrono::steady_clock::now();
        auto runtime = std::chrono::duration<double>(time_end - time_begin).count();
        treeSolutions[0].CTNodes = treeSolutions.size();
        treeSolutions[0].runtime = runtime;
        return treeSolutions[0];
    }

    open.insert(TreeNode(0, treeSolutions[0].answer, countHeuristic(treeSolutions[0])));
    while (!open.empty()) {
        auto time_end = std::chrono::steady_clock::now();
        auto runtime = std::chrono::duration<double>(time_end - time_begin).count();
        if (runtime > timeLimit) {
            return ScenarioResult();
        }
        auto vx = (*open.begin());
        open.erase(open.begin());
        int node = vx.v;
        /*
        std::cerr << "NODE " << node << '\n';
        std::cerr << "F = " << vx.f << '\n';
        std::cerr << "ALL LANDMARKS:\n";
        for (auto &x : treeAllLandmarks[node]) {
            Cell c = map.intToCell(x.first.second);
            std::cerr << x.first.first << " (" << c.first << ", "  << c.second << ") " << x.second << '\n';
        }
        for (int i = 0; i < treeLandmarks[node].size(); ++i) {
            std::cerr << "FOR " << i << '\n';
            for (auto &y: treeLandmarks[node][i]) {
                Cell c = map.intToCell(y.second);
                std::cerr << y.first << " (" << c.first << ", "  << c.second << ") "<< '\n';
            }
        }
        std::cerr << "Vertex constrains:\n";
        for (int i = 0; i < treeVertexConstraints[node].size(); ++i) {
            std::cerr << "FOR " << i << '\n';
            for (auto &y: treeVertexConstraints[node][i]) {
                Cell c = map.intToCell(y.second);
                std::cerr << y.first << " (" << c.first << ", "  << c.second << ") " << '\n';
            }
        }
        std::cerr << "Edge constrains:\n";
        for (int i = 0; i < treeEdgeConstraints[node].size(); ++i) {
            std::cerr << "FOR " << i << '\n';
            for (auto &y: treeEdgeConstraints[node][i]) {
                Cell c1 = map.intToCell(std::get<1>(y));
                Cell c2 = map.intToCell(std::get<2>(y));
                std::cerr << std::get<0>(y) << " (" << c1.first << ", "  << c1.second << ") " << " (" << c2.first << ", "  << c2.second << ") " << '\n';
            }
        }
        std::cerr << "\n\n";
         */

        auto conflicts = searchConflicts(treeSolutions[node]);

        //std::cerr << "SEARCH CONFLICTS DONE\n";

        if (conflicts.first.empty() && conflicts.second.empty()) {

            auto time_end = std::chrono::steady_clock::now();
            auto runtime = std::chrono::duration<double>(time_end - time_begin).count();
            treeSolutions[node].CTNodes = treeSolutions.size();
            treeSolutions[node].runtime = runtime;
            return treeSolutions[node];
        }

        Conflicts conflict = chooseConflict(conflicts, treeSolutions[node]);
        //std::cerr << "CHOOSE CONFLICTS DONE\n";

        solveConflicts(node, conflict);
        //std::cerr << "SOLVE CONFLICTS DONE\n";
    }
    return ScenarioResult();
}

bool CBS::isCardinalConflict(const VertexConflict &conflict, ScenarioResult &scenarioResult) {
    bool f1 = (scenarioResult.agents[conflict.a1].bdd.vertexLevelCounter[conflict.time] <= 1);
    bool f2 = (scenarioResult.agents[conflict.a2].bdd.vertexLevelCounter[conflict.time] <= 1);
    return f1 && f2;
}

bool CBS::isCardinalConflict(const EdgeConflict &conflict, ScenarioResult &scenarioResult) {
    bool f1 = (scenarioResult.agents[conflict.a1].bdd.edgesLevelCounter[conflict.time] <= 1);
    bool f2 = (scenarioResult.agents[conflict.a2].bdd.edgesLevelCounter[conflict.time] <= 1);
    return f1 && f2;
}

bool CBS::isSemiCardinalConflict(const VertexConflict &conflict, ScenarioResult &scenarioResult) {
    bool f1 = (scenarioResult.agents[conflict.a1].bdd.vertexLevelCounter[conflict.time] <= 1);
    bool f2 = (scenarioResult.agents[conflict.a2].bdd.vertexLevelCounter[conflict.time] <= 1);
    return f1 || f2;
}

bool CBS::isSemiCardinalConflict(const EdgeConflict &conflict, ScenarioResult &scenarioResult) {
    bool f1 = (scenarioResult.agents[conflict.a1].bdd.edgesLevelCounter[conflict.time] <= 1);
    bool f2 = (scenarioResult.agents[conflict.a2].bdd.edgesLevelCounter[conflict.time] <= 1);
    return f1 || f2;
}

Conflicts CBS::searchConflicts(ScenarioResult &scenarioResult) {
    int maxt = 0;
    for (auto &agent: scenarioResult.agents) {
        maxt = std::max(maxt, int(agent.path.size()));
    }
    std::vector<VertexConflict> vertexConflicts;
    std::vector<EdgeConflict> edgeConflicts;

    for (int t = 0; t < maxt; ++t) {
        std::unordered_map<int, std::vector<int>> positionsV;
        for (int i = 0; i < scenarioResult.agents.size(); ++i) {
            AgentResult agent = scenarioResult.agents[i];
            int pos;
            if (t < agent.path.size()) {
                pos = map.cellToInt(agent.path[t]);
            } else {
                pos = map.cellToInt(agent.path.back());
            }
            if (param->ICBSOptimization == ICBS_OPTIMIZATIONS::NONE) { // if no ICBSOptimization -> return first conflict
                if (!positionsV[pos].empty()) {
                    VertexConflict conflict = VertexConflict(positionsV[pos][0], i, pos, t);
                    return std::make_pair(std::vector<VertexConflict>(1, conflict), std::vector<EdgeConflict>());
                }
            }
            if (param->ICBSOptimization == ICBS_OPTIMIZATIONS::PC && param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::FIRST) { // if PC ICBSOptimization -> return first cardinal conflict
                if (!positionsV[pos].empty()) {
                    VertexConflict conflict = VertexConflict(positionsV[pos][0], i, pos, t);
                    if (isCardinalConflict(conflict, scenarioResult)) {
                        return std::make_pair(std::vector<VertexConflict>(1, conflict), std::vector<EdgeConflict>());
                    }
                }
            }
            positionsV[pos].push_back(i);
        }

        std::unordered_map<std::pair<int, int>, std::vector<int>, hash_VertexState> positionsE;
        for (int i = 0; i < scenarioResult.agents.size(); ++i) {
            AgentResult agent = scenarioResult.agents[i];
            if (t + 1 >= agent.path.size()) continue;
            int v = map.cellToInt(agent.path[t]);
            int to = map.cellToInt(agent.path[t + 1]);
            if (v == to) continue;
            if (param->ICBSOptimization == ICBS_OPTIMIZATIONS::NONE) { // if no ICBSOptimization -> return first conflict
                if (positionsE.count({to, v}) > 0) {
                    EdgeConflict conflict = EdgeConflict(i, positionsE[{to, v}][0], v, to, t);
                    return std::make_pair(std::vector<VertexConflict>(), std::vector<EdgeConflict>(1, conflict));
                }
            }
            if (param->ICBSOptimization == ICBS_OPTIMIZATIONS::PC && param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::FIRST) { // if PC ICBSOptimization -> return first cardinal conflict
                if (positionsE.count({to, v}) > 0) {
                    EdgeConflict conflict = EdgeConflict(i, positionsE[{to, v}][0], v, to, t);
                    if (isCardinalConflict(conflict, scenarioResult)) {
                        return std::make_pair(std::vector<VertexConflict>(), std::vector<EdgeConflict>(1, conflict));
                    }
                }
            }
            positionsE[{v, to}].push_back(i);
        }

        for (auto &x : positionsV) {
            for (int p1 = 0; p1 < x.second.size(); ++p1) {
                for (int p2 = p1 + 1; p2 < x.second.size(); ++p2) {
                    auto v1 = x.second[p1];
                    auto v2 = x.second[p2];
                    VertexConflict conflict = VertexConflict(v1, v2, x.first, t);
                    vertexConflicts.push_back(conflict);
                }
            }
        }

        std::unordered_set<std::pair<int, int>, hash_VertexState> used;
        for (auto &x : positionsE) {
            used.insert(x.first);
            std::pair<int, int> y = std::make_pair(x.first.second, x.first.first);
            if (used.count(y) > 0) continue;
            for (int v1 : x.second) {
                for (int v2 : positionsE[y]) {
                    EdgeConflict conflict = EdgeConflict(v1, v2, x.first.first, x.first.second, t);
                    edgeConflicts.push_back(conflict);
                }
            }
        }
    }
    return std::make_pair(vertexConflicts, edgeConflicts);
}

std::tuple<Conflicts, Conflicts, Conflicts> CBS::prioritizeConflicts(ScenarioResult &scenarioResult, Conflicts &conflicts) {
    Conflicts cardinal, semiCardinal, otherwise;
    for (VertexConflict &vertexConflict : conflicts.first) {
        if (isCardinalConflict(vertexConflict, scenarioResult)) {
            cardinal.first.push_back(vertexConflict);
            continue;
        }
        if (isSemiCardinalConflict(vertexConflict, scenarioResult)) {
            semiCardinal.first.push_back(vertexConflict);
            continue;
        }
        otherwise.first.push_back(vertexConflict);
    }
    for (EdgeConflict &edgeConflict : conflicts.second) {
        if (isCardinalConflict(edgeConflict, scenarioResult)) {
            cardinal.second.push_back(edgeConflict);
            continue;
        }
        if (isSemiCardinalConflict(edgeConflict, scenarioResult)) {
            semiCardinal.second.push_back(edgeConflict);
            continue;
        }
        otherwise.second.push_back(edgeConflict);
    }
    return { cardinal, semiCardinal, otherwise };
}

Conflicts CBS::chooseConflictWithSamePriority(Conflicts &conflicts, ScenarioResult &scenarioResult, int type) {
    //std::cerr << "TYPE = " << type << '\n';
    if (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::FIRST) {
        if (!conflicts.first.empty())
            return std::make_pair(std::vector<VertexConflict>(1, conflicts.first.front()),
                                  std::vector<EdgeConflict>(0));
        else
            return std::make_pair(std::vector<VertexConflict>(0),
                                  std::vector<EdgeConflict>(1, conflicts.second.front()));
    }
    if (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::RANDOM) {
        int pos = rnd() % (conflicts.first.size() + conflicts.second.size());
        if (pos < conflicts.first.size()) {
            return std::make_pair(std::vector<VertexConflict>(1, conflicts.first[pos]),
                                  std::vector<EdgeConflict>(0));
        } else {
            pos -= conflicts.first.size();
            return std::make_pair(std::vector<VertexConflict>(0),
                                  std::vector<EdgeConflict>(1, conflicts.second[pos]));
        }
    }
    if (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::WIDTH) {
        if (type != 2) {
            int pos = rnd() % (conflicts.first.size() + conflicts.second.size());
            if (pos < conflicts.first.size()) {
                return std::make_pair(std::vector<VertexConflict>(1, conflicts.first[pos]),
                                      std::vector<EdgeConflict>(0));
            } else {
                pos -= conflicts.first.size();
                return std::make_pair(std::vector<VertexConflict>(0),
                                      std::vector<EdgeConflict>(1, conflicts.second[pos]));
            }
        }
        VertexConflict minVertexConflict;
        EdgeConflict minEdgeConflict;

        int minVD = -1, minED = -1;

        //std::cerr << "VERTEX CONFLICTS\n";

        for (VertexConflict &conflict: conflicts.first) {
            int n = std::min(
                    (int) scenarioResult.agents[conflict.a1].bdd.vertexLevelCounter[conflict.time],
                    (int) scenarioResult.agents[conflict.a2].bdd.vertexLevelCounter[conflict.time]
            );
            //std::cerr << n << ' ';
            if (minVD == -1 || (n < minVD)) {
                minVertexConflict = conflict;
                minVD = n;
            }
        }
        //std::cerr << "\nEDGE CONFLICTS\n";
        for (EdgeConflict &conflict: conflicts.second) {
            int n = std::min(
                    (int) scenarioResult.agents[conflict.a1].bdd.edgesLevelCounter[conflict.time],
                    (int) scenarioResult.agents[conflict.a2].bdd.edgesLevelCounter[conflict.time]
            );
            //std::cerr << n << ' ';
            if (minED == -1 || (n < minED)) {
                minEdgeConflict = conflict;
                minED = n;
            }
        }
        //std::cerr << '\n';

        if (minED == -1) {
            return std::make_pair(std::vector<VertexConflict>(1, minVertexConflict),
                                  std::vector<EdgeConflict>(0));
        }
        if (minVD == -1) {
            return std::make_pair(std::vector<VertexConflict>(0),
                                  std::vector<EdgeConflict>(1, minEdgeConflict));
        }
        if (minVD <= minED) {
            return std::make_pair(std::vector<VertexConflict>(1, minVertexConflict),
                                  std::vector<EdgeConflict>(0));
        } else {
            return std::make_pair(std::vector<VertexConflict>(0),
                                  std::vector<EdgeConflict>(1, minEdgeConflict));
        }
    }
}

Conflicts CBS::chooseConflict(Conflicts &conflicts, ScenarioResult &scenarioResult) {
    if (param->ICBSOptimization == ICBS_OPTIMIZATIONS::NONE) {
        return chooseConflictWithSamePriority(conflicts, scenarioResult);
    }
    auto typesConflict = prioritizeConflicts(scenarioResult, conflicts);
    if (!std::get<0>(typesConflict).first.empty() || !std::get<0>(typesConflict).second.empty()) {
        return chooseConflictWithSamePriority(std::get<0>(typesConflict), scenarioResult, 0);
    }
    if (!std::get<1>(typesConflict).first.empty() || !std::get<1>(typesConflict).second.empty()) {
        return chooseConflictWithSamePriority(std::get<1>(typesConflict), scenarioResult, 1);
    }
    return chooseConflictWithSamePriority(std::get<2>(typesConflict), scenarioResult, 2);
}

void CBS::solveConflict(int node, VertexConflict &conflict) {
    std::vector<int> vertexes = { conflict.a1, conflict.a2 };
    for (auto &v: vertexes) {
        int currNode = treeVertexConstraints.size();
        treeVertexConstraints.push_back(treeVertexConstraints[node]);
        treeEdgeConstraints.push_back(treeEdgeConstraints[node]);
        treeLandmarks.push_back(treeLandmarks[node]);
        treeAllLandmarks.push_back(treeAllLandmarks[node]);
        treeTimeEndBlocked.push_back(treeTimeEndBlocked[node]);
        treeVertexConstraints[currNode][v].insert({conflict.time, conflict.v});

        if (conflict.v == map.cellToInt(scenario.agents[v].end)) {
            treeTimeEndBlocked[currNode][v].insert(conflict.time);
        }

        parent.push_back(node);
        std::vector<int> agents = { v };
        treeSolutions.push_back(lowLevelSearch(currNode, agents));
        if (treeSolutions[currNode].isCorrect) {
            open.insert(TreeNode(currNode, treeSolutions[currNode].answer, countHeuristic(treeSolutions[currNode]), node));
        }
    }
}

void CBS::solveConflict(int node, EdgeConflict &conflict) {
    std::vector<std::pair<int, bool>> edges = {std::make_pair(conflict.a1, 0), std::make_pair(conflict.a2, 1)};
    for (auto &v: edges) {
        int currNode = treeVertexConstraints.size();
        treeVertexConstraints.push_back(treeVertexConstraints[node]);
        treeEdgeConstraints.push_back(treeEdgeConstraints[node]);
        treeLandmarks.push_back(treeLandmarks[node]);
        treeAllLandmarks.push_back(treeAllLandmarks[node]);
        treeTimeEndBlocked.push_back(treeTimeEndBlocked[node]);
        EdgeState newConstraint;
        if (!v.second) {
            newConstraint = {conflict.time, conflict.v, conflict.to};
        } else {
            newConstraint = {conflict.time, conflict.to, conflict.v};
        }
        treeEdgeConstraints[currNode][v.first].insert(newConstraint);
        parent.push_back(node);
        std::vector<int> agents = { v.first };
        treeSolutions.push_back(lowLevelSearch(currNode, agents));
        if (treeSolutions[currNode].isCorrect) {
            open.insert(TreeNode(currNode, treeSolutions[currNode].answer, countHeuristic(treeSolutions[currNode]), node));
        }
    }
}

int CBS::getAgentPosAtTime(AgentResult &agentResult, int time) {
    if (agentResult.path.size() <= time) return map.cellToInt(agentResult.path.back());
    return map.cellToInt(agentResult.path[time]);
}

void CBS::solveConflictDC(int node, VertexConflict &conflict) {
    std::vector<int> vertexes = {conflict.a1, conflict.a2};
    int v;

    if (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::RANDOM ||
        param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::FIRST) {
        if (rnd() % 2) {
            v = conflict.a1;
        } else {
            v = conflict.a2;
        }
    }
    if (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::WIDTH) {
        //std::cerr << "TIME = " << conflict.time << '\n';
        //std::cerr << "SUKASUKASUKA" << (int) treeSolutions[node].agents[conflict.a1].bdd.vertexLevelCounter[conflict.time] << ' ' << (int) treeSolutions[node].agents[conflict.a2].bdd.vertexLevelCounter[conflict.time] << '\n';

        int d1 = treeSolutions[node].agents[conflict.a1].bdd.vertexLevelCounter[conflict.time];
        int d2 = treeSolutions[node].agents[conflict.a2].bdd.vertexLevelCounter[conflict.time];
        if (d1 <= d2) {
            v = conflict.a1;
        } else {
            v = conflict.a2;
        }
    }

    //std::cerr << "V = " <<v << '\n';

    int currNode;
    currNode = treeVertexConstraints.size();
    treeVertexConstraints.push_back(treeVertexConstraints[node]);
    treeEdgeConstraints.push_back(treeEdgeConstraints[node]);
    treeLandmarks.push_back(treeLandmarks[node]);
    treeAllLandmarks.push_back(treeAllLandmarks[node]);
    treeTimeEndBlocked.push_back(treeTimeEndBlocked[node]);
    treeVertexConstraints[currNode][v].insert({conflict.time, conflict.v});

    if (conflict.v == map.cellToInt(scenario.agents[v].end)) {
        treeTimeEndBlocked[currNode][v].insert(conflict.time);
    }

    parent.push_back(node);
    std::vector<int> tmpagents = { v };
    treeSolutions.push_back(lowLevelSearch(currNode, tmpagents));
    if (treeSolutions[currNode].isCorrect) {
        open.insert(TreeNode(currNode, treeSolutions[currNode].answer, countHeuristic(treeSolutions[currNode]), node));
    }


    if (treeAllLandmarks[node].count({conflict.time, conflict.v}) > 0) return;
    if (treeVertexConstraints[node][v].count({conflict.time, conflict.v}) > 0) return;
    currNode = treeVertexConstraints.size();
    treeVertexConstraints.push_back(treeVertexConstraints[node]);
    treeEdgeConstraints.push_back(treeEdgeConstraints[node]);
    treeLandmarks.push_back(treeLandmarks[node]);
    treeTimeEndBlocked.push_back(treeTimeEndBlocked[node]);
    treeLandmarks[currNode][v].insert({conflict.time, conflict.v});
    treeAllLandmarks.push_back(treeAllLandmarks[node]);
    treeAllLandmarks[currNode][{conflict.time, conflict.v}] = v;
    parent.push_back(node);

    std::vector<int> agents(0);

    for (int agentNum = 0; agentNum < treeSolutions[node].agents.size(); ++agentNum) {
        if (agentNum == v) continue;
        treeVertexConstraints[currNode][agentNum].insert({conflict.time, conflict.v});
        int agentPosAtTime = getAgentPosAtTime(treeSolutions[node].agents[agentNum], conflict.time);
        if (agentPosAtTime == conflict.v) {
            if (agentPosAtTime == map.cellToInt(scenario.agents[agentNum].end)) {
                treeTimeEndBlocked[currNode][agentNum].insert(conflict.time);
            }
            agents.push_back(agentNum);
        }
    }

    treeSolutions.push_back(lowLevelSearch(currNode, agents));
    if (treeSolutions[currNode].isCorrect) {
        open.insert(
                TreeNode(currNode, treeSolutions[currNode].answer, countHeuristic(treeSolutions[currNode]), node));
    }
}

void CBS::solveConflictDC(int node, EdgeConflict &conflict) {
    int v;
    bool revflag;
    /*
     * std::cerr << "EDGE CONFLICT!\n";
    Cell c1 = map.intToCell(conflict.v);
    Cell c2 = map.intToCell(conflict.to);
    std::cerr << conflict.time << ' ' << conflict.a1 << ' ' << conflict.a2 << " (" << c1.first << ", "  << c1.second << ") " << " (" << c2.first << ", "  << c2.second << ") " << '\n';
    */
    if (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::RANDOM || param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::FIRST) {
        if (rnd() % 2) {
            v = conflict.a1;
            revflag = false;
        } else {
            v = conflict.a2;
            revflag = true;
        }
    }
    if (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::WIDTH) {
        int d1 = treeSolutions[node].agents[conflict.a1].bdd.edgesLevelCounter[conflict.time];
        int d2 = treeSolutions[node].agents[conflict.a2].bdd.edgesLevelCounter[conflict.time];
        if (d1 <= d2) {
            v = conflict.a1;
            revflag = false;
        } else {
            v = conflict.a2;
            revflag = true;
        }
    }

    int currNode;
    currNode = treeVertexConstraints.size();
    treeVertexConstraints.push_back(treeVertexConstraints[node]);
    treeEdgeConstraints.push_back(treeEdgeConstraints[node]);
    treeLandmarks.push_back(treeLandmarks[node]);
    treeAllLandmarks.push_back(treeAllLandmarks[node]);
    treeTimeEndBlocked.push_back(treeTimeEndBlocked[node]);
    EdgeState newConstraint;
    if (!revflag) {
        newConstraint = {conflict.time, conflict.v, conflict.to};
    } else {
        newConstraint = {conflict.time, conflict.to, conflict.v};
    }
    treeEdgeConstraints[currNode][v].insert(newConstraint);
    parent.push_back(node);
    std::vector<int> tmpagents = { v };
    treeSolutions.push_back(lowLevelSearch(currNode, tmpagents));
    if (treeSolutions[currNode].isCorrect) {
        open.insert(TreeNode(currNode, treeSolutions[currNode].answer, countHeuristic(treeSolutions[currNode]), node));
    }

    int v1, v2;
    if (!revflag) {
        v1 = conflict.v;
        v2 = conflict.to;
    } else {
        v1 = conflict.to;
        v2 = conflict.v;
    }

    if (treeAllLandmarks[node].count({conflict.time, v1}) > 0) return;
    if (treeAllLandmarks[node].count({conflict.time + 1, v2}) > 0) return;
    if (treeVertexConstraints[node][v].count({conflict.time, v1}) > 0) return;
    if (treeVertexConstraints[node][v].count({conflict.time + 1, v2}) > 0) return;

    currNode = treeVertexConstraints.size();
    treeVertexConstraints.push_back(treeVertexConstraints[node]);
    treeEdgeConstraints.push_back(treeEdgeConstraints[node]);
    treeLandmarks.push_back(treeLandmarks[node]);
    treeTimeEndBlocked.push_back(treeTimeEndBlocked[node]);

    treeAllLandmarks.push_back(treeAllLandmarks[node]);
    treeAllLandmarks[currNode][{conflict.time, v1}] = v;
    treeAllLandmarks[currNode][{conflict.time + 1, v2}] = v;
    treeLandmarks[currNode][v].insert({conflict.time, v1});
    treeLandmarks[currNode][v].insert({conflict.time + 1, v2});
    parent.push_back(node);

    std::vector<int> agents(0);

    for (int agentNum = 0; agentNum < treeSolutions[node].agents.size(); ++agentNum) {
        if (agentNum == v) continue;
        treeVertexConstraints[currNode][agentNum].insert({conflict.time, v1});
        treeVertexConstraints[currNode][agentNum].insert({conflict.time + 1, v2});
        int agentPosAtTimeV1 = getAgentPosAtTime(treeSolutions[node].agents[agentNum], conflict.time);
        int agentPosAtTimeV2 = getAgentPosAtTime(treeSolutions[node].agents[agentNum], conflict.time + 1);
        if (agentPosAtTimeV1 == v1 || agentPosAtTimeV2 == v2) agents.push_back(agentNum);
    }

    treeSolutions.push_back(lowLevelSearch(currNode, agents));
    if (treeSolutions[currNode].isCorrect) {
        open.insert(TreeNode(currNode, treeSolutions[currNode].answer, countHeuristic(treeSolutions[currNode]), node));
    }
}

void CBS::solveConflicts(
        int node,
        Conflicts &conflicts
) {
    /*
    for (auto &x : conflicts.first) {
        std::cerr << "VERTEX CONFLICT\n";
        std::cerr << x.time << " (" << map.intToCell(x.v).first << ", " << map.intToCell(x.v).second << ")\n";
    }
    for (auto &x : conflicts.second) {
        std::cerr << "EDGE CONFLICT\n";
    }
     */
    for (VertexConflict &conflict: conflicts.first) {
        if (param->withDS) {
            solveConflictDC(node, conflict);
        } else {
            solveConflict(node, conflict);
        }
        return;
    }
    for (EdgeConflict &conflict: conflicts.second) {
        if (param->withDS) {
            solveConflictDC(node, conflict);
        } else {
            solveConflict(node, conflict);
        }
        return;
    }
}

ScenarioResult CBS::lowLevelSearch(
        int node,
        std::vector<int> &constraintAgentNum
) {
    //std::cerr << "LOW LEVEL SEARCH BEGIN\n";
    std::vector<AgentResult> res(scenario.agents.size());
    if (constraintAgentNum.empty()) {             // search for all agents
        for (auto &x: scenario.agents) {
            int agentNum = x.i;
            AgentResult agentResult = computePath(
                    scenario.agents[agentNum],
                    treeVertexConstraints[node][agentNum],
                    treeEdgeConstraints[node][agentNum],
                    treeLandmarks[node][agentNum],
                    treeAllLandmarks[node],
                    treeTimeEndBlocked[node][agentNum]
            );
            if (!agentResult.isFound) {
                return ScenarioResult();
            }
            /*
            std::cerr << agentNum << '\n';
            for (auto &x : agentResult.path) {
                std::cerr << x.first << ' '  << x.second << '\n';
            }
            std::cerr << '\n';
             */
            res[agentNum] = agentResult;
        }
    } else {                                    // search for changed agent
        res = treeSolutions[parent[node]].agents;
        for (auto agentNum: constraintAgentNum) {
            //std::cerr << agentNum << '\n';
            AgentResult agentResult = computePath(
                    scenario.agents[agentNum],
                    treeVertexConstraints[node][agentNum],
                    treeEdgeConstraints[node][agentNum],
                    treeLandmarks[node][agentNum],
                    treeAllLandmarks[node],
                    treeTimeEndBlocked[node][agentNum]
            );
            if (!agentResult.isFound) {
                return ScenarioResult();
            }
            res[agentNum] = agentResult;
        }
        /*
        for (auto &x : agentResult.path) {
            std::cerr << x.first << ' ' <<x.second << '\n';
        }
        std::cerr << '\n';
         */
    }
    //std::cerr << "LOW LEVEL SEARCH END\n";
    return ScenarioResult(res);
}

int CBS::computePathBetweenLandmarks(
        int agentNum,
        VertexConstraints &vertexConstraints,
        EdgeConstraints &edgeConstraints,
        std::unordered_map<VertexState, VertexInfo, hash_VertexState> &dist,
        std::unordered_map<VertexState, VertexState, hash_VertexState> &p,
        std::unordered_map<VertexState, int, hash_VertexState> &blocked,
        std::set<int> &timeEndIsBlocked,
        VertexState startState,
        VertexState endState,
        int endH
) {
    std::set<std::pair<VertexInfo, VertexState>> s;
    s.insert({dist[startState], startState});
    int timer = -1;
    while (!s.empty()) {
        auto vx = (*s.begin());
        s.erase(s.begin());
        VertexState vState = vx.second;
        int time = vx.second.first;
        if (endState.first != -1 && time > endState.first) {
            break;
        }
        int v = vx.second.second;
        if (v == endState.second) {
            if (endState.first == -1) {
                auto x = timeEndIsBlocked.upper_bound(time);
                if (x == timeEndIsBlocked.end()) {
                    timer = time;
                    break;
                }
            } else {
                if (time == endState.first) {
                    timer = time;
                    break;
                }
            }
        }
        for (auto &to : map.g[v].neighbors) {
            VertexState toState = {time + 1, to};
            int cost = 1;
            if (to == v && v == endH) cost = 0;
            if (vertexConstraints.count(toState) > 0) continue;
            if (edgeConstraints.count({time, v, to}) > 0) continue;
            if (blocked.count(toState) > 0 && blocked[toState] != agentNum) continue;
            if (blocked.count({time, to}) > 0 && blocked[{time, to}] != agentNum && blocked.count({time + 1, v}) > 0 && blocked[{time + 1, v}] != agentNum) continue;
            if (dist.count(toState) == 0 || dist[toState].g > dist[vState].g + cost) {
                VertexInfo nInfo = VertexInfo(dist[vState].g + cost, map.h(to, endH));
                s.erase({ dist[toState], toState } );
                dist[toState] = nInfo;
                p[toState] = vState;
                s.insert({ dist[toState], toState});
            }
        }
    }
    return timer;
}

AgentResult CBS::computePath(
        Agent &a,
        VertexConstraints &vertexConstraints,
        EdgeConstraints &edgeConstraints,
        std::set<VertexState> &landmarksSet,
        std::unordered_map<VertexState, int, hash_VertexState> &blocked,
        std::set<int> &timeEndIsBlocked
) {
    int start = map.cellToInt(a.start);
    int end = map.cellToInt(a.end);

    /*
    std::cerr << "LANDMARKS:\n";
    for (auto x : landmarksSet) {
        std::cerr << x.first << " (" << map.intToCell(x.second).first << ", "<< map.intToCell(x.second).second << ")\n";
    }
    std::cerr << '\n';
     */

    std::unordered_map<VertexState, VertexInfo, hash_VertexState> dist;
    std::unordered_map<VertexState, VertexState, hash_VertexState> p;
    VertexState startState = VertexState(0, start);
    dist[startState] = VertexInfo(0, map.h(start, end));

    std::vector<VertexState> landmarks { landmarksSet.begin(), landmarksSet.end() };
    int timer;
    for (int i = 0; i < landmarks.size(); ++i) {
        //std::cerr << "I = " << i << "\n";
        if (i + 1 < landmarks.size()) {
            //std::cerr << i << ' ' << landmarks[i].first << ' ' << map.intToCell(landmarks[i].second).first << ' ' << map.intToCell(landmarks[i].second).second << "\n";
            //std::cerr << i << ' ' << landmarks[i + 1].first << ' ' << map.intToCell(landmarks[i + 1].second).first << ' ' << map.intToCell(landmarks[i + 1].second).second << "\n";
            timer = computePathBetweenLandmarks(
                    a.i,
                    vertexConstraints,
                    edgeConstraints,
                    dist,
                    p,
                    blocked,
                    timeEndIsBlocked,
                    landmarks[i],
                    landmarks[i + 1],
                    end);
        } else {
            //std::cerr << i << ' ' << landmarks[i].first << ' ' << map.intToCell(landmarks[i].second).first << ' ' << map.intToCell(landmarks[i].second).second << "\n";
            //std::cerr << i << ' ' << -1 << ' ' << map.intToCell(end).first << ' ' << map.intToCell(end).second << "\n";
            timer = computePathBetweenLandmarks(
                    a.i,
                    vertexConstraints,
                    edgeConstraints,
                    dist,
                    p,
                    blocked,
                    timeEndIsBlocked,
                    landmarks[i],
                    VertexState(-1, end),
                    end);
        }
        if (timer == -1) {
            //std:: cerr << i << '\n';
            return AgentResult();
        }
    }
    //std::cerr << "found!\n";
    VertexState endState = VertexState(timer, end);
    int answer = dist[endState].g;
    std::vector<Cell> path;
    VertexState cur = endState;
    while (cur != startState) {
        path.push_back(map.intToCell(cur.second));
        cur = p[cur];
    }
    path.push_back(map.intToCell(start));
    std::reverse(path.begin(), path.end());
    AgentResult res = AgentResult(path, answer);
    //std::cerr << "KEKAS\n";
    if ((param->ICBSOptimization != ICBS_OPTIMIZATIONS::NONE) || (param->chooseConflictStrategy == CHOOSE_CONFLICT_STRATEGY::WIDTH)) {
        MDD mdd;
        std::unordered_set<VertexState, hash_VertexState> used;
        build_mdd(mdd, startState, endState, dist, p, used, end);
        res.bdd = mdd;
        /*
        std::cerr << "MDD\n";
        for (auto x : res.bdd.vertexLevelCounter) {
            std::cerr << x.first << ' ' << x.second << '\n';
        }
        std::cerr << '\n';
         */
    }
    //std::cerr << '\n';
    //std::cerr << "MEMAS\n";
    return res;
}

void CBS::build_mdd(
        MDD &mdd,
        VertexState startState,
        VertexState vState,
        std::unordered_map<VertexState, VertexInfo, hash_VertexState> &dist,
        std::unordered_map<VertexState, VertexState, hash_VertexState> &p,
        std::unordered_set<VertexState, hash_VertexState> &used,
        int end
) {
    if (used.count(vState) > 0) return;
    used.insert(vState);
    //std::cerr << vState.first << " (" << map.intToCell(vState.second).first << ", " << map.intToCell(vState.second).second << ")" << '\n';
    mdd.vertexLevelCounter[vState.first]++;
    if (vState == startState) {
        return;
    }
    for (auto &to : map.g[vState.second].neighbors) {
        int cost = 1;
        if (vState.second == end && to == vState.second) cost = 0;
        VertexState toState = { vState.first - 1, to };
        if (dist.count(toState) == 0) continue;
        if (dist[toState].g > dist[vState].g + cost) continue;
        mdd.edgesLevelCounter[toState.first]++;
        build_mdd(mdd, startState, toState, dist, p, used, end);
    }
}

int CBS::countHeuristic(ScenarioResult &scenarioResult) {
    if (param->ICBSOptimization != ICBS_OPTIMIZATIONS::H) return 0;
    Conflicts conflicts = searchConflicts(scenarioResult);
    Conflicts cardinal = std::get<0>(prioritizeConflicts(scenarioResult, conflicts));
    if (cardinal.first.empty() && cardinal.second.empty()) return 0;
    if (cardinal.first.size() + cardinal.second.size() == 1) return 1;
    return CGHeuristic().count(cardinal);
}