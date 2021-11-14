#include <iostream>
#include "src/map/Graph.h"
#include "src/scen/Scenario.h"
#include "src/io/Reader.h"
#include "src/scen/ScenarioResult.h"
#include "src/algorithms/MDSTAR_Lite.h"
#include "src/io/Writer.h"
#include "src/algorithms/PPAStar.h"
#include "src/algorithms/CBS.h"
#include <vector>
#include <string>

const std::string MAP_FILE_NAME = "minimap.txt";//"Berlin_1_256.map";
const std::string SCENARIO_FILE_NAME = "miniscen.txt";//"Berlin_1_256-even-1.scen";

int main(int argc, char* argv[]) {
    Map g = readMap(MAP_FILE_NAME);
    Scenario s = readScenario(SCENARIO_FILE_NAME);
    std::vector<int>  prior = {1, 2};
    ScenarioResult scenarioResult = PPAStar(prior).run(g, s);
    writeScenarioResult(SCENARIO_FILE_NAME + ".ans", scenarioResult);
    return 0;
}
