#ifndef _TREES_HPP_
#define _TREES_HPP_

#include <memory>
#include <algorithm>
#include <iostream>
#include <cstdlib>

template <class T, size_t h>
class RandTree
{
private:
	std::unique_ptr<RandTree<T, h-1>> left, right;
	T data;
	
public:
	constexpr static size_t size()
	{
		return (1<<h) - 1;
	}

	RandTree(T* ar):left(new RandTree<T, h-1>(ar)), right(new RandTree<T, h-1>(ar + size()/2 + 1)), data(ar[size()/2])
	{
	}

	bool search(const T& k) const
	{
		if(k == data)
			return true;
		if(k < data)
			return left->search(k);
		return right->search(k);
	}
};


template <class T>
class RandTree<T, 1>
{
private:
	T data;

public:
	constexpr static size_t size()
	{
		return 1;
	}

	RandTree(T* ar):data(*ar)
	{
		//std::cout << reinterpret_cast<size_t>(this) << std::endl;
	}

	bool search(const T& k) const
	{
		return k == data;
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
