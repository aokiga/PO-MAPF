#ifndef PO_MAPF_CONFIGURATIONREADER_H
#define PO_MAPF_CONFIGURATIONREADER_H


#include "../../configuration/LaunchParams.h"
#include "../../configuration/ConfigurationParams.h"
#include "../../configuration/BenchmarkParams.h"
#include <string>

class ConfigurationReader {
private:
    static LaunchParams readLaunchParams(std::ifstream &in);
    static BenchmarkParams readBenchmarkParams(std::ifstream &in);
public:
    static ConfigurationParams* readConfigurationFile(const std::string &fileName);
};


#endif //PO_MAPF_CONFIGURATIONREADER_H
