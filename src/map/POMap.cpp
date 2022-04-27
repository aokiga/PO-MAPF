#include <iostream>
#include "POMap.h"
#include <cmath>

std::vector<int> POMap::get_neighbors(int v) {
    std::vector<std::pair<int, int>> delta = {
            std::make_pair(-1, 0),
            std::make_pair(0, -1),
            std::make_pair(1, 0),
            std::make_pair(0, 1),
            //std::make_pair(-1, -1),
            //std::make_pair(1, -1),
            //std::make_pair(-1, 1),
            //std::make_pair(1, 1),
    };
    std::vector<int> neighbors = {};
    neighbors.push_back(v);
    auto c = this->intToCell(v);
        for (auto x: delta) {
            int ni = c.first + x.first;
            int nj = c.second + x.second;
            if (ni < 0 || nj < 0 || ni >= height || nj >= width) continue;
            int nv = ni * width + nj;
            if (!isKnown[nv]) {
                neighbors.push_back(nv);
                //std::cerr << g[nv].isObstacle << "\n";
                continue;
            }
            if (!g[nv].isObstacle) {
                neighbors.push_back(nv);
            }
        }
    return neighbors;
}

std::vector<int> POMap::make_visible(int v, int radius) {
    auto c = this->intToCell(v);
    isVisible.clear();
    isVisible.insert(v);
    std::vector<int> res = {};
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            //if (dx * dx + dy * dy > radius * radius) continue;
            int ni = c.first + dx;
            int nj = c.second + dy;
            if (ni < 0 || nj < 0 || ni >= height || nj >= width) continue;
            int nv = ni * width + nj;
            if (!isKnown[nv]) {
                res.push_back(nv);
            }
            isVisible.insert(nv);
            isKnown[nv] = true;
        }
    }
    return res;
}

bool POMap::isCellVisible(Cell a, Cell b, int radius) {
    int dx = a.first - b.first;
    int dy = a.second - b.second;
    //return dx * dx + dy * dy <= radius * radius;
    return abs(dx) <= radius && abs(dy) <= radius;
}

