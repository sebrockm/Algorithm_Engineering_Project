#include "Label.hpp"
#include "crossing.hpp"
#include "Heuristic1.hpp"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


void usage(const char* progname)
{
	cerr	<< endl
			<< "usage:" << endl
			<< "      " << progname << " -in dat1 -out dat2 [-rec n] reads dat1 and writes a solution to dat2 using Heuristic1 with n recursion steps (default is 0)" << endl
			<< "      " << progname << " -eval dat1          evaluates whether the solution in dat1 is correct" << endl;
}


void parse_options(int argc, char** argv, string& input_file, string& output_file, string& eval_file, int& recN)
{
	if(argc < 2)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	recN = 0;

	for(int i = 1; i < argc; i++)
	{
		if(string(argv[i]) == "-in")
		{ 
			if(++i < argc)
			{
				if(input_file.empty() && eval_file.empty())
				{
					input_file = argv[i];
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
		else if(string(argv[i]) == "-out")
		{
			if(++i < argc)
			{
				if(output_file.empty() && eval_file.empty())
				{
					output_file = argv[i];
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
		else if(string(argv[i]) == "-eval")
		{
			if(++i < argc)
			{
				if(output_file.empty() && input_file.empty() && eval_file.empty())
				{
					eval_file = argv[i];
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
		else if(string(argv[i]) == "-rec")
		{
			if(++i < argc)
			{
				recN = atoi(argv[i]);
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

	if(input_file.empty() ^ output_file.empty())
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
}



void writeSolution(vector<Label>& labels, const string& file_name, int recN)
{
	ofstream file(file_name);
	if(!file)
	{
		cerr << "cannot write " << file_name << endl;
		return;
	}

	int counter = 0;
	auto t1 = chrono::high_resolution_clock::now();

	Heuristic1 heu(labels, recN);

	for(int i = 0; i < 4; i++)//viermal wiederholen, weil vier cool ist
	{
		for(auto& label : labels)
		{
			if(label.b() == 0)
			{
				counter += heu.tryToEnable(label);
			}
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	cout << counter << "\t" << (chrono::duration_cast<chrono::duration<double>>(t2-t1)).count() << endl;

	bool ok = true;

	//Loesung in Datei schreiben
	if(!(file << labels.size() << "\n"))
	{
		cerr << "could not write to " << file_name << endl;
		ok = false;
	}
	for(unsigned i = 0; ok && i < labels.size(); i++)
	{
		if(!(file << labels[i].x() << " " << labels[i].y() << " " << labels[i].l() << " " << labels[i].h() << " " 
			<< labels[i].name() << " " << labels[i].b() << " " << labels[i].xs() << " " << labels[i].ys() << "\n"))
		{
			cerr << "could not write to " << file_name << endl;
			ok = false;
		}
	}
}


int fileParser(const string& file_name, vector<Label>& labels)
{
	ifstream file(file_name);
	if(!file)
	{
		cerr << "cannot open " << file_name << endl;
		exit(EXIT_FAILURE);
	}

	int count;
	if(!(file >> count))
	{
		cerr << "failed reading line 1 of " << file_name << endl;
		return 0;
	}

	int x, y, l, h;
	string t;
	string line;
	getline(file, line); //erstes '\n' ueberspringen
	for(int i = 0; i < count && getline(file, line); i++)
	{
		istringstream iss(line);

		if(!(iss >> x >> y >> l >> h >> t))
		{
			break;
		}
		labels.emplace_back(x, y, l, h, t, 0);
	}

	file.close();
	
	if((int)labels.size() < count)
	{
		cerr << "in file " << file_name << ": only " << labels.size()+1 << " lines could be parsed" << endl;
		exit(EXIT_FAILURE);
	}

	return count;
}


int evaluate(const string& file_name)
{
	ifstream file(file_name);
	if(!file)
	{
		cerr << "ERROR: Fehler beim Oeffnen " << file_name << endl;
		return -1;
	}

	int count;
	if(!(file >> count))
	{
		cerr << "ERROR: Fehler beim Parsen von " << file_name << " in Zeile 1" << endl;
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
	string* t = new string[count];
	int* b = new int[count];
	int* xs = new int[count];
	int* ys = new int[count];

	string line;
	getline(file, line); //erstes '\n' ueberspringen
	for(int i = 0; i < count && getline(file, line); i++)
	{
		istringstream iss(line);
		if(!(iss >> x[i] >> y[i] >> l[i] >> h[i] >> t[i] >> b[i] >> xs[i] >> ys[i]))
		{
			cerr << "ERROR: Fehler beim Parsen von " << file_name << " in Zeile " << i+2 << endl;
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
				cerr << "ERROR: Keine Ecke von " << t[i] << " liegt auf dem Punkt" << endl;
				good = false;
			}
			
			for(int j = i+1; j < count; j++)
			{
				if(b[j] == 1 //Ueberlappung der Rechtecke
					&& max(xs[i], xs[j]) < min(xs[i]+l[i], xs[j]+l[j])
					&& max(ys[i]-h[i], ys[j]-h[j]) < min(ys[i], ys[j]))
				{
					cerr << "ERROR: " << t[i] << " und " << t[j] << " ueberlappen sich" << endl;
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
	string input_file;
	string output_file;
	string eval_file;
	int recN;

	parse_options(argc, argv, input_file, output_file, eval_file, recN);

	if(!input_file.empty() && !output_file.empty())
	{
		vector<Label> labels;
		fileParser(input_file, labels);
		writeSolution(labels, output_file, recN);
	}
	if(!eval_file.empty())
	{
		int counter = evaluate(eval_file);
		if(counter >= 0)
		{
			cout << counter << endl;
		}
	}
}


