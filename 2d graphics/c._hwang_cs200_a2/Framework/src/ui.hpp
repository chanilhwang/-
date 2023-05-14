/******************************************************************************/
/*!
\file   ui.hpp
\par    Purpose: User interface class header.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   5/6/2015
*/
/******************************************************************************/

// Guard
#ifndef UI_HPP_
#define UI_HPP_

// Includes
#include "window.hpp"
#include "ellipse2d.hpp"
#include "resource.h"

// Global variables
extern Color                    gRedClr, gGreenClr, gBlueClr, gBlackClr, gWhiteClr;
extern std::vector<Ellipse2D>   gEllipseList;
extern std::string              gModalMsg;

extern int                      width, height;
extern int                      oldWidth, oldHeight;

extern int                      oldNumEllipses;

LRESULT CALLBACK    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif

