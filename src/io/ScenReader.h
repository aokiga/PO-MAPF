#ifndef PO_MAPF_SCENREADER_H
#define PO_MAPF_SCENREADER_H

#include "../scen/Scenario.h"

class ScenReader {
public:
    static Scenario readScenario(const std::string &fileName);
};


#endif //PO_MAPF_SCENREADER_H
