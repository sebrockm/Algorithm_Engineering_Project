#ifndef _HEU1PLUSSCIP_HPP_
#define _HEU1PLUSSCIP_HPP_

#include "scip/scip.h"
#include "objscip/objheur.h"
#include "Heuristic1.hpp"
#include "Label.hpp"
#include "scip_exception.hpp"

#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>
#include <map>
#include <set>
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

class Heu1PlusScip : public scip::ObjHeur
{
private:
    vector<Label>& labels;
    vector<vector<SCIP_VAR*>>& vars;
    map<int, Label*> scipVarpToLabelMap;
    Heuristic1* heu;

public:
    Heu1PlusScip(SCIP* scip, vector<Label>& labels, vector<vector<SCIP_VAR*>>& vars)
        :ObjHeur(scip, HEUR_NAME, HEUR_DESC, HEUR_DISPCHAR, HEUR_PRIORITY, HEUR_FREQ, HEUR_FREQOFS, HEUR_MAXDEPTH, HEUR_TIMING, HEUR_USESSUBSCIP), 
        labels(labels), vars(vars), heu(NULL)
    {
        for(int i = 0; i < labels.size(); ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                SCIP_VAR* var = vars[i][j];
                int index = SCIPvarGetProbindex(var);
                scipVarpToLabelMap.insert(pair<int, Label*>(index, &labels[i]));
            }
        }

        heu = new Heuristic1(labels, 4);
    }

    int callHeuristic(void)
    {
        int count = 0;
        for(auto& l : labels)
        {
            count += heu->tryToEnable(l);
        }

        return count;
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
                    if(!SCIPvarIsActive(SCIPvarGetProbvar(var)) && (SCIPgetSolVal(scip, sol, var) != (j==pos)))
                    {
                        cout << "ERROR" << endl;
                    } 
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
        callHeuristic();

        addScipSolutionFromHeur(scip);

        return SCIP_OKAY;
    }

    SCIP_DECL_HEURINIT(scip_initsol)
    {
        return SCIP_OKAY;
    }

    SCIP_DECL_HEUREXEC(scip_exec)
    {
        try
        {
            SCIP_SOL* sol;
            SCIP_CALL_EXC(SCIPcreateSol(scip, &sol, 0));

            SCIP_CALL_EXC(SCIPlinkLPSol(scip, sol));

            vector<Label*> untouchables;
            vector<Label*> touchables;

            for(int i = 0; i < (int)labels.size(); i++)
            {
                Label* l = &labels[i];
                l->disable();
                l->setPos(Label::tl);
                bool touchable = true;

                for(int j = 0; j < 4; j++)
                {
                    SCIP_VAR* var = vars[i][j];
                    SCIP_Real val = SCIPgetSolVal(scip, sol, var);

                    //abs(val-1.0) <= 0.00001

                    if(!SCIPvarIsActive(SCIPvarGetProbvar(var)))
                    {
                        touchable = false;
                    }
  
                    if(SCIPisFeasIntegral(scip, val) && val < 0.5)
                    {
                        labels[i].enable();
                        l->setPos((Label::Pos)j);
                        break;
                    }
                    //labels[i].rotateCW(); wieso?
                }

                if(touchable)
                {
                    touchables.push_back(l);
                }
                else
                {
                    untouchables.push_back(l);
                }
            }

            vector<bool> mask;
            for(auto& l : untouchables)
            {
                mask.push_back(l->b() == 1);
            }

            for(auto& l : touchables)
            {
                heu->tryToEnable(*l, &untouchables);               
            }

            for(int i = 0; i < untouchables.size(); ++i)
            {
                if(untouchables[i]->b() != mask[i])
                {
                    cout << "ERROR: untouchable Variable '" <<  << endl;
                    exit(-1);
                }
            }

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
                    default: break;
                    }
                }
                for(int j = 0; j < 4; j++)
                {
                    SCIP_CALL_EXC(SCIPsetSolVal(scip, sol, vars[i][j], j==pos));
                }
            }

            SCIP_Bool stored;
            SCIP_CALL_EXC(SCIPaddSolFree(scip, &sol, &stored));

            *result = SCIP_FOUNDSOL;
        }
        catch(SCIPException& e)
        {
            cerr << e.what() << endl;
            exit(e.getRetcode());
        }

        return SCIP_OKAY;


        //******************************** other tries...

        *result = SCIP_DIDNOTRUN;

        //!SCIPisRelaxSolValid(scip) immer true...
        if(SCIPgetLPSolstat(scip) != SCIP_LPSOLSTAT_OPTIMAL)
        {
            return SCIP_OKAY;
        }

        //get current feasible solutions
        int feasSolsN = SCIPgetNSols(scip);

        if(feasSolsN <= 0)
        {
            return SCIP_OKAY;
        }

        cout << "Current feasible Solutions: " << feasSolsN << endl;

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

        //set<Label*> fixedLabels;        

        for(int i = 0; i < nlpcands; ++i)
        {
            SCIP_VAR* var = lpcands[i];

            SCIP_Real obj = SCIPvarGetObj(var);
            SCIP_Real v = lpcandssol[i];
            //SCIPisFeasIntegral(var, v);
            SCIP_Real bestroundval = obj > 0.0 ? SCIPfeasFloor(scip, v) : SCIPfeasCeil(scip, v);

            int index = SCIPvarGetProbindex(var);

            auto it = scipVarpToLabelMap.find(index);

            if(it == scipVarpToLabelMap.end())
            {
                continue;
            }

            Label* l = it->second;

            l->setEnable(bestroundval > 0);
            l->setPos(bestroundval <= 0 ? (Label::Pos)0 : (Label::Pos)(index % 4));

            //fixedLabels.insert(l);
        }

        vector<Label*> untouchables;
        for(int i = 0; i < labels.size(); ++i)
        {
            Label* l = &labels[i];
            for(int j = 0; j < 4; ++j)
            {
                SCIP_VAR* var = vars[i][j];
                if(!SCIPvarIsActive(SCIPvarGetProbvar(var)))
                {
                    untouchables.push_back(l);
                    break;
                }
            }
        }

        for(auto& l : labels)
        {
//             Label* ptr = &l;
//             if(fixedLabels.find(ptr) != fixedLabels.end())
//             {
//                 continue;
//             }
            heu->tryToEnable(l, &untouchables);
        }

        addScipSolutionFromHeur(scip);

        *result = SCIP_FOUNDSOL;

        return SCIP_OKAY;
    }

    ~Heu1PlusScip(void)
    {
        if(heu)
        {
            delete heu;
        }
    }
};


#endif
