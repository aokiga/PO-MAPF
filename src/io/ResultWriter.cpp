#include "ResultWriter.h"
#include <fstream>

void ResultWriter::writeScenarioResult(const std::string &filename, ScenarioResult &scenarioResult) {
    std::ofstream out(filename);
    if (!scenarioResult.isCorrect) {
        out << -1 << '\n';
        out.close();
        return;
    }
    out << scenarioResult.agents.size() << '\n';
    for (int i = 0; i < scenarioResult.agents.size(); ++i) {
        out << i << '\n';
        out << scenarioResult.agents[i].answer << '\n';
        out << scenarioResult.agents[i].path.size() << '\n';
        for (auto &cell : scenarioResult.agents[i].path) {
            out << cell.first << ' ' << cell.second << '\n';
        }
        out << scenarioResult.agents[i].newVisible.size() << '\n';
        for (auto &j : scenarioResult.agents[i].newVisible) {
            out << j.size() << '\n';
            for (auto &cell : j) {
                out << cell.first << ' ' << cell.second << '\n';
            }
        }
    }
    out.close();
}

void ResultWriter::writeBenchmarkResult(const std::string &filename, BenchmarkParams *benchmarkParams, BenchmarkResult &benchmarkResult) {
    std::ofstream out(filename);
    out << benchmarkParams->NLaunch << '\n';
    out << benchmarkParams->addAgents << '\n';
    for (int t = 0; t < benchmarkParams->NLaunch; ++t) {
        out << t << "\n";
        for (int i = 0; i < benchmarkParams->params.size(); ++i) {
            out << benchmarkParams->params[i]->name << "\n";
            for (int j = 0; j < benchmarkResult.res[t][i].size(); ++j) {
                out << j + 1 << '\n';
                auto &scen = benchmarkResult.res[t][i][j];
                out << (scen.isCorrect ? "1" : "0") << '\n';
                if (scen.isCorrect) {
                    out << "ANSWER " << scen.answer << '\n';
                    out << "RUNTIME " << scen.runtime << '\n';
                    out << "CTNODES " << scen.CTNodes << '\n';
                }
            }
        }
    }
    out.close();
}
