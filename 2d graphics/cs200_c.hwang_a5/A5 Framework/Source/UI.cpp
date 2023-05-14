////////////////////////////////////////////////////////////////////////////////
/*!
@file	UI.cpp
@author	pghali@digipen.edu, sang.le@digipen.edu
@date	July 27, 2014
@par	CS200 Framework
@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

#include "Window.hpp"
#include "UI.hpp"
#include "Point2D.hpp"
#include "CircularPillar.hpp"
#include "PolygonalPillar.hpp"
#include "..\resource.h"
#include <cassert>
#include <string>
#include <ctime>
#include <iostream>
#include <fstream>

namespace CS200 {
  // Objects with file scope
  static enum ModeType			gActiveMode = MODE_PICK;
  static enum PillarsType			gActivePillar = PILLAR_CIRCLE;
  static enum OptionsType			gActiveOption = OPTION_NONE;

  // Function declarations with file scope
  static bool	IsMouseInPillar(LPARAM lParam);
  static bool IsMouseInPillar(const Point2D&, unsigned int&);
  static void InitializeModalMsg(std::string&);
  static inline float ABSF(float f) { return (f > 0.f) ? f : -f; }

  // Objects with project scope
  std::vector<Pillar*>	gPPillars, gPSecPillar;
  Pillar*					gPPickedPillar;
  Vector2D				gDisplacement;
  bool					gDrawBC = true;
  bool					gDrawAABB = true;
  std::string				gModalMsg;

  // Function declarations with project scope
  bool	ActiveMode(const enum ModeType);
  bool	ActivePillar(const enum PillarsType);
  bool	ActiveOption(const enum OptionsType);

  bool	InsertCircularPillar(std::vector<Pillar *>&, const std::vector<LPARAM>&, bool);
  bool	InsertTriangularPillar(std::vector<Pillar *>&, const std::vector<LPARAM>&, bool);
  bool	InsertRectangularPillar(std::vector<Pillar *>&, const std::vector<LPARAM>&, bool);
  bool	InsertPolygonalPillar(std::vector<Pillar *>&, const std::vector<LPARAM>&, bool);
  void	DeletePillars(std::vector<Pillar*>&);

  bool	WriteSceneToFile(const char*, std::vector<Pillar*>&);
  bool	ReadSceneFromFile(const char*, std::vector<Pillar*>&);

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Window Procedure
  @param	hwnd
  Window handle
  @param	msg
  Incoming message
  @param	wParam
  Message data
  @param	lParam
  Message data
  @return	Result of the callback
  */
  ////////////////////////////////////////////////////////////////////////////////
  LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT					ps;
    static HINSTANCE			hInstance;
    static std::vector<LPARAM>	sClickPts;
    static Point2D				sPickPts[2];

    switch (msg) { // Check  message type
    case WM_CREATE:
    {
      // Initialize pseudo-random number generator
      srand(static_cast<unsigned int>(time(NULL)));

      InitializeModalMsg(gModalMsg);

      // read saved scene from file
      std::string file_name("A5 Scene.txt");
      bool file_read_flag = ReadSceneFromFile(file_name.c_str(), gPPillars);
      if (file_read_flag == false) {
        std::string error_msg("Error reading scene data from file: ");
        error_msg += file_name; error_msg += "\n";
        MessageBox(hwnd, error_msg.c_str(), "CS200 A5 Framework", MB_OK);
      }

      CheckMenuItem(GetMenu(hwnd), ID_MODE_PICK, MF_CHECKED);
      CheckMenuItem(GetMenu(hwnd), ID_MODE_CREATE, MF_UNCHECKED);
      EnableMenuItem(GetMenu(hwnd), ID_SHAPES_CIRCLEPILLAR, MF_GRAYED);
      EnableMenuItem(GetMenu(hwnd), ID_SHAPES_TRIPILLAR, MF_GRAYED);
      EnableMenuItem(GetMenu(hwnd), ID_SHAPES_RECTPILLAR, MF_GRAYED);
      EnableMenuItem(GetMenu(hwnd), ID_SHAPES_TRIPILLAR, MF_GRAYED);
      EnableMenuItem(GetMenu(hwnd), ID_SHAPES_CONVEXPOLYPILLAR, MF_GRAYED);

      // Enable options only when there is atleast one pillar
      if (gPPillars.size())
      {
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_ENABLED);
        DrawMenuBar(hwnd);
      }
      else
      {
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_GRAYED);
      }

      // check if option enabled
      UINT option = (gDrawBC == true) ? MF_CHECKED : MF_UNCHECKED;
      CheckMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, option);
      option = (gDrawAABB == true) ? MF_CHECKED : MF_UNCHECKED;
      CheckMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, option);

      CheckMenuItem(GetMenu(hwnd), gActivePillar, MF_CHECKED);
      DrawMenuBar(hwnd);

      gPPickedPillar = 0;
      sClickPts.clear();

      return 0;
    }
    case WM_SIZING: return 0;
    case WM_LBUTTONUP:
    {
      if (ActiveMode(MODE_PICK) == true)
      {
        gPPickedPillar = 0;
        sPickPts[0].SetPosition(0.f, 0.f);
        sPickPts[1] = sPickPts[0];
        if (IsMouseInPillar(lParam) == true)
        {
          HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
          ::SetCursor(hCursor);
        }
      }

      return 0;
    }
    case WM_LBUTTONDOWN:
    {
      if (ActiveMode(MODE_PICK) == true)
      {
        if (gPPickedPillar) return 0;

        unsigned int	uidx;
        Point2D	pt(static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)));
        if (IsMouseInPillar(pt, uidx) == true)
        {
          gPPickedPillar = gPPillars[uidx];
          sPickPts[0] = pt;
          HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
          ::SetCursor(hCursor);
        }

        return 0;
      }
      else if (ActiveMode(MODE_CREATE) == true)
      {
        if (ActivePillar(PILLAR_CIRCLE))
        {
          sClickPts.push_back(lParam);
          if (2 == sClickPts.size())
          {
            if (!InsertCircularPillar(gPPillars, sClickPts, true))
              MessageBox(
                hwnd,
                "Circular pillar is intersecting with other pillars!!!\n",
                "CS200 A5 Framework",
                MB_OK);
            for (unsigned int ui = 0; ui < gPSecPillar.size(); ++ui)
              delete gPSecPillar[ui];
            gPSecPillar.clear();
            sClickPts.clear();
          }
        }
        else if (ActivePillar(PILLAR_TRI))
        {
          sClickPts.push_back(lParam);
          if (3 == sClickPts.size())
          {
            if (!InsertTriangularPillar(gPPillars, sClickPts, true))
              MessageBox(
                hwnd,
                "Triangular pillar is intersecting with other pillars!!!\n",
                "CS200 A5 Framework",
                MB_OK);
            for (unsigned int ui = 0; ui < gPSecPillar.size(); ++ui)
              delete gPSecPillar[ui];
            gPSecPillar.clear();
            sClickPts.clear();
          }
        }
        else if (ActivePillar(PILLAR_RECT))
        {
          sClickPts.push_back(lParam);
          if (2 == sClickPts.size())
          {
            if (!InsertRectangularPillar(gPPillars, sClickPts, true))
              MessageBox(
                hwnd,
                "Rectangular pillar is intersecting with other pillars!!!\n",
                "CS200 A5 Framework",
                MB_OK);
            for (unsigned int ui = 0; ui < gPSecPillar.size(); ++ui)
              delete gPSecPillar[ui];
            gPSecPillar.clear();
            sClickPts.clear();
          }
        }
        else if (ActivePillar(PILLAR_CONVEXPOLY))
        {
          sClickPts.push_back(lParam);
          if (sClickPts.size() >= 3)
          {
            Vertex2D *pVtx = new Vertex2D[sClickPts.size()];
            for (unsigned int ui = 0; ui < sClickPts.size(); ++ui)
              pVtx[ui].SetPosition(static_cast<float>(LOWORD(sClickPts[ui])), static_cast<float>(HIWORD(sClickPts[ui])));
            PolygonalPillar pc(pVtx, static_cast<int>(sClickPts.size()));
            delete[] pVtx;

            if (pc.IsConvex() == false) sClickPts.pop_back();
          }
        }

        if (gPPillars.size()) // Enable options only when there is atleast one pillar
        {
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_ENABLED);
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_ENABLED);
          DrawMenuBar(hwnd);
        }
      }

      return 0;
    }
    case WM_RBUTTONDOWN:
    {
      if (ActiveMode(MODE_CREATE) == true)
      {
        if (ActivePillar(PILLAR_CONVEXPOLY) && (sClickPts.size() >= 3))
        {
          if (!InsertPolygonalPillar(gPPillars, sClickPts, true))
            MessageBox(
              hwnd,
              "Convex polygonal pillar is intersecting with other pillars!!!\n",
              "CS200 A5 Framework",
              MB_OK);
          for (unsigned int ui = 0; ui < gPSecPillar.size(); ++ui)
            delete gPSecPillar[ui];
          gPSecPillar.clear();
          sClickPts.clear();
        }
      }

      if (gPPillars.size()) // Enable options only when there is atleast one pillar
      {
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_ENABLED);
        DrawMenuBar(hwnd);
      }

      return 0;
    }
    case WM_MOUSEMOVE:
    {
      if (ActiveMode(MODE_PICK) == true)
      {
        if (gPPickedPillar)
        {
          sPickPts[1].SetPosition(static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)));
          gDisplacement = sPickPts[1] - sPickPts[0];
          sPickPts[0] = sPickPts[1];
          HCURSOR hCursor = LoadCursor(NULL, IDC_SIZEALL);
          ::SetCursor(hCursor);
        }
        else
        {
          HCURSOR hCursor = (IsMouseInPillar(lParam) == true) ?
            LoadCursor(NULL, IDC_SIZEALL) : LoadCursor(NULL, IDC_CROSS);
          ::SetCursor(hCursor);
        }
      }
      else if (ActiveMode(MODE_CREATE) == true)
      {
        if (ActivePillar(PILLAR_CIRCLE) && sClickPts.size() == 1)
        {
          sClickPts.push_back(lParam);
          InsertCircularPillar(gPSecPillar, sClickPts, false);
          sClickPts.pop_back();
        }
        else if (ActivePillar(PILLAR_TRI) && sClickPts.size() == 2)
        {
          sClickPts.push_back(lParam);
          InsertTriangularPillar(gPSecPillar, sClickPts, false);
          sClickPts.pop_back();
        }
        else if (ActivePillar(PILLAR_RECT) && sClickPts.size() == 1)
        {
          sClickPts.push_back(lParam);
          InsertRectangularPillar(gPSecPillar, sClickPts, false);
          sClickPts.pop_back();
        }
        else if (ActivePillar(PILLAR_CONVEXPOLY) && (sClickPts.size() >= 2))
        {
          sClickPts.push_back(lParam);
          InsertPolygonalPillar(gPSecPillar, sClickPts, false);
          sClickPts.pop_back();
        }
      }

      return 0;
    }
    case WM_KEYDOWN: {
      switch (wParam) { // The keycode is specified by wParam
      case 0x43: { // Use 'C' key (0x43) to clear lists
        std::string file_name("A5 Scene.txt");
        bool file_write_flag = WriteSceneToFile("A5 Scene.txt", gPPillars);
        if (file_write_flag == false) {
          std::string error_msg("Error writing scene data to file: ");
          error_msg += file_name; error_msg += "\n";
          MessageBox(hwnd, error_msg.c_str(), "CS200 A5 Framework", MB_OK);
        }
        sClickPts.clear();
        DeletePillars(gPPillars);
        DeletePillars(gPSecPillar);
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_GRAYED);
        break;
      }
      }
      return 0;
    }
    case WM_COMMAND:	// Handle messages from Menu bar
    {
      HMENU hMenu = ::GetMenu(hwnd);
      switch (LOWORD(wParam))
      {
      case ID_MODE_PICK:
      {
        if (gActiveMode == LOWORD(wParam)) break;

        enum ModeType	tmpMode = gActiveMode;
        gActiveMode = MODE_PICK;
        CheckMenuItem(hMenu, gActiveMode, MF_CHECKED);
        CheckMenuItem(hMenu, tmpMode, MF_UNCHECKED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_CIRCLEPILLAR, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_TRIPILLAR, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_RECTPILLAR, MF_GRAYED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_CONVEXPOLYPILLAR, MF_GRAYED);

        if (gPPillars.size()) // Enable options only when there is atleast one pillar
        {
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_ENABLED);
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_ENABLED);
        }
        else
        {
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_GRAYED);
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_GRAYED);
        }

        DrawMenuBar(hwnd);
        gPPickedPillar = 0;
        sClickPts.clear();
        break;
      }
      case ID_MODE_CREATE:
      {
        if (gActiveMode == LOWORD(wParam)) break;

        enum ModeType	tmpMode = gActiveMode;
        gActiveMode = MODE_CREATE;
        CheckMenuItem(hMenu, gActiveMode, MF_CHECKED);
        CheckMenuItem(hMenu, tmpMode, MF_UNCHECKED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_CIRCLEPILLAR, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_TRIPILLAR, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_RECTPILLAR, MF_ENABLED);
        EnableMenuItem(GetMenu(hwnd), ID_SHAPES_CONVEXPOLYPILLAR, MF_ENABLED);
        if (gPPillars.size())
        {
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWBC, MF_ENABLED);
          EnableMenuItem(GetMenu(hwnd), ID_OPTIONS_DRAWAABB, MF_ENABLED);
        }
        CheckMenuItem(hMenu, gActivePillar, MF_CHECKED);
        DrawMenuBar(hwnd);
        gPPickedPillar = 0;
        sClickPts.clear();
        break;
      }
      case ID_SHAPES_CIRCLEPILLAR:
      {
        if (gActivePillar == LOWORD(wParam)) break;

        enum PillarsType tmpShape = gActivePillar;
        gActivePillar = PILLAR_CIRCLE;
        CheckMenuItem(hMenu, gActivePillar, MF_CHECKED);
        CheckMenuItem(hMenu, tmpShape, MF_UNCHECKED);
        sClickPts.clear();
        break;
      }
      case ID_SHAPES_TRIPILLAR:
      {
        if (gActivePillar == LOWORD(wParam)) break;

        enum PillarsType tmpShape = gActivePillar;
        gActivePillar = PILLAR_TRI;
        CheckMenuItem(hMenu, gActivePillar, MF_CHECKED);
        CheckMenuItem(hMenu, tmpShape, MF_UNCHECKED);
        sClickPts.clear();
        break;
      }
      case ID_SHAPES_RECTPILLAR:
      {
        if (gActivePillar == LOWORD(wParam)) break;

        enum PillarsType tmpShape = gActivePillar;
        gActivePillar = PILLAR_RECT;
        CheckMenuItem(hMenu, gActivePillar, MF_CHECKED);
        CheckMenuItem(hMenu, tmpShape, MF_UNCHECKED);
        sClickPts.clear();
        break;
      }
      case ID_SHAPES_CONVEXPOLYPILLAR:
      {
        if (gActivePillar == LOWORD(wParam)) break;

        enum PillarsType tmpShape = gActivePillar;
        gActivePillar = PILLAR_CONVEXPOLY;
        CheckMenuItem(hMenu, gActivePillar, MF_CHECKED);
        CheckMenuItem(hMenu, tmpShape, MF_UNCHECKED);
        sClickPts.clear();
        break;
      }
      case ID_OPTIONS_DRAWBC:
      {
        UINT option = (gDrawBC == true) ? MF_UNCHECKED : MF_CHECKED;
        gDrawBC ^= true;
        CheckMenuItem(hMenu, ID_OPTIONS_DRAWBC, option);
        break;
      }
      case ID_OPTIONS_DRAWAABB:
      {
        UINT option = (gDrawAABB == true) ? MF_UNCHECKED : MF_CHECKED;
        gDrawAABB ^= true;
        CheckMenuItem(hMenu, ID_OPTIONS_DRAWAABB, option);
        break;
      }
      case ID_HELP_ABOUT:
        MessageBox(hwnd, gModalMsg.c_str(), "CS200 A4 Help", MB_OK);
        break;
      }

      return 0;
    }
    case WM_PAINT:
      BeginPaint(hwnd, &ps);
      EndPaint(hwnd, &ps);
      return 0;
    case WM_DESTROY: return 0;
    case WM_CLOSE:
      DeletePillars(gPPillars);
      DeletePillars(gPSecPillar);
      PostQuitMessage(0);
      return 0;

    default: break;
    }

    // Dispatch any unhandled messages
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Checks if a mode is active
  @param	mode
  The mode to test
  @return	true if the mode is set to active, false if not
  */
  ////////////////////////////////////////////////////////////////////////////////
  static bool ActiveMode(const enum ModeType mode) {
    return (gActiveMode == mode);
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Checks if a pillar is active
  @param	pillar
  The pillar to test
  @return	true if the pillar is set to active, false if not
  */
  ////////////////////////////////////////////////////////////////////////////////
  static bool ActivePillar(const enum PillarsType pillar) {
    return (gActivePillar == pillar);
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Checks if an option is active
  @param	option
  The option to test
  @return	true if the option is set to active, false if not
  */
  ////////////////////////////////////////////////////////////////////////////////
  static bool ActiveOption(const enum OptionsType option) {
    return (gActiveOption == option);
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Checks if the mouse is inside or on a pillar
  @param	LPARAM lParam
  Mouse coordinates
  @return	true if mouse is intersecting with pillar; false otherwise
  */
  ////////////////////////////////////////////////////////////////////////////////
  static bool IsMouseInPillar(LPARAM lParam) {
    Point2D pt(static_cast<float>(LOWORD(lParam)), static_cast<float>(HIWORD(lParam)));
    for (unsigned int ui = 0; ui < gPPillars.size(); ++ui) {
      if (gPPillars[ui]->TestPillarVsPoint(pt)) return true;
    }
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Checks if the mouse is inside or on a pillar
  @param	const Point2D& pt
  Mouse coordinates
  @param	unsigned int& uidx
  Index of pillar object in global vector array of Pillar * gPPillars
  @return	true if mouse is intersecting with pillar; false otherwise
  index uidx of pillar that mouse is intersecting with
  */
  ////////////////////////////////////////////////////////////////////////////////
  static bool IsMouseInPillar(const Point2D& pt, unsigned int& uidx) {
    for (uidx = 0; uidx < gPPillars.size(); ++uidx) {
      if (gPPillars[uidx]->TestPillarVsPoint(pt)) return true;
    }
    return false;
  }

  //Color palette for pillars
  static const int sClrPaletteCnt = 24;
  static Color sClrPalette[sClrPaletteCnt] = {
    Color(231, 158, 24, 255),	Color(252, 108, 3, 255),
    Color(188, 67, 146, 255),	Color(254, 30, 185, 255),
    Color(87, 168, 168, 255),	Color(91, 113, 164, 255),
    Color(124, 91, 164, 255),	Color(155, 91, 164, 255),
    Color(193, 248, 7, 255),	Color(3, 252, 158, 255),
    Color(134, 158, 97, 255),	Color(202, 191, 53, 255),
    Color(255, 0, 128, 255),	Color(242, 13, 145, 255),
    Color(231, 24, 143, 255),	Color(216, 39, 140, 255),
    Color(165, 2, 253, 255),	Color(170, 22, 233, 255),
    Color(163, 40, 215, 255),	Color(156, 63, 192, 255),
    Color(1, 185, 254, 255),	Color(30, 176, 225, 255),
    Color(55, 160, 200, 255),	Color(83, 143, 172, 255)
  };

  static int sClrIdx;

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Adds circular primitve CircularPillar (derived from type Pillar)
  to pillar list
  @param	std::vector<Pillar*>& list
  The list to which primitive is added
  @param	std::vector<LPARAM>& lparamVtx[2]
  First element specifies center
  Second element is used to compute circle's radius (distance
  between center and second point)
  @param	bool bIsShapeModeled
  If true, then completely modeled primitive is added to list
  If false, temporary primitive is added to list after previous temporary
  primitive is deleted
  @return	bool
  True if pillar is not intersecting with other pillars
  False if pillar is not intersecting and therefore was not added to list
  */
  ////////////////////////////////////////////////////////////////////////////////
  bool InsertCircularPillar(std::vector<Pillar*>&		    list,
    const std::vector<LPARAM>&	lparamVtx,
    bool						            bIsShapeModeled) {
    Point2D center(static_cast<float>(LOWORD(lparamVtx[0])), static_cast<float>(HIWORD(lparamVtx[0])));
    Point2D offset(static_cast<float>(LOWORD(lparamVtx[1])), static_cast<float>(HIWORD(lparamVtx[1])));
    CircularPillar* pCP = new CircularPillar(center, offset, sClrPalette[sClrIdx%sClrPaletteCnt]);

    if (bIsShapeModeled == false) {
      if (!list.empty()) { list.pop_back(); }
      list.push_back(pCP);
      return true;
    }

    unsigned int ui;
    for (ui = 0; ui < list.size(); ++ui) { if (pCP->TestPillar(list[ui]) == true) break; }
    if (ui == list.size()) { // no intersection
      list.push_back(pCP);
      sClrIdx = rand();
      return true;
    }

    // otherwise, intersection has occurred ...
    delete pCP;
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Adds triangular primitve of type PolygonalPillar (derived from type Pillar)
  to the pillar list
  @param	std::vector<Pillar *>& list
  The list to which primitive is added
  @param	std::vector<LPARAM>& lparamVtx[3]
  @param	bool bIsShapeModeled
  If true, then completely modeled primitive is added to list
  If false, temporary primitive is added to list after previous temporary
  primitive is deleted
  @return	bool
  True if pillar is not intersecting with other pillars
  False if pillar is not intersecting and therefore was not added to list
  Notes:	Clockwise-oriented triangular primitves are discarded
  */
  ////////////////////////////////////////////////////////////////////////////////
  bool InsertTriangularPillar(std::vector<Pillar*>&		    list,
    const std::vector<LPARAM>&	lparamVtx,
    bool						            bIsShapeModeled) {
    Vertex2D tVtx[3];
    for (unsigned int ui = 0; ui < 3; ++ui) {
      tVtx[ui].SetPosition(static_cast<float>(LOWORD(lparamVtx[ui])), static_cast<float>(HIWORD(lparamVtx[ui])));
      tVtx[ui].SetColor(sClrPalette[(sClrIdx + ui) % sClrPaletteCnt]);
    }
    PolygonalPillar *pPP = new PolygonalPillar(tVtx, 3);
    if (pPP->IsConvex() == false) { // concave polygons are not added to list ...
      delete pPP;
      return true;
    }

    if (bIsShapeModeled == false) { // don't check for intersection for partially modeled pillars
      if (!list.empty()) { list.pop_back(); }
      list.push_back(pPP);
      return true;
    }

    unsigned int ui;
    for (ui = 0; ui < list.size(); ++ui) { if (pPP->TestPillar(list[ui]) == true) break; }
    if (ui == list.size()) { // no intersection
      list.push_back(pPP);
      sClrIdx = rand();	// change colors for next pillar
      return true;
    }

    // otherwise, intersection has occurred ...
    delete pPP;
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Adds rectangular primitve of type PolygonalPillar (derived from type Pillar)
  to the pillar list
  @param	std::vector<Pillar*>& list
  The list to which primitive is added
  @param	std::vector<LPARAM>& lparamVtx[2]
  Diametrically opposite points of a rectangular box
  @param	bool bIsShapeModeled
  If true, then completely modeled primitive is added to list
  If false, temporary primitive is added to list after previous temporary
  primitive is deleted
  @return	bool
  True if pillar is not intersecting with other pillars
  False if pillar is not intersecting and therefore was not added to list
  Notes:	The four vertices of rectangular primitives are always stored
  in device frame in the order: top-left, bottom-left, bottom-right,
  followed by top-right
  */
  ////////////////////////////////////////////////////////////////////////////////
  bool InsertRectangularPillar(std::vector<Pillar*>&		    list,
    const std::vector<LPARAM>&	lparamVtx,
    bool						            bIsShapeModeled) {
    // rectangle center, half-width, half-height
    float x = static_cast<float>(LOWORD(lparamVtx[0]));
    float cx = (x + static_cast<float>(LOWORD(lparamVtx[1]))) / 2.f;
    float y = static_cast<float>(HIWORD(lparamVtx[0]));
    float cy = (y + static_cast<float>(HIWORD(lparamVtx[1]))) / 2.f;
    float hw = ABSF(cx - x);
    float hh = ABSF(cy - y);

    Vertex2D rVtx[4] = {
      Vertex2D(Point2D(cx - hw, cy - hh), sClrPalette[sClrIdx%sClrPaletteCnt]),
      Vertex2D(Point2D(cx - hw, cy + hh), sClrPalette[(sClrIdx + 1) % sClrPaletteCnt]),
      Vertex2D(Point2D(cx + hw, cy + hh), sClrPalette[(sClrIdx + 2) % sClrPaletteCnt]),
      Vertex2D(Point2D(cx + hw, cy - hh), sClrPalette[(sClrIdx + 3) % sClrPaletteCnt])
    };

    PolygonalPillar* pPP = new PolygonalPillar(rVtx, 4);
    if (bIsShapeModeled == false) {
      if (!list.empty()) { list.pop_back(); }
      list.push_back(pPP);
      return true;
    }

    unsigned int ui;
    for (ui = 0; ui < list.size(); ++ui) { if (pPP->TestPillar(list[ui]) == true) break; }
    if (ui == list.size()) { // no intersection
      list.push_back(pPP);
      sClrIdx = rand();
      return true;
    }

    // otherwise, intersection has occurred ...
    delete pPP;
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Adds polygonal primitve of type PolygonalPillar (derived from type Pillar)
  to the pillar list
  @param	std::vector<Pillar*>& list
  The list to which primitive is added
  @param	std::vector<LPARAM>& lparamVtx[]
  List of polygon vertices
  @param	bool bIsShapeModeled
  If true, then completely modeled primitive is added to list
  If false, temporary primitive is added to list after previous temporary
  primitive is deleted
  @return	bool
  True if pillar is not intersecting with other pillars
  False if pillar is not intersecting and therefore was not added to list
  Notes:	Only convex, counter-clockwise oriented polygons are added to list
  */
  ////////////////////////////////////////////////////////////////////////////////
  bool InsertPolygonalPillar(std::vector<Pillar*>&		    list,
    const std::vector<LPARAM>&	lparamVtx,
    bool						            bIsShapeModeled) {
    Vertex2D* pVtx = new Vertex2D[lparamVtx.size()];
    for (unsigned int ui = 0; ui < lparamVtx.size(); ++ui) {
      pVtx[ui].SetPosition(static_cast<float>(LOWORD(lparamVtx[ui])), static_cast<float>(HIWORD(lparamVtx[ui])));
      pVtx[ui].SetColor(sClrPalette[(sClrIdx + ui) % sClrPaletteCnt]);
    }
    PolygonalPillar* pPP = new PolygonalPillar(pVtx, static_cast<int>(lparamVtx.size()));
    delete[] pVtx;

    if (pPP->IsConvex() == false) { // concave polygons are not added to list ...
      delete pPP;
      return true;
    }

    if (bIsShapeModeled == false) { // don't check for intersection for partially modeled pillars
      if (!list.empty()) { list.pop_back(); }
      list.push_back(pPP);
      return true;
    }

    unsigned int ui;
    for (ui = 0; ui < list.size(); ++ui) { if (pPP->TestPillar(list[ui]) == true) break; }
    if (ui == list.size()) { // no intersection
      list.push_back(pPP);
      sClrIdx = rand();	// change colors for next pillar
      return true;
    }

    // otherwise, intersection has occurred ...
    delete pPP;
    return false;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Delete's memory allocated to primitive's by each element in list
  Next, erases all elements in list
  @param	std::vector<Pillar *>&
  The list whose contents are to be erased
  */
  ////////////////////////////////////////////////////////////////////////////////
  void DeletePillars(std::vector<Pillar*>& list) {
    for (unsigned int ui = 0; ui < list.size(); ++ui) { delete list[ui]; }
    list.clear();
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Saves modeled scene to file
  @param	const char*
  Name of file to which scene is saved
  @param	std::vector<Pillar*>&
  The list of pillars to be saved to file
  */
  ////////////////////////////////////////////////////////////////////////////////
  bool WriteSceneToFile(const char* ps, std::vector<Pillar*>& list) {
    std::ofstream	ofs(ps);
    if (ofs.fail() == true) { return false; }

    ofs << static_cast<unsigned int>(list.size()) << "\n";
    for (unsigned int ui = 0; ui < list.size(); ++ui) {
      if (CircularPillar* pCP = dynamic_cast<CircularPillar*>(list[ui])) {
        ofs << "circular pillar\n";
        ofs << *pCP;
      }
      else if (PolygonalPillar* pPP = dynamic_cast<PolygonalPillar*>(list[ui])) {
        ofs << "polygonal pillar\n";
        ofs << *pPP;
      }
    }

    ofs.close();
    return true;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Saves modeled scene to file
  @param	const char*
  Name of file to which scene is saved
  @param	std::vector<Pillar *>&
  The list of pillars to be saved to file
  */
  ////////////////////////////////////////////////////////////////////////////////
  static Point2D ReadPositionLineFromFile(std::ifstream& ifs) {
    char str[100];
    ifs.getline(str, 100);

    float fx, fy;
    sscanf_s(str, " %f %f", &fx, &fy);
    return (Point2D(fx, fy));
  }

  static Color ReadColorLineFromFile(std::ifstream& ifs) {
    char str[100];
    ifs.getline(str, 100);

    int r, g, b, a;
    sscanf_s(str, " %d %d %d %d", &r, &g, &b, &a);

    return (Color(static_cast<unsigned char>(r),
      static_cast<unsigned char>(g),
      static_cast<unsigned char>(b),
      static_cast<unsigned char>(a)));
  }

  static int ReadIntLineFromFile(std::ifstream& ifs) {
    char str[100];
    ifs.getline(str, 100);

    int i;
    sscanf_s(str, " %d ", &i);
    return i;
  }

  ////////////////////////////////////////////////////////////////////////////////
  /*!
  @brief	Reads saved scene from file and writes the file contents to list
  of pillars
  @param	const char*
  Name of file from which scene is read
  @param	std::vector<Pillar *>&
  The vector container which will store the retrieved scene
  */
  ////////////////////////////////////////////////////////////////////////////////
  bool ReadSceneFromFile(const char* ps, std::vector<Pillar*>& list) {
    std::ifstream ifs(ps, std::ios::in);
    if (ifs.is_open() == false) { return false; }
    ifs.seekg(0, std::ios::beg);

    int pillarCnt = 0;
    char lineTxt[100];

    ifs.getline(lineTxt, 100);
    sscanf_s(lineTxt, " %d ", &pillarCnt);
    while (pillarCnt--) {
      ifs.getline(lineTxt, 100);
      std::string str(lineTxt);

      if (str == "circular pillar") {
        Point2D	center = ReadPositionLineFromFile(ifs);
        int		radius = ReadIntLineFromFile(ifs);
        Color	clr = ReadColorLineFromFile(ifs);

        CircularPillar*	pCP = new CircularPillar(center, radius, clr);
        list.push_back(pCP);
      }
      else if (str == "polygonal pillar") {
        int			numVtx = ReadIntLineFromFile(ifs);
        Vertex2D*	pVtx = new Vertex2D[numVtx];
        for (int i = 0; i < numVtx; ++i) {
          pVtx[i].SetPosition(ReadPositionLineFromFile(ifs));
          pVtx[i].SetColor(ReadColorLineFromFile(ifs));
        }

        PolygonalPillar*	pPP = new PolygonalPillar(pVtx, numVtx);
        list.push_back(pPP);
        delete[] pVtx;
      }
    }
    ifs.close();
    return true;
  }

  static void InitializeModalMsg(std::string& str) {
    str = "CS 200 A5 Framework";
    str += "\nCopyright 2005-2014 DigiPen Institute of Technology\n\n";
    str += "At start-up, a previously defined scene is read from \"A5 Scene.txt\"\n";
    str += "\nThe Pick mode is the default Mode menu selection. Use the mouse left-button to pick a solid pillar.\n";
    str += "Holding the left-button down and dragging the mouse allows the picked pillar to be repositioned.\n";
    str += "Note that the picked pillar cannot be repositioned such that it intersects with other pillars.\n";
    str += "\nThe Create selection in the Mode menu allows users to create new pillars. The application defines only\n";
    str += "two class types: CircularPillars and PolygonalPillars. However, for convenience, the interface allows\n";
    str += "the creation of additional polygonal pillars: Triangular, Rectangular, and Arbitrary Polygons.\n";
    str += "Circular pillars are created by two mouse clicks: the first click point defining circle center while the\n";
    str += "second click point is used to compute the radius as distance between the two click points.\n";
    str += "Triangular pillars require three mouse clicks. Note that only counter-clockwise oriented click points\n";
    str += "are considered valid input.\n";
    str += "Rectangular pillars require two mouse clicks describing diagonally opposite points of a box.\n";
    str += "Polygonal pillars can have an arbitrary number of vertices with the right mouse button used to signal\n";
    str += "end of user input.\n";
    str += "\nThe Drawing Options menu selections allow users to toggle the drawing of boundaries of bounding\n";
    str += "circles (BCs) and axis-aligned bounding boxes (AABBs) enclosing solid pillars.\n";
    str += "By default, both these settings are enabled.\n";
    str += "\nUse 'C' to save current scene to \"A5 Scene.txt\" followed by the deletion of all pillars in the scene.\n";
    str += "\nAs always, bug reports are appreciated and must be directed to sang.le@digipen.edu\n";
  }

}	// namespace CS200
