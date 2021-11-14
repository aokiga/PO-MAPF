//
// Created by Gleb.Oborin on 10/20/21.
//

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

    static double h(Cell s, Cell t);
    double h(int s, int t) const;
    static double distance(Cell s, Cell t);
    double distance(int s, int t) const;
};


#endif //PO_MAPF_MAP_H
