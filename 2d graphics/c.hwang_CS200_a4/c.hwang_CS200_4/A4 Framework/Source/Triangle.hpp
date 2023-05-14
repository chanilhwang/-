////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Triangle.hpp
	@autor	pghali@digipen.edu
	@date	June 15, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

// Includes
#include "Vertex2D.hpp"
#include "Texture.hpp"
#include "UI.hpp"

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
	// No need for default constructor, copy constructor, and destructor
	// No need for overloaded assignment operator

	// Auxiliary constructor ...
	Triangle(const Vertex2D& v0,
			const Vertex2D& v1,
			const Vertex2D& v2)
	{
		mVertex[0] = v0;
		mVertex[1] = v1;
		mVertex[2] = v2;
	}

	void Draw(const Window&			window,
			enum ColorCombineMode	ccMode,
			enum TexFuncUMode			uMode,
			enum TexFuncVMode			vMode) const;

	const Vertex2D& GetVertex(unsigned index) const				{ return mVertex[index]; }
	Vertex2D		GetVertex(unsigned index)					{ return mVertex[index]; }
	void			SetVertex(const Vertex2D& v, unsigned index){ mVertex[index] = v; }

private:
	Vertex2D	mVertex[3];
};	// class Triangle

}	// namespace CS200

#endif	// TRIANGLE_HPP_