//
// Created by Gleb.Oborin on 10/20/21.
//

#ifndef PO_MAPF_MDSTAR_LITE_H
#define PO_MAPF_MDSTAR_LITE_H


#include "../scen/ScenarioResult.h"
#include "../scen/Scenario.h"
#include "../map/Graph.h"
#include "MAPFAlgorithm.h"
#include "../map/Map.h"

class MDSTAR_Lite: public MAPFAlgorithm {
private:

public:
    ScenarioResult run(Map &g, Scenario &scen) override;
};


#endif //PO_MAPF_MDSTAR_LITE_H
