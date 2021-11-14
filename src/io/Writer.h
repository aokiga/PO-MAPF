//
// Created by Gleb.Oborin on 10/20/21.
//

#ifndef PO_MAPF_WRITER_H
#define PO_MAPF_WRITER_H

#include <fstream>
#include "../scen/ScenarioResult.h"

void writeScenarioResult(const std::string &filename, ScenarioResult &scenarioResult);

#endif //PO_MAPF_WRITER_H
