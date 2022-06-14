#ifndef PO_MAPF_POHCASTARPARAMREADER_H
#define PO_MAPF_POHCASTARPARAMREADER_H

#include "../../../../algorithms/PO-MAPF/POHCAStarParam.h"
#include <fstream>

class POHCAStarParamReader {
public:
    static POHCAStarParam readParam(std::ifstream &in);
};


#endif //PO_MAPF_POHCASTARPARAMREADER_H
