#include "MapReader.h"

#include <fstream>

Map MapReader::readMap(const std::string &fileName) {
    std::ifstream in(fileName);
    std::string tmp;
    in >> tmp >> tmp >> tmp;
    int height, width;
    in >> height >> tmp >> width;
    in >> tmp;
    std::vector<std::string> map(height);
    for (int i = 0; i < height; ++i) {
        in >> map[i];
    }
    in.close();
    return Map(map);
}