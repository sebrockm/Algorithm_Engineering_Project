#ifndef _CROSSING_HPP_
#define _CROSSING_HPP_

#include <vector>

#include "Label.hpp"


/**
 * Checks if Label pos crosses with any previously Labels beginning with first
 * @param first Iterator to Label to begin with
 * @param pos Iterator to Label to be checked
 * @return true if there are no crossings
 */
bool isPosOkUntilNow(std::vector<Label>::iterator first, std::vector<Label>::iterator pos);

/**
 * Returns a vector with pointers to Labels that are crossing label
 * @param label Label to be checked
 * @param first Iterator to first Label to be ckecked with
 * @param last Iterator to fiest Label not to be checked with
 * @return vector of pointers to Labels crossing with label
 */
std::vector<Label*> getCrossing(const Label& label, std::vector<Label>::iterator first, std::vector<Label>::iterator last);

#endif
