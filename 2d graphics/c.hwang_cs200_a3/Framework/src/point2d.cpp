/******************************************************************************/
/*!
\file   point2d.cpp
\par    Purpose: Implement functions for 2d point.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\par    Course: CS200
\par    Assignment 4
\date   28/10/2014
*/
/******************************************************************************/

#include "point2d.hpp"


/******************************************************************************/
/*!
\brief
        Overloaded output operator <<
\param  std::ostream& os
        Reference to an ostream object
\param  const Point2D& p
        Reference to Point2D type
\return
        Object of type ostream reference with value equivalent to the 
            ostream object against which the output operator is applied
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Point2D& p)
{
	os << p.mX << " " << p.mY  << std::endl;
	return os;
}

/******************************************************************************/
/*!
\brief
        Overloaded input operator >>
\param  std::istream& is
        Reference to an istream object
\param  const Point2D& p
        Reference to Point2D type
\return
        Object of type istream reference with value equivalent to the 
            istream object against which input operator is applied
*/
/******************************************************************************/
std::istream& operator>>(std::istream& is, Point2D& p)
{
	char str[100];
	is.getline(str, 100);

	float fx, fy;
	sscanf_s(str, " %f %f", &fx, &fy);
	p.SetPosition(fx, fy);

	return is;
}
