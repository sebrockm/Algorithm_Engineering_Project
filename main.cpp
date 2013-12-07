#include "Label.hpp"
#include "KDTree.hpp"
#include "crossing.hpp"
#include "Heuristic1.hpp"
#include "Heuristic2.hpp"

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
			<< "      " << progname << " -in dat1 -out dat2 [-heu 1] [-rec n] [-opt] reads dat1 and writes a solution to dat2 using Heuristic1 with n recursion steps (default is 0). If -opt is set, dat1 must contain a correct solution and Heuristic1 will try to optimize it." << endl
			<< "usage:" << progname << " -in dat1 -out dat2 -heu 2 reads dat1 and writes a solution to dat2 using Heuristic2" << endl
			<< "      " << progname << " -eval dat1          evaluates whether the solution in dat1 is correct." << endl;
}


void parse_options(int argc, char** argv, string& input_file, string& output_file, string& eval_file, int& heu, int& recN, bool& opt)
{
	if(argc < 2)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}

	heu = 1;
	recN = 0;
	opt = false;

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
				if(output_file.empty() && input_file.empty() && eval_file.empty() && recN == 0 && !opt)
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
		else if(string(argv[i]) == "-heu")
		{
			if(++i < argc)
			{
				heu = atoi(argv[i]);
				if(heu != 1 && heu != 2)
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
		else if(string(argv[i]) == "-opt")
		{
			opt = true;
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



void writeSolution(vector<Label>& labels, const string& file_name, int heu, int recN)
{
	ofstream file(file_name);
	if(!file)
	{
		cerr << "cannot write " << file_name << endl;
		return;
	}

	int counter = 0;
	int tmpCounter = 0;
	auto t1 = chrono::high_resolution_clock::now();

	sort(labels.begin(), labels.end(), [](const Label& l1, const Label& l2){return l1.h()*l1.l() < l2.h()*l2.l();});

	if(heu == 1)
	{
		Heuristic1 heu(labels, recN);

		int i = 1;
		do
		{
			tmpCounter = 0;
			for(unsigned j = 0; j < labels.size(); j++)
			{
				cout << i << ". Durchlauf: " << 100*j/labels.size() << "%\t" << counter+tmpCounter << "\r";
				if(labels[j].b() == 0)
				{
					tmpCounter += heu.tryToEnable(labels[j]);
				}
			}
			counter += tmpCounter;
			i++;
		}while(tmpCounter > 0);
	}
	else if(heu == 2)
	{
		Heuristic2 heu(labels);

		int i = 1;
		while((tmpCounter = heu.start()) > 0)
		{
			cout << i << ". Durchlauf" << "\r";
			counter += tmpCounter;
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	cout << "                                                                  \r";
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


int fileParser(const string& file_name, vector<Label>& labels, bool opt)
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
	int b, xs, ys;
	string line;
	getline(file, line); //erstes '\n' ueberspringen
	for(int i = 0; i < count && getline(file, line); i++)
	{
		istringstream iss(line);

		if(!(iss >> x >> y >> l >> h >> t))
		{
			break;
		}
		if(opt)
		{
			if(!(iss >> b >> xs >> ys))
			{
				break;
			}
			labels.emplace_back(x, y, l, h, t, b, xs, ys);
		}
		else
		{
			labels.emplace_back(x, y, l, h, t, 0);
		}
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
	int heu;
	int recN;
	bool opt;


	parse_options(argc, argv, input_file, output_file, eval_file, heu, recN, opt);

	if(!input_file.empty() && !output_file.empty())
	{
		vector<Label> labels;
		fileParser(input_file, labels, opt);

		writeSolution(labels, output_file, heu, recN);
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


