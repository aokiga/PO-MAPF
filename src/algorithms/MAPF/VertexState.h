#ifndef PO_MAPF_VERTEXSTATE_H
#define PO_MAPF_VERTEXSTATE_H

#include <tuple>

typedef std::pair<int, int> VertexState;
typedef std::tuple<int, int, int> EdgeState;

inline size_t hash_combine( size_t lhs, size_t rhs ) {
    lhs ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
    return lhs;
}

struct hash_VertexState {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const
    {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash_combine(hash1, hash2);
    }
};

struct hash_EdgeState {
    template <class T1, class T2, class T3>
    size_t operator()(const std::tuple<T1, T2, T3>& p) const
    {
        auto hash1 = std::hash<T1>{}(std::get<0>(p));
        auto hash2 = std::hash<T2>{}(std::get<1>(p));
        auto hash3 = std::hash<T3>{}(std::get<2>(p));
        return hash_combine(hash1, hash_combine(hash2, hash3));
    }
};

class VertexInfo {
public:
    int g{}, h{}, f{};

    VertexInfo() = default;

    VertexInfo(int _g, int _h) : g(_g), h(_h) {
        f = g + h;
    }
};

bool operator < (VertexInfo a, VertexInfo b);

#endif //PO_MAPF_VERTEXSTATE_H
