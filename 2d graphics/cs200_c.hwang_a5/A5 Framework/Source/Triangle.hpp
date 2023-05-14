////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Triangle.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

// Includes
#include "Window.hpp"
#include "Vertex2D.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Triangle
	@brief	Simple triangle class 
*/
////////////////////////////////////////////////////////////////////////////////
class Triangle
{
public:
	Triangle() { }
	// No need for a Triangle copy constructor
	// No need for a Triangle overloaded assignment operator
	// No need for a Triangle destructor

	// Auxiliary constructor
	Triangle(const Vertex2D& v0,
			const Vertex2D& v1,
			const Vertex2D& v2) { mVertex[0] = v0; mVertex[1] = v1; mVertex[2] = v2; }

	// Auxiliary functions to be implemented ...
	bool			IsCounterClockwise() const;
	void			Draw(const Window&) const;

	// Auxiliary functions implemented ...
	const Vertex2D&	GetVertex(const unsigned idx) const					{ return mVertex[idx]; }
	Vertex2D		GetVertex(const unsigned idx)						{ return mVertex[idx]; }
	void			SetVertex(const Vertex2D& v, const unsigned idx)	{ mVertex[idx] = v; }

private:
	Vertex2D	mVertex[3];
};	// class Triangle

}	// namespace CS200

#endif	// TRIANGLE_HPP_