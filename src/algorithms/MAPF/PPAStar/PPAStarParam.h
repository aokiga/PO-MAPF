#ifndef PO_MAPF_PPASTARPARAM_H
#define PO_MAPF_PPASTARPARAM_H

#include "../MAPFParam.h"
#include "../../../scen/Scenario.h"
#include <string>
#include <vector>

enum class Priority {
    RANDOM,
    LONGEST_FIRST,
    SHORTEST_FIRST
};

class PPAStarParam: public MAPFParam {
private:
    static std::vector<int> getRandomPrior(Scenario &scen);
    static std::vector<int> getLongestFirstPrior(Scenario &scen);
    static std::vector<int> getShortestFirstPrior(Scenario &scen);
public:
    Priority priority = Priority::RANDOM;

    PPAStarParam(PPAStarParam const &other) {
        this->priority = other.priority;
    }

    std::string getName() override {
        return "PPAStar";
    }

    PPAStarParam() = default;

    std::vector<int> getPrior(Scenario &Scen) const;
};


#endif //PO_MAPF_PPASTARPARAM_H
