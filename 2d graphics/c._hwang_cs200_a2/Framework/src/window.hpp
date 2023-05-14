/******************************************************************************/
/*!
\file   window.hpp
\par    Purpose: Declaring the window-related variables & functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\par    Course: CS200
\date   22/5/2015
*/
/******************************************************************************/

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


/****************************************************************************/
/*!
\class  Window

\brief  Launches a window in the Win32 environment.
*/
/****************************************************************************/
class Window
{
public:
    Window();
    ~Window();

    bool            CreateWindowContext(HINSTANCE hInstance,
                                        const std::string& windowName,
                                        const unsigned int width,
                                        const unsigned int height);
    unsigned int    Width() const;
    unsigned int    Height() const;
    void            ProcessMessages();
    void            Present();
    const HWND      GetWindowHandle() const { return mHwnd; }
    const HDC       GetDeviceContext() const { return mHdc; }
    bool            Running() const { return mRunning; }

private:

    // Prevent copying
    Window(const Window& rhs);
    Window& operator=(const Window& rhs);

    // Internal use
    bool            RegisterWndClass(HINSTANCE hInstance);
    bool            CreateDeviceRenderContext(HDC &mHdc, HGLRC &mHrc);
    void            UnregisterWndClass();

    unsigned int    mWidth, mHeight;

    // Main window
    HWND            mHwnd;          // The window handle
    HDC             mHdc;           // The device context of the window
    HGLRC           mHrc;           // The rendering context
    HINSTANCE       mHinstance;     // The instance ID of the application
    HMENU           mHmenu;
    bool            mRunning;
    bool            mMinimized;
    std::string     mWndName;

};  // class Window


#endif  // WINDOW_HPP_