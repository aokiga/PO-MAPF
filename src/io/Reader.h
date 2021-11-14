//
// Created by Gleb.Oborin on 10/20/21.
//

#ifndef PO_MAPF_READER_H
#define PO_MAPF_READER_H

#include <fstream>
#include <string>
#include "../scen/Scenario.h"
#include "../map/Map.h"

Map readMap(const std::string &fileName);
Scenario readScenario(const std::string &fileName);

#endif //PO_MAPF_READER_H
