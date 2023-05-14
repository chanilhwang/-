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
\date   21/5/2015
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


/******************************************************************************/
/*  Function prototypes */
void UpdateLines(DWORD& oldTime);
void DrawLines(Graphics &gl);
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
    gModalMsg = "CS200 LineDrawing Framework";
    gModalMsg += "\nCopyright 2005-2015 DigiPen Institute of Technology\n\n";

    gModalMsg += "To interactively define a new line segment, do the following:\n";
    gModalMsg += "   1. Click left mouse button at any viewport location.\n";
    gModalMsg += "   2. Holding left button down, drag mouse to new location and release left button.\n";
    gModalMsg += "\n";

    gModalMsg += "Use LineAlgorithm menu to toggle between Symmetric DDA and Midpoint algorithms.\n";

    gModalMsg += "\nUse left mouse button to select/pick previously modeled lines.\n";
    gModalMsg += "Picked lines can be interactively edited by moving mouse to edit boxes at line endpoints\n";
    gModalMsg += "Click left mouse button in the edit box of an endpoint and drag mouse to specify new endpoint location.\n";

    gModalMsg += "\nOther controls:\n";
    gModalMsg += "Use 'c' key to delete picked line.\n";
    gModalMsg += "Use 'C' key to delete entire scene.\n";
    gModalMsg += "Use 's' or 'S' key to save scene to file \"CS200LineDrawing.scn\"\n";
    gModalMsg += "Note that the contents of the saved file are read by the application at startup.\n";

    gModalMsg += "\nAs always, bug reports are appreciated and must be directed to sang.le@digipen.edu\n";

    hInstance = GetModuleHandle(NULL);

    Window window;
    oldWidth = width = 800;
    oldHeight = height = 600;
    
    if (!window.CreateWindowContext(hInstance, "CS200 Assignment Line Drawing", width, height))
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

        UpdateLines(oldTime);                   // Update the list of lines to be drawn
        DrawLines(gl);                          // Draw the lines to the offscreen buffer in gl

        gl.Render();                            // Render the buffer to the screen
        SwapBuffers(window.GetDeviceContext()); // Swap front-back buffers so that we can 
                                                // draw into one buffer while displaying the other
    }

    return 0;
}



/******************************************************************************/
/*!
\fn     void UpdateLines(DWORD& oldTime)
\brief  Top-level function to update the list of lines in the scene.
\param  oldTime
        Time from the previous update, used to keep the frame rate not too high.
*/
/******************************************************************************/
void UpdateLines(DWORD& oldTime)
{
    const float    TICKS_PER_SEC    =    1000.f;
    const float    FPS_30            =    TICKS_PER_SEC/30.f;

    // update position of picked line ...
    if (gPPickedLine)
    {
        gPPickedLine->Move(gDisplacement);
        gPPickedLineBox->Move(gDisplacement);
        gDisplacement.SetPosition(0.f, 0.f);
    }

    // hold to 30fps
    DWORD newTime;
    while (1)
    {
        // Get the updated time
        newTime = ::timeGetTime();
        if(newTime - oldTime <= FPS_30)    continue; // 1/30 second
        oldTime = newTime;    // Re-set the time
        break;
    }
}


/******************************************************************************/
/*!
\fn     void DrawLines(Graphics &gl)
\brief  Top-level function to clear the old lines and draw the current lines 
        into the offscreen buffer.
\param  gl
        Graphics instance for access to the buffer.
*/
/******************************************************************************/
void DrawLines(Graphics &gl)
{
    gl.ClearFB(gWhiteClr);

    if (ActiveLineAlgoType(LINE_ALGO_DDA) == true)
    {
        for (int i = 0; i < static_cast<int>(gLineList.size()); i++)
        {
            gLineList[i].GetStartVertex().SetColor(gRedClr);
            gLineList[i].GetEndVertex().SetColor(gRedClr);
            gLineList[i].DrawDDA(gl);
        }
    }
    else
    {
        for (int i = 0; i < static_cast<int>(gLineList.size()); i++)
        {
            gLineList[i].GetStartVertex().SetColor(gBlueClr);
            gLineList[i].GetEndVertex().SetColor(gBlueClr);
            gLineList[i].DrawMidpoint(gl);
        }
    }

    if (gSecLineList.size())
    {
        gSecLineList[0].GetStartVertex().SetColor(gBlackClr);
        gSecLineList[0].GetEndVertex().SetColor(gBlackClr);
        gSecLineList[0].DrawDDA(gl);
    }

    if (gPPickedLineBox)
    {
        extern bool gEditLine;
        extern bool gVertexEditFlag;

        if (gEditLine)
        {
            if (gVertexEditFlag == true)
                gPPickedLineBox->DrawEnd(gl, gBlackClr);
            else
                gPPickedLineBox->DrawStart(gl, gBlackClr);
        }
        else
        {
            gPPickedLineBox->Draw(gl, gBlackClr);
        }
    }
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
    }
}
