#ifndef PO_MAPF_BENCHMARKPARAMS_H
#define PO_MAPF_BENCHMARKPARAMS_H

#include "../algorithms/MAPF/MAPFParam.h"
#include <string>
#include <vector>
#include "ConfigurationParams.h"

class BenchmarkParams: public ConfigurationParams {
public:
    std::string getType() override {
        return "BENCHMARK";
    }

    int NLaunch { 0 };
    int NMaxAgents { 0 };
    int addAgents { 1 };
    std::vector<int> NAgents = std::vector<int>();

    std::vector<MAPFParam*> params = std::vector<MAPFParam*>();

    BenchmarkParams() = default;
    ~BenchmarkParams() = default;
};

#endif //PO_MAPF_BENCHMARKPARAMS_H
