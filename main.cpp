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


bool isPosOk(int index, int* l, int* h, int* b, int* xs, int* ys)
{
	//index mit allen davor auf Ueberschneidung testen
	for(int i = 0; i < index; i++)
	{
		if(b[i] == 1)
		{
			if(std::max(xs[i], xs[index]) < std::min(xs[i]+l[i], xs[index]+l[index])
					&& std::max(ys[i]-h[i], ys[index]-h[index]) < std::min(ys[i], ys[index]))
			{
				return false;
			}
		}
	}

	return true;
}

void writeSolution(int* x, int* y, int* l, int* h, std::string* t, int count, const char* file_name)
{
	std::ofstream file(file_name);
	if(!file)
	{
		std::cerr << "cannot write " << file_name << std::endl;
		return;
	}

	int* b = new int[count];
	int* xs = new int[count];
	int* ys = new int[count];

	for(int i = 0; i < count; i++)
	{
		//oben links auf Punkt ausprobieren
		xs[i] = x[i];
		ys[i] = y[i];
		if(isPosOk(i, l, h, b, xs, ys))
		{
			b[i] = 1;
			continue;
		}

		//oben rechts
		xs[i] -= l[i];
		if(isPosOk(i, l, h, b, xs, ys))
		{
			b[i] = 1;
			continue;
		}

		//unten rechts
		ys[i] += h[i];
		if(isPosOk(i, l, h, b, xs, ys))
		{
			b[i] = 1;
			continue;
		}

		//unten links
		xs[i] += l[i];
		if(isPosOk(i, l, h, b, xs, ys))
		{
			b[i] = 1;
			continue;
		}

		//keine Position noch frei
		b[i] = 0;
	}

	bool ok = true;

	//Loesung in Datei schreiben
	if(!(file << count << "\n"))
	{
		std::cerr << "could not write to " << file_name << std::endl;
		ok = false;
	}
	for(int i = 0; ok && i < count; i++)
	{
		if(!(file << x[i] << " " << y[i] << " " << l[i] << " " << h[i] << " " 
			<< t[i] << " " << b[i] << " " << xs[i] << " " << ys[i] << "\n"))
		{
			std::cerr << "could not write to " << file_name << std::endl;
			ok = false;
		}
	}

	delete [] b;
	delete [] xs;
	delete [] ys;
}


int fileParser(const char* file_name, const char* ofile_name)
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

	int* x = new int[count];
	int* y = new int[count];
	int* l = new int[count];
	int* h = new int[count];
	std::string* t = new std::string[count];

	int i;
	std::string line;
	std::getline(file, line); //erstes '\n' ueberspringen
	for(i = 0; i < count && std::getline(file, line); i++)
	{
		std::istringstream iss(line);

		if(!(iss >> x[i] >> y[i] >> l[i] >> h[i] >> t[i]))
		{
			break;
		}
	}

	file.close();
	
	if(i < count)
	{
		std::cerr << "in file " << file_name << ": only " << i+1 << " lines could be parsed" << std::endl;
	}
	else
	{
		writeSolution(x, y, l, h, t, count, ofile_name);
	}

	delete [] x;
	delete [] y;
	delete [] l;
	delete [] h;
	delete [] t;

	return count;
}


int evaluate(const char* file_name)
{
	std::ifstream file(file_name);
	if(!file)
	{
		std::cerr << "ERROR: Fehler beim Oeffnen " << file_name << std::endl;
		return -1;
	}

	int count;
	if(!(file >> count))
	{
		std::cerr << "ERROR: Fehler beim Parsen von " << file_name << " in Zeile 1" << std::endl;
		return -1;
	}

	if(count <= 0)
	{
		return -1;
	}

	int* x = new int[count];
	int* y = new int[count];
	int* l = new int[count];
	int* h = new int[count];
	std::string* t = new std::string[count];
	int* b = new int[count];
	int* xs = new int[count];
	int* ys = new int[count];

	std::string line;
	std::getline(file, line); //erstes '\n' ueberspringen
	for(int i = 0; i < count && std::getline(file, line); i++)
	{
		std::istringstream iss(line);
		if(!(iss >> x[i] >> y[i] >> l[i] >> h[i] >> t[i] >> b[i] >> xs[i] >> ys[i]))
		{
			std::cerr << "ERROR: Fehler beim Parsen von " << file_name << " in Zeile " << i+2 << std::endl;
			return -1;
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
					&& std::max(xs[i], xs[j]) < std::min(xs[i]+l[i], xs[j]+l[j])
					&& std::max(ys[i]-h[i], ys[j]-h[j]) < std::min(ys[i], ys[j]))
				{
					std::cerr << "ERROR: " << t[i] << " und " << t[j] << " ueberlappen sich" << std::endl;
					good = false;
				}
			}
		}
	}


	delete [] x;
	delete [] y;
	delete [] l;
	delete [] h;
	delete [] t;
	delete [] b;
	delete [] xs;
	delete [] ys;

	return good ? counter : -1;
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
		fileParser(input_file, output_file);
	}
	if(eval_file)
	{
		int counter = evaluate(eval_file);
		if(counter >= 0)
		{
			std::cout << counter << " beschriftete Punkte" << std::endl;
		}
	}
}


