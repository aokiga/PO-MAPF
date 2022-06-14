#ifndef PO_MAPF_POHCA_STAR_H
#define PO_MAPF_POHCA_STAR_H


#include <unordered_map>
#include "../MAPF/MAPFAlgorithm.h"
#include "../MAPF/Constraints.h"
#include "../../map/POMap.h"
#include "POHCAStarParam.h"
#include <set>
#include <random>

class POHCA_Star: public MAPFAlgorithm {
private:
    POHCAStarParam* param;
    Scenario scenario;
    std::vector<Cell> curpos, prevcurpos;
    std::vector<POMap> map;
    std::vector<std::vector<std::vector<Cell>>> newVisible;

    double moveLimit;

    std::set<int> exchangeMap(int agentNum, std::vector<int> &agents);
    std::vector<std::vector<int>> buildVisibilityGraph();
    static std::vector<std::vector<int>> buildGraphClosure(std::vector<std::vector<int>> &graph);
    void registerNewVisibleCells(int agentNum, std::set<int> &cells);

    std::vector<std::vector<int>> lastSeen, prevlastSeen;

    AgentResult computePathForAgent(Agent &a, VertexConstraints &vertexConstraints, EdgeConstraints &edgeConstraints, Cell startCell);

    bool checkTimeLimit(double timeBegin) const;

    void predictCells(int agentNum, int otherAgentNum, VertexConstraints &vc, EdgeConstraints& ec, std::mt19937 &gg);
    void predictCellsMovementStrategy(int agentNum, int otherAgentNum, VertexConstraints &vc, EdgeConstraints& ec, std::mt19937 &gg);

    void countMoves(
            int agentNum,
            std::vector<int> &bestMove,
            std::vector<int> &waitMove,
            std::vector<int> &backMove,
            VertexConstraints &vc,
            EdgeConstraints &ec
    );
public:
    POHCA_Star(Map &g, Scenario &scen, MAPFParam* paramMAPF, int _tl = -1): scenario(scen), moveLimit(_tl) {
        param = dynamic_cast<POHCAStarParam*>(paramMAPF);

        int n = scenario.agents.size();

        lastSeen.assign(n, std::vector<int>(n, -1));
        prevlastSeen.assign(n, std::vector<int>(n, -1));
        map.assign(n, POMap(g));
        curpos.resize(n);
        prevcurpos.resize(n);
        newVisible.resize(n);
    }

    ScenarioResult run() override;
};


#endif //PO_MAPF_POHCA_STAR_H
