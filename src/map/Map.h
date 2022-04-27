#ifndef PO_MAPF_MAP_H
#define PO_MAPF_MAP_H

#include "Graph.h"
#include <iostream>

typedef std::pair<int, int> Cell;

std::ostream& operator<<(std::ostream& os, const Cell& dt);

class Map: public Graph {
public:
    int width { 0 }, height { 0 };
    explicit Map(std::vector<std::string> &map);

    Map() = default;

    int cellToInt(Cell c) const;
    Cell intToCell(int x) const;

    static int h(Cell s, Cell t);
    int h(int s, int t) const;

    std::vector<int> get_neighbors(int v) override;
};


#endif //PO_MAPF_MAP_H
