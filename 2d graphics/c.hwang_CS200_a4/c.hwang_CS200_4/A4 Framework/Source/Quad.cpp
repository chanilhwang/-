////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Quad.cpp
	@autor	pghali@digipen.edu
	@date	June 04, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Includes
#include "Quad.hpp"

// global objects

namespace CS200
{
extern Texture	gTexMapList[TEXTURE_MAX];
extern enum TextureType	gTexMapType;

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Renders the Quad object to the input window
	@param	window
			Window to draw to
	@Notes	1. Discard clockwise oriented quads.
*/
////////////////////////////////////////////////////////////////////////////////
void Quad::
Draw(const Window&			window,
	enum ColorCombineMode	ccMode,
	enum TexFuncUMode			uMode,
	enum TexFuncVMode			vMode) const
{
	Triangle t0(mVertex[0], mVertex[1], mVertex[2]), t1(mVertex[0], mVertex[2], mVertex[3]);
	t0.Draw(window, ccMode, uMode, vMode);
	t1.Draw(window, ccMode, uMode, vMode);
}

}	// namespace CS200
