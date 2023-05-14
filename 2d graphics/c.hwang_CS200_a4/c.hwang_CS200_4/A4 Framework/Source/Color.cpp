////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Color.cpp
	@author	pghali@digipen.edu
	@date	May 09, 2005
	@par	CS200 Framework
	@par	Copyright 2005 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Includes
#include "Color.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded output operator <<
	@param	std::ostream& os
			Reference to an ostream object
	@param	const Color& c
			Reference to Color type
	@return	object of type ostream reference with value equivalent to the 
			ostream object against which the output operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::ostream&
operator<<(std::ostream& os, const Color& c)
{
	os << static_cast<int>(c.mColorRGBA[R]);
	os << " " << static_cast<int>(c.mColorRGBA[G]);
	os << " " << static_cast<int>(c.mColorRGBA[B]);
	os << " " << static_cast<int>(c.mColorRGBA[A]) << "\n";
	return os;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded input operator >>
	@param	std::istream& is
			Reference to an istream object
	@param	const Color& c
			Reference to Color type
	@return	object of type istream reference with value equivalent to the 
			istream object against which input operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::istream&
operator>>(std::istream& is, Color& c)
{
	char str[100];
	is.getline(str, 100);

	int r, g, b, a;
	sscanf_s(str, " %d %d %d %d", &r, &g, &b, &a);

	c.SetRGBA(	static_cast<unsigned char>(r),
				static_cast<unsigned char>(g),
				static_cast<unsigned char>(b),
				static_cast<unsigned char>(a));
	
	return is;
}

}	// namespace CS200
