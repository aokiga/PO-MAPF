#include "CGHeuristic.h"

#include <map>
#include <iostream>

bool CGHeuristic::isCover(std::vector<std::vector<int>> &graph, int k, int m) {
    int n = graph.size();
    int mask = (1 << k) - 1;
    int lim = (1 << n);
    while (mask < lim) {
        std::vector<std::vector<int>> tmp(n, std::vector<int>(n, 0));
        for (int j = 1, v = 1; j < lim; j <<= 1, v++) {
            if (mask & j) {
                for (auto to : graph[v]) {
                    tmp[v][to] = 1;
                    tmp[to][v] = 1;
                }
            }
        }
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                sum += tmp[i][j];
            }
        }
        if (sum == 2 * m) return true;
        int c = mask & -mask;
        int r = mask + c;
        mask = (((r^mask) >> 2) / c) | r;
    }
    return false;
}

int CGHeuristic::findMinCover(std::vector<std::vector<int>> &graph, int e) {
    int left = 1, right = graph.size();
    while (right > left) {
        int m = (left + right) >> 1;
        if (!isCover(graph, m, e))
            left = m + 1;
        else
            right = m;
    }
    return left;
}

int CGHeuristic::count(Conflicts &conflicts) {
    int n = 0, m = 0;
    std::vector<int> revpos(0);
    std::map<int, int> pos;
    std::vector<std::vector<int>> graph;
    for (VertexConflict conflict : conflicts.first) {
        if (pos.count(conflict.a1) == 0) {
            pos[conflict.a1] = n++;
            revpos.push_back(conflict.a1);
            graph.push_back(std::vector<int>());
        }
        int p1 = pos[conflict.a1];
        if (pos.count(conflict.a2) == 0) {
            pos[conflict.a2] = n++;
            revpos.push_back(conflict.a2);
            graph.push_back(std::vector<int>());
        }
        int p2 = pos[conflict.a2];
        graph[p1].push_back(p2);
        graph[p2].push_back(p1);
        ++m;
    }
    for (EdgeConflict conflict : conflicts.second) {
        if (pos.count(conflict.a1) == 0) {
            pos[conflict.a1] = n++;
            revpos.push_back(conflict.a1);
            graph.push_back(std::vector<int>());
        }
        int p1 = pos[conflict.a1];
        if (pos.count(conflict.a2) == 0) {
            pos[conflict.a2] = n++;
            revpos.push_back(conflict.a2);
            graph.push_back(std::vector<int>());
        }
        int p2 = pos[conflict.a2];
        graph[p1].push_back(p2);
        graph[p2].push_back(p1);
        ++m;
    }
    int res = findMinCover(graph, m);
    std::cerr << res << '\n';
    return res;
}
