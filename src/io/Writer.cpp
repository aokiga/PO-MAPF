//
// Created by Gleb.Oborin on 10/20/21.
//

#include "Writer.h"
#include <fstream>

void writeScenarioResult(const std::string &filename, ScenarioResult &scenarioResult) {
    std::ofstream out(filename);
    if (!scenarioResult.isCorrect) {
        out << -1 << '\n';
        out.close();
        return;
    }
    out << scenarioResult.agents.size() << '\n';
    for (int i = 0; i < scenarioResult.agents.size(); ++i) {
        out << i << '\n';
        out << scenarioResult.agents[i].answer << ' ' << scenarioResult.agents[i].trueAnswer << '\n';
        out << scenarioResult.agents[i].path.size() << '\n';
        for (auto &cell : scenarioResult.agents[i].path) {
            out << cell.first << ' ' << cell.second << '\n';
        }
    }
    out.close();
}