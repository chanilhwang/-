////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Window.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

// Includes
#include <windows.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include "Color.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Window
	@brief	Launches a window in the Win32 environment
*/
////////////////////////////////////////////////////////////////////////////////
class Window
{
public:
    Window();
    ~Window();

    bool			Create(HINSTANCE hInstance,
							const std::string& windowName,
							const unsigned int width, 
							const unsigned int height);
    unsigned int	Width() const;
    unsigned int	Height() const;
    void			SetPixel(const unsigned int x,
							const unsigned int y,
							const Color& color) const;
    void			ProcessMessages();
    void			Present();
	bool			Running() const;
	const HWND		GetWindowHandle() const { return mHwnd; }
	
private:

    // Prevent copying
	Window(const Window& rhs);
	Window& operator=(const Window& rhs);

	// Internal use
    bool			RegisterWndClass(HINSTANCE hInstance);
    void			UnregisterWndClass();
    void			CreateSecondaryBuffer(const unsigned int width, const unsigned int height);

	unsigned int	mWidth, mHeight;
	bool			mRunning;

    // Main window
	HWND			mHwnd;				// The window handle
	HDC				mHdc;				// The device context of the window
	HINSTANCE		mWindowInstance;	// The instance ID of the application
	std::string		mClassName;

    // Secondary window
    HBITMAP			mBitmap;
    HBITMAP			mOldBitmap;
    HDC				mBitmapDC;
    unsigned int*	mSurface;

};  // class Window

}   // namespace CS200

#endif  // WINDOW_HPP_