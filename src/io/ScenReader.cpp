#include "ScenReader.h"
#include <fstream>

Scenario ScenReader::readScenario(const std::string &fileName) {
    std::ifstream in(fileName);
    std::string tmp;
    in >> tmp >> tmp;
    std::vector<Agent> agents;
    int i = 0;
    while (in >> tmp) {
        in >> tmp >> tmp >> tmp;
        int p1x, p1y;
        in >> p1y >> p1x;
        int p2x, p2y;
        in >> p2y >> p2x;
        double ans;
        in >> ans;
        agents.emplace_back(Agent(i,
                                  std::make_pair(p1x, p1y),
                                  std::make_pair(p2x, p2y)
        ));
        ++i;
    }
    in.close();
    return Scenario(agents);
}
