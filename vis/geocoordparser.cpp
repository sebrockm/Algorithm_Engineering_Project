#include <iostream>
#include <fstream>

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
			int x = (-10+go)*60+mo;
			int y = (-50+gn)*60+mn;
			output <<  x << " " << y << " "
				<< std::max(15,so) << " " << std::max(10,sn) << " "
				<< name << " 1 " << x << " " << y
				<< std::endl;
		}
		input>>a;
	}

}
