#ifndef PO_MAPF_CBS_H
#define PO_MAPF_CBS_H

#include "../MAPFAlgorithm.h"
#include <set>
#include "../Constraints.h"
#include "../Conflicts.h"
#include "CBSParam.h"
#include "TreeNode.h"
#include <unordered_map>
#include <unordered_set>

class CBS: public MAPFAlgorithm {
private:
    Map map;
    Scenario scenario;
    CBSParam* param;

    std::vector<ScenarioResult> treeSolutions;
    std::vector<std::vector<VertexConstraints>> treeVertexConstraints;
    std::vector<std::unordered_map<VertexState, int, hash_VertexState>> treeAllLandmarks;
    std::vector<std::vector<EdgeConstraints>> treeEdgeConstraints;
    std::vector<std::vector<std::set<VertexState>>> treeLandmarks;
    std::vector<std::vector<std::set<int>>> treeTimeEndBlocked;
    std::vector<int> parent;

    std::set<TreeNode> open;

    ScenarioResult lowLevelSearch(
            int node,
            std::vector<int> &constraintAgentNum
    );

    void solveConflicts(
            int node,
            std::pair<std::vector<VertexConflict>, std::vector<EdgeConflict>> &conflicts);

    std::pair<std::vector<VertexConflict>, std::vector<EdgeConflict>> searchConflicts(ScenarioResult &scenarioResult);

    AgentResult computePath(
            Agent &a,
            VertexConstraints &vertexConstraints,
            EdgeConstraints &edgeConstraints,
            std::set<VertexState> &landmarks,
            std::unordered_map<VertexState, int, hash_VertexState> &blocked,
            std::set<int> &timeEndIsBlocked);

    int computePathBetweenLandmarks(
            int agentNum,
            VertexConstraints &vertexConstraints,
            EdgeConstraints &edgeConstraints,
            std::unordered_map<VertexState, VertexInfo, hash_VertexState> &dist,
            std::unordered_map<VertexState, VertexState, hash_VertexState> &p,
            std::unordered_map<VertexState, int, hash_VertexState> &blocked,
            std::set<int> &timeEndIsBlocked,
            VertexState startState,
            VertexState endState,
            int endH);

    void build_mdd(
            MDD &mdd,
            VertexState startState,
            VertexState vState,
            std::unordered_map<VertexState, VertexInfo, hash_VertexState> &dist,
            std::unordered_map<VertexState, VertexState, hash_VertexState> &p,
            std::unordered_set<VertexState, hash_VertexState> &used, int end);

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


    int getAgentPosAtTime(AgentResult &agentResult, int time);
public:
    ScenarioResult run() override;

    double timeLimit;

    CBS(Map &_g, Scenario &_scen, MAPFParam *_param, double _tl = -1) : map(_g), scenario(_scen), param(nullptr), timeLimit(_tl) {
        param = dynamic_cast<CBSParam*>(_param);
    }
};


#endif //PO_MAPF_CBS_H
