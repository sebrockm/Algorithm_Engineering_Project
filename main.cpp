#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

void usage(const char* progname)
{
	std::cerr	<< std::endl
				<< "usage:" << std::endl
				<< "      " << progname << " -in dat1 -out dat2  reads dat1 and writes a solution to dat2" << std::endl
				<< "      " << progname << " -eval dat1          evaluates whether the solution in dat1 is correct" << std::endl;
}


void parse_options(int argc, char** argv, char** input_file, char** output_file, char** eval_file)
{
	if(argc < 2)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	for(int i = 1; i < argc; i++)
	{
		if(std::string(argv[i]) == "-in")
		{ 
			if(++i < argc)
			{
				if(!*input_file && !*eval_file)
				{
					*input_file = argv[i];
				}
				else
				{
					usage(argv[0]);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				usage(argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		else if(std::string(argv[i]) == "-out")
		{
			if(++i < argc)
			{
				if(!*output_file && !*eval_file)
				{
					*output_file = argv[i];
				}
				else
				{
					usage(argv[0]);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				usage(argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		else if(std::string(argv[i]) == "-eval")
		{
			if(++i < argc)
			{
				if(!*output_file && !*input_file && !*eval_file)
				{
					*eval_file = argv[i];
				}
				else
				{
					usage(argv[0]);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				usage(argv[0]);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			usage(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	if(!*input_file ^ !*output_file)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}


int fileParser(const char* file_name)
{
	std::ifstream file(file_name);
	if(!file)
	{
		std::cerr << "cannot open " << file_name << std::endl;
		exit(EXIT_FAILURE);
	}

	int count;
	if(!(file >> count))
	{
		std::cerr << "failed reading line 1 of " << file_name << std::endl;
		return 0;
	}

	int i;
	std::string line;
	for(i = 1; i <= count && std::getline(file, line); i++)
	{
		std::istringstream iss(line);

		int x, y, l, h;
		std::string t;
		if(!(iss >> x >> y >> l >> h >> t))
		{
			break;
		}

		std::cout << x << " " << y << " " << l << " " << h << " " << t << std::endl;
		//TODO Werte speichern statt ausgeben und Problem Loesen
	}
	
	if(i <= count)
	{
		std::cerr << "only " << i+1 << " lines of " << count+1 << " could be parsed" << std::endl;
	}

	return count;
}


bool evaluate(const char* file_name)
{
	std::ifstream stream(file_name);
	if(!stream)
	{
		std::cerr << "ERROR: Fehler beim Oeffnen der Datei" << std::endl;
		return false;
	}

	int count;
	if(!(stream >> count))
	{
		std::cerr << "ERROR: Fehler beim Parsen der Datei" << std::endl;
		return false;
	}

	if(count <= 0)
	{
		return false;
	}

	int* x = new int[count];
	int* y = new int[count];
	int* l = new int[count];
	int* h = new int[count];
	std::string* t = new std::string[count];
	int* b = new int[count];
	int* xs = new int[count];
	int* ys = new int[count];
	std::string mull;

	for(int i = 0; i < count; i++)
	{
		if(!(stream >> x[i] >> y[i] >> l[i] >> h[i] >> t[i] >> b[i] >> xs[i] >> ys[i]))
		{
			std::cerr << "ERROR: Fehler beim Parsen der Datei" << std::endl;
			return false;
		}
	}

	bool good = true;
	int counter = 0; //Anzahl beschrifteter Punkte

	for(int i = 0; i < count; i++)
	{
		if(b[i] == 1)
		{
			counter++;

			if((x[i] != xs[i] && x[i] != xs[i]+l[i]) || (y[i] != ys[i] && y[i] != ys[i]-h[i]))
			{
				std::cerr << "ERROR: Keine Ecke von " << t[i] << " liegt auf dem Punkt" << std::endl;
				good = false;
			}
			
			for(int j = i+1; j < count; j++)
			{
				if(b[j] == 1 //Ueberlappung der Rechtecke
						&& ((xs[i] < xs[j] && xs[j] < xs[i]+l[i]) || (xs[j] < xs[i] && xs[i] < xs[j]+l[j]))
						&& ((ys[i] > ys[j] && ys[j] > ys[i]-h[i]) || (ys[j] > ys[i] && ys[i] > ys[j]-h[j])))
				{
					std::cerr << "ERROR: " << t[i] << " und " << t[j] << " ueberlappen sich" << std::endl;
					good = false;
				}
			}
		}
	}

	std::cout << counter << std::endl;

	delete [] x;
	delete [] y;
	delete [] l;
	delete [] h;
	delete [] t;
	delete [] b;
	delete [] xs;
	delete [] ys;

	return good;
}


int main(int argc, char** argv)
{
	char* input_file = 0;
	char* output_file = 0;
	char* eval_file = 0;

	parse_options(argc, argv, &input_file, &output_file, &eval_file);


	std::cout 	<< "input_file: " << (input_file?input_file:"") << std::endl
				<< "output_file: " << (output_file?output_file:"") << std::endl
				<< "eval_file: " << (eval_file?eval_file:"") << std::endl;

	if(input_file && output_file)
	{
		fileParser(input_file);
	}
	if(eval_file)
	{
		evaluate(eval_file);
	}
}


