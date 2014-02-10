#include "Solver.hpp"

#include "scip_exception.hpp"

#include "objscip/objscip.h"
#include "scip/cons_linear.h"
#include "scip/scipdefplugins.h"
#include "Heu1PlusScip.hpp"

#include <iostream>
#include <sstream>
#include <string>


using namespace std;

#define SCIP_WITH_HEU
#define SCIP_CONS_3
#define SCIP_CONS_4
//#define SCIP_CONS_5

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
    //vars(labels.size(), {0,0,0,0}),
    positionCons(labels.size()), heu(NULL)
{
    int n = (int)labels.size();

    for(int i = 0; i < n; ++i)
    {
        vars.push_back(std::vector<SCIP_VAR*>(4));

        for(int j = 0; j < 4; ++j)
            varData.push_back(ScipVarLabelData());
    }

    try
    {
        SCIP_CALL_EXC(SCIPcreate(&scip));
        SCIP_CALL_EXC(SCIPincludeDefaultPlugins(scip));
		//SCIP_CALL_EXC(SCIPsetMessagehdlr(scip, NULL));
        SCIP_CALL_EXC(SCIPcreateProbBasic(scip, "labeling"));
        SCIP_CALL_EXC(SCIPsetObjsense(scip, SCIP_OBJSENSE_MAXIMIZE));

        //Variablen
        for(int i = 0; i < n; i++)
        {
            Label* label = &labels[i];
            for(int p = 0; p < 4; p++)
            {
                stringstream ss;
                ss << "b#" << i << "#" << p;
                SCIP_CALL_EXC(SCIPcreateVarBasic(scip, &vars[i][p], ss.str().c_str(), 
                            0, //left bound
                            1, //right bound
                            1, //objective
                            SCIP_VARTYPE_INTEGER));
                
                varData[4*i + p].labelPtr = label;
                varData[4*i + p].pos = (Label::Pos)p;
                SCIPvarSetData(vars[i][p], (SCIP_VARDATA*)(&varData[4*i + p]));
                SCIP_CALL_EXC(SCIPaddVar(scip, vars[i][p]));
            }
        }

#ifdef SCIP_WITH_HEU
        heu = new Heu1PlusScip(scip, labels, vars);
        SCIP_CALL_EXC(SCIPincludeObjHeur(scip, heu, false));
#endif
        
        //Constraints
        for(int i = 0; i < n; i++)
        {
            SCIP_Real vals[] = {1,1,1,1};
            stringstream ss;
            ss << "sum b#" << i << "#p <= 1";
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
            for(int j = i+1; j < n; j++)
            {
                labels[i].setPos((Label::Pos)0);
                for(int p = 0; p < 4; p++)
                {
                    labels[j].setPos((Label::Pos)0);
                    for(int q = 0; q < 4; q++)
                    {
                        if(labelCouldCross(labels[i], labels[j]))
                        {
                            bool found3 = false;
#ifdef SCIP_CONS_3
                            for(int k = j+1; k < n; k++)
                            {
                                labels[k].setPos((Label::Pos)0);
                                for(int r = 0; r < 4; r++)
                                {
                                    if(labelCouldCross(labels[i], labels[k]) && labelCouldCross(labels[j], labels[k]))
                                    {
                                        found3 = true;
                                        bool found4 = false;
#ifdef SCIP_CONS_4
                                        for(int l = k+1; l < n; l++)
                                        {
                                            labels[l].setPos((Label::Pos)0);
                                            for(int s = 0; s < 4; s++)
                                            {
                                                if(labelCouldCross(labels[i], labels[l]) && labelCouldCross(labels[j], labels[l]) && labelCouldCross(labels[k], labels[l]))
                                                {
                                                    found4 = true;
                                                    bool found5 = false;
#ifdef SCIP_CONS_5
                                                    for(int m = l+1; m < n; m++)
                                                    {
                                                        for(int t = 0; t < 4; t++)
                                                        {
                                                            if(labelCouldCross(labels[i], labels[m]) && labelCouldCross(labels[j], labels[m]) 
                                                                    && labelCouldCross(labels[k], labels[m]) && labelCouldCross(labels[l], labels[m]))
                                                            {
                                                                found5 = true;
                                                                stringstream sssss;
                                                                sssss << "b#" << i << "#" << p << " + b#" << j << "#" << q 
                                                                    << " + b#" << k << "#" << r << " + b#" << l << "#" << s << " + b#" << m << "#" << t << " <= 1";

                                                                SCIP_VAR* vvvv[] = {vars[i][p], vars[j][q], vars[k][r], vars[l][s], vars[m][t]};
                                                                SCIP_Real vvvvals[] = {1,1,1,1,1};
                                                                crossCons.push_back(NULL);
                                                                SCIP_CALL_EXC(SCIPcreateConsBasicLinear(scip, &crossCons.back(), sssss.str().c_str(),
                                                                            5, //variable count
                                                                            vvvv, //variables
                                                                            vvvvals, //values
                                                                            0., //lhs
                                                                            1.)); //rhs

                                                                SCIP_CALL_EXC(SCIPaddCons(scip, crossCons.back()));
                                                            }
                                                            labels[m].rotateCW();
                                                        }
                                                    }
#endif//5
                                                    if(!found5)
                                                    {
                                                        stringstream ssss;
                                                        ssss << "b#" << i << "#" << p << " + b#" << j << "#" << q 
                                                            << " + b#" << k << "#" << r << " + b#" << l << "#" << s << " <= 1";

                                                        SCIP_VAR* vvv[] = {vars[i][p], vars[j][q], vars[k][r], vars[l][s]};
                                                        SCIP_Real vvvals[] = {1,1,1,1};
                                                        crossCons.push_back(NULL);
                                                        SCIP_CALL_EXC(SCIPcreateConsBasicLinear(scip, &crossCons.back(), ssss.str().c_str(),
                                                                    4, //variable count
                                                                    vvv, //variables
                                                                    vvvals, //values
                                                                    0., //lhs
                                                                    1.)); //rhs

                                                        SCIP_CALL_EXC(SCIPaddCons(scip, crossCons.back()));
                                                    }
                                                }
                                                labels[l].rotateCW();
                                            }
                                        }
#endif//4
                                        if(!found4)
                                        {
                                            stringstream sss;
                                            sss << "b#" << i << "#" << p << " + b#" << j << "#" << q << " + b#" << k << "#" << r <<" <= 1";

                                            SCIP_VAR* vv[] = {vars[i][p], vars[j][q], vars[k][r]};
                                            SCIP_Real vvals[] = {1,1,1};
                                            crossCons.push_back(NULL);
                                            SCIP_CALL_EXC(SCIPcreateConsBasicLinear(scip, &crossCons.back(), sss.str().c_str(),
                                                        3, //variable count
                                                        vv, //variables
                                                        vvals, //values
                                                        0., //lhs
                                                        1.)); //rhs

                                            SCIP_CALL_EXC(SCIPaddCons(scip, crossCons.back()));
                                        }
                                    }
                                    labels[k].rotateCW();
                                }
                            }
#endif//3
                            if(!found3)
                            {
                                stringstream ss;
                                ss << "b#" << i << "#" << p << " + " << "b#" << j << "#" << q << " <= 1";

                                SCIP_VAR* v[] = {vars[i][p], vars[j][q]};
                                SCIP_Real vals[] = {1,1};
                                crossCons.push_back(NULL);
                                SCIP_CALL_EXC(SCIPcreateConsBasicLinear(scip, &crossCons.back(), ss.str().c_str(),
                                            2, //variable count
                                            v, //variables
                                            vals, //values
                                            0., //lhs
                                            1.)); //rhs

                                SCIP_CALL_EXC(SCIPaddCons(scip, crossCons.back()));
                            }
                        }
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
        cout << "nopt" << -1 << "\t" << 0 << endl;
        exit(e.getRetcode());
    }
}

int Solver::solve()
{
    int n = (int)labels.size();

    try
    {
        //timelimit halbe Stunde
        SCIP_CALL(SCIPsetRealParam(scip, "limits/time", 7.5*60));

        //solve
        SCIP_CALL_EXC(SCIPsolve(scip));

        SCIP_Status status = SCIPgetStatus(scip);
        if(status != SCIP_STATUS_OPTIMAL)
        {
            printStatus(status);
            cout << "nopt\t";
        }
        else
        {
            cout << "opt\t";
        }

        SCIP_SOL* sol = SCIPgetBestSol(scip);
        if(sol == NULL)
        {
            cerr << "was war da denn los?" << endl;
            return false;
        }

		int counter = 0;
        for(int i = 0; i < n; i++)
        {
            labels[i].unfix();
            labels[i].setPos((Label::Pos)0);
            labels[i].disable();
            for(int p = 0; p < 4; p++)
            {
                double d = SCIPgetSolVal(scip, sol, vars[i][p]);
                if(d > 0.5)
                {
                    labels[i].enable();
					counter++;
                    break;
                }
                labels[i].rotateCW();
            }
        }

		return counter;
    }
    catch(SCIPException& e)
    {
        cerr << e.what() << endl;
        cout << "nopt" << -1 << "\t" << 0 << endl;
        exit(e.getRetcode());
    }

    return 0;
}

Solver::~Solver()
{
    try
    {
        if(heu)
        {
            delete heu;
        }

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
            SCIP_CALL_EXC(SCIPreleaseCons(scip, &a)); 
        }

        //SCIP_CALL_EXC(SCIPfree(&scip));
    }
    catch(SCIPException& e)
    {
        cerr << e.what() << endl;
        cout << "nopt" << -1 << "\t" << 0 << endl;
        exit(e.getRetcode());
    }
}
