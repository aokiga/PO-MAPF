#ifndef PO_MAPF_RESULTWRITER_H
#define PO_MAPF_RESULTWRITER_H

#include <fstream>
#include "../scen/ScenarioResult.h"
#include "../scen/BenchmarkResult.h"
#include "../configuration/BenchmarkParams.h"

class ResultWriter {
public:
    static void writeScenarioResult(const std::string &filename, ScenarioResult &scenarioResult);
    static void writeBenchmarkResult(const std::string &filenname, BenchmarkParams *benchmapkParams, BenchmarkResult &benchmarkResult);
};
#endif //PO_MAPF_RESULTWRITER_H
