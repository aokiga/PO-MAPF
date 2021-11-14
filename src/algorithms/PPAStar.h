//
// Created by Gleb.Oborin on 10/31/21.
//

#ifndef PO_MAPF_PPASTAR_H
#define PO_MAPF_PPASTAR_H


#include "MAPFAlgorithm.h"
#include "../scen/ScenarioResult.h"
#include "../scen/Scenario.h"
#include <vector>
#include <set>

class PPAStar: public MAPFAlgorithm {
private:
    std::vector<int> prior;
public:
    explicit PPAStar(std::vector<int> &prior) {
        this->prior = prior;
    }

    ScenarioResult run(Map &g, Scenario &scen) override;
};


#endif //PO_MAPF_PPASTAR_H
