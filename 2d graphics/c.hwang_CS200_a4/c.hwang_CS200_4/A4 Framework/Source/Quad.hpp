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
#ifndef QUAD_HPP_
#define QUAD_HPP_

// Includes
#include "Vertex2D.hpp"
#include "Triangle.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "UI.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Quad
	@brief	Simple class for 4-gons
*/
////////////////////////////////////////////////////////////////////////////////
class Quad
{
public:
	// No need for a Quad copy constructor
	// No need for a Quad overloaded assignment operator
	// No need for a Quad destructor

	// Auxiliary constructor
	Quad(const Vertex2D& v0,
		const Vertex2D& v1,
		const Vertex2D& v2,
		const Vertex2D& v3)
	{
		mVertex[0] = v0; mVertex[1] = v1; mVertex[2] = v2; mVertex[3] = v3;
	}

	void			Draw(const Window&			window, 
						enum ColorCombineMode	ccMode,
						enum TexFuncUMode			uMode,
						enum TexFuncVMode			vMode) const;

	const Vertex2D&	GetVertex(unsigned index) const			{ return mVertex[index]; }
	Vertex2D&		GetVertex(unsigned index)				{ return mVertex[index]; }
	void			SetVertex(const Vertex2D& v, unsigned index)	{ mVertex[index] = v; }
	void			SetVertexU(float u, unsigned index)				{ mVertex[index].SetU(u); }
	void			SetVertexV(float v, unsigned index)				{ mVertex[index].SetV(v); }
	void			SetVertexUV(float u, float v, unsigned index)	{ mVertex[index].SetUV(u, v); }

private:
	Vertex2D		mVertex[4];
};	// class Quad

}	// namespace CS200

#endif	// QUAD_HPP_