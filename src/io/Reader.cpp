//
// Created by Gleb.Oborin on 10/20/21.
//

#include "Reader.h"

Map readMap(const std::string &fileName) {
    std::ifstream in(fileName);
    std::string tmp;
    in >> tmp >> tmp >> tmp;
    int height, width;
    in >> height >> tmp >> width;
    in >> tmp;
    std::vector<std::string> map(height);
    for (int i = 0; i < height; ++i) {
        in >> map[i];
    }
    in.close();
    return Map(map);
}

Scenario readScenario(const std::string &fileName) {
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
                std::make_pair(p2x, p2y),
                ans
        ));
        ++i;
    }
    in.close();
    return Scenario(agents);
}
