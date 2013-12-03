#include "crossing.hpp"
#include "Label.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

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


vector<Label*> getCrossing(const Label& label, vector<Label*>::iterator first, vector<Label*>::iterator last)
{
	vector<Label*> res;
	if(label.b() == 0)
		return res;

	for(;first != last; ++first)
	{
		if(&label != *first)//label selbst ausschließen
		{
			if(labelCross(label, **first))
			{
				res.push_back(*first);
			}
		}
	}

	return res;
}


vector<Label*> getCrossing(const Label& label,  KDTree* tree)
{
	if(label.b() == 0)
		return vector<Label*>();

	if(!tree->left && !tree->right)
	{
		vector<Label*> res = getCrossing(label, tree->labels.begin(), tree->labels.end());
		sort(res.begin(), res.end());
		return res;
	}

	vector<Label*> left, right;

	if(tree->axis == KDTree::xAxis)
	{
		if(label.minX() < tree->splitPlane)
		{
			left = getCrossing(label, tree->left.get());
		}
		if(label.maxX() > tree->splitPlane)
		{
			right = getCrossing(label, tree->right.get());
		}
	}
	else
	{
		if(label.minY() < tree->splitPlane)
		{
			left = getCrossing(label, tree->left.get());
		}
		if(label.maxY() > tree->splitPlane)
		{
			right = getCrossing(label, tree->right.get());
		}
	}

	vector<Label*> res(left.size() + right.size());
	auto it = set_union(left.begin(), left.end(), right.begin(), right.end(), res.begin());
	res.resize(it - res.begin());

	return res;
}
