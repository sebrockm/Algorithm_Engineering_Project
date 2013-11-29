#include "Label.hpp"

#include <algorithm>


bool labelCross(const Label& label1, const Label& label2)
{
	 return label1.b() == 1 && label2.b() == 1 &&
		std::max(label1.xs(), label2.xs()) < std::min(label1.xs()+label1.l(), label2.xs()+label2.l()) &&
		std::max(label1.ys()-label1.h(), label2.ys()-label2.h()) < std::min(label1.ys(), label2.ys());
}
