#ifndef PO_MAPF_CONSTRAINTS_H
#define PO_MAPF_CONSTRAINTS_H

#include <unordered_set>
#include "VertexState.h"

typedef std::unordered_set<VertexState, hash_VertexState> VertexConstraints;
typedef std::unordered_set<EdgeState, hash_EdgeState> EdgeConstraints;

//typedef std::set<VertexState> VertexConstraints;
//typedef std::set<EdgeState> EdgeConstraints;


#endif //PO_MAPF_CONSTRAINTS_H
