//
// Created by Gleb.Oborin on 11/4/21.
//

#ifndef PO_MAPF_PATHPLANNINGWITHCONSTRAINTS_H
#define PO_MAPF_PATHPLANNINGWITHCONSTRAINTS_H

#include <set>
#include "../scen/Agent.h"
#include "../scen/AgentResult.h"

typedef std::pair<int, int> state;

class PathPlanningWithConstraints {
public:
    static AgentResult computePath(Map &g, Agent &a, std::set<state> &constraints);
};


#endif //PO_MAPF_PATHPLANNINGWITHCONSTRAINTS_H
