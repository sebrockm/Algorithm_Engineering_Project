#ifndef _HEURISTIC1_HPP_
#define _HEURISTIC1_HPP_


#include "Label.hpp"
#include "KDTree.hpp"

#include <vector>

class Heuristic1{
private:
	std::vector<Label>& _labels;
	int _maxDepth;
	std::shared_ptr<KDTree> tree;

	/**
	 * Tries to enable a label by recursivly moving all crossing labels.
	 * The label must be visible, otherwise the function will do nothing.
	 * In case of success this function will set its new position.
	 * In case of no success the user must disable die label (label.b() = 0) himself.
	 * @param label the label to try to enable
	 * @param first iterator to the first of all labels
	 * @param last iterator to the last of all labels
	 * @param maxDepth the maximum depth of recursion
	 * @param untouchables vector of pointers to labels that shall not be moved by the function
	 * @param rollbacks vector of pointers to labels with their old positions, to perform a rollback
	 * @return true if label was successfully enabled, false otherwise
	 */
	bool tryToEnable(Label& label, int maxDepth, vector<Label*>& untouchables, vector<pair<Label*, Label::Pos>>& rollbacks);

	void doRollback(vector<pair<Label*, Label::Pos>>& rollbacks);

public:
	/**
	 * Creates a new Heuristic1 instancel
	 * @param labels the vector of labels the heuristic shall operate on
	 * @param macDepth the maximum depth of recursion
	 */
	Heuristic1(std::vector<Label>& labels, int maxDepth);

	/**
	 * Tries to enable a label by recursivly moving all crossing labels.
	 * In case of success this function will set the label's new position.
	 * @param label the label to try to enable
	 * @return true if label was successfully enabled, false otherwise
	 */
	bool tryToEnable(Label& label);

};

#endif
