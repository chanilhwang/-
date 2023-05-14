////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Color.hpp
	@autor	pghali@digipen.edu
	@date	May 09, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef COLOR_HPP_
#define COLOR_HPP_

// Includes
#include <iostream>

namespace CS200
{

enum ColorComponent{R = 0, G, B, A};

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Color
	@brief	Simple color class 
*/
////////////////////////////////////////////////////////////////////////////////
class Color
{
	// overloaded output operator
	friend std::ostream& operator<<(std::ostream&, const Color&);
	friend std::istream& operator>>(std::istream&, Color&);

public:
	// default constructor
	Color(	const unsigned char r = 0,
			const unsigned char g = 0,
			const unsigned char b = 0,
			const unsigned char a = 255)
		{
			mColorRGBA[R] = r;
			mColorRGBA[G] = g;
			mColorRGBA[B] = b;
			mColorRGBA[A] = a;
		}

	// No need for a Color copy constructor
	// No need for a Color overloaded assignment operator
	// No need for a Color destructor

	// Auxiliary functions 
	unsigned char GetR() const				{ return mColorRGBA[R]; }
	unsigned char GetG() const				{ return mColorRGBA[G]; }
	unsigned char GetB() const				{ return mColorRGBA[B]; }
	unsigned char GetA() const				{ return mColorRGBA[A]; }

	void SetR(const unsigned char r)		{ mColorRGBA[R] = r; }
	void SetG(const unsigned char g)		{ mColorRGBA[G] = g; }
	void SetB(const unsigned char b)		{ mColorRGBA[B] = b; }
	void SetA(const unsigned char a)		{ mColorRGBA[A] = a; }
	void SetRGBA(	const unsigned char r,
					const unsigned char g,
					const unsigned char b,
					const unsigned char a)	{ SetR(r); SetG(g); SetB(b); SetA(a); }

private:
	unsigned char mColorRGBA[4];
};	// class Color

}	// namespace CS200

#endif	// COLOR_HPP_