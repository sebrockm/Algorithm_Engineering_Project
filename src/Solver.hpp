#ifndef _SOLVER_HPP_
#define _SOLVER_HPP_


#include <vector>

#include "scip/scip.h"

#include "Label.hpp"
#include "Heu1PlusScip.hpp"


using namespace std;

class Solver
{
private:
    vector<Label>& labels;
    SCIP* scip;
    vector<vector<SCIP_VAR*>> vars;
    vector<ScipVarLabelData> varData;
    vector<SCIP_CONS*> positionCons;
    vector<SCIP_CONS*> crossCons;
    Heu1PlusScip* heu;
    void printStatus(SCIP_Status s) const;

public:
    Solver(vector<Label>& labels);
    ~Solver();
    int solve();
};

#endif
