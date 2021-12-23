#include "PPAStarParam.h"

std::vector<int> PPAStarParam::getPrior(Scenario &scen) const {
    switch (priority) {
        case (Priority::RANDOM):
            return getRandomPrior(scen);
        case (Priority::LONGEST_FIRST):
            return getLongestFirstPrior(scen);
        case (Priority::SHORTEST_FIRST):
            return getShortestFirstPrior(scen);
    }
}

std::vector<int> PPAStarParam::getRandomPrior(Scenario &scen) {
    std::vector<int> res(scen.agents.size());
    for (int &re : res) {
        re = rand();
    }
    return res;
}
int getPathLength(Cell start, Cell end) {
    return abs(start.first - end.first) + abs(start.second - end.second);
}

std::vector<int> PPAStarParam::getLongestFirstPrior(Scenario &scen) {
    std::vector<int> res(scen.agents.size());
    std::vector<std::pair<int, int>> tmp(scen.agents.size());
    for (int i = 0; i < res.size(); ++i) {
        tmp[i] = std::make_pair(getPathLength(scen.agents[i].start, scen.agents[i].end), i);
    }
    sort(tmp.begin(), tmp.end());
    int cnt = 0;
    for (int i = 0; i < res.size(); ++i) {
        res[tmp[i].second] = cnt++;
    }
    return res;
}

std::vector<int> PPAStarParam::getShortestFirstPrior(Scenario &scen) {
    std::vector<int> res(scen.agents.size());
    std::vector<std::pair<int, int>> tmp(scen.agents.size());
    for (int i = 0; i < res.size(); ++i) {
        tmp[i] = std::make_pair(getPathLength(scen.agents[i].start, scen.agents[i].end), i);
    }
    sort(tmp.begin(), tmp.end());
    reverse(tmp.begin(), tmp.end());
    int cnt = 0;
    for (int i = 0; i < res.size(); ++i) {
        res[tmp[i].second] = cnt++;
    }
    return res;
}
