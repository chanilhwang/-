////////////////////////////////////////////////////////////////////////////////
/*!
	@file	UI.hpp
	@autor	pghali@digipen.edu
	@date	May 10, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef UI_HPP_
#define UI_HPP_

#include "Quad.hpp"
#include "..\resource.h"

namespace CS200
{
// forward declaration
class Quad;

enum ModeType
{ 
	MODE_CREATE		=	ID_MODE_CREATEQUAD,
	MODE_TEXCOORD	=	ID_MODE_EDITTEXCOORDS,
	MODE_CLRCOORD	=	ID_MODE_EDITCLRCOORDS,
	MODE_TEST		=	ID_MODE_TEST
};

enum TextureType
{
	TEXTURE_ONE		=	0,		// checkerboard
	TEXTURE_TWO,				// duck hi-res
	TEXTURE_THREE,				// water-lily hi-res
	TEXTURE_FOUR,				// duck low-res
	TEXTURE_FIVE,				// water-lily low-res
	TEXTURE_MAX
};

enum ColorCombineMode
{ 
	CC_REPLACE	=	ID_COLORCOMBINER_REPLACE,	// texture mapping
	CC_DECAL	=	ID_COLORCOMBINER_DECAL,		// alpha blending
	CC_MODULATE	=	ID_COLORCOMBINER_MODULATE,	// modulation of r, g, b components
	CC_GOURARD	=	ID_COLORCOMBINER_GOURARD	// vertex color interpolation
};

enum TexFuncUMode
{
	TEXFUNCU_CLAMP			=	ID_TEXFUNCU_CLAMP,
	TEXFUNCU_REPEAT			=	ID_TEXFUNCU_REPEAT,
	TEXFUNCU_MIRROREDREPEAT	=	ID_TEXFUNCU_MIRROREDREPEAT
};

enum TexFuncVMode
{
	TEXFUNCV_CLAMP			=	ID_TEXFUNCV_CLAMP,
	TEXFUNCV_REPEAT			=	ID_TEXFUNCV_REPEAT,
	TEXFUNCV_MIRROREDREPEAT	=	ID_TEXFUNCV_MIRROREDREPEAT
};

extern enum ColorCombineMode	gCCMode, gPrevCCMode;
extern enum ModeType			gInputMode, gPrevInputMode;
extern enum TextureType			gTexMapType, gPrevTexObject;
extern enum TexFuncUMode		gTexFuncUMode, gPrevTexFuncUMode;
extern enum TexFuncVMode		gTexFuncVMode, gPrevTexFuncVMode;

extern std::string				gModalMsg;
extern std::vector<Quad>*		gPActiveList;
extern Texture					gTexMapList[TEXTURE_MAX];
extern Vertex2D*				gpPickedVtx;

extern bool ModeIsActive(const enum ModeType mode);

}   // namespace CS200

#endif

