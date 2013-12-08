#include "Heuristic2.hpp"
#include "crossing.hpp"

#include <utility>
#include <algorithm>
#include <iostream>

using namespace std;

Heuristic2::Heuristic2(vector<Label>& labels)
	:_labels(labels)
{
	vector<Label*> tmp;
	for(auto& l : labels)
	{
		tmp.push_back(&l);
	}
	tree = createKDTree(tmp.begin(), tmp.end(), 12, 10);
}

bool Heuristic2::tryToEnable(Label& label)
{
	vector<pair<Label::Pos, Label*>> crossers;//werden evtl zugunsten von label disabled

	for(int i = 0; i < 4; i++)
	{
		auto wouldCrossing = getWouldCrossing(label, tree.get());

		if(wouldCrossing.empty())
		{
			label.enable();
			return true;
		}
		if(wouldCrossing.size() > 1)
		{
			continue;
		}

		crossers.emplace_back(label.getPos(), wouldCrossing.front());
		label.rotateCW();
	}

	//jede moegliche Position von label einzeln betrachten
	for(auto& dir : crossers)
	{
		label.enable();
		label.setPos(dir.first);

		auto dirCrossers = getCouldCrossing(*dir.second, tree.get());//alle, die gerade disabled sind, aber dir.second schneiden koennten
		for(auto& a : dirCrossers)
		{
			a.second->setPos(a.first);
		}


		//alle rausschmeissen, die ...
		auto nend = remove_if(dirCrossers.begin(), dirCrossers.end(), [&label, this](pair<Label::Pos, Label*> p)
				{
					if(p.second == &label)//das gerade betrachtete Label selber sind
						return true;

					p.second->setPos(p.first);
					auto pCrossers = getWouldCrossing(*p.second, tree.get());

					if(pCrossers.size() != 1)//mehr als eine Ueberschneidung haben
						return true;
					if(pCrossers.front() == &label) //oder eine Ueberschneidung mit dem gerade betrachteten Label haben
						return true;

					return false;
				});

		dirCrossers.resize(nend - dirCrossers.begin());
		if(dirCrossers.size() <= 0)//keiner uebrig geblieben
		{
			continue;
		}

		dir.second->disable();
		dirCrossers.front().second->enable();
		dirCrossers.front().second->setPos(dirCrossers.front().first);

		return true;
	}
	label.disable();

	return false;
}
