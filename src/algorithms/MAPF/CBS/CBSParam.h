#ifndef PO_MAPF_CBSPARAM_H
#define PO_MAPF_CBSPARAM_H


#include "../MAPFParam.h"

enum class ICBS_OPTIMIZATIONS {
    NONE,
    PC,
    H
};

enum class CHOOSE_CONFLICT_STRATEGY {
    FIRST,
    RANDOM,
    WIDTH
};

class CBSParam: public MAPFParam {
public:
    std::string getName() override {
        return "CBS";
    }

    ICBS_OPTIMIZATIONS ICBSOptimization = ICBS_OPTIMIZATIONS::NONE;

    CHOOSE_CONFLICT_STRATEGY chooseConflictStrategy = CHOOSE_CONFLICT_STRATEGY::FIRST;

    bool withDS = false;

    CBSParam() = default;

    CBSParam(CBSParam &other) {
        ICBSOptimization = other.ICBSOptimization;
        withDS = other.withDS;
    }
};


#endif //PO_MAPF_CBSPARAM_H
