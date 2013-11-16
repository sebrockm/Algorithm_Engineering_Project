#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

int main(int argc, char** argv)
{
	std::ifstream input(argv[1]);
	std::ofstream output(argv[2]);

	output << 76 << std::endl;
	
	while(input)
	{
		int gn, mn, sn, go, mo, so;
		std::string name;
		int a;
		char b;

		if(input>>name>>gn>>b>>mn>>b>>sn>>b>>b
				>>go>>b>>mo>>b>>so>>b>>b)
		{
			double phi = -10 + go + mo/60.0 + so/3600.0;
			double theta = -50 + gn + mn/60.0 + sn/3600.0;
			theta = 90.0 - theta;
			double r = 637.1;

			const double PI = 3.14159265359;

			phi *= PI/180;
			theta *= PI/180;

			int z = (int)(r*sin(theta)*cos(phi));
			int x = (int)(r*sin(theta)*sin(phi));
			int y = (int)(r*cos(theta));

			output <<  x << " " << y << " "
				<< 2*name.size() << " " << rand()%2+1 << " "
				<< name << " 1 " << x << " " << y
				<< std::endl;
		}
		input>>a;
	}

}
