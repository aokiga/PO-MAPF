#include "Map.h"
#include <cmath>
#include <iostream>
#include <ostream>

std::ostream& operator<<(std::ostream& os, const Cell& dt) {
    os << "(" << dt.first << ", " << dt.second << ")";
    return os;
}

Map::Map(std::vector<std::string> &map) {
    this->height = map.size();
    this->width = map[0].size();
    this->size = width * height;
    this->g.resize(size);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int v = i * width + j;
            g[v] = Node(map[i][j]);
        }
    }
}

int Map::cellToInt(Cell c) const {
    return c.first * width + c.second;
}

Cell Map::intToCell(int x) const {
    return { x / width, x % width };
}

int Map::h(Cell s, Cell t) {
    return abs(s.first - t.first) + abs(s.second - t.second);
}

int Map::h(int s, int t) const {
    return Map::h(intToCell(s), intToCell(t));
}

std::vector<int> Map::get_neighbors(int v) {
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
    if (!g[v].isObstacle) {
        for (auto x: delta) {
            int ni = c.first + x.first;
            int nj = c.second + x.second;
            if (ni < 0 || nj < 0 || ni >= height || nj >= width) continue;
            int nv = ni * width + nj;
            if (!g[nv].isObstacle) {
                neighbors.push_back(nv);
            }
        }
    }
    return neighbors;
}

