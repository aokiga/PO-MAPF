#include "PPAStarParamReader.h"

#include <fstream>

PPAStarParam PPAStarParamReader::readParam(std::ifstream &in) {
    PPAStarParam res;
    std::string tmp;
    in >> tmp;
    while (true) {
        in >> tmp;
        if (tmp == "}") {
            break;
        }
        if (tmp == "NAME") {
            std::string prior;
            in >> prior;
            res.name = prior;
            continue;
        }
        if (tmp == "PRIORITY") {
            std::string prior;
            in >> prior;
            if (prior == "RANDOM") {
                res.priority = Priority::RANDOM;
                continue;
            }
            if (prior == "LONGEST_FIRST") {
                res.priority = Priority::LONGEST_FIRST;
                continue;
            }
            if (prior == "SHORTEST_FIRST") {
                res.priority = Priority::SHORTEST_FIRST;
                continue;
            }
        }
    }
    return res;
}
