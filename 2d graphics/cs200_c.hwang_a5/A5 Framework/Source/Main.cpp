////////////////////////////////////////////////////////////////////////////////
/*!
@file	Main.cpp
@author	pghali@digipen.edu, sang.le@digipen.edu
@date	July 27, 2014
@par	CS200 Framework
@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Includes
#include <windows.h>
#include <string>
#include "Window.hpp"
#include "UI.hpp"

// Local Function Prototypes
static void	Update(CS200::Window&, DWORD&);
static void	Draw(CS200::Window&);

////////////////////////////////////////////////////////////////////////////////
/*!
@brief	WinMain
@param	hInstance
Window instance
@param	hInstancePrev
Not used
@param	lpCmdLine
Command line input -- not used
@param	nCmdShow
Not used
@return	0 for success, anything else for error
*/
////////////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
  // Create the main window
  CS200::Window window;
  if (!window.Create(hInstance, "CS 200 A5 Framework", 800, 600)) return 1;
  MessageBox(window.GetWindowHandle(), CS200::gModalMsg.c_str(), "CS 200 A5 Help", MB_OK);

  DWORD oldTime = ::timeGetTime();
  while (window.Running())
  {
    Update(window, oldTime);
    Draw(window);
    window.Present(); // Swap the window surfaces
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
/*!
@brief	Top-level function to read user input & update game objects
@param	CS200::Window&
The active Window object
@param	DWORD&
The previous time
*/
////////////////////////////////////////////////////////////////////////////////
static void Update(CS200::Window& window, DWORD& oldTime)
{
  const float	TICKS_PER_SEC = 1000.f;
  const float	FPS_30 = TICKS_PER_SEC / 30.f;

  window.ProcessMessages();

  // update position of picked pillar ...
  if (CS200::gPPickedPillar)
  {
    (CS200::gPPickedPillar)->Move(CS200::gDisplacement);

    for (unsigned int ui = 0; ui < (CS200::gPPillars).size(); ++ui)
    {
      if ((CS200::gPPillars)[ui] == CS200::gPPickedPillar)
        continue;
      if ((CS200::gPPickedPillar)->TestPillar((CS200::gPPillars)[ui]) == true)
      {
        CS200::Point2D tmp(-(CS200::gDisplacement).GetX(), -(CS200::gDisplacement).GetY());
        (CS200::gPPickedPillar)->Move(tmp);
        break;
      }
    }

    (CS200::gDisplacement).SetPosition(0.f, 0.f);
  }

  // hold framerate to 30fps
  DWORD newTime;
  while (1)
  {
    // Get the updated time
    newTime = ::timeGetTime();
    if ((newTime - oldTime) < FPS_30)	// 1/30 second
      continue;

    oldTime = newTime;	// Re-set the time
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////
/*!
@brief	Top-level function to render all game objects
@param	CS200::Window& window
The active Window object
*/
////////////////////////////////////////////////////////////////////////////////
static void Draw(CS200::Window& window)
{
  for (unsigned int ui = 0; ui < (CS200::gPPillars).size(); ++ui)
  {
    (CS200::gPPillars[ui])->Draw(window);
    if (CS200::gDrawBC == true)
      (CS200::gPPillars)[ui]->DrawBC(window);
    if (CS200::gDrawAABB == true)
      (CS200::gPPillars)[ui]->DrawAABB(window);
  }

  if ((CS200::gPSecPillar).size() > 0)
    (CS200::gPSecPillar)[0]->Draw(window);
}
