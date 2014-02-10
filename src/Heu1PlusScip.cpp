#include "Heu1PlusScip.hpp"
using namespace std;

Heu1PlusScip::Heu1PlusScip(SCIP* scip, vector<Label>& _labels, vector<vector<SCIP_VAR*>>& vars)
    :ObjHeur(scip, HEUR_NAME, HEUR_DESC, HEUR_DISPCHAR, HEUR_PRIORITY, HEUR_FREQ, HEUR_FREQOFS, HEUR_MAXDEPTH, HEUR_TIMING, HEUR_USESSUBSCIP), 
    labels(_labels), vars(vars), heu(_labels, HEUR_REC_D), heu2(_labels)
{
    //cout << HEUR_NAME << endl;
}

void Heu1PlusScip::callHeuristic(vector<Label*>* ut)
{
    //cout << "ut.size(): " << (ut?ut->size():0) << endl;
    int count = 0;
    do 
    {
        count = 0;
        for(auto& l : labels)
        {
            if(l.b() == 0 && l.fixedCount() < 4)
            {
               if(heu.tryToEnable(l, ut))
               {
                   count++;
               }
            }
        }
        //cout << "heu found " << count << endl;
    } while (count > 0);
}

SCIP_Bool Heu1PlusScip::copyLabelsToScipState(SCIP* scip, SCIP_HEUR* heur)
{
    try
    {
        SCIP_SOL* sol;
        SCIP_CALL_EXC(SCIPcreateSol(scip, &sol, heur));

        SCIP_VAR** _vars = SCIPgetVars(scip);
        int ncount = SCIPgetNVars(scip);

        int counter = 0;
        for(int i = 0; i < ncount; i++)
        {
            ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(_vars[i]);
            bool set = data->labelPtr->b() == 1 && data->labelPtr->getPos() == data->pos;
            SCIP_CALL_EXC(SCIPsetSolVal(scip, sol, _vars[i], set));
            counter += set;
        }
/*
        for(int i = 0; i < (int)labels.size(); i++)
        {
            for(int j = 0; j < 4; j++)
            {                    
                SCIP_CALL_EXC(SCIPsetSolVal(scip, sol, vars[i][j], labels[i].b()==1 && (int)labels[i].getPos()==j));
                if(labels[i].b()==1 && (int)labels[i].getPos()==j)
                {
                    counter++;
                }
            }
        }
*/
        SCIP_Bool stored;
        SCIP_CALL_EXC(SCIPaddSolFree(scip, &sol, &stored));
        //cout << counter << " in copyLabelsToScipState, stored: " << stored << endl;

        return stored;
    }
    catch(SCIPException& e)
    {
        cerr << e.what() << endl;
        exit(e.getRetcode());
    }

    return FALSE;
}

void Heu1PlusScip::copyScipStateToLabels(Scip* scip)
{
    try
    {
        for(auto& l : labels)
        {
            l.unfix();
            l.disable();
        }

        int counter = 0;
        SCIP_VAR** _vars = SCIPgetVars(scip);
        int ncount = SCIPgetNVars(scip);

        for(int i = 0; i < ncount; i++)
        {
            SCIP_Real val = SCIPgetSolVal(scip, NULL, _vars[i]);//values of current solution
            ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(_vars[i]);

            if(SCIPisFeasIntegral(scip, val) && val > 0.5)
            {
                data->labelPtr->setPos(data->pos);
                data->labelPtr->enable();
                counter++;
            }
        }

        _vars = SCIPgetFixedVars(scip);
        ncount = SCIPgetNFixedVars(scip);

        for(int i = 0; i < ncount; i++)
        {
            SCIP_Real val = SCIPgetSolVal(scip, NULL, _vars[i]);//values of current solution
            ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(_vars[i]);

            if(SCIPisFeasIntegral(scip, val) && val > 0.5)
            {
                data->labelPtr->setPos(data->pos);
                data->labelPtr->enable();
                counter++;
            }
            data->labelPtr->_isFixed[(int)data->pos] = true;
        }
/*
        if(SCIPgetNSols(scip) > 0)
        {
            for(int i = 0; i < (int)labels.size(); ++i)
            {
                labels[i]._isFixed = true;
                labels[i].disable();

                for(int j = 0; j < 4; ++j)
                {
                    SCIP_VAR* v = vars[i][j];
                    ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(v);

                    //gibt beim ersten Aufruf nur 1er o_O
                    //auch wenn die vars von SCIPgetVar() kommen
                    //auch wenn sol statt NULL übergeben
                    SCIP_Real val = SCIPgetSolVal(scip, NULL, v);//values of current solution

                    if(SCIPisFeasIntegral(scip, val) && val > 0.5)
                    {
                        labels[i].enable();
                        labels[i].setPos(data->pos);
                        counter++;
                        cout << SCIPvarGetName(v) << endl;
                        break;
                    }
                }
            }
        }
        else
        {
            cout << "noch keine lsg" << endl;
        }
        cout << counter << " in copyScipStateToLabels" << endl;

        //alle nicht fixierten nachträglich entfixen
        SCIP_VAR** _vars = SCIPgetVars(scip);
        int ncount = SCIPgetNVars(scip);
        cout << labels.size()*4-ncount << " deleted" << endl;
        for(int i = 0; i < ncount; ++i)
        {
            ScipVarLabelData* data = (ScipVarLabelData*)SCIPvarGetData(_vars[i]);
            data->labelPtr->_isFixed = false;
        }
*/
    }
    catch(SCIPException& e)
    {
        cerr << e.what() << endl;
        exit(e.getRetcode());
    }
}

SCIP_RETCODE Heu1PlusScip::scip_init(SCIP* scip, SCIP_HEUR* heur)
{
    //copyScipStateToLabels(scip);
    for(auto& l : labels)
    {
        l.unfix();
        l.disable();
    }

    callHeuristic();

    copyLabelsToScipState(scip, heur);

    //cout << "Heur1 init" << endl;

    return SCIP_OKAY;
}

SCIP_RETCODE Heu1PlusScip::scip_initsol(SCIP* scip, SCIP_HEUR* heur)
{
    //heurInitsolRounding(scip, roundingHeu);

    //sol = RoundingHeuGetSolution();
    //SCIP_CALL_EXC(SCIPlinkLPSol(scip, sol));
    return SCIP_OKAY;
}

SCIP_RETCODE Heu1PlusScip::scip_exec(SCIP* scip, SCIP_HEUR* heur, SCIP_HEURTIMING heurtiming, SCIP_RESULT* result)
{
    try
    {
        *result = SCIP_DIDNOTRUN;

        if(SCIPgetLPSolstat(scip) != SCIP_LPSOLSTAT_OPTIMAL)
        {
            //cout << "current LP State not optimal: canceling heuristic" << endl;
            return SCIP_OKAY;
        }

        //get current feasible solutions
        int feasSolsN = SCIPgetNSols(scip);

        if(feasSolsN <= 0)
        {
            return SCIP_OKAY;
        }

        copyScipStateToLabels(scip);

        /*
        vector<Label*> untouchables;
        for(Label& l : labels)
        {
            if(l.fixedCount() == 4)
            {
                untouchables.push_back(&l);
            }
        }
        */
        //cout << untouchables.size() << " untouchables" << endl;

        //callHeuristic(&untouchables);
        callHeuristic();

        auto stored = copyLabelsToScipState(scip, heur);

        if(!stored)
        {
            //cout << "storing solution failed!" << endl;
        }
        else
        {
            *result = SCIP_FOUNDSOL;
        }
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

Heu1PlusScip::~Heu1PlusScip(void)
{
}


