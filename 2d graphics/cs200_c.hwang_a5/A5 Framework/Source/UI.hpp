////////////////////////////////////////////////////////////////////////////////
/*!
@file	UI.hpp
@author	pghali@digipen.edu, sang.le@digipen.edu
@date	July 27, 2014
@par	CS200 Framework
@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef UI_HPP_
#define UI_HPP_

#include "Pillar.hpp"
#include "..\resource.h"

namespace CS200
{

  enum ModeType
  {
    MODE_PICK = ID_MODE_PICK,
    MODE_CREATE = ID_MODE_CREATE
  };

  enum PillarsType
  {
    PILLAR_CIRCLE = ID_SHAPES_CIRCLEPILLAR,
    PILLAR_TRI = ID_SHAPES_TRIPILLAR,
    PILLAR_RECT = ID_SHAPES_RECTPILLAR,
    PILLAR_CONVEXPOLY = ID_SHAPES_CONVEXPOLYPILLAR
  };

  enum OptionsType
  {
    OPTION_NONE = 0,
    OPTION_BC = ID_OPTIONS_DRAWBC,
    OPTION_AABB = ID_OPTIONS_DRAWAABB
  };

  extern LRESULT CALLBACK	MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

  extern bool ActiveMode(const enum ModeType mode);
  extern bool ActivePillar(const enum PillarsType shape);
  extern bool ActiveOption(const enum OptionsType option);

  extern enum ModeType	gActiveMode;
  extern enum PillarsType	gActivePillar;
  extern enum OptionsType	gActiveOption;

  // flags to toggle drawing of BC and AABB
  extern bool					gDrawBC, gDrawAABB;
  extern std::vector<Pillar*>	gPPillars;
  extern std::vector<Pillar*>	gPSecPillar;
  extern Pillar*				gPPickedPillar;
  extern Vector2D				gDisplacement; // relative mouse movement

  extern std::string			gModalMsg;

}		// namespace CS200

#endif	// UI_HPP_
