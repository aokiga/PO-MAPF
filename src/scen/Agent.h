//
// Created by Gleb.Oborin on 10/20/21.
//

#include "../map/Map.h"

#ifndef PO_MAPF_AGENT_H
#define PO_MAPF_AGENT_H

class Agent {
public:
    Cell start, end;
    int i;
    double answer;
    Agent(int i, Cell s, Cell t, double ans) : i(i), start(s), end(t), answer(ans) {}
};


#endif //PO_MAPF_AGENT_H
