#include "stdafx.h"
#include "SnakeType.h"

skPoint::skPoint() : dx(0), dy(0)
{
}

skPoint::skPoint(int x, int y) : dx(x), dy(y)
{
}

skPoint::skPoint(const skPoint& ep) : dx(ep.dx), dy(ep.dy)
{
}

skPoint::skPoint(const skVector& ev) : dx(ev.dx+0.5), dy(ev.dy+0.5)
{
}

skPoint& skPoint::operator=(const skPoint& ep)
{
	dx = ep.dx;
    dy = ep.dy;
    return *this;
}

bool skPoint::operator==( const skPoint& ep) const
{
	return ( dx==ep.dx ) && ( dy==ep.dy );
}

bool skPoint::operator!=( const skPoint& ep) const
{
	return !(*this == ep);
}

bool operator<(const skPoint& p, const skPoint& q)
{
	if(p.dx != q.dx) return p.dx < q.dx;
	else return p.dy < q.dy;
}

//////////////////////////////////////////////////////////////////////////

skVector::skVector() : dx(0), dy(0)
{
}

skVector::skVector(double x, double y) : dx(x), dy(y)
{
}

skVector::skVector(const skVector& ev) : dx(ev.dx), dy(ev.dy)
{
}

skVector::skVector(const skPoint& ep) : dx(ep.dx), dy(ep.dy)
{
}

skVector& skVector::operator=(const skVector& ev)
{
	dx = ev.dx;
	dy = ev.dy;
	return *this;
}

bool skVector::operator==( const skVector& ev) const
{
	return ( abs(dx-ev.dx) < EPSILON ) && ( abs(dy-ev.dy) < EPSILON );
}

bool skVector::operator!=( const skVector& ev) const
{
	return !(*this == ev);
}

double skVector::GetModule()
{
	return sqrt(dx*dx + dy*dy);
}

//////////////////////////////////////////////////////////////////////////

skVector operator+( const skVector& left, const skVector& right) 
{
	skVector sum;
	sum.dx = left.dx + right.dx;
	sum.dy = left.dy + right.dy;
	return sum;
}

skVector skVector::operator+=( const skVector& skv)
{
	dx += skv.dx;
	dy += skv.dy;
	return *this;
}

skVector operator-( const skVector& left, const skVector& right)
{
	skVector diff;
	diff.dx = left.dx - right.dx;
	diff.dy = left.dy - right.dy;
	return diff;
}

skVector skVector::operator-=( const skVector& skv)
{
	dx -= skv.dx;
	dy -= skv.dy;
	return *this;
}

skVector operator*( const skVector& left, const double& val)
{
	skVector result;
	result.dx = left.dx * val;
	result.dy = left.dy * val;
	return result;
}

skVector operator*( const double& val, const skVector& right )
{
	skVector result;
	result.dx = val * right.dx;
	result.dy = val * right.dy;
	return result;
}

skVector skVector::operator*=( const double& val)
{
	dx *= val;
	dy *= val;
	return *this;
}

skVector operator/( const skVector& numerator, const double& val )
{
	skVector result;
	result.dx = numerator.dx / val;
	result.dy = numerator.dy / val;
	return result;
}

skVector skVector::operator/=( const double& val)
{
	dx /= val;
	dy /= val;
	return *this;
}

// µã³Ë
double operator*( const skVector& left, const skVector& right)
{
    return left.dx * right.dx + left.dy * right.dy;
}

