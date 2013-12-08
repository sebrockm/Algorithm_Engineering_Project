#ifndef _TREES_HPP_
#define _TREES_HPP_

#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <utility>

using namespace std;


template <class T, size_t h>
class RandTree
{
private:
	struct RandTreeNode
	{
		RandTreeNode *left, *right;
		T data;

		RandTreeNode():left(0), right(0){}
	};

	RandTreeNode nodes[(1<<h)-1];
	RandTreeNode* root;

	void set(RandTreeNode** toSet, size_t hh, pair<T, size_t>* ar)
	{
		size_t middle = (1<<(hh-1)) - 1;
		*toSet = &nodes[ar[middle].second];

		if(hh > 1)
		{
			set(&(*toSet)->left, hh-1, ar);
			set(&(*toSet)->right, hh-1, ar + middle + 1);
		}
	}

public:
	constexpr static size_t size()
	{
		return (1<<h) - 1;
	}

	RandTree(T* ar)
	{
		pair<T, size_t> tmp[size()];//gibt an, welcher Knoten sich wo in nodes befindet

		for(size_t i = 0; i < size(); i++)
		{
			nodes[i].data = ar[i];
			tmp[i] = {ar[i], i};
		}

		sort(tmp, tmp+size(), [](const pair<T, size_t>& p1, const pair<T, size_t>& p2){return p1.first < p2.first;});

		if(h == 27)
			cout << "vor set" << endl;

		set(&root, h, tmp);

		if(h == 27)
			cout << "nach set" << endl;
	}

	bool search(const T& k) const
	{
		RandTreeNode* node = root;
		while(node->left && node->right)
		{
			if(k < node->data)
				node = node->left;
			else if(k > node->data)
				node = node->right;
			else
				return true;
		}

		return node->data == k;
	}
};


template <class T, size_t h>
class LevelSortedTree
{
private:
	T data[(1<<h)-1];

	void fill(T* ar, size_t hh, size_t pos)
	{
		auto middle = ar + (1<<(hh-1)) - 1;
		data[pos] = *middle;
		if(hh > 1)
		{
			fill(ar, hh-1, 2*pos+1);
			fill(middle+1, hh-1, 2*pos+2);
		}
	}

public:
	constexpr static size_t size()
	{
		return (1<<h) - 1;
	}

	LevelSortedTree(T* ar)
	{
		fill(ar, h, 0);
	}


	bool search(const T& k) const
	{
		size_t pos = 0;
		for(size_t i = 0; i < h; i++)
		{
			if(data[pos] == k)
				return true;
			if(k > data[pos])
				pos = 2*pos + 2;
			else
				pos = 2*pos + 1;
		}

		return false;
	}
};

#endif
