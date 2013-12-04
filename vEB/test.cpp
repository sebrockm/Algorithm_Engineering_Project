#include "VEB.hpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;


int main()
{
	srand(time(0));
	const unsigned H = 4;
	const unsigned long long N = (1ull << H) - 1;
	VEB<int, H> veb;
	cout << "N=" << N << " sizeof(veb)/sizeof(int)=" << sizeof(veb)/sizeof(int) << endl;

	vector<int> v = {52,27,75,14,7,21,40,34,46,63,58,70,89,81,94};

	/*for(unsigned i=0; i<N; i++)
	{
		v.push_back(rand());
	}*/

	sort(v.begin(), v.end());

	for(unsigned i=0; i<N; i++)
	{
		veb[i] = v[i];
	}

	for(unsigned i=0; i<N; i++)
	{
		if(veb[i] != v[i])
			cout << "AHHHH" << endl;
	}

	for(unsigned i=0; i<N; i++)
	{
		if(!veb.search(v[i]))
			cout << "BEHHHH" << endl;
	}

	//cout << "0 ist " << (veb.search(0)?"":"nicht") << " drin, veb[0]=" << veb[0] << endl;

        int* p = (int*)&veb;
        for(unsigned i=0; i<N; i++)
            cout << p[i] << ",";
}
