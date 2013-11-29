#include "heuristic1.hpp"
#include "crossing.hpp"
#include "Label.hpp"

#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;


bool tryToEnable(Label& label, vector<Label>::iterator first, vector<Label>::iterator last, int maxDepth, vector<Label*>& untouchables) 
{
	label.b() = 1;
	label.setPos(Label::br);
	vector<pair<Label::Pos, vector<Label*>>> crossers; //zu jeder Position die Überschneidungen

	//für jede der 4 möglichen Positionen die Überschneidungen finden
	for(int i = 0; i < 4; i++)
	{
		crossers.emplace_back(label.getPos(), getCrossing(label, first, last));
		if(crossers.back().second.size() == 0)
		{
			untouchables.push_back(&label);//label erfolgreich verschoben, darum in Zukunft nicht mehr anfassen
			return true;
		}

		//wenn ein untouchable unter den crossers ist, gibt es in diese Richtung keine Lösung
		remove_if(crossers.begin(), crossers.end(), [&untouchables] (const pair<Label::Pos, vector<Label*>>& val) -> bool 
				{
					//suche unter den crossers nach untouchables
					return find_first_of(val.second.begin(), val.second.end(), untouchables.begin(), untouchables.end()) != val.second.end();
				});

		label.rotateCW();
	}
	if(maxDepth == 0)//keine weitere Rekursion erlaubt und oben keine Lösung gefunden
	{
		label.b() = 0;
		return false;
	}
	
	for(auto& cross : crossers)//in jede richtung versuchen, die Situation rekursiv aufzulösen
	{
		//für jede Richtung das aktuelle Label zu den untouchables hinzufügen
		auto newUntouchables = untouchables;
		newUntouchables.push_back(&label);

		label.setPos(cross.first);
		bool posPossible = true;

		for(auto& lab : cross.second)//versuche für diese Richtung alle kreuzenden rekursiv zu verschieben 
		{
			if(find(untouchables.begin(), untouchables.end(), lab) != untouchables.end())//lab ist untouchable
			{
				//sollte niemals eintreten
				cerr << "Es ist doch noch ein untouchable drin" << std::endl;
				posPossible = false;
				break;//also kann in diese Richtung nicht verschoben werden
			}

			if(!tryToEnable(*lab, first, last, maxDepth-1, newUntouchables))//lab ließ sich nicht rekursiv verschieben
			{
				posPossible = false;
				break;
			}
		}
		if(posPossible)//Richtung gefunden, bei der sich die Situation rekursiv auflösen ließ
		{
			untouchables = newUntouchables;
			return true;
		}
	}
		
	//keine Möglichkeit gefunden
	label.b() = 0;
	return false;
}
