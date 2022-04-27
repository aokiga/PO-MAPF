#ifndef PO_MAPF_MPPASTARPARAMREADER_H
#define PO_MAPF_MPPASTARPARAMREADER_H

#include "../../../../algorithms/PO-MAPF/Centralized/MPPAStarParam.h"
#include <fstream>

class MPPAStarParamReader {
public:
    static MPPAStarParam readParam(std::ifstream &in);
};


#endif //PO_MAPF_MPPASTARPARAMREADER_H
