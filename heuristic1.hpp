#ifndef _HEURISTIC1_HPP_
#define _HEURISTIC1_HPP_


#include "Label.hpp"

#include <vector>



/**
 * Tries to enable an label by recursivly moving all crossing labels.
 * In case of success this function will set label.b to 1 and set its new position.
 * @param label the label to try to enable
 * @param first iterator to the first of all labels
 * @param last iterator to the last of all labels
 * @param maxDepth the maximum depth of recursion
 * @param untouchables vector of pointers to labels that shall not be moved by the function
 */
bool tryToEnable(Label& label, std::vector<Label>::iterator first, std::vector<Label>::iterator last, int maxDepth, std::vector<Label*>& untouchables);

#endif
