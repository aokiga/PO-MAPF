//
// Created by Gleb.Oborin on 10/20/21.
//

#include <vector>
#include "Agent.h"

#ifndef PO_MAPF_SCENARIO_H
#define PO_MAPF_SCENARIO_H

class Scenario {
public:
    std::vector<Agent> agents;

    explicit Scenario(std::vector<Agent> &a) : agents(a) {}
};


#endif //PO_MAPF_SCENARIO_H
