#include "Solver.hpp"

#include "objscip/objscip.h"
#include "scip_exception.hpp"
#include "scip/cons_linear.h"
#include "scip/scipdefplugins.h"

#include <iostream>
#include <sstream>
#include <string>


using namespace std;


void Solver::printStatus(SCIP_Status s) const
{
    switch(s)
    {
        default:
        case SCIP_STATUS_UNKNOWN:
            cerr << "the solving status is not yet known" << endl; break;

        case SCIP_STATUS_USERINTERRUPT:
            cerr << "the user interrupted the solving process (by pressing CTRL-C)" << endl; break;

        case SCIP_STATUS_NODELIMIT:
            cerr << "the solving process was interrupted because the node limit was reached" << endl; break;

        case SCIP_STATUS_TOTALNODELIMIT:
            cerr << "the solving process was interrupted because the total node limit was reached (incl. restarts)" << endl; break;

        case SCIP_STATUS_STALLNODELIMIT:
            cerr << "the solving process was interrupted because the node limit was reached" << endl; break;

        case SCIP_STATUS_TIMELIMIT:
            cerr << "the solving process was interrupted because the time limit was reached" << endl; break;

        case SCIP_STATUS_MEMLIMIT:
            cerr << "the solving process was interrupted because the memory limit was reached" << endl; break;

        case SCIP_STATUS_GAPLIMIT:
            cerr << "the solving process was interrupted because the gap limit was reached" << endl; break;

        case SCIP_STATUS_SOLLIMIT:
            cerr << "the solving process was interrupted because the solution limit was reached" << endl; break;

        case SCIP_STATUS_BESTSOLLIMIT:
            cerr << "the solving process was interrupted because the solution improvement limit was reached" << endl; break;

        case SCIP_STATUS_OPTIMAL:
            break;

        case SCIP_STATUS_INFEASIBLE:
            cerr << "the problem was proven to be infeasible" << endl; break;

        case SCIP_STATUS_UNBOUNDED:
            cerr << "the problem was proven to be unbounded" << endl; break;

        case SCIP_STATUS_INFORUNBD:
            cerr << "the problem was proven to be either infeasible or unbounded" << endl; break;
    }
}

Solver::Solver(vector<Label>& labels)
    :labels(labels),
    vars(labels.size(), {0,0,0,0}),
    positionCons(labels.size()),
    crossCons((labels.size()-1))
{
    int n = labels.size();

    try
    {
        SCIP_CALL_EXC(SCIPcreate(&scip));
        SCIP_CALL_EXC(SCIPcreateProbBasic(scip, "labeling"));
        SCIP_CALL_EXC(SCIPsetObjsense(scip, SCIP_OBJSENSE_MAXIMIZE));
        SCIP_CALL_EXC(SCIPincludeConshdlrLinear(scip));
        //SCIP_CALL_EXC(SCIPincludeNodeselEstimate(scip));
        SCIP_CALL_EXC(SCIPincludeNodeselHybridestim(scip));
        //SCIP_CALL_EXC(SCIPincludeNodeselBfs(scip));
        
        //Variablen
        for(int i = 0; i < n; i++)
        {
            for(int p = 0; p < 4; p++)
            {
                stringstream ss;
                ss << "b#" << i+1 << "#" << p+1;
                SCIP_CALL_EXC(SCIPcreateVarBasic(scip, &vars[i][p], ss.str().c_str(), 
                            0, //left bound
                            1, //right bound
                            1, //objective
                            SCIP_VARTYPE_BINARY));
                SCIP_CALL_EXC(SCIPaddVar(scip, vars[i][p]));
            }
        }

        //Constraints
        for(int i = 0; i < n; i++)
        {
            SCIP_Real vals[] = {1,1,1,1};
            stringstream ss;
            ss << "sum b#" << i+1 << "#p <= 1";
            SCIP_CALL_EXC(SCIPcreateConsBasicLinear(scip, &positionCons[i], ss.str().c_str(),
                    4, //variable count
                    &vars[i][0], //variables
                    vals, //values
                    0., //lhs
                    1.)); //rhs
            SCIP_CALL_EXC(SCIPaddCons(scip, positionCons[i]));
        }

        for(int i = 0; i < n-1; i++)
        {
            crossCons[i] = vector<vector<vector<SCIP_CONS*>>>(n-1-i);
            for(int j = i+1; j < n; j++)
            {
                int jj = j-i-1;
                crossCons[i][jj] = vector<vector<SCIP_CONS*>>(4);
                labels[i].setPos(Label::tl);
                for(int p = 0; p < 4; p++)
                {
                    crossCons[i][jj][p] = vector<SCIP_CONS*>(4);
                    labels[j].setPos(Label::tl);
                    for(int q = 0; q < 4; q++)
                    {
                        stringstream ss;
                        ss << "b#" << i+1 << "#" << p+1 << " + " << "b#" << j+1 << "#" << q+1 
                            << " + " << "c#" << i+1 << "#" << p+1 << "#" << j+1 << "#" << q+1 << " <= 2";

                        SCIP_VAR* v[] = {vars[i][p], vars[j][q]};
                        SCIP_Real vals[] = {1,1};
                        SCIP_CALL_EXC(SCIPcreateConsBasicLinear(scip, &crossCons[i][jj][p][q], ss.str().c_str(),
                                    2, //variable count
                                    v, //variables
                                    vals, //values
                                    0., //lhs
                                    2. - labelCouldCross(labels[i], labels[j]))); //rhs

                        SCIP_CALL_EXC(SCIPaddCons(scip, crossCons[i][jj][p][q]));

                        labels[j].rotateCW();
                    }   
                    labels[i].rotateCW();
                }   
            }
        }
    }
    catch(SCIPException& e)
    {
        cerr << e.what() << endl;
        exit(e.getRetcode());
    }
}

bool Solver::solve()
{
    int n = labels.size();

    try
    {
        //timelimit halbe Stunde
        SCIP_CALL(SCIPsetRealParam(scip, "limits/time", 1800));

        //solve
        SCIP_CALL_EXC(SCIPsolve(scip));
        SCIP_Status status = SCIPgetStatus(scip);
        if(status != SCIP_STATUS_OPTIMAL)
        {
            printStatus(status);
            return false;
        }

        SCIP_SOL* sol = SCIPgetBestSol(scip);
        if(sol == NULL)
        {
            cerr << "was war da denn los?" << endl;
            return false;
        }

        cout << "best: " << SCIPgetSolOrigObj(scip, sol) << endl;

        for(int i = 0; i < n; i++)
        {
            labels[i].setPos(Label::tl);
            labels[i].disable();
            for(int p = 0; p < 4; p++)
            {
                if(SCIPgetSolVal(scip, sol, vars[i][p]) > 0.5)
                {
                    labels[i].enable();
                    break;
                }
                labels[i].rotateCW();
            }
        }
    }
    catch(SCIPException& e)
    {
        cerr << e.what() << endl;
        exit(e.getRetcode());
    }

    return true;
}

Solver::~Solver()
{
    try
    {
        for(auto& a : vars)
        {
            for(auto& b : a)
            {
                SCIP_CALL_EXC(SCIPreleaseVar(scip, &b));
            }
        }

        for(auto& a : positionCons)
        {
            SCIP_CALL_EXC(SCIPreleaseCons(scip, &a)); 
        }

        for(auto& a : crossCons)
        {
            for(auto& b : a)
            {
                for(auto& c : b)
                {
                    for(auto& d : c)
                    {
                        SCIP_CALL_EXC(SCIPreleaseCons(scip, &d)); 
                    }
                }
            }
        }

        SCIP_CALL_EXC(SCIPfree(&scip));
    }
    catch(SCIPException& e)
    {
        cerr << e.what() << endl;
        exit(e.getRetcode());
    }
}
