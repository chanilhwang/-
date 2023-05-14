/******************************************************************************/
/*!
\file   ui.hpp
\par    Purpose: User interface class header.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   22/5/2015
*/
/******************************************************************************/

// Guard
#ifndef UI_HPP_
#define UI_HPP_

// Includes
#include "Window.hpp"
#include "Point2D.hpp"
#include "Line2D.hpp"
#include "Box2D.hpp"
#include "resource.h"

extern std::string          gModalMsg;
extern std::vector<Line2D>  gLineList, gSecLineList;
extern Vector2D             gDisplacement;
extern Color                gRedClr, gBlueClr, gBlackClr, gWhiteClr;
extern Line2D*              gPPickedLine;
extern LineBox2D*           gPPickedLineBox;

extern int                  width, height;
extern int                  oldWidth, oldHeight;

enum LineAlgoType
{
    LINE_ALGO_DDA       =   IDM_DDA,
    LINE_ALGO_MIDPOINT  =   IDM_MIDPOINT
};

extern              LineAlgoType    gActiveLineType;

bool                ActiveLineAlgoType(LineAlgoType);
LRESULT CALLBACK    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif

