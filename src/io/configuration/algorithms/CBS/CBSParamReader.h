#ifndef PO_MAPF_CBSPARAMREADER_H
#define PO_MAPF_CBSPARAMREADER_H

#include "../../../../algorithms/MAPF/CBS/CBSParam.h"
#include <fstream>

class CBSParamReader {
public:
    static CBSParam readParam(std::ifstream &in);
};


#endif //PO_MAPF_CBSPARAMREADER_H
