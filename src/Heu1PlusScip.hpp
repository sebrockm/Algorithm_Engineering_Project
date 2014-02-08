#ifndef _HEU1PLUSSCIP_HPP_
#define _HEU1PLUSSCIP_HPP_

#include "scip/scip.h"
#include "objscip/objheur.h"
#include "Heuristic1.hpp"
#include "Heuristic2.hpp"
#include "Label.hpp"
#include "scip_exception.hpp"
#include "crossing.hpp"

#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>

//#include "heur_rounding.h"

using namespace std;

#define HEUR_NAME             "Heu1+Scip"
#define HEUR_DESC             "Scip using Heuristic1"
#define HEUR_DISPCHAR         'H'
#define HEUR_PRIORITY         1
#define HEUR_FREQ             20
#define HEUR_FREQOFS          0
#define HEUR_MAXDEPTH         -1
#define HEUR_TIMING           SCIP_HEURTIMING_AFTERNODE//(0xFFFFFFFF & (1 << 1))
#define HEUR_USESSUBSCIP      FALSE  /**< does the heuristic use a secondary SCIP instance? */
#define HEUR_CHECK_UNTOUCHABLES
#define HEUR_REC_D 4

struct ScipVarLabelData
{
    Label* labelPtr;
    Label::Pos pos;

    ScipVarLabelData(void)
    {
        labelPtr = NULL;
        pos = (Label::Pos)0;
    }
};


class Heu1PlusScip : public scip::ObjHeur
{
private:
    vector<Label>& labels;
    vector<vector<SCIP_VAR*>>& vars;
    Heuristic1 heu;
    Heuristic2 heu2;

    void callHeuristic(vector<Label*>* ut = NULL);
    SCIP_Bool copyLabelsToScipState(SCIP* scip, SCIP_HEUR* heur = NULL);
    void copyScipStateToLabels(Scip* scip);

public:
    Heu1PlusScip(SCIP* scip, vector<Label>& _labels, vector<vector<SCIP_VAR*>>& vars);

    SCIP_DECL_HEURINIT(scip_init);

    SCIP_DECL_HEURINIT(scip_initsol);

    SCIP_DECL_HEUREXEC(scip_exec);

    ~Heu1PlusScip(void);
};


#endif
