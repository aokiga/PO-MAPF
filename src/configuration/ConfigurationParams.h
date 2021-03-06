#ifndef PO_MAPF_CONFIGURATIONPARAMS_H
#define PO_MAPF_CONFIGURATIONPARAMS_H

class ConfigurationParams {
public:
    virtual std::string getType() = 0;
    std::string mapFileName {};
    std::string pathToData {};
    int moveLimit = 10000;
    std::string scenFileName = "";
};

#endif //PO_MAPF_CONFIGURATIONPARAMS_H
