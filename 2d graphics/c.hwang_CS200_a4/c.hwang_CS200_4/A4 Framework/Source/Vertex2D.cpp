////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Vertex2D.cpp
	@author	pghali@digipen.edu
	@date	June 04, 2005
	@par	CS200 Framework
	@par	Copyright 2005 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Includes
#include "Vertex2D.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded output operator <<
	@param	std::ostream& os
			Reference to an ostream object
	@param	const Vertex2D& p
			Reference to Vertex2D type
	@return	object of type ostream reference with value equivalent to the 
			ostream object against which the output operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::ostream&
operator<<(std::ostream& os, const Vertex2D& v)
{
	os << v.mPos;
	os << v.mClr;

	return os;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded input operator >>
	@param	std::istream& is
			Reference to an istream object
	@param	const Vertex2D& p
			Reference to Vertex2D type
	@return	object of type istream reference with value equivalent to the 
			istream object against which input operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::istream&
operator>>(std::istream& is, Vertex2D& v)
{
	Point2D pos;
	Color	clr;

	is >> pos;
	is >> clr;

	v.SetPosition(pos);
	v.SetColor(clr);

	return is;

}

}	// namespace CS200

