#ifndef PO_MAPF_MAP_H
#define PO_MAPF_MAP_H

#include "Graph.h"

typedef std::pair<int, int> Cell;

class Map: public Graph {
public:
    int width { 0 }, height { 0 };
    explicit Map(std::vector<std::string> &map);

    int cellToInt(Cell c) const;
    Cell intToCell(int x) const;

    static int h(Cell s, Cell t);
    int h(int s, int t) const;
};


#endif //PO_MAPF_MAP_H
