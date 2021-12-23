#include <vector>
#include "Agent.h"

#ifndef PO_MAPF_SCENARIO_H
#define PO_MAPF_SCENARIO_H

class Scenario {
public:
    std::vector<Agent> agents = std::vector<Agent>();

    explicit Scenario(std::vector<Agent> &a) : agents(a) {}

    Scenario() = default;
};


#endif //PO_MAPF_SCENARIO_H
