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
#include <map>
#include <set>

#include "heur_rounding.h"

using namespace std;

#define HEUR_NAME             "Heu1+Scip"
#define HEUR_DESC             "Scip using Heuristic1"
#define HEUR_DISPCHAR         'H'
#define HEUR_PRIORITY         100
#define HEUR_FREQ             1
#define HEUR_FREQOFS          0
#define HEUR_MAXDEPTH         -1
#define HEUR_TIMING           SCIP_HEURTIMING_DURINGLPLOOP //(0xFFFFFFFF & (1 << 1))
#define HEUR_USESSUBSCIP      FALSE  /**< does the heuristic use a secondary SCIP instance? */
#define HEUR_CHECK_UNTOUCHABLES
#define HEUR_REC_D 4

struct ScipVarLabelData
{
    Label* labelPtr;
    Label::Pos pos;
    int posVal;

    ScipVarLabelData(void)
    {
        labelPtr = NULL;
        pos = Label::Pos::tl;
    }
};

struct LabelVars
{
    SCIP_VAR* vars[4];

    LabelVars(void)
    {

    }

    LabelVars(const LabelVars& v)
    {
        for(int i =0 ; i < 4; ++i)
        {
            vars[i] = v.vars[i];
        }
    }
};

class Heu1PlusScip : public scip::ObjHeur
{
private:
    vector<Label>& labels;
    vector<vector<SCIP_VAR*>>& vars;
    Heuristic1* heu;
    Heuristic2* heu2;
    SCIP_HEUR* roundingHeu;

public:
    Heu1PlusScip(SCIP* scip, vector<Label>& _labels, vector<vector<SCIP_VAR*>>& vars)
        :ObjHeur(scip, HEUR_NAME, HEUR_DESC, HEUR_DISPCHAR, HEUR_PRIORITY, HEUR_FREQ, HEUR_FREQOFS, HEUR_MAXDEPTH, HEUR_TIMING, HEUR_USESSUBSCIP), 
        labels(_labels), vars(vars), heu(NULL), heu2(NULL)
    {
        heu = new Heuristic1(labels, HEUR_REC_D);
        heu2 = new Heuristic2(labels);
    }

    void callHeuristic(void)
    {
        int count = 0;
         do 
         {
            count = 0;
            for(auto& l : labels)
            {
                if(l.b() == 0)
                {
                    count += heu2->tryToEnable(l);
                }
            }
         } while (count > 0);
    }

    void addScipSolutionFromHeur(SCIP* scip)
    {
        try
        {
            SCIP_SOL* sol;
            SCIP_CALL_EXC(SCIPcreateSol(scip, &sol, 0));

            for(int i = 0; i < (int)labels.size(); i++)
            {
                int pos = -1;
                if(labels[i].b() == 1)
                {
                    switch(labels[i].getPos())
                    {
                    case Label::tl: pos = 0; break;
                    case Label::bl: pos = 1; break;
                    case Label::br: pos = 2; break;
                    case Label::tr: pos = 3; break;
                    default:    break;
                    }
                }
                for(int j = 0; j < 4; j++)
                {                    
                    SCIP_VAR* var = vars[i][j];
                    /*if(!SCIPvarIsActive(SCIPvarGetProbvar(var)) && (SCIPgetSolVal(scip, sol, var) != (j==pos)))
                    {
                        cout << "ERROR" << endl;
                    } */
                    SCIP_CALL_EXC(SCIPsetSolVal(scip, sol, vars[i][j], j==pos));
                }
            }
            SCIP_Bool stored;
            SCIP_CALL_EXC(SCIPaddSolFree(scip, &sol, &stored));
        }
        catch(SCIPException& e)
        {
            cerr << e.what() << endl;
            exit(e.getRetcode());
        }
    }

    SCIP_DECL_HEURINIT(scip_init)
    {
        SCIP_CALL_EXC(SCIPincludeHeurRounding(scip, &roundingHeu));

        heurInitRounding(scip, roundingHeu);

        callHeuristic();

        addScipSolutionFromHeur(scip);

        cout << "Heur1 init" << endl;

        return SCIP_OKAY;
    }

    SCIP_SOL* sol;
    SCIP_DECL_HEURINIT(scip_initsol)
    {
        heurInitsolRounding(scip, roundingHeu);

        sol = RoundingHeuGetSolution();
        //SCIP_CALL_EXC(SCIPlinkLPSol(scip, sol));
        return SCIP_OKAY;
    }

    void copyScipStateToLabels(Scip* scip)
    {
        for(auto& it : labels)
        {
            it._isFixed = true;
            it.disable();
        }

        for(int i = 0; i < labels.size(); ++i)
        {
            Label* lp = &labels[i];
            for(int j = 0; j < 4; ++j)
            {
                SCIP_VAR* v = vars[i][j];
                ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(v);

                SCIP_Real val = SCIPgetSolVal(scip, sol, v);

                if(SCIPisFeasIntegral(scip, val) && val > 0.5)
                {
                    lp->enable();
                    lp->setPos(data->pos);
                    data->posVal = 1;
                    break;
                }
                else
                {
                    data->posVal = 0;
                }
            }
        }

        SCIP_VAR** _vars = SCIPgetVars(scip);
        int ncount = SCIPgetNVars(scip);
        //set touchies
        for(int i = 0; i < ncount; ++i)
        {
            SCIP_VAR* var = _vars[i];
            ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(var);
            Label* lp = data->labelPtr;

            lp->_isFixed = false;

            Label::Pos pos = data->pos;

            SCIP_Real val = SCIPgetSolVal(scip, sol, var);

            if(SCIPisFeasIntegral(scip, val) && val > 0.5)
            {
                lp->enable();
                lp->setPos(pos);
            }
        }
    }

    SCIP_DECL_HEUREXEC(scip_exec)
    {
        try
        {
            *result = SCIP_DIDNOTRUN;

            //!SCIPisRelaxSolValid(scip) immer true...

            if(SCIPgetLPSolstat(scip) != SCIP_LPSOLSTAT_OPTIMAL)
            {
                cout << "current LP State not optimal: canceling heuristic" << endl;
                return SCIP_OKAY;
            }

            //get current feasible solutions
            int feasSolsN = SCIPgetNSols(scip);

           // cout << "current feasible solutions: " << feasSolsN << endl;

            if(feasSolsN <= 0)
            {
                return SCIP_OKAY;
            }

            SCIP_RESULT res;
            heurExecRounding(scip, roundingHeu, HEUR_TIMING, &res);

            if(res == SCIP_DIDNOTFIND)
            {
                *result = SCIP_DIDNOTFIND;
                return SCIP_OKAY;
            }

            //cout << "rounding heur found a solution..." << endl;

            copyScipStateToLabels(scip);

            vector<Label*> untouchables;
            for(Label& l : labels)
            {
                if(l._isFixed)
                {
                    untouchables.push_back(&l);
                }
            }

            //write back
            double bound = SCIPgetPrimalbound(scip);
            //cout << "current PB: " << bound << endl; 
            int lce = 0;
            for(Label& l : labels)
            {
                lce += l.b();
            }

            SCIP_VAR** _vars = SCIPgetVars(scip);
            int ncount = SCIPgetNVars(scip);

            //cout << "labels enabled: " << lce << endl;
            int count = 0;
            do 
            {
                count = 0;
                for(int i = 0; i < ncount; i++)
                {
                    SCIP_VAR* var = _vars[i];
                    ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(var);
                    if(data->labelPtr->b() == 0)
                    {
                        count += heu->tryToEnable(*data->labelPtr, &untouchables);
                        SCIP_CALL_EXC(SCIPsetSolVal(scip, sol, _vars[i], data->posVal));
                    }
                }

                if(count)
                {
                    //cout << "could enable: " << count << endl;
                }
            } while (count > 0);

            SCIP_Bool stored = 0;
            SCIP_CALL_EXC(SCIPtrySol(scip, sol, FALSE, FALSE, FALSE, TRUE, &stored));

            if(!stored)
            {
                //cout << "storing solution failed!";
            }
            else
            {
                double newBound = SCIPgetPrimalbound(scip);
                if(newBound > bound)
                {
                    cout << "new PB: " << bound; 
                    cout << endl << endl;
                }
            }

            *result = SCIP_FOUNDSOL;
        }
        catch(SCIPException& e)
        {
            cerr << e.what() << endl;
            exit(e.getRetcode());
        }

        return SCIP_OKAY;


        //******************************** other tries...

        SCIP_VAR** lpcands;
        SCIP_Real* lpcandssol;
        int nlpcands;

        /* get fractional variables, that should be integral */
        SCIP_CALL(SCIPgetLPBranchCands(scip, &lpcands, &lpcandssol, NULL, &nlpcands, NULL));

        /* only call heuristic, if LP solution is fractional */
        if(nlpcands == 0)
        {
            return SCIP_OKAY;
        }

        return SCIP_OKAY;
    }

    ~Heu1PlusScip(void)
    {
        if(heu)
        {
            delete heu;
            delete heu2;
        }
    }
};


#endif
