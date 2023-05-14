/******************************************************************************/
/*!
\file   ui.cpp
\par    Purpose: User interface class implementation.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   5/6/2015
*/
/******************************************************************************/

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "ui.hpp"
#include "window.hpp"

// Global variables
std::string             gModalMsg;

int                     width, height;
int                     oldWidth, oldHeight;

Color                   gRedClr(255, 0, 0, 255);
Color                   gGreenClr(0, 255, 0, 255);
Color                   gBlueClr(0, 0, 255, 255);
Color                   gBlackClr(0, 0, 0, 255);
Color                   gWhiteClr(255, 255, 255, 255);

std::vector<Ellipse2D>  gEllipseList;
int                     oldNumEllipses;


void AddEllipse(std::vector<Ellipse2D>& list, LPARAM clickVtx[2], Color& c);


/******************************************************************************/
/*!
\fn     LRESULT CALLBACK WndProc(   HWND hwnd, UINT message, 
                                    WPARAM wParam, LPARAM lParam)
\brief
        Windows message handler.
\param  hwnd
        Handler to the window.
\param  message
        The message based on user input (mouse/keyboard/menu...)
\param  wParam
        Additional message parameters. See examples below.
\param  lParam
        Additional message parameters. See examples below.
*/
/******************************************************************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static bool     sEllipseFirstTime = true;
    static LPARAM   sEllipseClickPt[2];

    static Color    sEllipseClr[3] = { gRedClr, gGreenClr, gBlueClr };
    static int      sEllipseClrIdx = 0;

    switch (msg)    // Check  message type
    {
        case WM_SIZE:       // Window resizing
        {
            width = LOWORD(lParam);
            height = HIWORD(lParam);
            return 0;
        }

        case WM_GETMINMAXINFO:  // Use this to set the minimal window height allowed
        {
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            mmi->ptMinTrackSize.y = 80;
            return 0;
        }

        case WM_LBUTTONDOWN: // left mouse button clicks clicked
        {
            // If it's the first pass, set first box corner
            if (sEllipseFirstTime == true)
            {
                sEllipseClickPt[0] = lParam;
                sEllipseFirstTime = false;
            }
            else
            {
                sEllipseClickPt[1] = lParam;
                AddEllipse(gEllipseList, sEllipseClickPt, sEllipseClr[sEllipseClrIdx]);
                sEllipseClrIdx = (sEllipseClrIdx + 1) % 3;
                sEllipseFirstTime = true;
            }
            return 0;
        }

        case WM_CHAR:   // some key is pressed
        {
            switch (wParam)
            {
                case 'C': case 'c':     // Press 'c' or 'C' to clear the ellipses
                {
                    std::cout << "C pressed\n";
                    gEllipseList.clear();
                    sEllipseFirstTime = true;
                    break;
                }
            }
            return 0;
        }

        case WM_COMMAND:    // Handle messages from the Menu bar
        {
            // Get a handle to the menu
            HMENU hMenu = ::GetMenu(hwnd);
            switch(LOWORD(wParam))
            {
                case IDM_ABOUT:     // Display help message
                    MessageBox(hwnd, gModalMsg.c_str(), "CS200 A1 Framework", MB_OK);
                    break;
            }
            return 0;
        }

        case WM_SYSCOMMAND:  // look for screensavers and powersave mode
        {
            switch (wParam)
            {
                case SC_SCREENSAVE:     // screensaver trying to start
                case SC_MONITORPOWER:   // monitor going to powersave mode
                    // returning 0 prevents either from happening
                    return 0;
                default:
                    break;
            }
        } break;

        case WM_DESTROY:    // Destroy message
            return 0;

        case WM_CLOSE:      // Close message
            gEllipseList.clear();
            PostQuitMessage(0);
            return 0;

        default:            // Default handling
            break;
    }

    // Dispatch any unhandled messages
    return DefWindowProc(hwnd, msg, wParam, lParam);
}


/******************************************************************************/
/*!
\fn     void AddEllipse(std::vector<Ellipse2D>& list, LPARAM boxVtx[2], Color& c)
\brief
        Adds an ellipse to the list.
\param  list
        The list to which ellipse is added.
\param  start
        Box vertex.
\param  end
        Opposite box vertex.
\param  c
        Fill color of ellipse.
*/
/******************************************************************************/
void AddEllipse(std::vector<Ellipse2D>& list, LPARAM boxVtx[2], Color& c)
{
    unsigned int x0    =    LOWORD(boxVtx[0]);
    unsigned int y0    =    HIWORD(boxVtx[0]);
    unsigned int x1    =    LOWORD(boxVtx[1]);
    unsigned int y1    =    HIWORD(boxVtx[1]);

    if ((x0+x1) % 2)
    {
        if (x0 < x1) x1--; else x0--;
    }
    if ((y0+y1) % 2)
    {
        if (y0 < y1) y1--; else y0--;
    }

    int cX = (x0+x1)/2, cY = (y0+y1)/2;
    int a = x1-cX, b = y1-cY;
    a = (a < 0) ? -a : a;
    b = (b < 0) ? -b : b;

    list.push_back(Ellipse2D(cX, cY, a, b, c));
}