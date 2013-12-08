#ifndef _HEURISTIC2_HPP_
#define _HEURISTIC2_HPP_

#include "Label.hpp"
#include "KDTree.hpp"

#include <vector>
#include <memory>


class Heuristic2
{
private:
	std::vector<Label> _labels;
	std::shared_ptr<KDTree> tree;

public:
	Heuristic2(std::vector<Label>& labels);

	bool tryToEnable(Label& label);
};


#endif
