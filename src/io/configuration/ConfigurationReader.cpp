#include "ConfigurationReader.h"
#include "algorithms/PPAStar/PPAStarParamReader.h"
#include "../../algorithms/MAPF/CBS/CBSParam.h"
#include "algorithms/CBS/CBSParamReader.h"
#include "../../configuration/BenchmarkParams.h"
#include "../../algorithms/PO-MAPF/POHCAStarParam.h"
#include "algorithms/MPPAStar/POHCAStarParamReader.h"
#include <fstream>
#include <string>
#include <iostream>

ConfigurationParams* ConfigurationReader::readConfigurationFile(const std::string& fileName) {
    ConfigurationParams *res = nullptr;
    std::ifstream in(fileName);
    std::string tmp;
    in >> tmp;
    if (tmp == "LAUNCH") {
        in >> tmp;
        res = new LaunchParams(readLaunchParams(in));
    }
    if (tmp == "BENCHMARK") {
        in >> tmp;
        res = new BenchmarkParams(readBenchmarkParams(in));
    }
    in.close();
    return res;
}

LaunchParams ConfigurationReader::readLaunchParams(std::ifstream &in) {
    LaunchParams res;

    std::string tmp;
    while(true) {
        in >> tmp;
        if (tmp == "}") {
            break;
        }
        if (tmp == "MAP_FILE_NAME") {
            in >> res.mapFileName;
            continue;
        }
        if (tmp == "SCEN_FILE_NAME") {
            in >> res.scenFileName;
            continue;
        }
        if (tmp == "MOVE_LIMIT") {
            in >> res.moveLimit;
            continue;
        }
        if (tmp == "TMP_PATH") {
            in >> res.pathToData;
            continue;
        }
        if (tmp == "ALGORITHM") {
            std::string name;
            in >> name;
            if (name == "CBS") {
                res.param = new CBSParam(CBSParamReader::readParam(in));
                continue;
            }
            if (name == "PPAStar") {
                res.param = new PPAStarParam(PPAStarParamReader::readParam(in));
                continue;
            }
            if (name == "MPPAStar") {
                res.param = new POHCAStarParam(POHCAStarParamReader::readParam(in));
                continue;
            }
        }
    }
    return res;
}

BenchmarkParams ConfigurationReader::readBenchmarkParams(std::ifstream &in) {
    BenchmarkParams res;

    std::string tmp;

    while(true) {
        in >> tmp;
        if (tmp == "}") {
            break;
        }
        if (tmp == "MAP_FILE_NAME") {
            in >> res.mapFileName;
            continue;
        }
        if (tmp == "SCEN_FILE_NAME") {
            in >> res.scenFileName;
            continue;
        }
        if (tmp == "MOVE_LIMIT") {
            in >> res.moveLimit;
            continue;
        }
        if (tmp == "TMP_PATH") {
            in >> res.pathToData;
            continue;
        }
        if (tmp == "NLAUNCH") {
            in >> res.NLaunch;
            continue;
        }
        if (tmp == "MAX_AGENT_NUM") {
            in >> res.NMaxAgents;
            continue;
        }
        if (tmp == "NAGENTS") {
            std::string tmpp;
            in >> tmpp;
            while (true) {
                in >> tmpp;
                if (tmpp == "}") {
                    break;
                }
                int x = std::stoi(tmpp);
                res.NAgents.push_back(x);
            }
            continue;
        }
        if (tmp == "ADD_AGENTS") {
            in >> res.addAgents;
            continue;
        }
        if (tmp == "ALGORITHM") {
            std::string name;
            in >> name;
            if (name == "CBS") {
                res.params.push_back(new CBSParam(CBSParamReader::readParam(in)));
                continue;
            }
            if (name == "PPAStar") {
                res.params.push_back(new PPAStarParam(PPAStarParamReader::readParam(in)));
                continue;
            }
            if (name == "POHCAStar") {
                res.params.push_back(new POHCAStarParam(POHCAStarParamReader::readParam(in)));
                continue;
            }
        }
    }
    return res;
}
