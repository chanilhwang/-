////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Main.cpp
	@autor	pghali@digipen.edu
	@date	May 10, 2005
	@par	CS200 Framework
	@par	Copyright 2005-2007 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Includes
#include <windows.h>
#include <string>
#include "UI.hpp"

// Local Function Prototypes
static void		Update(CS200::Window&, DWORD&);
static void		Draw(CS200::Window&);

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
    if(!window.Create(hInstance, "CS200 A4 Framework", 800, 600)) return 1;
	MessageBox(window.GetWindowHandle(), CS200::gModalMsg.c_str(), "CS200 A4 Help", MB_OK);

    DWORD oldTime = ::timeGetTime();
    while(window.Running())
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
	@param	window
			The active Window object
	@param	oldTime
			The previous time
*/
////////////////////////////////////////////////////////////////////////////////
static void Update(CS200::Window & window, DWORD & oldTime)
{
	static const float TICKS_PER_SEC	=	1000.f;
	static const float FPS_30			=	(TICKS_PER_SEC/30.f);

   window.ProcessMessages();	// Handle window messages
	
	DWORD newTime;
	while (1)	// hold to 30fps
	{
		newTime = ::timeGetTime();	// Get the updated time
		if(newTime - oldTime <= FPS_30)	continue; // 1/30 second
		oldTime = newTime;	// Re-set the time
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief	Top-level function to render all game objects 
	@param	window
			The active Window object
*/
////////////////////////////////////////////////////////////////////////////////
static void Draw(CS200::Window & window)
{
	if (CS200::ModeIsActive(CS200::MODE_TEST))
	{
		// First, draw black outline of triangle
		// (100, 75) -> (100, 525) -> (700, 525) -> (700, 75) -> (100, 75)
		CS200::Color blk(0, 0, 0, 255);

		int x, y;
#if 0
		for (x = 200, y = 100; y <= 475; ++y)
			window.SetPixel(x, y, blk);
		for (y = 475; x <= 700; ++x)
			window.SetPixel(x, y, blk);
		for (x = 700; y >= 100; --y)
			window.SetPixel(x, y, blk);
		for (y = 100; x >= 200; --x)
			window.SetPixel(x, y, blk);
#else
		for (x = 100, y = 75; y <= 525; ++y)
			window.SetPixel(x, y, blk);
		for (y = 525; x <= 700; ++x)
			window.SetPixel(x, y, blk);
		for (x = 700; y >= 75; --y)
			window.SetPixel(x, y, blk);
		for (y = 75; x >= 100; --x)
			window.SetPixel(x, y, blk);
#endif

		switch(CS200::gTexFuncUMode)
		{
		case CS200::TEXFUNCU_CLAMP:
			(*(CS200::gPActiveList))[0].SetVertexU(0.f, 0);
			(*(CS200::gPActiveList))[0].SetVertexU(0.f, 1);
			(*(CS200::gPActiveList))[0].SetVertexU(1.f, 2);
			(*(CS200::gPActiveList))[0].SetVertexU(1.f, 3);
			break;
		case CS200::TEXFUNCU_REPEAT:
		case CS200::TEXFUNCU_MIRROREDREPEAT:
			(*(CS200::gPActiveList))[0].SetVertexU(0.f, 0);
			(*(CS200::gPActiveList))[0].SetVertexU(0.f, 1);
			(*(CS200::gPActiveList))[0].SetVertexU(3.f, 2);
			(*(CS200::gPActiveList))[0].SetVertexU(3.f, 3);
			break;
		}

		switch(CS200::gTexFuncVMode)
		{
		case CS200::TEXFUNCV_CLAMP:
			(*(CS200::gPActiveList))[0].SetVertexV(0.f, 0);
			(*(CS200::gPActiveList))[0].SetVertexV(1.f, 1);
			(*(CS200::gPActiveList))[0].SetVertexV(1.f, 2);
			(*(CS200::gPActiveList))[0].SetVertexV(0.f, 3);
			break;
		case CS200::TEXFUNCV_REPEAT:
		case CS200::TEXFUNCV_MIRROREDREPEAT:
			(*(CS200::gPActiveList))[0].SetVertexV(0.f, 0);
			(*(CS200::gPActiveList))[0].SetVertexV(3.f, 1);
			(*(CS200::gPActiveList))[0].SetVertexV(3.f, 2);
			(*(CS200::gPActiveList))[0].SetVertexV(0.f, 3);
			break;
		}
	}

	for (int i = 0; i < static_cast<int>(CS200::gPActiveList->size()); ++i)
	{
#if 0
		switch(CS200::gTexFuncUMode)
		{
		case TEXFUNCU_CLAMP:
			(*gPActiveList)[i].SetVertexU(0.f, 0);
			(*gPActiveList)[i].SetVertexU(0.f, 1);
			(*gPActiveList)[i].SetVertexU(1.f, 2);
			(*gPActiveList)[i].SetVertexU(1.f, 3);
			break;
		case TEXFUNCU_REPEAT:
		case TEXFUNCU_MIRROREDREPEAT:
			(*gPActiveList)[i].SetVertexU(0.f, 0);
			(*gPActiveList)[i].SetVertexU(0.f, 1);
			(*gPActiveList)[i].SetVertexU(3.f, 2);
			(*gPActiveList)[i].SetVertexU(3.f, 3);
			break;
		}

		switch(CS200::gTexFuncVMode)
		{
		case TEXFUNCV_CLAMP:
			(*gPActiveList)[i].SetVertexV(0.f, 0);
			(*gPActiveList)[i].SetVertexV(1.f, 1);
			(*gPActiveList)[i].SetVertexV(1.f, 2);
			(*gPActiveList)[i].SetVertexV(0.f, 3);
			break;
		case TEXFUNCV_REPEAT:
		case TEXFUNCV_MIRROREDREPEAT:
			(*gPActiveList)[i].SetVertexV(0.f, 0);
			(*gPActiveList)[i].SetVertexV(3.f, 1);
			(*gPActiveList)[i].SetVertexV(3.f, 2);
			(*gPActiveList)[i].SetVertexV(0.f, 3);
			break;
		}
#endif

		(*(CS200::gPActiveList))[i].Draw(window, CS200::gCCMode, CS200::gTexFuncUMode, CS200::gTexFuncVMode);
	}

	if(CS200::ModeIsActive(CS200::MODE_TEXCOORD))
	{
		int idx = (CS200::gTexMapType == CS200::TEXTURE_TWO) ? CS200::TEXTURE_FOUR : ((CS200::gTexMapType == CS200::TEXTURE_THREE) ? CS200::TEXTURE_FIVE : CS200::gTexMapType);
		
		CS200::gTexMapList[idx].Draw(window, 5, 5);
		CS200::gTexMapList[idx].DrawAlpha(window, 5, 5 + CS200::gTexMapList[idx].GetHeight());

		if (!CS200::gpPickedVtx) return;

		for(int d = 0; d < 9; ++d)
		{
			float fx = 3.5f + d/3.f + CS200::gpPickedVtx->GetU()*(CS200::gTexMapList[idx].GetWidth() - 0.5f);
			float fy = 4.5f + static_cast<float>(d%3) + CS200::gpPickedVtx->GetV() * (CS200::gTexMapList[idx].GetHeight() - 0.5f);
			window.SetPixel( static_cast<int>(fx), static_cast<int>(fy), CS200::Color(255,0,0,255));

			fx = -1.5f + d/3 + CS200::gpPickedVtx->GetPosition().GetX();
			fy = -1.5f + static_cast<float>(d%3) + CS200::gpPickedVtx->GetPosition().GetY();
			window.SetPixel( static_cast<int>(fx), static_cast<int>(fy), CS200::Color(255,0,0,255));
		}
	}
}

