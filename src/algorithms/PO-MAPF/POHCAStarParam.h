#ifndef PO_MAPF_POHCASTARPARAM_H
#define PO_MAPF_POHCASTARPARAM_H

#include <vector>
#include "../../scen/Scenario.h"
#include "../MAPF/MAPFParam.h"
#include "../MAPF/PPAStar/PPAStarParam.h"

enum class ExchangeMode {
    ALL,
    VISIBLE_CHAIN, // TODO("rename")
    ONLY_VISIBLE
};

enum class ConflictResolutionMode {
    NONE,
    RANDOM,
    BEST,
    WAIT
};

enum class NoPathStrategy {
    AGENT,
    SQUARE,
    POSITION,
    SQUARE_P,
    POSITION_P,
    MOVEMENT,
};

class POHCAStarParam: public MAPFParam {
private:
    static std::vector<int> getRandomPrior(Scenario &scen);
    static std::vector<int> getLongestFirstPrior(Scenario &scen);
    static std::vector<int> getShortestFirstPrior(Scenario &scen);

    static int getPathLength(Cell start, Cell end);
public:
    Priority priority = Priority::RANDOM;
    ConflictResolutionMode conflictResolutionMode = ConflictResolutionMode::NONE;
    NoPathStrategy noPathStrategy = NoPathStrategy::AGENT;

    bool sendPathFlag = false;
    bool sendMapFlag = false;
    bool fastReplanningFlag = false;
    bool savePathsFlag = false;

    ExchangeMode exchangeMode = ExchangeMode::VISIBLE_CHAIN;
    int visionRadius = 3;

    POHCAStarParam(POHCAStarParam const &other)  : MAPFParam(other) {
        this->priority = other.priority;
    }

    std::string getName() override {
        return "POHCAStar";
    }

    POHCAStarParam() = default;

    std::vector<int> getPrior(Scenario &Scen) const;
};


#endif //PO_MAPF_POHCASTARPARAM_H
