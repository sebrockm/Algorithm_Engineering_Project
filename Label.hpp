#ifndef _LABEL_HPP_
#define _LABEL_HPP_

#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

class Label
{
public:
	enum Pos
	{
		br = 0,//bottom right
		bl,//bottom left
		tl,//top left
		tr//top right
	};

private:
	int _x, _y, _l, _h;
	std::string _name;
	int _b;
	Pos _p;

public:
	Label(int x, int y, int l, int h, string name, int b);

	Label(int x, int y, int l, int h, string name, int b, int xs, int ys);

	void setPos(Pos p);

	Pos getPos() const;

	void rotateCCW();

	void rotateCW();
	
	int x() const;
	int y() const;
	int l() const;
	int h() const;
	string name() const;
	int b() const;
	void enable();
	void disable();
	int xs() const;
	int ys() const;
};

/**
 * Checks whether two Labels are crossing.
 * @param label1 
 * @param label2
 * @return true if the Labels are crossing, false otherwise
 */
bool labelCross(const Label& label1, const Label& label2);

#endif

