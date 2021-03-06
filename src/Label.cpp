#include "Label.hpp"

#include <algorithm>


bool labelCross(const Label& label1, const Label& label2)
{
	 return label1.b() == 1 && label2.b() == 1 && labelCouldCross(label1, label2);
}

bool labelWouldCross(const Label& label1, const Label& label2)
{
	return label1.b() + label2.b() == 1 && labelCouldCross(label1, label2);
}

bool labelCouldCross(const Label& label1, const Label& label2)
{
        return label1.xs() < label2.xs()+label2.l() && label2.xs() < label1.xs()+label1.l() &&
		label1.ys()-label1.h() < label2.ys() && label2.ys()-label2.h() < label1.ys();
}

Label::Label(int x, int y, int l, int h, string name, int b)
	:_x(x), _y(y), _l(l), _h(h), _name(name), _b(b), _p((Pos)0),_isFixed{0,0,0,0} {}

Label::Label(int x, int y, int l, int h, string name, int b, int xs, int ys)
	:_x(x), _y(y), _l(l), _h(h), _name(name), _b(b), _p((Pos)0),_isFixed{0,0,0,0} 
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

Label::Label(const Label& cpy)
{
    _p = cpy._p;
    _x = cpy._x;
    _y = cpy._y;
    _name = cpy._name;
    _b = cpy._b;
    _l = cpy._l;
    _h = cpy._h;
    _isFixed[0] = cpy._isFixed[0];
    _isFixed[1] = cpy._isFixed[1];
    _isFixed[2] = cpy._isFixed[2];
    _isFixed[3] = cpy._isFixed[3];
}

void Label::setPos(Pos p)
{
    if(fixedCount() == 4 && p != _p)
		throw invalid_argument("alles fix, aber pos soll geaendert werden");

	_p = p;
    while(_isFixed[(int)_p])
        rotateCW();
}

Label::Pos Label::getPos() const
{
	return _p;
}

void Label::rotateCCW()
{
    if(fixedCount() == 4)
        return;
	_p = (Pos)(((int)_p+3)%4);
    while(_isFixed[(int)_p])
        rotateCCW();
}

void Label::rotateCW()
{
    if(fixedCount() == 4)
        return;
	_p = (Pos)(((int)_p+1)%4);
    while(_isFixed[(int)_p])
        rotateCW();
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
    if(_b != 1 && _isFixed[(int)_p])
		throw invalid_argument("versuche gefixte pos zu enablen");
	_b = 1;
}
void Label::disable()
{
    if(_b != 0 && fixedCount() > 0)
		throw invalid_argument("versuche gefixte pos zu disablen");
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
