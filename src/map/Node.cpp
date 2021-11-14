//
// Created by Gleb.Oborin on 10/20/21.
//

#include "Node.h"

Node::Node(char c) {
    if (c == '.') {
        this->isObstacle = false;
    } else {
        this->isObstacle = true;
    }
}
