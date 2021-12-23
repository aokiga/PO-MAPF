#include "TreeNode.h"

bool operator < (TreeNode a, TreeNode b) {
    if (a.f != b.f) return a.f < b.f;
    if (a.g != b.g) return a.g > b.g;
    return a.v < b.v;
}