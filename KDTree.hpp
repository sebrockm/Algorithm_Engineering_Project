#ifndef _KDTREE_HPP_
#define _KDTREE_HPP_

#include "Label.hpp"

#include <vector>
#include <memory>

using namespace std;

struct KDTree
{
	shared_ptr<KDTree> left, right;
	vector<Label*> labels;
	int splitPlane;
	enum splitAxis
	{
		xAxis,
		yAxis
	} axis;
};


shared_ptr<KDTree> createKDTree(vector<Label*>::iterator first, vector<Label*>::iterator last, int maxDepth, int minElemsPerLeaf);


#endif
