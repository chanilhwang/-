/******************************************************************************/
/*!
\file   window.cpp
\par    Purpose: Defining the window-related variables & functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\par    Course: CS200
\date   22/5/2015
*/
/******************************************************************************/

// Includes
#include "window.hpp"
#include "ui.hpp"
#include "resource.h"


/******************************************************************************/
/*!
\fn     Window::Window()
\brief
        Window default constructor.
*/
/******************************************************************************/
Window::Window() :  mWidth(0), mHeight(0), mRunning(false), mHwnd(0),
                    mHdc(0), mHinstance(0), mWndName("CS200 Assignment Ellipse Drawing")
{}


/******************************************************************************/
/*!
\fn     Window::~Window()
\brief
        Window destructor.
*/
/******************************************************************************/
Window::~Window()
{
    // Get rid of the main window
    UnregisterWndClass();
    ::ReleaseDC(mHwnd, mHdc);

    mRunning = false;

    DestroyMenu(mHmenu);

    // if we have an RC, release it
    if (mHrc)
    {
        // release the RC
        if (!wglMakeCurrent(NULL, NULL))
            MessageBox(NULL, "Unable to release rendering context", "Error", MB_OK | MB_ICONINFORMATION);

        // delete the RC
        if (!wglDeleteContext(mHrc))
            MessageBox(NULL, "Unable to delete rendering context", "Error", MB_OK | MB_ICONINFORMATION);

        mHrc = NULL;
    }

    // release the DC if we have one
    if (mHdc && !ReleaseDC(mHwnd, mHdc))
    {
        MessageBox(NULL, "Unable to release device context", "Error", MB_OK | MB_ICONINFORMATION);
        mHdc = NULL;
    }

    // destroy the window if we have a valid handle
    if (mHwnd && !DestroyWindow(mHwnd))
    {
        MessageBox(NULL, "Unable to destroy window", "Error", MB_OK | MB_ICONINFORMATION);
        mHwnd = NULL;
    }

    // unregister our class so we can create a new one if we need to
    if (!UnregisterClass(mWndName.c_str(), mHinstance))
    {
        MessageBox(NULL, "Unable to unregister window class", "Error", MB_OK | MB_ICONINFORMATION);
        mHinstance = NULL;
    }

}


/******************************************************************************/
/*!
\fn     unsigned int Window::Width() const
\brief
        Returns the width of the Window.
\return
        Width of the Window.
*/
/******************************************************************************/
unsigned int Window::Width() const
{
    return mWidth;
}


/******************************************************************************/
/*!
\fn     unsigned int Window::Height() const
\brief
        Returns the height of the Window.
\return
        Height of the Window.
*/
/******************************************************************************/
unsigned int Window::Height() const
{
    return mHeight;
}


/******************************************************************************/
/*!
\fn     bool Window::RegisterWndClass(HINSTANCE hInstance)
\brief
        Registers the WNDCLASS with Windows.
\param  hInstance
        Window instance.
\return
        true for success, false for error.
*/
/******************************************************************************/
bool Window::RegisterWndClass(HINSTANCE hInstance)
{
    WNDCLASSEX  wc;    // window class

    // fill out the window class structure
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // default icon
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);      // windows logo small icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);      // default arrow
    wc.hbrBackground = NULL;     // no background needed
    wc.lpszMenuName = NULL;     // no menu
    wc.lpszClassName = mWndName.c_str();

    // register the windows class
    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Unable to register the window class", "Error", MB_OK | MB_ICONEXCLAMATION);
        // exit and return false
        return false;
    }

    return true;
}


/******************************************************************************/
/*!
\fn     void Window::UnregisterWndClass()
\brief
        Unregisters the WNDCLASS with Windows.
*/
/******************************************************************************/
void Window::UnregisterWndClass()
{
    UnregisterClass(mWndName.c_str(), mHinstance);
}


/******************************************************************************/
/*!
\fn     bool Window::CreateWindowContext(   HINSTANCE hInstance, 
                                            const std::string& windowName, 
                                            const unsigned int width, 
                                            const unsigned int height)
\brief
        Creates the Window, as well as the device context and rendering context.
        This must be called to complete initialization of the Window object.
\param  hInstance
        Window instance.
\param  windowName
        Window's name.
\param  width
        Window's width.
\param  height
        Window's height.
\return
        true if the Window creation was successful, false if not.
*/
/******************************************************************************/
bool Window::CreateWindowContext(   HINSTANCE hInstance, const std::string& windowName, 
                                    const unsigned int width, const unsigned int height)
{
    if (!RegisterWndClass(hInstance))
        return false;

    mWidth = width;
    mHeight = height;
    mHinstance = hInstance;
    mRunning = true;

    // set up the window we're rendering to so that the top left corner is at (0,0)
    // and the bottom right corner is (height,width)
    RECT  windowRect;
    windowRect.left = 0;
    windowRect.right = (LONG)width;
    windowRect.top = 0;
    windowRect.bottom = (LONG)height;

    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    // change the size of the rect to account for borders, etc. set by the style
    AdjustWindowRectEx(&windowRect, dwStyle, true, dwExStyle);

    mHmenu = ::LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU));

    // class registered, so now create our window
    mHwnd = CreateWindowEx( dwExStyle,          // extended style
                            mWndName.c_str(),   // class name
                            windowName.c_str(), // app name
                            dwStyle |           // window style
                            WS_CLIPCHILDREN |   // required for
                            WS_CLIPSIBLINGS,    // using OpenGL
                            0, 0,               // x,y coordinate
                            windowRect.right - windowRect.left, // width
                            windowRect.bottom - windowRect.top, // height
                            NULL,               // handle to parent
                            mHmenu,             // handle to menu, use NULL if not needed
                            hInstance,          // application instance
                            NULL);              // no extra params


    // see if our window handle is valid
    if (!mHwnd)
    {
        MessageBox(NULL, "Unable to create window", "Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if (!CreateDeviceRenderContext(mHdc, mHrc))
        return false;

    // show the window in the forground, and set the keyboard focus to it
    ShowWindow(mHwnd, SW_SHOW);
    SetForegroundWindow(mHwnd);
    SetFocus(mHwnd);

    return true;
}


/******************************************************************************/
/*!
\fn     bool Window::CreateDeviceRenderContext(HDC &mHdc, HGLRC &mHrc)
\brief
        Creates the device context and rendering context. Then any graphics API
        can be used in this rendering context.
\param  mHdc
        Handler to the device context.
\param  mHrc
        Handler to the rendering context.
\return
        true if the device/rendering context creation was successful, false if not.
*/
/******************************************************************************/
bool Window::CreateDeviceRenderContext(HDC &mHdc, HGLRC &mHrc)
{
    // get a device context
    if (!(mHdc = GetDC(mHwnd)))
    {
        MessageBox(NULL, "Unable to create device context", "Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    // set the pixel format we want
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of structure
        1,                              // default version
        PFD_DRAW_TO_WINDOW |            // window drawing support
        PFD_SUPPORT_OPENGL |            // OpenGL support
        PFD_DOUBLEBUFFER,               // double buffering support
        PFD_TYPE_RGBA,                  // RGBA color mode
        32,                             // 32 bit color mode
        0, 0, 0, 0, 0, 0,               // ignore color bits, non-palettized mode
        0,                              // no alpha buffer
        0,                              // ignore shift bit
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // ignore accumulation bits
        16,                             // 16 bit z-buffer size
        8,                              // no stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main drawing plane
        0,                              // reserved
        0, 0, 0 };                      // layer masks ignored

    GLuint  pixelFormat;

    // choose best matching pixel format
    if (!(pixelFormat = ChoosePixelFormat(mHdc, &pfd)))
    {
        MessageBox(NULL, "Can't find an appropriate pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    // set pixel format to device context
    if (!SetPixelFormat(mHdc, pixelFormat, &pfd))
    {
        MessageBox(NULL, "Unable to set pixel format", "Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }


    // create the OpenGL rendering context
    if (!(mHrc = wglCreateContext(mHdc)))
    {
        MessageBox(NULL, "Unable to create OpenGL rendering context", "Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    // now make the rendering context the active one
    if (!wglMakeCurrent(mHdc, mHrc))
    {
        MessageBox(NULL, "Unable to activate OpenGL rendering context", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    return true;
}


/******************************************************************************/
/*!
\fn     void Window::ProcessMessages()
\brief
        Processes messages for window.
        Sets the internal running state to false if a close message is
        in the queue.
*/
/******************************************************************************/
void Window::ProcessMessages()
{
    MSG msg;

    if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)   // do we receive a WM_QUIT message?
            mRunning = false;         // if so, time to quit the application
        else
        {
            TranslateMessage(&msg);     // translate and dispatch to event queue
            DispatchMessage(&msg);
        }
    }

}


