#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

int main(int argc, char** argv)
{
	ifstream is(argv[1]);

	vector<string> v;
	map<string, bool> m;

	while(is)
	{
		string tmp;
		getline(is, tmp);
		if(tmp.length() < 10)
			continue;
		
		size_t pos = tmp.find_first_of(',');
		if(pos == string::npos)
			pos = 0;
		string country = tmp.substr(pos+2);
		pos = country.find_first_of('\t');
		country = country.substr(0, pos);

		if(m.find(country) == m.end())
		{
			//cout << country << "?" << endl;
			//char c;
			//cin >> c;
			//m[country] = c=='j';
			m[country] = country.find("USA")!=string::npos || country.find("Kanada")!=string::npos;
		}

		if(m[country])
			v.push_back(tmp);
	}
	is.close();


	ofstream os(argv[2]);
	for(auto a:v)
	{
		for(auto& c : a)
		{
			if(c=='\t')
				c = ' ';
		}
		os << a << endl;
	}

}
