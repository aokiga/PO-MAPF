#ifndef PO_MAPF_MPPASTARPARAM_H
#define PO_MAPF_MPPASTARPARAM_H

#include <vector>
#include "../../../scen/Scenario.h"
#include "../../MAPF/MAPFParam.h"
#include "../../MAPF/PPAStar/PPAStarParam.h"

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
   //MOVEMENT TODO()
};

class MPPAStarParam: public MAPFParam {
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

    MPPAStarParam(MPPAStarParam const &other)  : MAPFParam(other) {
        this->priority = other.priority;
    }

    std::string getName() override {
        return "MPPAStar";
    }

    MPPAStarParam() = default;

    std::vector<int> getPrior(Scenario &Scen) const;
};


#endif //PO_MAPF_MPPASTARPARAM_H
