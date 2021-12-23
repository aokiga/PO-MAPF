#ifndef MAPFALGORITHM_H
#define MAPFALGORITHM_H

#include "../../map/Map.h"
#include "../../scen/Scenario.h"
#include "../../scen/ScenarioResult.h"
#include "MAPFParam.h"

class MAPFAlgorithm {
public:
    virtual ScenarioResult run() = 0; // TODO(Change MAP to GRAPH)
    double timeLimit;
};


#endif //MAPFALGORITHM_H
