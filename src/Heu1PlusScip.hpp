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
using namespace std;



class Heu1PlusScip : public scip::ObjHeur
{
private:
    vector<Label>& labels;
    vector<vector<SCIP_VAR*>>& vars;

public:
    Heu1PlusScip(SCIP* scip, vector<Label>& labels, vector<vector<SCIP_VAR*>>& vars)
        :ObjHeur(scip, "Heu1+Scip", "Scip using Heuristic1",
                'H', 100, 1, 0, -1, SCIP_HEURTIMING_AFTERNODE, false),
        labels(labels), vars(vars)
    {
    }

    virtual SCIP_DECL_HEURINIT(scip_initsol)
    {
        /*
        try
        {
            Heuristic1 heu1(labels, 4);
            
            int count = 0;
            for(auto& l : labels)
            {
                count += heu1.tryToEnable(l);
            }
            cout << endl << endl << count << endl << endl;


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
                    SCIP_CALL_EXC(SCIPsetSolVal(scip, sol, vars[i][j], j==pos));
                }
            }

            cout << "addSolFree" << endl;
            SCIP_Bool stored;
            SCIP_CALL_EXC(SCIPaddSolFree(scip, sol, &stored));
            cout << "addSolFree" << stored << endl;

        }
        catch(SCIPException& e)
        {
            cerr << e.what() << endl;
            exit(e.getRetcode());
        }
        */
        return SCIP_OKAY;
    }

    virtual SCIP_DECL_HEUREXEC(scip_exec)
    {
        try
        {
            SCIP_SOL* sol;
            SCIP_CALL_EXC(SCIPcreateSol(scip, &sol, 0));

            for(int i = 0; i < (int)labels.size(); i++)
            {
                labels[i].disable();
                labels[i].setPos(Label::tl);
                for(int j = 0; j < 4; j++)
                {
                    SCIP_Real val = SCIPgetSolVal(scip, sol, vars[i][j]);
                    if(abs(val-1.0) <= 0.00001)
                    {
                        labels[i].enable();
                        break;
                    }
                    labels[i].rotateCW();
                }
            }


            //TODO auf gefixte Variablen nicht aendern....
            Heuristic1 heu1(labels, 4);
            for(auto& l : labels)
            {
                heu1.tryToEnable(l);
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
                        default:    break;
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
    }
};


#endif
