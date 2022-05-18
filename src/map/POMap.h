#ifndef PO_MAPF_POMAP_H
#define PO_MAPF_POMAP_H

#include <unordered_set>
#include "Map.h"

class POMap: public Map {
public:
    std::unordered_set<int> isVisible;
    std::vector<bool> isKnown;

    explicit POMap(std::vector<std::string> &map): Map(map) {
        isKnown.assign(this->size, false);
    }

    explicit POMap(Map &m) {
        this->height = m.height;
        this->width = m.width;
        this->size = width * height;
        this->g.resize(size);
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                int v = i * width + j;
                g[v] = m.g[v];
            }
        }

        isKnown.assign(this->size, false);
    }

    static bool isCellVisible(Cell a, Cell b, int radius);

    std::vector<int> get_neighbors(int v) override;

    std::vector<int> make_visible(int v, int radius);

    bool is_correct(int v);
};


#endif //PO_MAPF_POMAP_H
