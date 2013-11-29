#ifndef _LABEL_HPP_
#define _LABEL_HPP_

#include <string>
#include <stdexcept>

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
	Label(int x, int y, int l, int h, string name, int b)
		:_x(x), _y(y), _l(l), _h(h), _name(name), _b(b) {}

	Label(int x, int y, int l, int h, string name, int b, int xs, int ys)
		:_x(x), _y(y), _l(l), _h(h), _name(name), _b(b) 
	{
		if(xs == x)
		{
			if(ys == y)
				_p = br;
			else if(ys == y+h)
				_p = tr;
			else
				throw invalid_argument("ys does not match y nor y+h");
		}
		else if(xs == x-l)
		{
			if(ys == y)
				_p = bl;
			else if(ys == y+h)
				_p = tl;
			else
				throw invalid_argument("ys does not match y nor y+h");
		}
		else
		{
			throw invalid_argument("xs does not match x nor x-l");
		}
	}

	void setPos(Pos p)
	{
		_p = p;
	}

	Pos getPos() const
	{
		return _p;
	}

	void rotateCCW()
	{
		_p = (Pos)(((int)_p+3)%4);
	}

	void rotateCW()
	{
		_p = (Pos)(((int)_p+1)%4);
	}
	
	int x() const
	{
		return _x;
	}
	int y() const
	{
		return _y;
	}
	int l() const 
	{
		return _l;
	}
	int h() const
	{
		return _h;
	}
	string name() const
	{
		return _name;
	}
	int& b()
	{
		return _b;
	}
	const int& b() const
	{
		return _b;
	}
	int xs() const
	{
		return (_p==br || _b==tr) ? _x : _x-_l;
	}
	int ys() const
	{
		return (_p==br || _p==bl) ? _y : _y+_h;
	}
};

/**
 * Checks whether two Labels are crossing.
 * @param label1 
 * @param label2
 * @return true if the Labels are crossing, false otherwise
 */
bool labelCross(const Label& label1, const Label& label2);

#endif

