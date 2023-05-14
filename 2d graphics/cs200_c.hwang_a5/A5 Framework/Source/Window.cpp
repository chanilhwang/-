////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Window.cpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////


// Includes
#include "Window.hpp"
#include "UI.hpp"
#include "..\resource.h"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Window default constructor
*/
////////////////////////////////////////////////////////////////////////////////
Window::Window() :	mWidth(0), mHeight(0), mRunning(false), mHwnd(0), 
					mHdc(0), mWindowInstance(0), mClassName("WndClass"), 
                    mBitmap(0), mOldBitmap(0), mBitmapDC(0), mSurface(0)
{}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Window destructor
*/
////////////////////////////////////////////////////////////////////////////////
Window::~Window()
{
    // Get rid of the secondary window
    mBitmap = (HBITMAP)::SelectObject(mBitmapDC, mOldBitmap);
    ::DeleteObject(mBitmap);
    ::ReleaseDC(NULL, mBitmapDC);

    // Get rid of the main window
    UnregisterWndClass();
    ::ReleaseDC(mHwnd, mHdc);
    
    mRunning = false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Returns the width of the Window
	@return	Width of the Window
*/
////////////////////////////////////////////////////////////////////////////////
unsigned int Window::Width() const
{
	return mWidth;
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Returns the height of the Window
	@return	Height of the Window
*/
////////////////////////////////////////////////////////////////////////////////
unsigned int Window::Height() const
{
	return mHeight;
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Returns the status of the Window (running or not running)
	@return	true if the Window is runing, false if not
*/
////////////////////////////////////////////////////////////////////////////////
bool Window::Running() const
{
	return mRunning;
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Registers the WNDCLASS with Windows
	@param	hInstance
			Windows window instance
	@return	true for success, false for error
*/
////////////////////////////////////////////////////////////////////////////////
bool Window::RegisterWndClass(HINSTANCE hInstance)
{
     WNDCLASS wc;

	// fill out the window style structure...mostly default options
    wc.style			=	CS_DBLCLKS;
	wc.lpfnWndProc		=	MyWndProc;
    wc.cbClsExtra		=	0;
    wc.cbWndExtra		=	0;
    wc.hInstance		=	hInstance;
    wc.hIcon			=	NULL;
    wc.hCursor			=	LoadCursor(NULL, IDC_CROSS);
    wc.hbrBackground	=	(HBRUSH)::GetStockObject(WHITE_BRUSH);//(HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName		=	NULL;
    wc.lpszClassName	=	mClassName.c_str();

    if (!::RegisterClass(&wc)) return false;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Un-registers the WNDCLASS with Windows
*/
////////////////////////////////////////////////////////////////////////////////
void Window::UnregisterWndClass()
{
    ::UnregisterClass(mClassName.c_str(), mWindowInstance);
} 

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Creates the Window
	@param	hInstance
			Windows window instance
	@param	windowName
			The name of the window
	@param	width
			Width of the Window
	@param	height
			Height of the Window
	@return	true if the Window creation was successful, false if not
	@par	This must be called to complete initialization of the Window object
*/
////////////////////////////////////////////////////////////////////////////////
bool Window::Create(HINSTANCE hInstance, const std::string& windowName, const unsigned int width, const unsigned int height)
{
    if(!RegisterWndClass(hInstance))
        return false;

    HMENU hMenu = ::LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

    mHwnd = ::CreateWindow(	mClassName.c_str(),		// Window class name
							windowName.c_str(),		// Window text (title)
							WS_OVERLAPPEDWINDOW,	// Window style
							CW_USEDEFAULT,			// Let Windows decide
							CW_USEDEFAULT,			// Let Windows decide
							width,					// Window size x
							height,					// Window size y
							NULL,					// No parent window
							hMenu,					// No menu
							hInstance,				// App's HINSTANCE
							NULL);					// No data to WM_CREATE

    if(NULL == mHwnd) return false;

    mHdc = ::GetDC(mHwnd);
    ::ShowWindow(mHwnd, SW_SHOWNORMAL);

    mRunning = true;
    mWidth = width;
    mHeight = height;
    mWindowInstance = hInstance;

    // Create the secondary buffer
    CreateSecondaryBuffer(width, height);
    memset(mSurface, 0xFF, sizeof(int) * Width() * Height());   // Set the back surface to white

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Creates a secondary rendering buffer
	@param	width
			Width of the secondary buffer
	@param	height
			Height of the secondary buffer
*/
////////////////////////////////////////////////////////////////////////////////
void Window::CreateSecondaryBuffer(const unsigned int width, const unsigned int height)
{
   BITMAPINFOHEADER bmpinfo;

	bmpinfo.biBitCount		=	32;
	bmpinfo.biClrImportant	=	0;
	bmpinfo.biClrUsed		=	0;
	bmpinfo.biCompression	=	BI_RGB;
	bmpinfo.biHeight		=	height;
	bmpinfo.biPlanes		=	1;
	bmpinfo.biSize			=	sizeof(BITMAPINFOHEADER);
	bmpinfo.biSizeImage		=	0;
	bmpinfo.biWidth			=	width;
	bmpinfo.biXPelsPerMeter	=	0;
	bmpinfo.biYPelsPerMeter	=	0;

    HDC tmpDC				=	::CreateDC("Display", 0, 0, 0);
    mBitmapDC				=	::CreateCompatibleDC(tmpDC);

	// if mBitmapDC is null, something very bad hapened
    mBitmap	=	::CreateDIBSection(	mBitmapDC,
									(BITMAPINFO *)&bmpinfo,
									DIB_RGB_COLORS,
									(void **)&mSurface,
									NULL,
									NULL);
	mOldBitmap = (HBITMAP)SelectObject(mBitmapDC, mBitmap);
    ::DeleteDC(tmpDC);
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Swaps the primary and secondary buffer by blitting to the primary surface
	@par	Has a side effect of clearing the screen after blitting
*/
////////////////////////////////////////////////////////////////////////////////
void Window::Present()
{
    BitBlt(	mHdc,
			0,
			0,
			Width(),
			Height(),
			mBitmapDC,
			0,
			0,
			SRCCOPY);
	memset(	mSurface,
			0xFF,
			sizeof(int) * Width() * Height());
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Sets a pixel on the back buffer surface
	@param	x
			X coordinate of the pixel
	@param	y
			Y coordinate of the pixel
	@param	color
			Color of the pixel
*/
////////////////////////////////////////////////////////////////////////////////
void Window::SetPixel(const unsigned int x, const unsigned int y, const Color& color) const
{
    if( (x < Width() && x > 0) && (y < Height() && y > 0) )
	{
		unsigned int fnlcolor	=	0;

		fnlcolor	=	color.GetR() << 16;
		fnlcolor	=	(color.GetG() << 8) | fnlcolor;
		fnlcolor	=	color.GetB() | fnlcolor;

		*(mSurface + ((Height() - y) * Width()) + x) =	fnlcolor; 
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Processes messages from Windows
	@par	Sets the internal running state to false if a close message is 
			in the queue.
*/
////////////////////////////////////////////////////////////////////////////////
void Window::ProcessMessages()
{
	MSG msg;
	if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		// Signal that the window should be terminated on a quit message
		if(WM_QUIT == msg.message)	mRunning = false;
			
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}


}	// namespace CS200
