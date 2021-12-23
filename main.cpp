#include <iostream>
#include "src/map/Graph.h"
#include "src/scen/Scenario.h"
#include "src/scen/ScenarioResult.h"
#include "src/scen/ScenarioGenerator.h"
#include "src/io/ResultWriter.h"

#include "src/io/MapReader.h"
#include "src/algorithms/MAPF/CBS/CBS.h"
#include "src/configuration/LaunchParams.h"
#include "src/io/configuration/ConfigurationReader.h"
#include "src/io/ScenReader.h"
#include "src/algorithms/MAPF/PPAStar/PPAStar.h"
#include "src/scen/BenchmarkResult.h"

#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>

ScenarioResult launchAlgorithm(Map &map, Scenario &scen, MAPFParam* param, double timeLimit) {
    if (param->getName() == "CBS") {
        return CBS(map, scen, param, timeLimit).run();
    }
    if (param->getName() == "PPAStar") {
        return PPAStar(map, scen, param, timeLimit).run();
    }
}

void launch(ConfigurationParams *configurationParams) {
    LaunchParams* launchParams = dynamic_cast<LaunchParams*>(configurationParams);
    Map g = MapReader::readMap(launchParams->pathToData + launchParams->mapFileName);
    Scenario s = ScenReader::readScenario(launchParams->pathToData + launchParams->scenFileName);
    double time_begin = clock();
    ScenarioResult scenarioResult = launchAlgorithm(g, s, launchParams->param, configurationParams->timeLimit);
    double time_end = clock();
    double runtime = (time_end - time_begin) / CLOCKS_PER_SEC;
    std::cout << "Run time: " << runtime << "seconds.\n";
    ResultWriter::writeScenarioResult(launchParams->pathToData + launchParams->mapFileName + ".ans", scenarioResult);
}

void benchmark(ConfigurationParams *configurationParams) {
    BenchmarkParams *benchmarkParams = dynamic_cast<BenchmarkParams*>(configurationParams);

    Map g = MapReader::readMap(benchmarkParams->pathToData + benchmarkParams->mapFileName);
    Scenario s;

    if (benchmarkParams->scenFileName.empty()) {
        int maxAgents = 0;
        for (auto agentN : benchmarkParams->NAgents) {
            maxAgents = std::max(maxAgents, agentN);
        }
        std::cerr << maxAgents << '\n';
        s = ScenarioGenerator::generate(g, maxAgents);
    } else {
        s = ScenReader::readScenario(benchmarkParams->pathToData + benchmarkParams->scenFileName);
    }
    BenchmarkResult result;
    std::mt19937 gg(std::chrono::steady_clock::now().time_since_epoch().count());
    result.res.resize(benchmarkParams->NLaunch);
    for (int t = 0; t < benchmarkParams->NLaunch; ++t) {
        result.res[t].resize(benchmarkParams->params.size());

        std::cerr << t + 1 << "/" << benchmarkParams->NLaunch << "\n";
        std::cerr << "MAP GENERATED\n";

        std::shuffle(s.agents.begin(), s.agents.end(), gg);

        for (int i = 0; i < benchmarkParams->params.size(); ++i) {
            std::cerr << "LAUNCH " << benchmarkParams->params[i]->name << "\n";

            for (int ai = 0; ai < benchmarkParams->NAgents.size(); ++ai) {
                int agentN = benchmarkParams->NAgents[ai];
                std::cerr << agentN << " AGENTS NOW\n";

                Scenario tmpScen;
                for (int j = 0; j < agentN; ++j) {
                    tmpScen.agents.push_back(s.agents[j]);
                    tmpScen.agents[j].i = j;
                }

                auto time_start = std::chrono::steady_clock::now();
                ScenarioResult scenarioResult = launchAlgorithm(g, tmpScen, benchmarkParams->params[i],
                                                                benchmarkParams->timeLimit);
                auto time_end = std::chrono::steady_clock::now();
                auto runtime = std::chrono::duration<double>(time_end - time_start);
                std::cerr << "Run time: " << runtime.count() << "seconds.\n";

                result.res[t][i].push_back(scenarioResult);

                if (!scenarioResult.isCorrect) break;
            }
        }
    }
    ResultWriter::writeBenchmarkResult(benchmarkParams->pathToData + benchmarkParams->mapFileName + ".ben", benchmarkParams, result);
}

int main(int argc, char* argv[]) {
    ConfigurationParams *configurationParams = ConfigurationReader::readConfigurationFile("../param.conf");
    if (configurationParams->getType() == "LAUNCH") {
        launch(configurationParams);
    }
    if (configurationParams->getType() == "BENCHMARK") {
        benchmark(configurationParams);
    }
    return 0;
}
