#ifndef PO_MAPF_PPASTARPARAMREADER_H
#define PO_MAPF_PPASTARPARAMREADER_H

#include "../../../../algorithms/MAPF/PPAStar/PPAStarParam.h"
#include <fstream>

class PPAStarParamReader {
public:
    static PPAStarParam readParam(std::ifstream &in);
};


#endif //PO_MAPF_PPASTARPARAMREADER_H
