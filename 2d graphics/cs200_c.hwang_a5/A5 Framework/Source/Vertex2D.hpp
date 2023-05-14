////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Vertex2D.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef VERTEX2D_HPP_
#define VERTEX2D_HPP_

// Includes
#include <iostream>
#include "Point2D.hpp"
#include "Color.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Vertex2D
	@brief	Simple vertex class 
*/
////////////////////////////////////////////////////////////////////////////////
class Vertex2D
{
	friend std::ostream& operator<<(std::ostream&, const Vertex2D&);
	friend std::istream& operator>>(std::istream&, Vertex2D&);
public:
	// default constructor
	Vertex2D(	float x			= 0.f, 
				float y			= 0.f, 
				unsigned char r	= 0,
				unsigned char g = 0,
				unsigned char b = 0,
				unsigned char a = 255
			) : mPos(x, y), mClr(r, g, b, a) { }

	// No need for a Vertex2D copy constructor
	// No need for a Vertex2D overloaded assignment operator
	// No need for a Vertex2D destructor
	
	// Auxiliary constructor
	Vertex2D(const Point2D& p, const Color& c) : mPos(p), mClr(c) { }
	Vertex2D(float x, float y, const Color& c) : mPos(x, y), mClr(c) { }

	// Auxiliary functions
	void			SetPosition(const Point2D& p)	{ mPos = p; }
	void			SetPosition(float x, float y)	{ mPos.SetX(x); mPos.SetY(y); }
	void			SetColor(const Color& c)		{ mClr	=	c; }

	const Point2D&	GetPosition() const				{ return mPos; }
	Point2D&		GetPosition()					{ return mPos; }
	const Color&	GetColor() const				{ return mClr; }
	Color&			GetColor()						{ return mClr; }

private:
	Point2D mPos;	// position of vertex
	Color	mClr;	// color of vertex
};

}		// namespace CS200

#endif //VERTEX2D_HPP_
