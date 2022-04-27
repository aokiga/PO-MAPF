#include "MPPA_Star.h"
#include <set>
#include <iostream>
#include <map>
#include <random>
#include <chrono>

AgentResult MPPA_Star::computePathForAgent(
        Agent &a,
        VertexConstraints &vertexConstraints,
        EdgeConstraints &edgeConstraints,
        Cell startCell
        //std::unordered_map<int, int> &blockedV
        ) {
    int ai = a.i;
    int start = map[ai].cellToInt(startCell);
    int end = map[ai].cellToInt(a.end);
    //std::cerr << startCell << ' ' << a.end << '\n';
    std::set<std::pair<VertexInfo, VertexState>> s;
    VertexState startState = VertexState(0, start);
    s.insert({VertexInfo(0, map[ai].h(start, end)), startState});
    std::unordered_map<VertexState, VertexInfo, hash_VertexState> dist;
    std::unordered_map<VertexState, VertexState, hash_VertexState> p;
    dist[startState] = VertexInfo(0, map[ai].h(start, end));
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
        for (auto &to : map[ai].get_neighbors(v)) {

            VertexState toState = {time + 1, to};
            if (vertexConstraints.count(toState) > 0) continue;
            if (edgeConstraints.count({time, v, to}) > 0) continue;
            //if (blockedV.count(v) > 0 && time >= blockedV[v]) continue;
            if (dist.count(toState) == 0 || dist[toState].g > dist[vState].g + 1) {
                VertexInfo nInfo = VertexInfo(dist[vState].g + 1, map[ai].h(v, to));
                s.erase({ dist[toState], toState } );
                dist[toState] = nInfo;
                p[toState] = vState;
                s.insert({ dist[toState], toState});
            }
        }
    }
    if (!foundPath) {
        return {};
    }
    int answer = dist[endState].g;
    std::vector<Cell> path;
    VertexState cur = endState;
    while (cur != startState) {
        path.push_back(map[ai].intToCell(cur.second));
        cur = p[cur];
    }
    path.push_back(map[ai].intToCell(start));
    std::reverse(path.begin(), path.end());
    return AgentResult(path, answer);
}


void printAgentsInfo(std::vector<Agent> &agents) {
    for (auto &x : agents) {
        std::cerr << x.start << ' ' << x.end << '\n';
    }
}

void predictCellsAgentStrategy(int agentPos, int otherAgentPos, VertexConstraints &vc, EdgeConstraints& ec) {
    vc.insert(std::make_pair(1, otherAgentPos));
    ec.insert(std::make_tuple(0, agentPos, otherAgentPos));
}

void predictCellsSquareStrategy(int agentPos, int otherAgentPos, POMap& map, VertexConstraints &vc, EdgeConstraints& ec) {
    predictCellsAgentStrategy(agentPos, otherAgentPos, vc, ec);
    std::vector<int> neighbors = map.get_neighbors(otherAgentPos);
    for (auto v : neighbors) {
        vc.insert(std::make_pair(1, v));
        ec.insert(std::make_tuple(0, agentPos, v));
    }
}

void predictCellsPositionStrategy(int agentPos, int otherAgentPos, POMap& map, VertexConstraints &vc, EdgeConstraints& ec) {
    predictCellsAgentStrategy(agentPos, otherAgentPos, vc, ec);
    Cell agentCell = map.intToCell(agentPos);
    Cell otherAgentCell = map.intToCell(otherAgentPos);
    int dx = agentCell.first - otherAgentCell.first;
    int dy = agentCell.second - otherAgentCell.second;
    if (dx != 0) {
        Cell nc = otherAgentCell;
        if (dx > 0) {
            nc.first++;
        } else {
            nc.first--;
        }
        int nv = map.cellToInt(nc);
        vc.insert(std::make_pair(1, nv));
        ec.insert(std::make_tuple(0, agentPos, nv));
    }
    if (dy != 0) {
        Cell nc = otherAgentCell;
        if (dy > 0) {
            nc.second++;
        } else {
            nc.second--;
        }
        int nv = map.cellToInt(nc);
        vc.insert(std::make_pair(1, nv));
        ec.insert(std::make_tuple(0, agentPos, nv));
    }
}

void MPPA_Star::predictCells(int agentNum, int otherAgentNum, VertexConstraints &vc, EdgeConstraints& ec) {
    int curv = map[agentNum].cellToInt(curpos[agentNum]);
    int curt = map[agentNum].cellToInt(curpos[otherAgentNum]);
    switch (param->noPathStrategy) {
        case (NoPathStrategy::AGENT):
            predictCellsAgentStrategy(curv, curt, vc, ec);
            break;
        case (NoPathStrategy::SQUARE):
            predictCellsSquareStrategy(curv, curt, map[agentNum], vc, ec);
            break;
        case (NoPathStrategy::POSITION):
            predictCellsPositionStrategy(curv, curt, map[agentNum], vc, ec);
            break;
    }
}

void printCells(std::vector<int> &cells, POMap &map) {
    for (auto v : cells) {
        std::cerr << map.intToCell(v) << ' ';
    }
    std::cerr << "\n";
}

void printMoves(std::vector<int> &bestMoves, std::vector<int> &waitMoves, std::vector<int> &backMoves, POMap &map) {
    std::cerr << "BEST MOVES: ";
    printCells(bestMoves, map);
    std::cerr << "WAIT MOVES: ";
    printCells(waitMoves, map);
    std::cerr << "BACK MOVES: ";
    printCells(backMoves, map);
}

void MPPA_Star::countMoves(
        int agentNum,
        std::vector<int> &bestMove,
        std::vector<int> &waitMove,
        std::vector<int> &backMove,
        VertexConstraints &vc,
        EdgeConstraints &ec
) {
    int curv = map[agentNum].cellToInt(curpos[agentNum]);
    std::vector<int> neighbors = map[agentNum].get_neighbors(curv);
    int dist = map[agentNum].h(curpos[agentNum], scenario.agents[agentNum].end);
    for (auto v : neighbors) {
        if (vc.count(std::make_pair(1, v))) {
            continue;
        }
        if (ec.count(std::make_tuple(0, curv, v))) {
            continue;
        }
        Cell VC = map[agentNum].intToCell(v);
        int newDist = map[agentNum].h(VC, scenario.agents[agentNum].end);
        if (dist > newDist) bestMove.push_back(v);
        if (dist == newDist) waitMove.push_back(v);
        if (dist < newDist) backMove.push_back(v);
    }
    printMoves(bestMove, waitMove, backMove, map[agentNum]);
}

int chooseNextMoveRandom(
        std::vector<int> &bestMove,
        std::vector<int> &waitMove,
        std::vector<int> &backMove,
        std::mt19937 &gg
) {
    int movesN = (int)backMove.size() + (int)bestMove.size() + (int)waitMove.size();
    int pos = std::abs(int(gg())) % movesN;
    int nextMove;
    if (pos >= bestMove.size()) {
        pos -= int(bestMove.size());
        if (pos >= waitMove.size()) {
            pos -= (int)waitMove.size();
            nextMove = backMove[pos];
        } else {
            nextMove = waitMove[pos];
        }
    } else {
        nextMove = bestMove[pos];
    }
    return nextMove;
}

int chooseNextMoveBest(
        std::vector<int> &bestMove,
        std::vector<int> &waitMove,
        std::vector<int> &backMove,
        std::mt19937 &gg
) {
    int nextMove = -1;
    if (!bestMove.empty()) {
        int pos = std::abs(int(gg())) % ((int)bestMove.size());
        nextMove = bestMove[pos];
    }
    if (nextMove == -1 && !waitMove.empty()) {
        int pos = std::abs(int(gg())) % ((int)waitMove.size());
        nextMove = waitMove[pos];
    }
    if (nextMove == -1 && !backMove.empty()) {
        int pos = std::abs(int(gg())) % ((int)backMove.size());
        nextMove = backMove[pos];
    }
    return nextMove;
}

int chooseNextMoveWait(
        std::vector<int> &bestMove,
        std::vector<int> &waitMove,
        std::vector<int> &backMove,
        std::mt19937 &gg
) {
    int nextMove = -1;
    if (nextMove == -1 && !waitMove.empty()) {
        int pos = std::abs(int(gg())) % ((int)waitMove.size());
        nextMove = waitMove[pos];
    }
    if (nextMove == -1 && !backMove.empty()) {
        int pos = std::abs(int(gg())) % ((int)backMove.size());
        nextMove = backMove[pos];
    }
    if (nextMove == -1 && !bestMove.empty()) {
        int pos = std::abs(int(gg())) % ((int)bestMove.size());
        nextMove = bestMove[pos];
    }
    return nextMove;
}

int chooseNextMove(
        std::vector<int> &bestMove,
        std::vector<int> &waitMove,
        std::vector<int> &backMove,
        ConflictResolutionMode& strategy,
        std::mt19937 &gg
) {
    switch (strategy) {
        case ConflictResolutionMode::NONE:
            return -1;
        case ConflictResolutionMode::RANDOM:
            return chooseNextMoveRandom(bestMove, waitMove, backMove, gg);
        case ConflictResolutionMode::BEST:
            return chooseNextMoveBest(bestMove, waitMove, backMove, gg);
        case ConflictResolutionMode::WAIT:
            return chooseNextMoveWait(bestMove, waitMove, backMove, gg);
    }
}

void markBadDfs(int v, std::map<int, std::vector<std::pair<int, int>>> &g, std::set<int> &used, std::vector<int> &badAgents) {
    used.insert(v);
    for (auto toi : g[v]) {
        int to = toi.first;
        int i = toi.second;
        badAgents[i] = 1;
        if (!g.count(to)) continue;
        if (used.count(v) > 0) continue;
        markBadDfs(to, g, used, badAgents);
    }
}

ScenarioResult MPPA_Star::run() {
    std::mt19937 gg(std::chrono::steady_clock::now().time_since_epoch().count());

    //printAgentsInfo(scenario.agents);

    std::vector<int> prior = param->getPrior(scenario);
    std::vector<std::pair<int, int>> schedule(scenario.agents.size());
    for (int i = 0; i < scenario.agents.size(); ++i) {
        schedule[i] = std::make_pair(prior[i], i);
    }
    std::sort(schedule.begin(), schedule.end());
    std::reverse(schedule.begin(), schedule.end());
    std::vector<std::vector<Cell>> paths(scenario.agents.size());

    std::vector<AgentResult> res(scenario.agents.size());
    std::unordered_map<int, int> blockedV;

    int n = (int)scenario.agents.size();

    for (int i = 0; i < n; ++i) {
        int start = map[i].cellToInt(scenario.agents[i].start);
        auto cc = map[i].make_visible(start, param->visionRadius);
        newVisible[i].push_back(std::vector<Cell>(0));
        for (int c: cc) {
            newVisible[i][newVisible[i].size() - 1].push_back(map[i].intToCell(c));
        }
    }

    auto time_begin = (double)clock();

    for (int i = 0; i < n; ++i) {
        curpos[i] = scenario.agents[i].start;
        paths[i].push_back(curpos[i]);
    }

    auto visibilityGraph = buildVisibilityGraph();
    if (param->exchangeMode == ExchangeMode::VISIBLE_CHAIN) {
        visibilityGraph = buildGraphClosure(visibilityGraph);
    }

    for (int i = 0; i < n; ++i) {
        auto agents = visibilityGraph[i];
        auto cc = exchangeMap(i, agents);
        registerNewVisibleCells(i, cc);
        for (int c: cc) {
            newVisible[i][newVisible[i].size() - 1].push_back(map[i].intToCell(c));
        }
    }

    std::set<int> needReplanning;

    for (int i = 0; i < n; ++i) {
        needReplanning.insert(i);
    }

    std::vector<AgentResult> res1(scenario.agents.size());

    int moveCounter = 0;
    int finished = 0;

    while (true) {
        //for (auto x : needReplanning) {
        //    std::cerr << x << ' ';
        //}
        //std::cerr << '\n';

        for (auto agentNum: needReplanning) {
            res1[agentNum] = AgentResult();
        }

        if (param->fastReplanningFlag) {
            schedule.clear();
            for (auto agentNum: needReplanning) {
                schedule.emplace_back(prior[agentNum], agentNum);
            }
            std::sort(schedule.begin(), schedule.end());
            std::reverse(schedule.begin(), schedule.end());
        }

        for (auto x : schedule) {

            int agentNum = x.second;
            //std::cerr << agentNum << '\n';

            VertexConstraints vertexConstraints;
            EdgeConstraints edgeConstraints;

            std::vector<int> visibleAgents = visibilityGraph[agentNum];

           //std::cerr << "REPLANNING FOR " << x.second << '\n';

            for (int t: visibleAgents) {
                if (curpos[t] == scenario.agents[t].end) continue;
                if (!param->sendPathFlag) {
                    predictCells(agentNum, t, vertexConstraints, edgeConstraints);
                    continue;
                }
                if (needReplanning.count(t) > 0 && prior[t] < x.first) continue;
                AgentResult agentResult = res1[t];
                int timer = 0;
                for (int i = 0; i < agentResult.path.size(); ++i) {
                    auto curV = agentResult.path[i];
                    int v = map[agentNum].cellToInt(curV);
                    vertexConstraints.insert(std::make_pair(timer, v));
                    if (i + 1 < agentResult.path.size()) {
                        auto curTo = agentResult.path[i + 1];
                        int to = map[agentNum].cellToInt(curTo);
                        edgeConstraints.insert(std::make_tuple(timer, to, v));
                    }
                    timer++;
                }
            }

            AgentResult agentResult = computePathForAgent(scenario.agents[agentNum], vertexConstraints,
                                                          edgeConstraints, curpos[agentNum]);

            if (!agentResult.isFound) {
                //std::cerr << "Not found!\n";
                if (param->conflictResolutionMode == ConflictResolutionMode::NONE) {
                    return {};
                }
                std::vector<int> bestMove(0), waitMove(0), backMove(0);
                countMoves(agentNum, bestMove, waitMove, backMove, vertexConstraints, edgeConstraints);

                int movesN = (int)bestMove.size() + (int)waitMove.size() + (int)backMove.size();
                int nextMove;
                if (movesN == 0) {
                    nextMove = map[agentNum].cellToInt(curpos[agentNum]);
                } else {
                    nextMove = chooseNextMove(bestMove, waitMove, backMove, param->conflictResolutionMode, gg);
                }

                AgentResult tmpAgentResult = {};
                tmpAgentResult.path = { curpos[agentNum], map[agentNum].intToCell(nextMove) };
                tmpAgentResult.isFound = false;
                agentResult = tmpAgentResult;
            }

            res1[agentNum] = agentResult;
        }

        bool finishFlag;

        while (true) {

            finishFlag = true;

            std::map<std::pair<int, int>, int> ss;
            std::map<int, std::vector<std::pair<int, int>>> rg;
            std::vector<Cell> nextpos(n);

            std::set<int> bad = {};

            for (int i = 0; i < n; ++i) {
                if (curpos[i] == scenario.agents[i].end) continue;
                nextpos[i] = res1[i].path[1];
                //std::cerr << i << ' ' << curpos[i] << ' ' << nextpos[i] << '\n';
                int v = map[i].cellToInt(curpos[i]);
                int to = map[i].cellToInt(nextpos[i]);
                if (ss.count(std::make_pair(to, v)) && to != v) {
                    bad.insert(to);
                    bad.insert(v);
                }
                ss[std::make_pair(v, to)] = i;
                rg[to].push_back(std::make_pair(v, i));
            }
            //std::cerr << '\n';

            for (const auto& x : rg) {
                int pos = x.first;
                if (x.second.size() > 1) {
                    bad.insert(pos);
                    //std::cerr << "BAD!\n";
                    //std::cerr << x.second[0] << ' ' << x.second[1] << '\n';
                }
            }

            std::set<int> used;
            std::vector<int> badAgents(n, 0);
            for (auto v : bad) {
                markBadDfs(v, rg, used, badAgents);
            }

            for (int i = 0; i < n; ++i) {
                if (badAgents[i]) {
                    //std::cerr << "AGENT " << i << " IS BAD!!!\n";
                    paths[i].push_back(curpos[i]);
                    //needReplanning.insert(i);
                } else {
                    if (res1[i].path.size() >= 2) {
                        //std::cerr << i << " moved from (" << curpos[i].first << ", " << curpos[i].second <<") to (" << res1[i].path[1].first << ", " << res1[i].path[1].second << ")\n";
                        curpos[i] = res1[i].path[1];
                        paths[i].push_back(curpos[i]);
                        res1[i].path.erase(res1[i].path.begin());
                    }
                }
            }

            moveCounter++;
            if (moveCounter > moveLimit) {
                ScenarioResult sres = ScenarioResult();
                sres.isCorrect = false;
                sres.CTNodes = 0;
                for (int i = 0; i < n; ++i) {
                    if (curpos[i] == scenario.agents[i].end) {
                        sres.CTNodes++;
                        sres.isCorrect = true;
                    }
                }
                std::cerr << sres.CTNodes << '\n';
                return sres;
            }


            for (int i = 0; i < n; ++i) {
                auto cc = map[i].make_visible(map[i].cellToInt(curpos[i]), param->visionRadius);
                newVisible[i].push_back(std::vector<Cell>(0));
                for (int c: cc) {
                    newVisible[i][newVisible[i].size() - 1].push_back(map[i].intToCell(c));
                }
                if (curpos[i] != scenario.agents[i].end) finishFlag = false;
            }

            visibilityGraph = buildVisibilityGraph();
            if (param->exchangeMode == ExchangeMode::VISIBLE_CHAIN) {
                visibilityGraph = buildGraphClosure(visibilityGraph);
            }

            for (int i = 0; i < n; ++i) {
                auto agents = visibilityGraph[i];
                auto cc = exchangeMap(i, agents);
                registerNewVisibleCells(i, cc);
                for (int c: cc) {
                    newVisible[i][newVisible[i].size() - 1].push_back(map[i].intToCell(c));
                }
            }

            if (finishFlag) break;


            VertexConstraints vc;
            EdgeConstraints ec;
            std::map<std::pair<int, int>, int> vcc;
            std::map<std::tuple<int, int, int>, int> ecc;

            bool conflictFlag = false;

            if (param->fastReplanningFlag) {
                needReplanning.clear();
                for (int i = 0; i < n; ++i) {
                    if (!badAgents[i]) continue;
                    needReplanning.insert(i);
                    conflictFlag = true;
                }
                for (int i = 0; i < n; ++i) {
                    if (!res1[i].isFound) {
                        conflictFlag = true;
                        needReplanning.insert(i);
                        continue;
                    }
                    if (!(param->sendPathFlag)) {
                        if (visibilityGraph[i].empty()) continue;
                        VertexConstraints vvc;
                        EdgeConstraints eec;
                        for (auto j: visibilityGraph[i]) {
                            predictCells(i, j, vvc, eec);
                        }
                        for (int j = 0; j < res1[i].path.size(); ++j) {
                            auto c = res1[i].path[j];
                            int v = map[i].cellToInt(c);
                            if (vvc.count(std::make_pair(j, v))) {
                                needReplanning.insert(i);
                                conflictFlag = true;
                            }
                        }
                    }
                    for (int j = 0; j < res1[i].path.size(); ++j) {
                        auto c = res1[i].path[j];
                        int v = map[i].cellToInt(c);
                        if (map[i].isKnown[v] && map[i].g[v].isObstacle) {
                            needReplanning.insert(i);
                            conflictFlag = true;
                        }
                        if (param->sendPathFlag) {
                            if (vc.count(std::make_pair(j, v))) {
                                needReplanning.insert(i);
                                needReplanning.insert(vcc[std::make_pair(j, v)]);
                                conflictFlag = true;
                            } else {
                                vc.insert(std::make_pair(j, v));
                                vcc[std::make_pair(j, v)] = i;
                            }
                            if (j == 0) continue;
                            auto pc = res1[i].path[j - 1];
                            int pv = map[i].cellToInt(pc);
                            if (ec.count(std::make_tuple(j - 1, v, pv))) {
                                needReplanning.insert(i);
                                needReplanning.insert(ecc[std::make_tuple(j - 1, v, pv)]);
                                conflictFlag = true;
                            } else {
                                ec.insert(std::make_tuple(j - 1, pv, v));
                                ecc[std::make_tuple(j - 1, pv, v)] = i;
                            }
                        }
                    }
                }
            } else {
                for (int i = 0; i < n; ++i) {
                    if (!res1[i].isFound) {
                        conflictFlag = true;
                        break;
                    }
                    for (int j = 0; j < res1[i].path.size(); ++j) {
                        auto c = res1[i].path[j];
                        int v = map[i].cellToInt(c);
                        if (map[i].isKnown[v] && map[i].g[v].isObstacle) {
                            conflictFlag = true;
                            break;
                        }
                        if (param->sendPathFlag) {
                            if (vc.count(std::make_pair(j, v))) {
                                conflictFlag = true;
                                break;
                            }
                            vc.insert(std::make_pair(j, v));
                        }
                    }
                    if (conflictFlag) break;
                }
            }
            if (conflictFlag) break;
        }


        if (finishFlag) break;
    }

    for (int i = 0; i < n; ++i) {
        res[i] = AgentResult(paths[i], (int)paths[i].size());
        res[i].newVisible = newVisible[i];
    }
    ScenarioResult sres = ScenarioResult(res);
    //std::cerr << "ANSWER:" << sres.answer << '\n';
    auto currtime = (double)clock();
    double runtime = (currtime - time_begin) / CLOCKS_PER_SEC;
    //std::cerr << "TIME: " << runtime << '\n';
    sres.runtime = runtime;
    sres.CTNodes = 0;
    for (int i = 0; i < n; ++i) {
        if (curpos[i] == scenario.agents[i].end) {
            sres.CTNodes++;
            sres.isCorrect = true;
        }
    }
    std::cerr << sres.CTNodes << '\n';
    return sres;
}

std::vector<std::vector<int>> MPPA_Star::buildVisibilityGraph() {
    int n = int(scenario.agents.size());
    std::vector<std::vector<int>> vg(n);
    for (int agentNum = 0; agentNum < n; agentNum++) {
        std::vector<int> ag = {};
        for (int i = 0; i < n; ++i) {
            if (i == agentNum) continue;
            if (param->exchangeMode == ExchangeMode::ALL || map[i].isCellVisible(curpos[agentNum], curpos[i], param->visionRadius)) {
                vg[agentNum].push_back(i);
            }
        }
    }
    return vg;
}

void dfs(int v, int color, std::vector<int> &used, std::vector<std::vector<int>> &g) {
    used[v] = color;
    for (auto &to: g[v]) {
        if (used[to]) continue;
        dfs(to, color, used, g);
    }
}

std::vector<std::vector<int>> MPPA_Star::buildGraphClosure(std::vector<std::vector<int>> &graph) {
    int n = (int)graph.size();
    int k = 0;
    std::vector<int> used(n, 0);
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i, ++k, used, graph);
        }
    }
    std::vector<std::vector<int>> comp(k);
    for (int i = 0; i < n; ++i) {
        used[i]--;
        comp[used[i]].push_back(i);
    }
    std::vector<std::vector<int>> ng(n);
    for (int i = 0; i < n; ++i) {
        for (auto &x: comp[used[i]]) {
            if (x == i) continue;
            ng[i].push_back(x);
        }
    }
    return ng;
}

std::set<int> MPPA_Star::exchangeMap(int agentNum, std::vector<int> &agents) {
    std::set<int> res = {};
    if (!(param->sendMapFlag)) return res;
    for (int &i: agents) {
        for (int j = lastSeen[agentNum][i] + 1; j < newVisible[i].size(); ++j) {
            for (auto &c: newVisible[i][j]) {
                int v = map[agentNum].cellToInt(c);
                if (!map[agentNum].isKnown[v]) {
                    res.insert(v);
                }
            }
        }
        lastSeen[agentNum][i] = (int)newVisible[i].size() - 1;
    }
    return res;
}

void MPPA_Star::registerNewVisibleCells(int agentNum, std::set<int> &cells) {
    for (auto &v: cells) {
        map[agentNum].isKnown[v] = true;
    }
}

bool MPPA_Star::checkTimeLimit(double timeBegin) const {
    auto currtime = (double)clock();
    double runtime = (currtime - timeBegin) / CLOCKS_PER_SEC;
    return runtime > timeLimit;
}
