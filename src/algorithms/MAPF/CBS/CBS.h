#ifndef PO_MAPF_CBS_H
#define PO_MAPF_CBS_H

#include "../MAPFAlgorithm.h"
#include <set>
#include "../Constraints.h"
#include "../Conflicts.h"
#include "CBSParam.h"
#include "TreeNode.h"

class CBS: public MAPFAlgorithm {
private:
    Map map;
    Scenario scenario;
    CBSParam* param;

    std::vector<ScenarioResult> treeSolutions;
    std::vector<std::vector<VertexConstraints>> treeVertexConstraints;
    std::vector<std::map<VertexState, int>> treeAllLandmarks;
    std::vector<std::vector<EdgeConstraints>> treeEdgeConstraints;
    std::vector<std::vector<VertexConstraints>> treeLandmarks;
    std::vector<int> parent;

    std::set<TreeNode> open;

    ScenarioResult lowLevelSearch(
            int node,
            int constraintAgentNum = -1
    );

    void solveConflicts(
            int node,
            std::pair<std::vector<VertexConflict>, std::vector<EdgeConflict>> &conflicts);

    std::pair<std::vector<VertexConflict>, std::vector<EdgeConflict>> searchConflicts(ScenarioResult &scenarioResult);

    AgentResult computePath(
            Agent &a,
            std::set<VertexState> &vertexConstraints,
            std::set<EdgeState> &edgeConstraints,
            std::set<VertexState> &landmarks,
            std::map<VertexState, int> &blocked);

    int computePathBetweenLandmarks(
            int agentNum,
            std::set<VertexState> &vertexConstraints,
            std::set<EdgeState> &edgeConstraints,
            std::map<VertexState, VertexInfo> &dist,
            std::map<VertexState, VertexState> &p,
            std::map<VertexState, int> &blocked,
            VertexState startState,
            VertexState endState,
            int endH);

    void build_bdd(
            BDD &bdd,
            VertexState startState,
            VertexState vState,
            std::map<VertexState, VertexInfo> &dist,
            std::map<VertexState, VertexState> &p,
            std::set<VertexState> &used);

    static std::tuple<Conflicts, Conflicts, Conflicts> prioritizeConflicts(ScenarioResult &scenarioResult, Conflicts &conflicts);

    int countHeuristic(ScenarioResult &scen);

    static bool isCardinalConflict(const VertexConflict &conflict, ScenarioResult &scenarioResult);
    static bool isCardinalConflict(const EdgeConflict &conflict, ScenarioResult &scenarioResult);
    static bool isSemiCardinalConflict(const VertexConflict &conflict, ScenarioResult &scenarioResult);
    static bool isSemiCardinalConflict(const EdgeConflict &conflict, ScenarioResult &scenarioResult);


    void solveConflict(int node, VertexConflict &vertexConflict);
    void solveConflict(int node, EdgeConflict &edgeConflict);
    void solveConflictDC(int node, VertexConflict &conflict);
    void solveConflictDC(int node, EdgeConflict &conflict);

    Conflicts chooseConflict(Conflicts &conflicts, ScenarioResult &scenarioResult);
    Conflicts chooseConflictWithSamePriority(Conflicts &conflicts, ScenarioResult &scenarioResult, int type = 2);
public:
    ScenarioResult run() override;

    double timeLimit;

    CBS(Map &_g, Scenario &_scen, MAPFParam *_param, double _tl = -1) : map(_g), scenario(_scen), param(nullptr), timeLimit(_tl) {
        param = dynamic_cast<CBSParam*>(_param);
    }
};


#endif //PO_MAPF_CBS_H
