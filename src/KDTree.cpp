#include "KDTree.hpp"

#include <algorithm>
#include <iostream>

using namespace std;


shared_ptr<KDTree> createKDTree(vector<Label*>::iterator first, vector<Label*>::iterator last, int maxDepth, int minElemsPerLeaf)
{
	shared_ptr<KDTree> tree(new KDTree);
	tree->labels = vector<Label*>(first, last);
	tree->left = 0;
	tree->right = 0;

	if(last - first <= minElemsPerLeaf || maxDepth == 0)
	{
		return tree;
	}

	int minX = (*min_element(tree->labels.begin(), tree->labels.end(), [](Label* l1, Label* l2){return l1->minX() < l2->minX();}))->minX();
	int maxX = (*max_element(tree->labels.begin(), tree->labels.end(), [](Label* l1, Label* l2){return l1->maxX() < l2->maxX();}))->maxX();
	int minY = (*min_element(tree->labels.begin(), tree->labels.end(), [](Label* l1, Label* l2){return l1->minY() < l2->minY();}))->minY();
	int maxY = (*max_element(tree->labels.begin(), tree->labels.end(), [](Label* l1, Label* l2){return l1->maxY() < l2->maxY();}))->maxY();

	if(maxX - minX > maxY - minY)
	{
		int avgX = 0;
		for(auto it = first; it != last; ++it)
			avgX += (*it)->x();
		avgX /= (last-first);

		tree->splitPlane = avgX;
		tree->axis = KDTree::xAxis;

		auto middle = stable_partition(tree->labels.begin(), tree->labels.end(), [&avgX](Label* l){return l->minX() < avgX;});//alle, die mindestens um 1 links von avgX sind, stehen vorne
		tree->left = createKDTree(tree->labels.begin(), middle, maxDepth-1, minElemsPerLeaf);
		
		middle = stable_partition(tree->labels.begin(), tree->labels.end(), [&avgX](Label* l){return !(l->maxX() > avgX);});//alle, die mindestens um 1 rechts von avgX sind, stehen hinten
		tree->right = createKDTree(middle, tree->labels.end(), maxDepth-1, minElemsPerLeaf);
	}
	else
	{
		int avgY = 0;
		for(auto it = first; it != last; ++it)
			avgY += (*it)->y();
		avgY /= (last-first);

		tree->splitPlane = avgY;
		tree->axis = KDTree::yAxis;

		auto middle = stable_partition(tree->labels.begin(), tree->labels.end(), [&avgY](Label* l){return l->minY() < avgY;});//alle, die mindestens um 1 links von avgY sind, stehen vorne
		tree->left = createKDTree(tree->labels.begin(), middle, maxDepth-1, minElemsPerLeaf);
		
		middle = stable_partition(tree->labels.begin(), tree->labels.end(), [&avgY](Label* l){return !(l->maxY() > avgY);});//alle, die mindestens um 1 rechts von avgY sind, stehen hinten
		tree->right = createKDTree(middle, tree->labels.end(), maxDepth-1, minElemsPerLeaf);
	}

	return tree;
}

