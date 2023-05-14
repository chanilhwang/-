////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Texture.cpp
	@autor	pghali@digipen.edu
	@date	June 04, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Includes
#include "Texture.hpp"
#include <memory.h>

namespace CS200
{
	
////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture default constructor
*/
////////////////////////////////////////////////////////////////////////////////
Texture::Texture()
{
	mWidth		=	0;
	mHeight	=	0;
	mPtrToFirstTexel		=	0;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture copy constructor
	@param	rhs
			Input Texture
*/
////////////////////////////////////////////////////////////////////////////////
Texture::
Texture(const Texture& rhs)
{
	mWidth	=	rhs.mWidth;
	mHeight	=	rhs.mHeight;

	SetTexelData(rhs.mPtrToFirstTexel, rhs.mWidth, rhs.mHeight);
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture assignment operator
	@param	rhs
			The input Texture
*/
////////////////////////////////////////////////////////////////////////////////
Texture& Texture::
operator=(const Texture& rhs)
{
	if(this != &rhs)
	{
		mWidth	=	rhs.mWidth;
		mHeight	=	rhs.mHeight;

		SetTexelData(rhs.mPtrToFirstTexel, rhs.mWidth, rhs.mHeight);
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture destructor
*/
////////////////////////////////////////////////////////////////////////////////
Texture::
~Texture()
{
	delete [] mPtrToFirstTexel;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture auxiliary function: Assigns dimensions of texture map,
	releases memory used, assigns appropriate memory, and copies color data to newly
	allocated memory
	@param	pData
				Address of first element of array of color values
			width
				Columns in color map
			height
				Rows in color map
*/
////////////////////////////////////////////////////////////////////////////////
void Texture::
SetTexelData(const Color* pData, int width, int height)
{
	delete [] mPtrToFirstTexel;

	mWidth				=	width;
	mHeight				=	height;
	mPtrToFirstTexel	=	new Color[mWidth * mHeight];

	memset(mPtrToFirstTexel, 0, mWidth * mHeight * sizeof(Color));
	memcpy(mPtrToFirstTexel, pData, mWidth * mHeight * sizeof(Color));
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture auxiliary function: Returns address of first element of color array
	@return	The address of first element of color array
*/
////////////////////////////////////////////////////////////////////////////////
const Color* Texture
::GetTexelData() const
{
	return mPtrToFirstTexel;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture auxiliary function: Returns horizontal width of color array
	@return	The width of texture map
*/
////////////////////////////////////////////////////////////////////////////////
int Texture::
GetWidth() const
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture auxiliary function: Returns vertical height of color array
	@return	The height of texture map
*/
////////////////////////////////////////////////////////////////////////////////
int Texture::
GetHeight() const
{
	return mHeight;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture auxiliary function: Returns color value of texel (x, y)
	@param	uint
				Column location of texel
			vint
				Row location of texel
	@return	The color value at texel location (x, y) within texture map
*/
////////////////////////////////////////////////////////////////////////////////
const Color& Texture::
GetTexel(int uint, int vint) const
{
	if(uint < 0 || uint >= mWidth || vint < 0 || vint >= mHeight)
		return mPtrToFirstTexel[0];
	return mPtrToFirstTexel[uint + vint * mWidth];
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture auxiliary function: Returns color value of texel (x, y)
	@param	uint
				Column location of texel
			vint
				Row location of texel
	@return	The color value at texel location (x, y) within texture map
*/
////////////////////////////////////////////////////////////////////////////////
Color& Texture::
GetTexel(int uint, int vint)
{
	if(uint < 0 || uint >= mWidth || vint < 0 || vint >= mHeight)
		return mPtrToFirstTexel[0];
	return mPtrToFirstTexel[uint + vint * mWidth];
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Texture auxiliary function: Sets color value of texel (x, y)
	@param	uint
				Column location of texel
			vint
				Row location of texel
			clr
				Color coordinates
*/
////////////////////////////////////////////////////////////////////////////////
void Texture::
SetTexel(int uint, int vint, const Color& clr)
{
	mPtrToFirstTexel[uint + vint * mWidth] = clr;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Renders (r, g, b) coordinates of each texel of texture map to input window
	@param	window
				Window to draw to
			uint
				Horizontal offset of first column of color map from top-left window corner
			vint
				Vertical offset of first row of color map from top-left window corner
*/
////////////////////////////////////////////////////////////////////////////////
void Texture::
Draw(const Window& window, int uint, int vint) const
{
	int drawht	=	mHeight + vint;
	int drawwd	=	mWidth + uint;
	for (int v = vint; v < drawht; ++v)
		for (int u = uint; u < drawwd; ++u)
			window.SetPixel(u, v, GetTexel(u-uint, v-vint));
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Renders alpha value of each texel of texture map to input window
	@param	window
				Window to draw to
			uint
				Horizontal offset of first column of alpha map from top-left window corner
			y
				Vertical offset of first row of alpha map from top-left window corner
*/
////////////////////////////////////////////////////////////////////////////////
void Texture::
DrawAlpha(const Window& window, int uint, int vint) const
{
	int drawht	=	mHeight + vint;
	int drawwd	=	mWidth + uint;
	for (int v = vint; v < drawht; ++v)
		for (int u = uint; u < drawwd; ++u)
		{
			int alpha = GetTexel(u-uint, v-vint).GetA();
			window.SetPixel(u, v, Color(alpha, alpha, alpha, 255));
		}
}


}	// namespace CS200
