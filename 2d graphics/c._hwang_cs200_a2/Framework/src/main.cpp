/******************************************************************************/
/*!
\file   main.cpp
\par    Purpose: Main entry of the program
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\par    Course: CS200
\date   5/6/2015
*/
/******************************************************************************/

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>        /*  For WinAPI applications */
#include <mmsystem.h>       /*  timeGetTime func */
#include <iostream>

#pragma comment (lib, "winmm.lib")  /*  timeGetTime func */

#include "resource.h"
#include "ui.hpp"
#include "graphics.hpp"


const float    TICKS_PER_SEC = 1000.f;
const float    FPS_30 = TICKS_PER_SEC / 30.f;

/******************************************************************************/
/*  Function prototypes */
void UpdateEllipses(DWORD& oldTime);
void DrawEllipses(Graphics &gl);
void UpdateViewport(Graphics &gl);


/******************************************************************************/
/*!
\fn     int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                            LPSTR lpCmdLine, int nShowCmd)
\brief
        Main function of the program. It:
        - Allocates a console for debugging purpose.
        - Creates the window and device context/rendering context.
        - Intializes OpenGL function pointers.
        - Set up all graphics-related stuffs.
        - Program loop:
            - Handles interactions with the user (mouse, menu bar, ...).
            - Update viewport and list of lines if needed.
            - Draw the lines to offscreen buffer.
            - Render buffer to the screen.
        - Destructors will clean up graphics-related stuffs and kill the device context/
          rendering context/window upon program termination.

\param  hInstance
        Handler for the current instance of the program.
\param  hPrevInstance
        Handler for the previous instance of the program.
\param  lpCmdLine
        Program command line.
\param  nShowCmd
        Control how the window is to be shown (HIDDEN/MINIMIZED/MAXIMIZED/...)
\return 
        Returns 0 if program runs successfully, non-0 otherwise.
*/
/******************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    // By default WinAPI program turns off the console. But we turn it on again for debugging purpose.
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    // Initialize modal dialog message ...
    gModalMsg = "CS200 Ellipse Drawing Framework";
    gModalMsg += "\nCopyright 2005-2015 DigiPen Institute of Technology\n\n";

    gModalMsg += "\nTo interactively define a new ellipse, do the following:\n";
    gModalMsg += "  1. Click left mouse button at two different viewport locations.\n";
    gModalMsg += "  2. These two viewport points define the opposite corner of a box.\n";
    gModalMsg += "  3. Ellipse center, major radius, minor radius are specified as center,\n";
    gModalMsg += "     half-width and half-height of box, respectively.\n";
    gModalMsg += "\nUse 'C' or 'c' to clear previously drawn primitives.\n";

    gModalMsg += "\nAs always, bug reports are appreciated and must be directed to sang.le@digipen.edu\n";

    hInstance = GetModuleHandle(NULL);

    Window window;
    oldWidth = width = 800;
    oldHeight = height = 600;
    oldNumEllipses = 0;
    
    if (!window.CreateWindowContext(hInstance, "CS200 Assignment Ellipse Drawing", width, height))
        return 1;
    MessageBox(window.GetWindowHandle(), gModalMsg.c_str(), "Information", MB_OK);

    Graphics gl(width, height);

    gl.InitGLExtensions();
    gl.SetUp();

    DWORD oldTime = timeGetTime();

    while (window.Running())
    {
        window.ProcessMessages();               // Handle window messages

        UpdateViewport(gl);                     // Update viewport/offscreen buffer size if needed

        UpdateEllipses(oldTime);                   // Update the list of lines to be drawn

        HCURSOR hCurrCursor = GetCursor();
        SetCursor(LoadCursor(NULL, IDC_WAIT));
        DrawEllipses(gl);                          // Draw the lines to the offscreen buffer in gl
        SetCursor(hCurrCursor);

        gl.Render();                            // Render the buffer to the screen
        SwapBuffers(window.GetDeviceContext()); // Swap front-back buffers so that we can 
                                                // draw into one buffer while displaying the other
    }

    return 0;
}



/******************************************************************************/
/*!
\fn     void UpdateEllipses(DWORD& oldTime)
\brief  Top-level function to update the list of objects in the scene.
\param  oldTime
        Time from the previous update, used to keep the frame rate not too high.
*/
/******************************************************************************/
void UpdateEllipses(DWORD& oldTime)
{
    // hold to 30fps
    DWORD newTime;

    // No position update for ellipse, actually ...

    // Just keep frame rate not higher than 30fps
    while (1)
    {
        // Get the updated time
        newTime = ::timeGetTime();
        if (newTime - oldTime <= FPS_30)    continue; // 1/30 second
        oldTime = newTime;    // Re-set the time
        break;
    }
}


/******************************************************************************/
/*!
\fn     void DrawEllipse(Graphics &gl, int ID)
\brief  Top-level function to draw the object, given its ID, 
        to the offscreen buffer.
\param  gl
        Graphics instance for access to the buffer.
\param  ID
        ID of the object we want to draw.
*/
/******************************************************************************/
void DrawEllipse(Graphics &gl, int ID)
{
    // Draw outline of 2D box containing ellipse
    int centerX = gEllipseList[ID].GetCenterX();
    int centerY = gEllipseList[ID].GetCenterY();
    int a = gEllipseList[ID].GetMajorRadius();
    int b = gEllipseList[ID].GetMinorRadius();

    int xl = centerX - a, xr = centerX + a, yt = centerY - b, yb = centerY + b;

    for (int x = xl; x <= xr; x++)
    {
        gl.SetPix(x, yt, gBlackClr);
        gl.SetPix(x, yb, gBlackClr);
    }

    for (int y = yt; y <= yb; y++)
    {
        gl.SetPix(xl, y, gBlackClr);
        gl.SetPix(xr, y, gBlackClr);
    }

    gEllipseList[ID].DrawMidpoint(gl);
}


/******************************************************************************/
/*!
\fn     void DrawEllipses(Graphics &gl)
\brief  Top-level function to clear the old objects and draw the current objects 
        into the offscreen buffer.
\param  gl
        Graphics instance for access to the buffer.
*/
/******************************************************************************/
void DrawEllipses(Graphics &gl)
{
    int numEllipses = gEllipseList.size();

    if (oldNumEllipses == numEllipses)
        return;

    if (numEllipses == 0)
    {
        gl.ClearFB(gWhiteClr);
        return;
    }

    oldNumEllipses = numEllipses;

    int lastID = numEllipses - 1;

    DrawEllipse(gl, lastID);
}


/******************************************************************************/
/*!
\fn     void UpdateViewport(Graphics &gl)
\brief  Top-level function to update the viewport size if needed.
\param  gl
        Graphics instance for access to the viewport resizing function.
*/
/******************************************************************************/
void UpdateViewport(Graphics &gl)
{
    if (oldWidth != width || oldHeight != height)
    {
        oldWidth = width;
        oldHeight = height;
        gl.Resize(width, height);

        // Redraw the ellipses in the new viewport
        int numEllipses = gEllipseList.size();
        for (int i = 0; i < numEllipses; ++i)
            DrawEllipse(gl, i);
    }
}
