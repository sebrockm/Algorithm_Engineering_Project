#include "Heuristic1.hpp"
#include "crossing.hpp"
#include "Label.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

Heuristic1::Heuristic1(vector<Label>& labels, int maxDepth)
	:_labels(labels), _maxDepth(maxDepth) 
{
	vector<Label*> tmp;
	for(auto& l : labels)
	{
		tmp.push_back(&l);
	}
	tree = createKDTree(tmp.begin(), tmp.end(), 12, 10);
}

void Heuristic1::doRollback(vector<pair<Label*, Label::Pos>>& rollbacks)
{
	while(!rollbacks.empty())
	{
		auto r = rollbacks.back();
		rollbacks.pop_back();
		r.first->setPos(r.second);
	}
}

bool Heuristic1::tryToEnable(Label& label)
{
	vector<Label*> untouchables;
	vector<pair<Label*, Label::Pos>> rollbacks;

	label.enable();
	label.setPos(Label::tl);

	if(!tryToEnable(label, _maxDepth, untouchables, rollbacks))
	{
		doRollback(rollbacks);
		label.disable();
		return false;
	}
	return true;
}


bool Heuristic1::tryToEnable(Label& label, int maxDepth, vector<Label*>& untouchables, vector<pair<Label*, Label::Pos>>& rollbacks)
{
	auto oldPos = label.getPos();
	vector<pair<Label::Pos, vector<Label*>>> crossers; //zu jeder Position die Überschneidungen

	//für jede der 4 möglichen Positionen die Überschneidungen finden
	for(int i = 0; i < 4; i++)
	{
		crossers.emplace_back(label.getPos(), getCrossing(label, tree.get()));
		if(crossers.back().second.empty())
		{
			untouchables.push_back(&label);//label erfolgreich verschoben, darum in anderen Rekursionszweigen nicht mehr anfassen
			rollbacks.emplace_back(&label, oldPos);
			return true;
		}

		label.rotateCW();
	}
	if(maxDepth == 0)//keine weitere Rekursion erlaubt und oben keine Lösung gefunden
	{
		label.setPos(oldPos);
		return false;
	}
	
	//wenn ein untouchable unter den crossers ist, gibt es in diese Richtung keine Lösung
	auto nend = remove_if(crossers.begin(), crossers.end(), [&untouchables] (const pair<Label::Pos, vector<Label*>>& val) -> bool 
			{
				//suche unter den crossers nach untouchables
				return find_first_of(val.second.begin(), val.second.end(), untouchables.begin(), untouchables.end()) != val.second.end();
			});
	crossers.erase(nend, crossers.end());

	for(auto& dir : crossers)//in jede richtung versuchen, die Situation rekursiv aufzulösen
	{
		//für jede Richtung das aktuelle Label zu den untouchables hinzufügen
		auto newUntouchables = untouchables;
		newUntouchables.push_back(&label);

		vector<pair<Label*, Label::Pos>> newRollbacks;

		label.setPos(dir.first);
		bool posPossible = true;

		for(auto& cross : dir.second)//versuche für diese Richtung alle kreuzenden rekursiv zu verschieben 
		{
			if(!tryToEnable(*cross, maxDepth-1, newUntouchables, newRollbacks))//cross ließ sich nicht rekursiv verschieben
			{
				doRollback(newRollbacks);
				posPossible = false;
				break;
			}
		}
		if(posPossible)//Richtung gefunden, bei der sich die Situation rekursiv auflösen ließ
		{
			rollbacks.emplace_back(&label, oldPos);
			rollbacks.insert(rollbacks.end(), newRollbacks.begin(), newRollbacks.end());
			return true;
		}
	}
		
	//keine Möglichkeit gefunden
	label.setPos(oldPos);
	return false;
}


