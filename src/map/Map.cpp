#include "Map.h"
#include <cmath>

Map::Map(std::vector<std::string> &map) {
    this->height = map.size();
    this->width = map[0].size();
    this->size = width * height;
    this->g.resize(size);
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
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int v = i * width + j;
            g[v] = Node(map[i][j]);
            g[v].neighbors.push_back(v);
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int v = i * width + j;
            if (g[v].isObstacle) continue;
            for (auto x : delta) {
                int ni = i + x.first;
                int nj = j + x.second;
                if (ni < 0 || nj < 0 || ni >= height || nj >= width) continue;
                int nv = ni * width + nj;
                if (!g[nv].isObstacle) {
                    g[v].neighbors.push_back(nv);
                }
            }
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