//
// Created by Gleb.Oborin on 10/20/21.
//

#ifndef MAPFALGORITHM_H
#define MAPFALGORITHM_H

#include "../map/Map.h"
#include "../scen/Scenario.h"
#include "../scen/ScenarioResult.h"

class MAPFAlgorithm {
public:
    virtual ScenarioResult run(Map &g, Scenario &scen) = 0; // TODO(Change MAP to GRAPH)
};


#endif //MAPFALGORITHM_H
