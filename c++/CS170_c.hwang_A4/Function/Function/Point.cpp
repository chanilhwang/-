#include "Point.h"  // Point members
#include <cmath>    // sqrt, atan, sin, cos

namespace CS170
{

const double PI = 3.1415926535897; //!< Length of half a circle of radius 1
const double EPSILON = 0.00001;    //!< Smallest difference in two doubles

///////////////////////////////////////////////////////////////////////////////
// private member functions 

double Point::DegreesToRadians(double degrees) const
{
  return (degrees * PI / 180.0);
}

double Point::RadiansToDegrees(double radians) const
{
  return (radians * 180.0 / PI);
}
///////////////////////////////////////////////////////////////////////////////
// 16 public member functions (2 constructors, 14 operators) 
Point::Point() : m_x(0), m_y(0) {}
Point::Point(double xaxis, double yaxis)
{
	if (xaxis > -EPSILON && xaxis < EPSILON)
		xaxis = 0.0;
	if (yaxis > -EPSILON && yaxis < EPSILON)
		yaxis = 0.0;
	m_x = xaxis;
	m_y = yaxis;
}
Point Point::operator%(double _d) const
{
	double Radian = DegreesToRadians(_d);
	Point p(cos(Radian) * m_x - sin(Radian) * m_y,
			sin(Radian) * m_x + cos(Radian) * m_y);
	return p;
}
double Point::operator-(const Point& _point) const
{
	return sqrt((m_x - _point.m_x) * (m_x - _point.m_x) + (m_y - _point.m_y) * (m_y - _point.m_y));
}
Point Point::operator^(const Point& _point) const
{
	Point p((m_x + _point.m_x) / 2.0,
			(m_y + _point.m_y) / 2.0);
	return p;
}
Point& Point::operator+=(const Point& _point)
{
	m_x += _point.m_x;
	m_y += _point.m_y;
	return *this;
}
Point& Point::operator+=(double _d)
{
	m_x += _d;
	m_y += _d;
	return *this;
}
Point Point::operator-(double _d) const
{
	Point p(m_x - _d,
			m_y - _d);
	return p;
}
Point& Point::operator++()
{
	m_x += 1;
	m_y += 1;
	return *this;
}
Point Point::operator++(int)
{
	Point temp(m_x, m_y);
	++*this;
	return temp;
}
Point& Point::operator--()
{
	m_x -= 1;
	m_y -= 1;
	return *this;
}
Point Point::operator--(int)
{
	Point temp(m_x, m_y);
	--(*this);
	return temp;
}
Point Point::operator-() const
{
	Point p(-m_x, -m_y);
	return p;
}
Point Point::operator+(double _d) const
{
	Point p(m_x + _d,
			m_y + _d);
	return p;
}
Point Point::operator+(const Point& _point) const
{
	Point p(m_x + _point.m_x,
			m_y + _point.m_y);
	return p;
}
Point Point::operator*( double _d) const
{
	Point p(m_x * _d,
			m_y * _d);
	return p;
}

///////////////////////////////////////////////////////////////////////////////
// 2 friend functions (operators)
std::ostream &operator<<(std::ostream &os, const Point& _point)
{
	os << "(";
	os << _point.m_x << ", " << _point.m_y;
	os << ")";
	return os;
}
std::istream &operator >> (std::istream &is, Point& _point)
{
	is >> _point.m_x;
	is >> _point.m_y;
	return is;
}

///////////////////////////////////////////////////////////////////////////////
// 2 non-members, non-friends (operators)
Point operator*( double _d,  Point& _point)
{
	return _point * _d;
}
Point operator+( double _d,  Point& _point)
{
	return _point + _d;
}

} // namespace CS170