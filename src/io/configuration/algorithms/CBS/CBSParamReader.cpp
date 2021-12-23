#include "CBSParamReader.h"

CBSParam CBSParamReader::readParam(std::ifstream &in) {
    CBSParam res;
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
        if (tmp == "ICBS_OPTIMIZATION") {
            std::string prior;
            in >> prior;
            if (prior == "NONE") {
                res.ICBSOptimization = ICBS_OPTIMIZATIONS::NONE;
                continue;
            }
            if (prior == "PC") {
                res.ICBSOptimization = ICBS_OPTIMIZATIONS::PC;
                continue;
            }
            if (prior == "H") {
                res.ICBSOptimization = ICBS_OPTIMIZATIONS::H;
                continue;
            }
        }
        if (tmp == "WITH_DS") {
            res.withDS = true;
            continue;
        }
        if (tmp == "CHOOSE_CONFLICT_STRATEGY") {
            std::string prior;
            in >> prior;
            if (prior == "FIRST") {
                res.chooseConflictStrategy = CHOOSE_CONFLICT_STRATEGY::FIRST;
                continue;
            }
            if (prior == "RANDOM") {
                res.chooseConflictStrategy = CHOOSE_CONFLICT_STRATEGY::RANDOM;
                continue;
            }
            if (prior == "WIDTH") {
                res.chooseConflictStrategy = CHOOSE_CONFLICT_STRATEGY::WIDTH;
                continue;
            }
        }
    }
    return res;
}
