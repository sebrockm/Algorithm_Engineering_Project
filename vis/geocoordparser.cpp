#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdlib>

int main(int argc, char** argv)
{
	std::ifstream input(argv[1]);
	std::ofstream output(argv[2]);

	if(!input)
	{
		std::cout << "kann " << argv[1] << " nicht oeffnen" << std::endl;
		return 1;
	}

	output << 1232 << std::endl;
	
	while(input)
	{
		std::string line;
		std::getline(input, line);

		if(line.length() < 10)
			continue;

		std::string name = line.substr(0, line.find_first_of(','));
		for(auto& c : name)
			if(c==' ')
				c = '_';

		int gn, mn, go, mo;
		std::string b;
		char ost, nord;

		std::stringstream ss(line.substr(line.find_first_of("0123456789")));


		if(ss >>gn>>b>>mn>>b>>nord
				>>go>>b>>mo>>b>>ost)
		{
			if(nord == 'S')
			{
				gn = -gn;
				mn = -mn;
			}
			if(ost == 'W')
			{
				go = -go;
				mo = -mo;
			}

				
			double phi = go + mo/60.0;
			double theta = gn + mn/60.0;
			theta = 90.0 - theta;
			double r = 637.1;

			const double PI = 3.14159265359;

			phi *= PI/180;
			theta *= PI/180;

			//int z = (int)(r*sin(theta)*cos(phi));
			//int x = (int)(r*sin(theta)*sin(phi));
			//int y = (int)(r*cos(theta));
			int x = (int)(r*phi);
			int y = (int)(r*cos(theta));

			//std::cout << (z<0?name:"") << z << std::endl;

			output <<  x << " " << y << " "
				<< 8*name.size() << " " << rand()%12+4 << " "
				<< name << " 1 " << x << " " << y
				<< std::endl;
		}
	}

}
