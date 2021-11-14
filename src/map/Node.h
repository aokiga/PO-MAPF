//
// Created by Gleb.Oborin on 10/20/21.
//

#include <vector>

#ifndef PO_MAPF_NODE_H
#define PO_MAPF_NODE_H

class Node {
public:
    bool isObstacle { false }, isBlocked { false };

    std::vector<int> neighbors;

    Node() = default;
    Node(char c);
};


#endif //PO_MAPF_NODE_H
