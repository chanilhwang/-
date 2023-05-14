////////////////////////////////////////////////////////////////////////////////
/*!
	@file	UI.cpp
	@autor	pghali@digipen.edu
	@date	May 12, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <string>
#include "UI.hpp"
#include "..\resource.h"

namespace CS200
{
Vertex2D*						gpPickedVtx;
std::string					gModalMsg;
Texture							gTexMapList[TEXTURE_MAX];
std::vector<Quad>*	gPActiveList;

static std::vector<Quad> sQuadList, sTestList;
#if 0
static Quad	sTestQuad(Vertex2D(200.f, 100.f, 255, 0, 0, 255, 0.f, 0.f),	// set up 500 X 375 test rectangle ...
											Vertex2D(200.f, 475.f, 0, 255, 0, 255, 0.f, 1.f),
											Vertex2D(700.f, 475.f, 0, 0, 255, 255, 1.f, 1.f),
											Vertex2D(700.f, 100.f, 127, 127, 127, 255, 1.f, 0.f));
#else
/*
static Quad	sTestQuad(Vertex2D(100.f, 75.f, 255, 0, 0, 255, 0.f, 0.f),	// set up 600 X 450 test rectangle ...
											Vertex2D(100.f, 525.f, 0, 255, 0, 255, 0.f, 1.f),
											Vertex2D(700.f, 525.f, 0, 0, 255, 255, 1.f, 1.f),
											Vertex2D(700.f, 75.f, 127, 127, 127, 255, 1.f, 0.f));
*/
static Quad	sTestQuad(Vertex2D(99.5f, 74.5f, 255, 0, 0, 255, 0.f, 0.f),	// set up 600 X 450 test rectangle ...
											Vertex2D(99.5f, 524.5f, 0, 255, 0, 255, 0.f, 1.f),
											Vertex2D(699.5f, 524.5f, 0, 0, 255, 255, 1.f, 1.f),
											Vertex2D(699.5f, 74.5f, 127, 127, 127, 255, 1.f, 0.f));
#endif
static int	sPickedQuadIdx = -1;

// Global objects
enum ModeType					gInputMode				=	MODE_CREATE;
enum ModeType					gPrevInputMode		=	MODE_CREATE;
enum TextureType			gTexMapType				=	TEXTURE_ONE;
enum TextureType			gPrevTexObject		=	TEXTURE_ONE;
enum ColorCombineMode	gCCMode						=	CC_REPLACE;
enum ColorCombineMode	gPrevCCMode				=	CC_REPLACE;
enum TexFuncUMode			gTexFuncUMode			=	TEXFUNCU_CLAMP;
enum TexFuncUMode			gPrevTexFuncUMode	=	TEXFUNCU_CLAMP;
enum TexFuncVMode			gTexFuncVMode			=	TEXFUNCV_CLAMP;
enum TexFuncVMode			gPrevTexFuncVMode	=	TEXFUNCV_CLAMP;

// Functions with project scope ...
LRESULT CALLBACK MyWndProc(HWND, UINT, WPARAM, LPARAM);

// Functions with file scope ...
static void				AddQuad(std::vector<Quad>&, LPARAM [4]);
static void				AddQuad(std::vector<Quad>*, Quad&);
static void				ClearQuad(std::vector<Quad>*);
static void				InitializeTextures();
static Color*			ParseBMPFile(const char*, int&, int&);
inline static int ABS(int i) { return ((i >= 0) ? i : -i); }
static void				InitializeModalMsg(std::string&);
static Vertex2D*	GetClosestVertex(unsigned int, unsigned int, float);
static void				DisableEditMenus(HWND);
static void				EnableEditMenus(HWND);

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
LRESULT CALLBACK MyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static unsigned int	sQuadVtx = 0;
	static bool			sEditTexCoordFirstTime = true;
	static LPARAM		sQuadClickPt[4];
	PAINTSTRUCT			ps;
	static HINSTANCE	hInstance;

	// Check  message type
  switch (msg)
  {
	case WM_CREATE:
		{
			InitializeModalMsg(gModalMsg);
			InitializeTextures();
			AddQuad(&sTestList, sTestQuad);
			gPActiveList	=	&sQuadList;
			
			CheckMenuItem(GetMenu(hwnd), ID_MODE_CREATEQUAD, MF_CHECKED);
			CheckMenuItem(GetMenu(hwnd), ID_TEXTURE_CHECKERBOARD, MF_CHECKED);
			CheckMenuItem(GetMenu(hwnd), ID_COLORCOMBINER_REPLACE, MF_CHECKED);
			CheckMenuItem(GetMenu(hwnd), ID_TEXFUNCU_CLAMP, MF_CHECKED);
			CheckMenuItem(GetMenu(hwnd), ID_TEXFUNCV_CLAMP, MF_CHECKED);

			DisableEditMenus(hwnd);

			return 0;
		}

  case WM_SIZING:
	return TRUE;
	
  case WM_LBUTTONDOWN:
      {
		if(ModeIsActive(MODE_CREATE)) // Quadrilateral vertices
		{
			sQuadClickPt[sQuadVtx%4]	=	lParam;
			if ((sQuadVtx%4) == 3)
			{
				AddQuad(sQuadList, sQuadClickPt);
				sQuadVtx = 0;
				if (!sQuadList.size())	DisableEditMenus(hwnd);
				else					EnableEditMenus(hwnd);
			}
			else
				sQuadVtx++;
		}
		else if(ModeIsActive(MODE_TEXCOORD)) // Edit quad's texture coordinates
		{
			if(true == sEditTexCoordFirstTime)
			{
				if(gpPickedVtx = GetClosestVertex(LOWORD(lParam), HIWORD(lParam), 10.f))	sEditTexCoordFirstTime = false;
			}
			else if(false == sEditTexCoordFirstTime)
			{
				unsigned int x = LOWORD(lParam), y = HIWORD(lParam);

				Vertex2D *pVtx = GetClosestVertex(x, y, 10.f);	// Has a vertex been chosen?
				if (pVtx) gpPickedVtx = pVtx;
				else // no vertex but is pick point inside color map?
				{
					int idx = (gTexMapType == TEXTURE_TWO) ? TEXTURE_FOUR : ((gTexMapType == TEXTURE_THREE) ? TEXTURE_FIVE : gTexMapType);
					int ix = x - 5, iy = y - 5;
					int iw = gTexMapList[idx].GetWidth(), ih = gTexMapList[idx].GetHeight();
					if (0 <= ix && ix < iw && 0 <= iy && iy < ih)
					{
						float fw = 1.f/static_cast<float>(iw), fh = 1.f/static_cast<float>(ih);
						gpPickedVtx->SetUV(ix*fw + fw/2.f, iy*fh + fh/2.f);
					}
				}
			}
		}
		else if(ModeIsActive(MODE_CLRCOORD))	// Edit quad's color coordinates

		{
			if(gpPickedVtx = GetClosestVertex(LOWORD(lParam), HIWORD(lParam), 20.f))
			{
				CHOOSECOLOR		cc;							// common dialog box structure 
				static COLORREF	acrCustClr[16];				// array of custom colors 
				static DWORD	rgbCurrent = RGB(255,0,0);	// initial color selection

				ZeroMemory(&cc, sizeof(cc));				// Initialize CHOOSECOLOR
				cc.lStructSize = sizeof(cc);
				cc.hwndOwner = hwnd;
				cc.lpCustColors = (LPDWORD) acrCustClr;
				cc.rgbResult = rgbCurrent;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;
						
				if (ChooseColor(&cc)==TRUE)
					gpPickedVtx->SetColor(CS200::Color(GetRValue(cc.rgbResult),GetGValue(cc.rgbResult),GetBValue(cc.rgbResult),255));
			}
		}
		}

		return 0;

    case WM_KEYDOWN:
        {
            switch(wParam)		// The keycode is specified by wParam
            {
				case 0x43:      // Use 'C' key (0x43)to clear lists
                    {
						if (gInputMode != MODE_TEST)
						{
							ClearQuad(&sQuadList);
							if (gInputMode == MODE_TEXCOORD || gInputMode == MODE_CLRCOORD)
							{
								gPrevInputMode = gInputMode = MODE_CREATE;
								CheckMenuItem(GetMenu(hwnd), gInputMode, MF_CHECKED);
								CheckMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_UNCHECKED);
								CheckMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_UNCHECKED);
							}
							
							DisableEditMenus(hwnd);

							sPickedQuadIdx			=	-1;
							sQuadVtx				=	0;
							sEditTexCoordFirstTime	=	true;
						}
                        break;
                    }
            }
        
			return 0;
        }

    case WM_COMMAND:	// Handle messages from the Menu bar
        {
            HMENU hMenu = ::GetMenu(hwnd);
			static unsigned int wrapu = 0, wrapv = 1;

            switch(LOWORD(wParam))
            {
				case ID_MODE_CREATEQUAD:
					{
					if (!sQuadList.size())	DisableEditMenus(hwnd);
					else					EnableEditMenus(hwnd);
					

					gInputMode = MODE_CREATE;
					if (gPrevInputMode == gInputMode) return 0;
					CheckMenuItem(hMenu, gInputMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevInputMode, MF_UNCHECKED);
					gPrevInputMode = gInputMode;
					gPActiveList			=	&sQuadList;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_MODE_EDITTEXCOORDS:
					{
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_CLAMP, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_REPEAT, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_MIRROREDREPEAT, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_CLAMP, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_REPEAT, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_MIRROREDREPEAT, MF_GRAYED);

					gInputMode = MODE_TEXCOORD;
					if (gPrevInputMode == gInputMode) return 0;
					CheckMenuItem(hMenu, gInputMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevInputMode, MF_UNCHECKED);
					gPrevInputMode = gInputMode;
					gPActiveList			=	&sQuadList;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_MODE_EDITCLRCOORDS:
					{
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_CLAMP, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_REPEAT, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_MIRROREDREPEAT, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_CLAMP, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_REPEAT, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_MIRROREDREPEAT, MF_GRAYED);

					gInputMode = MODE_CLRCOORD;
					if (gPrevInputMode == gInputMode) return 0;
					CheckMenuItem(hMenu, gInputMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevInputMode, MF_UNCHECKED);
					gPrevInputMode = gInputMode;
					gPActiveList			=	&sQuadList;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_MODE_TEST:
					{
					EnableEditMenus(hwnd);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);

					gInputMode = MODE_TEST;
					if (gPrevInputMode == gInputMode) return 0;

					CheckMenuItem(hMenu, gInputMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevInputMode, MF_UNCHECKED);
					gPrevInputMode = gInputMode;
					gPActiveList			=	&sTestList;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
				}
				case ID_COLORCOMBINER_REPLACE:
					{
					gCCMode = CC_REPLACE;
					if (gPrevCCMode == gCCMode) return 0;
					CheckMenuItem(hMenu, gCCMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevCCMode, MF_UNCHECKED);
					gPrevCCMode = gCCMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_COLORCOMBINER_DECAL:
					{
					gCCMode = CC_DECAL;
					if (gPrevCCMode == gCCMode) return 0;
					CheckMenuItem(hMenu, gCCMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevCCMode, MF_UNCHECKED);
					gPrevCCMode = gCCMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_COLORCOMBINER_MODULATE:
					{
					gCCMode = CC_MODULATE;
					if (gPrevCCMode == gCCMode) return 0;
					CheckMenuItem(hMenu, gCCMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevCCMode, MF_UNCHECKED);
					gPrevCCMode = gCCMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				
				case ID_COLORCOMBINER_GOURARD:
					{
					gCCMode = CC_GOURARD;
					if (gPrevCCMode == gCCMode) return 0;
					CheckMenuItem(hMenu, gCCMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevCCMode, MF_UNCHECKED);
					gPrevCCMode = gCCMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_TEXTURE_CHECKERBOARD:
					{
					gTexMapType = TEXTURE_ONE;
					CheckMenuItem(hMenu, ID_TEXTURE_CHECKERBOARD, MF_CHECKED);
					CheckMenuItem(hMenu, ID_TEXTURE_DUCK, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_TEXTURE_LILY, MF_UNCHECKED);
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_TEXTURE_DUCK:
					{
					gTexMapType = TEXTURE_TWO;
					CheckMenuItem(hMenu, ID_TEXTURE_CHECKERBOARD, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_TEXTURE_DUCK, MF_CHECKED);
					CheckMenuItem(hMenu, ID_TEXTURE_LILY, MF_UNCHECKED);
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
				case ID_TEXTURE_LILY:
					{
					gTexMapType = TEXTURE_THREE;
					CheckMenuItem(hMenu, ID_TEXTURE_CHECKERBOARD, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_TEXTURE_DUCK, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_TEXTURE_LILY, MF_CHECKED);
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}
	
				case ID_TEXFUNCU_CLAMP:
					{
					Quad* pq = 0;
					if (gInputMode == ID_MODE_CREATEQUAD && sQuadList.size())
						pq = (sPickedQuadIdx == -1) ? &sQuadList[sQuadList.size()-1] : &sQuadList[sPickedQuadIdx];
					else if (gInputMode == ID_MODE_TEST)
						pq = &sTestList[0];

					if (pq)
					{
						pq->SetVertexU(0.f, 0);	pq->SetVertexU(0.f, 1);
						pq->SetVertexU(1.f, 2);	pq->SetVertexU(1.f, 3);
					}

					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);
					DrawMenuBar(hwnd);

					gTexFuncUMode = TEXFUNCU_CLAMP;
					if (gPrevTexFuncUMode == gTexFuncUMode) return 0;
					CheckMenuItem(hMenu, gTexFuncUMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevTexFuncUMode, MF_UNCHECKED);

					gPrevTexFuncUMode			= gTexFuncUMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;
					}

				case ID_TEXFUNCU_REPEAT :
					{
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);
					DrawMenuBar(hwnd);

					gTexFuncUMode = TEXFUNCU_REPEAT;
					if (gPrevTexFuncUMode == gTexFuncUMode) return 0;
					CheckMenuItem(hMenu, gTexFuncUMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevTexFuncUMode, MF_UNCHECKED);

					Quad* pq = (gInputMode == ID_MODE_CREATEQUAD) ? pq = &sQuadList[sQuadList.size()-1] : &sTestList[0];
					pq->SetVertexU(0.f, 0);	pq->SetVertexU(0.f, 1);
					pq->SetVertexU(3.f, 2);	pq->SetVertexU(3.f, 3);

					gPrevTexFuncUMode			= gTexFuncUMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;
		
					break;
					}
				case ID_TEXFUNCU_MIRROREDREPEAT:
					{
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);
					DrawMenuBar(hwnd);

					gTexFuncUMode = TEXFUNCU_MIRROREDREPEAT;
					if (gPrevTexFuncUMode == gTexFuncUMode) return 0;
					CheckMenuItem(hMenu, gTexFuncUMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevTexFuncUMode, MF_UNCHECKED);

					Quad* pq = (gInputMode == ID_MODE_CREATEQUAD) ? pq = &sQuadList[sQuadList.size()-1] : &sTestList[0];
					pq->SetVertexU(0.f, 0);	pq->SetVertexU(0.f, 1);
					pq->SetVertexU(3.f, 2);	pq->SetVertexU(3.f, 3);

					gPrevTexFuncUMode			= gTexFuncUMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;
		
					break;
					}	

				case ID_TEXFUNCV_CLAMP:
					{
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);
					DrawMenuBar(hwnd);

					gTexFuncVMode = TEXFUNCV_CLAMP;
					if (gPrevTexFuncVMode == gTexFuncVMode) return 0;
					CheckMenuItem(hMenu, gTexFuncVMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevTexFuncVMode, MF_UNCHECKED);

					Quad* pq = (gInputMode == ID_MODE_CREATEQUAD) ? pq = &sQuadList[sQuadList.size()-1] : &sTestList[0];
					pq->SetVertexV(0.f, 0);	pq->SetVertexV(1.f, 1);
					pq->SetVertexV(1.f, 2);	pq->SetVertexV(0.f, 3);

					gPrevTexFuncVMode			= gTexFuncVMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;
		
					break;
					}	

				case ID_TEXFUNCV_REPEAT:
					{
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);
					DrawMenuBar(hwnd);

					Quad* pq = 0;
					if (gInputMode == ID_MODE_CREATEQUAD && sQuadList.size())
						pq = (sPickedQuadIdx == -1) ? &sQuadList[sQuadList.size()-1] : &sQuadList[sPickedQuadIdx];
					else if (gInputMode == ID_MODE_TEST)
						pq = &sTestList[0];

					if (pq)
					{
						pq->SetVertexV(0.f, 0);	pq->SetVertexV(3.f, 1);
						pq->SetVertexV(3.f, 2);	pq->SetVertexV(0.f, 3);
					}

					gTexFuncVMode = TEXFUNCV_REPEAT;
					if (gPrevTexFuncVMode == gTexFuncVMode) return 0;
					CheckMenuItem(hMenu, gTexFuncVMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevTexFuncVMode, MF_UNCHECKED);

					gPrevTexFuncVMode			= gTexFuncVMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;

					break;

					}
				case ID_TEXFUNCV_MIRROREDREPEAT :
					{
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
					EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);
					DrawMenuBar(hwnd);

					gTexFuncVMode = TEXFUNCV_MIRROREDREPEAT;
					if (gPrevTexFuncVMode == gTexFuncVMode) return 0;
					CheckMenuItem(hMenu, gTexFuncVMode, MF_CHECKED);
					CheckMenuItem(hMenu, gPrevTexFuncVMode, MF_UNCHECKED);

					Quad* pq = (gInputMode == ID_MODE_CREATEQUAD) ? pq = &sQuadList[sQuadList.size()-1] : &sTestList[0];
					pq->SetVertexV(0.f, 0);	pq->SetVertexV(3.f, 1);
					pq->SetVertexV(3.f, 2);	pq->SetVertexV(0.f, 3);

					gPrevTexFuncVMode			= gTexFuncVMode;
					sQuadVtx				=	0;
					sEditTexCoordFirstTime	=	true;
		
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

        case WM_DESTROY:
            return 0;

		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;

        default:
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam); // Dispatch any unhandled messages
}

static void DisableEditMenus(HWND hwnd)
{
	EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_GRAYED);
	EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_GRAYED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_CLAMP, MF_GRAYED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_REPEAT, MF_GRAYED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_MIRROREDREPEAT, MF_GRAYED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_CLAMP, MF_GRAYED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_REPEAT, MF_GRAYED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_MIRROREDREPEAT, MF_GRAYED);
	DrawMenuBar(hwnd);
}

static void EnableEditMenus(HWND hwnd)
{
	EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITTEXCOORDS, MF_ENABLED);
	EnableMenuItem(GetMenu(hwnd), ID_MODE_EDITCLRCOORDS, MF_ENABLED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_CLAMP, MF_ENABLED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_REPEAT, MF_ENABLED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCU_MIRROREDREPEAT, MF_ENABLED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_CLAMP, MF_ENABLED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_REPEAT, MF_ENABLED);
	EnableMenuItem(GetMenu(hwnd), ID_TEXFUNCV_MIRROREDREPEAT, MF_ENABLED);
	DrawMenuBar(hwnd);
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Checks if a mode is active
	@param	mode
			The mode to test
	@return	true if the mode is set to active, false if not
*/
////////////////////////////////////////////////////////////////////////////////
bool ModeIsActive(const enum ModeType mode)
{
	return (gInputMode == mode);
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Adds a quadrilateral to the list
	@param	std::vector<Quad>	
			The list to which quadrilateral is added
	@param	q	
			The quadrilateral to add
*/
////////////////////////////////////////////////////////////////////////////////
static void AddQuad(std::vector<Quad>* listP, Quad& q)
{
	// discard clockwise oriented quadrilaterals ...
	for (int i = 0; i < 4; ++i)
	{
		CS200::Point2D e0 = q.GetVertex((i+1)%4).GetPosition() - q.GetVertex(i%4).GetPosition();
		CS200::Point2D e1 = q.GetVertex((i+2)%4).GetPosition() - q.GetVertex((i+1)%4).GetPosition();
		if (e0.ScalarTripleProduct(e1) >= 0.f)
			return;
	}

	listP->push_back(q);
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Adds a quadrilateral to the list
	@param	std::vector<Quad>	
			The list to which quadrilateral is added
	@param	LPARAM [4]	
			Array of quadrilateral vertices
*/
////////////////////////////////////////////////////////////////////////////////
static void AddQuad(std::vector<Quad>& listR, LPARAM quadVtx[4])
{
	

	Quad q
		(	
		Vertex2D(static_cast<float>(LOWORD(quadVtx[0])), static_cast<float>(HIWORD(quadVtx[0])), 255, 0, 0, 255, 0.f, 0.f),
		Vertex2D(static_cast<float>(LOWORD(quadVtx[1])), static_cast<float>(HIWORD(quadVtx[1])), 0, 255, 0, 255, 0.f, 1.f),
		Vertex2D(static_cast<float>(LOWORD(quadVtx[2])), static_cast<float>(HIWORD(quadVtx[2])), 0, 0, 255, 255, 1.f, 1.f),
		Vertex2D(static_cast<float>(LOWORD(quadVtx[3])), static_cast<float>(HIWORD(quadVtx[3])), 127, 127, 127, 255, 1.f, 0.f)
		);

	// discard clockwise oriented quadrilaterals ...
	for (int i = 0; i < 4; ++i)
	{
		Point2D e0 = q.GetVertex((i+1)%4).GetPosition() - q.GetVertex(i%4).GetPosition();
		Point2D e1 = q.GetVertex((i+2)%4).GetPosition() - q.GetVertex((i+1)%4).GetPosition();
		if (e0.ScalarTripleProduct(e1) >= 0.f)
			return;
	}

	switch(CS200::gTexFuncUMode)
	{
	case TEXFUNCU_CLAMP:
		q.SetVertexU(0.f, 0);
		q.SetVertexU(0.f, 1);
		q.SetVertexU(1.f, 2);
		q.SetVertexU(1.f, 3);
		break;
	case TEXFUNCU_REPEAT:
	case TEXFUNCU_MIRROREDREPEAT:
		q.SetVertexU(0.f, 0);
		q.SetVertexU(0.f, 1);
		q.SetVertexU(3.f, 2);
		q.SetVertexU(3.f, 3);
		break;
	}

	switch(CS200::gTexFuncVMode)
	{
	case TEXFUNCV_CLAMP:
		q.SetVertexV(0.f, 0);
		q.SetVertexV(1.f, 1);
		q.SetVertexV(1.f, 2);
		q.SetVertexV(0.f, 3);
		break;
	case TEXFUNCV_REPEAT:
	case TEXFUNCV_MIRROREDREPEAT:
		q.SetVertexV(0.f, 0);
		q.SetVertexV(3.f, 1);
		q.SetVertexV(3.f, 2);
		q.SetVertexV(0.f, 3);
		break;
	}

	listR.push_back(q);
}


////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Erases  quadrilateral list
	@param	std::vector<CS200::Quad>	
			The list which is to be erased
*/
////////////////////////////////////////////////////////////////////////////////
static void ClearQuad(std::vector<Quad>* listP)
{
	listP->clear();
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Initializes textures
	checkerboard texture: built procedurally
	duck texture: extracted from duck.BMP
	waterlily texture: extracted from waterlily.BMP
	Alpha maps for all texture are built procedurally
*/
////////////////////////////////////////////////////////////////////////////////
static void InitializeTextures()
{
	// 1st bitmap: checkerboard
#if 0
	int		width	= 128, height = 128;
#else
	int		width	= 600, height = 450;
#endif
	Color	*data	= new Color[width*height];
	const int checkSize = 50;

	// build alpha map
	float xc	= width/2.f - 0.5f;
	float yc	= height/2.f - 0.5f;
	float rad	= static_cast<float>((width > height) ? width : height);
	float sqrad	=	(rad * rad)/4.f;
	for (int i = 0, rowClr = 0, idx = 0; i < height; ++i, (rowClr = (i%checkSize) ? rowClr : (rowClr^1)))
		for (int j = 0, colClr = rowClr; j < width; ++j, ++idx, (colClr = (j%checkSize) ? colClr : (colClr^1)))
		{
			float dist = (xc - (float) j) * (xc - (float) j) + (yc - (float) i) * (yc - (float) i);
			float falpha = ((sqrad - dist) * 255.f)/sqrad;
			int ialpha = (int) (falpha + 0.5f);
			ialpha = (ialpha < 0) ? 0 : ialpha;
			ialpha = (ialpha > 255) ? 255 : ialpha;
			data[idx] = (colClr == 0) ? Color(234, 157, 253, ialpha) : Color(185, 255, 185, ialpha);
		}
	gTexMapList[TEXTURE_ONE].SetTexelData(data, width, height); delete [] data;
	
	// 2nd bitmap: duck
	data = ParseBMPFile("duck.bmp", width, height);
	for (int i = 0; i < width*height; ++i)	//hack to create alpha map for duck texture
		if (data[i].GetR()==255 && data[i].GetG()==255 && data[i].GetB()==255) data[i].SetRGBA(185, 255, 185, 0);
		else	data[i].SetA(255);
	gTexMapList[TEXTURE_TWO].SetTexelData(data, width, height); delete [] data;
	// low-res duck map for editing texture coordinates ...
	data = ParseBMPFile("ducklow.bmp", width, height);
	for (int i = 0; i < width*height; ++i)
		if (data[i].GetR()==255 && data[i].GetG()==255 && data[i].GetB()==255)	data[i].SetRGBA(185, 255, 185, 0);
		else	data[i].SetA(255);
	gTexMapList[TEXTURE_FOUR].SetTexelData(data, width, height); delete [] data;

	// 3rd bitmap: waterlily
	data = ParseBMPFile("lily.bmp", width, height);
	for (int i = 0; i < width*height; ++i)	//hack to create alpha map for lily texture
		if (data[i].GetR()<20 && data[i].GetG()<20 && data[i].GetB()<20) data[i].SetA(0);
		else	data[i].SetA(255);
	gTexMapList[TEXTURE_THREE].SetTexelData(data, width, height); delete [] data;
	// low-res water-lily map for editing texture coordinates ...
	data = ParseBMPFile("lilylow.bmp", width, height);
	for (int i = 0; i < width*height; ++i)	//hack to create alpha map for lily texture
		if (data[i].GetR()<20 && data[i].GetG()<20 && data[i].GetB()<20) data[i].SetA(0);
		else	data[i].SetA(255);
	gTexMapList[TEXTURE_FIVE].SetTexelData(data, width, height); delete [] data;
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Parses BMP files  quadrilateral list
	@param	name	
				Input: BMP file source
			width
				Output: Horizontal size of image data
			height
				Output: Vertical size of image data
	@notes	Only parses 24-bit uncompressed BMP files!!!
*/
////////////////////////////////////////////////////////////////////////////////
static Color* ParseBMPFile(const char* name, int& width, int& height)
{
	std::ifstream	ifs(name,std::ios::binary);
	if (ifs.fail() == true)	throw std::runtime_error("ParseBMPFile: Could not open file!!!");

	// read file header
	BITMAPFILEHEADER fileHeader;
	ifs.read(reinterpret_cast< char* >(&fileHeader), sizeof(BITMAPFILEHEADER));
	if (ifs.gcount() != sizeof(BITMAPFILEHEADER))
		throw std::runtime_error("ParseBMPFile: Problem reading file header!!!");
	if (fileHeader.bfType != 'MB')
		throw std::runtime_error("ParseBMPFile: Not a BMP file!!!");

	BITMAPINFOHEADER infoHeader;
	ifs.read(reinterpret_cast< char* >(&infoHeader), sizeof(BITMAPINFOHEADER));
	if (ifs.gcount() != sizeof(BITMAPINFOHEADER))
		throw std::runtime_error("ParseBMPFile: Problem reading file info header!!!");
	if (infoHeader.biBitCount != 24)
		throw std::runtime_error("ParseBMPFile: Not a 24-bit RGB bitmap!!!");
	if (infoHeader.biCompression != BI_RGB)
		throw std::runtime_error("ParseBMPFile: Not an uncompressed bitmap!!!");

	// some BI_RGB files have image size set to 0 ...
	int imageSize	=	(infoHeader.biSizeImage) ? infoHeader.biSizeImage :
		(infoHeader.biWidth*ABS(infoHeader.biHeight)*((infoHeader.biBitCount+7)/8));

	char *pImgData	=	new char [imageSize];
	ifs.read(pImgData, imageSize);
	if (ifs.gcount() != imageSize)
		throw std::runtime_error("ParseBMPFile: Problem reading RGB image data!!!");

	bool invWidthFlag	=	(infoHeader.biHeight > 0) ? true : false;
	width				=	infoHeader.biWidth;
	height				=	(infoHeader.biHeight < 0) ? -infoHeader.biHeight : infoHeader.biHeight;

	Color *pBM			=	new Color [width*height];

	if (!invWidthFlag)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0, rowOffset = y*width, colOffset = y*width; x < width; ++x, ++colOffset)
			{
				int srcIdx	=	colOffset * 3;

				pBM[colOffset].SetB(pImgData[srcIdx]);
				pBM[colOffset].SetG(pImgData[srcIdx+1]);
				pBM[colOffset].SetR(pImgData[srcIdx+2]);

			}
		}
	}
	else
	{
		for (int y = 0, dstIdx = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x, ++dstIdx)
			{
				int srcIdx	=	(imageSize-3) - dstIdx * 3;

				pBM[dstIdx].SetB(pImgData[srcIdx]);
				pBM[dstIdx].SetG(pImgData[srcIdx+1]);
				pBM[dstIdx].SetR(pImgData[srcIdx+2]);

			}
		}
	}

	delete [] pImgData;

	return pBM;
}

static void InitializeModalMsg(std::string& str)
{
		// Initialize modal dialog message ...
	str	= "CS200 A4 Framework";
	str	+= "\nCopyright 2005-2007 DigiPen Institute of Technology\n\n";

	str	+=	"A4's modeling primitive is a quadrilateral. Quads are rendered by splitting them into two triangles and rendering each\n";
	str	+=	"triangle independently. The \"Mode\" menu allows user to toggle between creating quads, editing quads, or testing mode.\n";
		
	str	+= "\nTo model a new quadrilateral, do the following:\n";
	str	+= "  1. First, select \"Create Quad\" submenu in \"Mode\" menu.\n";
	str	+= "  2. Click left mouse button at 4 counter-clockwise ordered viewport positions. Clockwise ordered quads are discarded.\n";
	str	+= "      First clicked viewport position is assigned index 0 in quad's vertex list and so on. See A4 specifications document\n";
	str	+= "      for texture coordinate values assigned to vertices for the CLAMP, REPEAT, and MIRROREDREPEAT modes.\n";

	str	+= "  3. Use 'C' or 'c' to clear previously drawn primitives.\n";

	str	+= "\nTo edit vertex texture coordinates of a previously modeled quadrilateral, do the following:\n";
	str	+= "  1. First, select \"Edit Texture Coordinates\" submenu in \"Mode\" menu.\n";
	str	+= "  2. Second, select a vertex of the quad to be edited.\n";
	str	+= "  3. Next, select a new set of texture coordinates for the selected vertex by clicking left mouse button in\n";
	str	+= "     color map drawn in top-left corner of viewport.\n";

	str	+= "\nTo edit vertex color coordinates of a previously modeled quadrilateral, do the following:\n";
	str	+= "  1. First, select \"Edit Color Coordinates\" submenu in \"Mode\" menu.\n";
	str	+= "  2. Second, select a vertex of the quad to be edited.\n";
	str	+= "  3. Next, select a new set of color coordinates for the selected vertex using the color box.\n";

	str	+= "\nTo test your triangle rasterizer, select \"Test\" submenu in \"Mode\" menu. The test mode creates a 500 x 375 rectangle\n";
	str	+= "that is rendered by splitting it into two triangles, and rendering each triangle using top-left tie breaking fill rule.\n";

	str	+= "\nUse \"Texture\" menu to toggle between the 3 previously defined texture objects.\n";

	str	+= "\nUse \"Color Combiner\" menu to toggle between these four different color combine modes:\n";
	str	+= "  1. \"Replace\": Pixel color is texel color.\n";
	str	+= "  2. \"Decal\": Pixel color is a linearly interpolated value between vertex color and texel color\n";
	str	+= "     using texel alpha as the interpolant.\n";
	str	+= "  3. \"Modulate\": Pixel color is pairwise multiplication of texel and vertex color components.\n";
	str	+= "  4. \"Gourard Shading\": Pixel color is interpolated vertex color.\n";

	str	+= "\nUse \"TexFuncU\" and \"TexFuncV\" menu options to implement functions that transform texture coordinates. These functions are\n";
	str	+= "independently implemented along horizontal and vertical directions. The following texture functions are implemented:\n";
	str	+= "  1. \"Repeat\"\n";
	str	+= "  2. \"Mirror\"\n";
	str	+= "  3. \"Clamp (to edge)\"\n";

	str	+= "\nAs always, bug reports are appreciated and must be directed to pghali@digipen.edu\n";
}


////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Determines if mouse click point is "close" to a quad vertex
	@param	x	
				x-coordinate of mouse click point
			y	
				y-coordinate of mouse click point
			rad
				"Closeness" factor
	@return	Returns memory address of quad vertex "close" to click point if one
			exists; otherwise returns 0
	Note: Function also sets sPickedQuadIdx to quad in list to which picked vertex belongs.
*/
////////////////////////////////////////////////////////////////////////////////
static Vertex2D* GetClosestVertex(unsigned int x, unsigned int y, float rad)
{
	for(unsigned int i = 0; i < gPActiveList->size(); ++i)
		for(unsigned int v = 0; v < 4; ++v)
		{
			float dx = x - (*gPActiveList)[i].GetVertex(v).GetPosition().GetX();
			float dy = y - (*gPActiveList)[i].GetVertex(v).GetPosition().GetY();

			if(dx*dx + dy*dy <= rad*rad) 
			{
				sPickedQuadIdx = i;
				return & ((*gPActiveList)[i].GetVertex(v));
			}
		}

	return 0;
}

}	// namespace CS200
