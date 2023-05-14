////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Point2D.cpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Includes
#include "Point2D.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded output operator <<
	@param	std::ostream& os
			Reference to an ostream object
	@param	const Point2D& p
			Reference to Point2D type
	@return	object of type ostream reference with value equivalent to the 
			ostream object against which the output operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Point2D& p)
{
	os << p.mX << " " << p.mY  << "\n";
	return os;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded input operator >>
	@param	std::istream& is
			Reference to an istream object
	@param	const Point2D& p
			Reference to Point2D type
	@return	object of type istream reference with value equivalent to the 
			istream object against which the input operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::istream& operator>>(std::istream& is, Point2D& p)
{
	char str[100];
	is.getline(str, 100);

	float fx, fy;
	sscanf_s(str, " %f %f", &fx, &fy);
	p.SetPosition(fx, fy);

	return is;
}

}   // namespace CS200