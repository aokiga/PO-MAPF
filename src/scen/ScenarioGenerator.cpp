#include "ScenarioGenerator.h"

#include <unordered_set>
#include <random>
#include <cmath>
#include <chrono>
#include <iostream>

std::mt19937 gg(std::chrono::steady_clock::now().time_since_epoch().count());

Cell generateFreeCell(const Map &m, std::unordered_set<int> &used) {
    while (true) {
        int v = std::abs(int(gg())) % m.size;
        if (m.g[v].isBlocked) continue;
        if (used.count(v)) continue;
        used.insert(v);
        return m.intToCell(v);
    }
}

Scenario ScenarioGenerator::generate(const Map &m, int NAGENTS) {
    std::unordered_set<int> used;
    std::vector<Agent> res = std::vector<Agent>(NAGENTS);
    for (int i = 0; i < NAGENTS; ++i) {
        res[i].i = i;
        res[i].start = generateFreeCell(m, used);
        res[i].end = generateFreeCell(m, used);
    }
    return Scenario(res);
}
