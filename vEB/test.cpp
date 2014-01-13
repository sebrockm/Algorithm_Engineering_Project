#include "VEB.hpp"
#include "Trees.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

using namespace std;


int main()
{
	srand(time(0));
	const size_t N = (1ull << H) - 1;
	const size_t R = 1000000;

	cout << H << " ";

	vector<int> v(N);
	generate(v.begin(), v.end(), rand);


	vector<int> search(R);
	generate(search.begin(), search.end(), rand);

	vector<bool> ergs(R);

	//sorted Tree
	{
		sort(v.begin(), v.end());
		auto t1 = chrono::high_resolution_clock::now();
		for(unsigned i=0; i<R; i++)
		{
			ergs[i] = binary_search(v.begin(), v.end(), search[i]);
		}
		auto t2 = chrono::high_resolution_clock::now();
		cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << " ";
	}

	//RandTree
	/*{
		random_shuffle(v.begin(), v.end());
		RandTree<int, H> randt(v.data());
		auto t1 = chrono::high_resolution_clock::now();
		for(unsigned i=0; i<R; i++)
		{
			//randt.search(search[i]);
			if(ergs[i] != randt.search(search[i]))
				cout << "gna1" << endl;
		}
		auto t2 = chrono::high_resolution_clock::now();
		cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << " ";
	}*/

	//Level sorted Tree
	{
		//sort(v.begin(), v.end());
		LevelSortedTree<int, H> lst(v.data());
		RandTree<int, H> rt((int*)&lst);
		auto t1 = chrono::high_resolution_clock::now();
		for(unsigned i=0; i<R; i++)
		{
			//lst.search(search[i]);
			if(ergs[i] != rt.search(search[i]))
				cout << "gna3" << endl;
		}
		auto t2 = chrono::high_resolution_clock::now();
		cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << " ";
	}

	//vEB
	{
		VEB<int, H> veb(v.data());
		RandTree<int, H> rt((int*)&veb);
		auto t1 = chrono::high_resolution_clock::now();
		for(unsigned i=0; i<R; i++)
		{
			//rt.search(search[i]);
			if(ergs[i] != rt.search(search[i]))
				cout << "gna4" << endl;
		}
		auto t2 = chrono::high_resolution_clock::now();
		cout << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count();
	}

	cout << endl;
}
