#include "Label.hpp"

#include <algorithm>


bool labelCross(const Label& label1, const Label& label2)
{
	 return label1.b() == 1 && label2.b() == 1 &&
		std::max(label1.xs(), label2.xs()) < std::min(label1.xs()+label1.l(), label2.xs()+label2.l()) &&
		std::max(label1.ys()-label1.h(), label2.ys()-label2.h()) < std::min(label1.ys(), label2.ys());
}

Label::Label(int x, int y, int l, int h, string name, int b)
	:_x(x), _y(y), _l(l), _h(h), _name(name), _b(b) {}

Label::Label(int x, int y, int l, int h, string name, int b, int xs, int ys)
	:_x(x), _y(y), _l(l), _h(h), _name(name), _b(b) 
{
	if(xs == x)
	{
		if(ys == y)
			_p = br;
		else if(ys == y+h)
			_p = tr;
		else
			throw invalid_argument(name + "ys does not match y nor y+h");
	}
	else if(xs == x-l)
	{
		if(ys == y)
			_p = bl;
		else if(ys == y+h)
			_p = tl;
		else
			throw invalid_argument(name + "ys does not match y nor y+h");
	}
	else
	{
		throw invalid_argument(name + "xs does not match x nor x-l");
	}
}

void Label::setPos(Pos p)
{
	_p = p;
}

Label::Pos Label::getPos() const
{
	return _p;
}

void Label::rotateCCW()
{
	_p = (Pos)(((int)_p+3)%4);
}

void Label::rotateCW()
{
	_p = (Pos)(((int)_p+1)%4);
}

int Label::x() const
{
	return _x;
}
int Label::y() const
{
	return _y;
}
int Label::l() const 
{
	return _l;
}
int Label::h() const
{
	return _h;
}
string Label::name() const
{
	return _name;
}
int Label::b() const
{
	return _b;
}
void Label::enable()
{
	_b = 1;
}
void Label::disable()
{
	_b = 0;
}
int Label::xs() const
{
	return (_p==br || _p==tr) ? _x : _x-_l;
}
int Label::ys() const
{
	return (_p==br || _p==bl) ? _y : _y+_h;
}
int Label::maxX() const
{
	return _x + _l;
}
int Label::minX() const
{
	return _x - _l;
}
int Label::maxY() const
{
	return _y + _h;
}
int Label::minY() const
{
	return _y - _h;
}
