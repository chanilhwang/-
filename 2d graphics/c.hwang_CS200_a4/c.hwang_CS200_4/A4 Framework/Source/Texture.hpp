////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Texture.hpp
	@autor	pghali@digipen.edu
	@date	May 09, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

// Includes
#include "Window.hpp"
#include "Color.hpp"

namespace CS200
{
	
////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Texture
	@brief	Simple texture class 
*/
////////////////////////////////////////////////////////////////////////////////
class Texture
{
public:
	Texture();
	Texture(const Texture&);
	Texture & operator=(const Texture&);
	~Texture();

	// Retrieve texel located at texel coordinates (uint, vint)
	const Color&	GetTexel(int uint, int vint) const;
	Color&			GetTexel(int uint, int vint);

	// Getter 
	const Color*	GetTexelData() const;
	int				GetWidth() const;
	int				GetHeight() const;

	// Setter
	void			SetTexelData(const Color* _data, int width, int height);
	void			SetTexel(int uint, int vint, const Color& color);

	void			Draw(const Window & window, int x, int y) const;
	void			DrawAlpha(const Window & window,int x, int y) const;

private:
	Color*	mPtrToFirstTexel;
	int		mWidth, mHeight;
};	// class Texture

}

#endif	// TEXTURE_HPP_
