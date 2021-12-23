#ifndef PO_MAPF_MAPFPARAM_H
#define PO_MAPF_MAPFPARAM_H


#include <string>

class MAPFParam {
public:
    virtual std::string getName() = 0;
    std::string name;
};


#endif //PO_MAPF_MAPFPARAM_H
