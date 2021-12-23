#ifndef PO_MAPF_BENCHMARKRESULT_H
#define PO_MAPF_BENCHMARKRESULT_H

class BenchmarkResult {
public:
    std::vector<std::vector<std::vector<ScenarioResult>>> res;

    BenchmarkResult() = default;
};

#endif //PO_MAPF_BENCHMARKRESULT_H
