#include "crossing.hpp"
#include "Label.hpp"

#include <vector>

using namespace std;

bool isPosOkUntilNow(vector<Label>::iterator first, vector<Label>::iterator pos)
{
	if(pos->b() == 0)
		return true;

	for(; first != pos; ++first)
	{
		if(labelCross(*first, *pos))
			return false;
	}

	return true;
}


vector<Label*> getCrossing(const Label& label, vector<Label>::iterator first, vector<Label>::iterator last)
{
	vector<Label*> res;
	if(label.b() == 0)
		return res;

	for(;first != last; ++first)
	{
		if(&label != &*first)//label selbst ausschließen
		{
			if(labelCross(label, *first))
			{
				res.push_back(&*first);
			}
		}
	}

	return res;
}
