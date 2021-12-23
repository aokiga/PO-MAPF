#ifndef PO_MAPF_MAPREADER_H
#define PO_MAPF_MAPREADER_H

#include "../map/Map.h"

class MapReader {
public:
    static Map readMap(const std::string &fileName);
};
#endif //PO_MAPF_MAPREADER_H
