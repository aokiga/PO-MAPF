#include "../map/Map.h"

#ifndef PO_MAPF_AGENT_H
#define PO_MAPF_AGENT_H

class Agent {
public:
    Cell start, end;
    int i;
    Agent(int i, Cell s, Cell t) : i(i), start(s), end(t) {}

    Agent() = default;
};


#endif //PO_MAPF_AGENT_H
