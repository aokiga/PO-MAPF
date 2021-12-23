#include "Node.h"

Node::Node(char c) {
    if (c == '.') {
        this->isObstacle = false;
    } else {
        this->isObstacle = true;
    }
}
