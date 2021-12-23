#ifndef PO_MAPF_SCENARIOGENERATOR_H
#define PO_MAPF_SCENARIOGENERATOR_H

#include "Scenario.h"

class ScenarioGenerator {
public:
    static Scenario generate(const Map &m, int NAGENTS);
};


#endif //PO_MAPF_SCENARIOGENERATOR_H
