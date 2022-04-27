#include "MPPAStarParamReader.h"

#include <string>
#include <fstream>

MPPAStarParam MPPAStarParamReader::readParam(std::ifstream &in) {
    MPPAStarParam res;
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
        if (tmp == "SEND_MAP") {
            res.sendMapFlag = true;
            continue;
        }
        if (tmp == "SEND_PATH") {
            res.sendPathFlag = true;
            continue;
        }
        if (tmp == "SAVE_PATHS") {
            res.savePathsFlag = true;
            continue;
        }
        if (tmp == "FAST_REPLANNING") {
            res.fastReplanningFlag = true;
            continue;
        }
        if (tmp == "EXCHANGE_MODE") {
            std::string prior;
            in >> prior;
            if (prior == "ALL") {
                res.exchangeMode = ExchangeMode::ALL;
                continue;
            }
            if (prior == "VISIBLE_CHAIN") {
                res.exchangeMode = ExchangeMode::VISIBLE_CHAIN;
                continue;
            }
            if (prior == "ONLY_VISIBLE") {
                res.exchangeMode = ExchangeMode::ONLY_VISIBLE;
                continue;
            }
        }
        if (tmp == "CONFLICT_RESOLUTION_MODE") {
            std::string prior;
            in >> prior;
            if (prior == "NONE") {
                res.conflictResolutionMode = ConflictResolutionMode::NONE;
                continue;
            }
            if (prior == "RANDOM") {
                res.conflictResolutionMode = ConflictResolutionMode::RANDOM;
                continue;
            }
            if (prior == "BEST") {
                res.conflictResolutionMode = ConflictResolutionMode::BEST;
                continue;
            }

            if (prior == "WAIT") {
                res.conflictResolutionMode = ConflictResolutionMode::WAIT;
                continue;
            }
        }

        if (tmp == "NO_PATH_STRATEGY") {
            std::string prior;
            in >> prior;
            if (prior == "AGENT") {
                res.noPathStrategy = NoPathStrategy::AGENT;
                continue;
            }
            if (prior == "SQUARE") {
                res.noPathStrategy = NoPathStrategy::SQUARE;
                continue;
            }
            if (prior == "POSITION") {
                res.noPathStrategy = NoPathStrategy::POSITION;
                continue;
            }
        }

        if (tmp == "RADIUS") {
            int prior;
            in >> prior;
            res.visionRadius = prior;
            continue;
        }
    }
    return res;
}