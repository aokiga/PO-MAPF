//
// Created by Gleb.Oborin on 11/4/21.
//

#ifndef PO_MAPF_CBS_H
#define PO_MAPF_CBS_H


#include "MAPFAlgorithm.h"
#include <set>

class CBS: public MAPFAlgorithm {
private:
    std::vector<int> prior;
    ScenarioResult lowLevelSearch(Map &g, Scenario &scen, std::vector<std::set<std::pair<int, int>>> &constraints);
public:
    ScenarioResult run(Map &g, Scenario &scen) override;

    explicit CBS(std::vector<int> &prior) {
        this->prior = prior;
    }
};


#endif //PO_MAPF_CBS_H
