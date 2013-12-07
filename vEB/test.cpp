#include "VEB.hpp"
#include "Trees.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

using namespace std;

template <class Iter>
bool mybinary(Iter first, Iter last, int what)
{
	while(last > first)
	{
		auto middle = first + (last-first)/2;
		if(what < *middle)
			last = middle;
		else if(what > *middle)
			first = middle + 1;
		else 
			return true;
	}
	return false;
}


int main()
{
	srand(time(0));
	const size_t N = (1ull << H) - 1;
	const size_t R = 1000000;

	cout << H << " ";

	vector<int> v(N);
	generate(v.begin(), v.end(), rand);

	sort(v.begin(), v.end());

	vector<int> search(R);
	generate_n(search.begin(), R, rand);

	vector<bool> ergs(R);

	auto t1 = chrono::high_resolution_clock::now();
	for(unsigned i=0; i<R; i++)
	{
		ergs[i] = binary_search(v.begin(), v.end(), search[i]);
	}
	auto t2 = chrono::high_resolution_clock::now();
	cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << " ";

	/*
	t1 = chrono::high_resolution_clock::now();
	for(unsigned i=0; i<R; i++)
	{
		if(ergs[i] != mybinary(v.begin(), v.end(), search[i]))
			cout << "gna2" << endl;
	}
	t2 = chrono::high_resolution_clock::now();
	cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << " ";
	*/

	{
		RandTree<int, H> randt(v.data());
		t1 = chrono::high_resolution_clock::now();
		for(unsigned i=0; i<R; i++)
		{
			if(ergs[i] != randt.search(search[i]))
				cout << "gna1" << endl;
		}
		t2 = chrono::high_resolution_clock::now();
		cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << " ";
	}

	{
		LevelSortedTree<int, H> lst(v.data());
		t1 = chrono::high_resolution_clock::now();
		for(unsigned i=0; i<R; i++)
		{
			if(ergs[i] != lst.search(search[i]))
				cout << "gna3" << endl;
		}
		t2 = chrono::high_resolution_clock::now();
		cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << " ";
	}

	{
		VEB<int, H> veb(v.data());
		t1 = chrono::high_resolution_clock::now();
		for(unsigned i=0; i<R; i++)
		{
			if(ergs[i] != veb.search(search[i]))
				cout << "gna4" << endl;
		}
		t2 = chrono::high_resolution_clock::now();
		cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count()<< " ";
	}

	cout << endl;
}
