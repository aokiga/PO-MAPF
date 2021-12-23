#ifndef PO_MAPF_TREENODE_H
#define PO_MAPF_TREENODE_H


class TreeNode {
public:
    int v {}, p = -1;
    int g {}, h {}, f {};
    TreeNode() = default;
    TreeNode(int _v, int _g, int _h = 0, int _p = -1) : v(_v), g(_g), h(_h), f(0), p(_p) {
        f = g + h;
    }
};

bool operator < (TreeNode a, TreeNode b);

#endif //PO_MAPF_TREENODE_H
