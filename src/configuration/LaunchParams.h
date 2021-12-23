#ifndef PO_MAPF_LAUNCHPARAMS_H
#define PO_MAPF_LAUNCHPARAMS_H

#include <string>
#include "../algorithms/MAPF/MAPFParam.h"
#include "ConfigurationParams.h"

class LaunchParams: public ConfigurationParams {
public:
    std::string getType() override {
        return "LAUNCH";
    }

    MAPFParam* param = nullptr;

    LaunchParams() = default;
    ~LaunchParams() = default;
};


#endif //PO_MAPF_LAUNCHPARAMS_H
