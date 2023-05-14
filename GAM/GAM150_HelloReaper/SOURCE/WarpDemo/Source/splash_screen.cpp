/******************************************************************************
FileName: splash_screen.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "WEInput.h"
#include "WEStateManager.h"
#include "WEApplication.h"
#include "WEMtx44.h"
#include "WEGraphics.h"
#include "WEVec2.h"
#include <stdio.h>
#include "splash_screen.h"
#include "states_enum.h"
#include "hreaper_general.h"
#include "draw_texture.h"


namespace
{
	float YOffSet = 4.0f;
	bool IsFullScreen = false;


	//Textures;
	int Blank;
	int HeadPad;
	int DigipenLogo;
	int DigipenWriting;
	int Fmod;
	int Powered;
	int MiruLogo;
	int Reco;
	int PictureCount;
	float PictureTime;;

	float Alpha;
	
	//Not2.0f
	#define EACH_PICTURE_TIME 2.0f

	enum SplashOrders
	{
		DIGIPENLOGO, FMOD, MIRULOGO, HEADPAD, END
	};
}

void SplashLoad()
{
	Blank = WEGraphics::LoadTexture("Textures\\Blank.tga");
	HeadPad = WEGraphics::LoadTexture("Textures\\Splash\\HeadPad.tga");
	DigipenLogo = WEGraphics::LoadTexture("Textures\\Splash\\DigiLogo.tga");
	DigipenWriting = WEGraphics::LoadTexture("Textures\\Splash\\DigiPen_RGB_White.tga");
	Fmod = WEGraphics::LoadTexture("Textures\\Splash\\fmod_logo.tga");
	Powered = WEGraphics::LoadTexture("Textures\\Splash\\powered.tga");
	MiruLogo = WEGraphics::LoadTexture("Textures\\Splash\\miru_logo.tga");
	Reco = WEGraphics::LoadTexture("Textures\\Splash\\recommend.tga");
}

void SplashInit()
{
	PictureCount = 0;
	PictureTime = 0.0f;

	WEGraphics::SetBackgroundColor(0.098039f, 0.094117f, 0.113725f);
}

void SplashUpdate(float dt)
{
	WEVec2 topLeft;
	WEGraphics::GetWorldTopLeft(topLeft);

	// if (WEInput::IsAnyTriggered())
	// {
	// 	WEStateManager::SetNextState(STATE_GAME_PLAYING);
	// }

	//Start Drawing
	WEMtx44 transform;

	WEGraphics::StartDraw();

	WEGraphics::SetTextureColor(0xFFFFFFFF);
	WEGraphics::SetTextureColor((unsigned char)(255),
								(unsigned char)(255), 
								(unsigned char)(255), 
								(unsigned char)(Alpha));

	if (WEInput::IsAnyTriggered())
		if(PictureCount < END && PictureCount != DIGIPENLOGO)
			PictureCount++;
	switch(PictureCount)
	{
		case DIGIPENLOGO:
		{
			WEGraphics::SetTexture(DigipenWriting);
			WEMtx44::MakeTransform(transform, 80.0f, 20.0f,
                            0.0f,
                            0.0f, 0.0f,
                            0.0f);

			WEGraphics::Draw(transform); 
		}break;

		case FMOD:
		{
			WEGraphics::SetTexture(Fmod);
			WEMtx44::MakeTransform(transform, 64.0f, 16.0f,
                            0.0f,
                            0.0f, -3.0f,
                            0.0f);

			WEGraphics::Draw(transform); 

			WEGraphics::SetTexture(Powered);
			WEMtx44::MakeTransform(transform, 64.0f, 16.0f,
                            0.0f,
                            0.0f, 15.0f,
                            0.0f);

			WEGraphics::Draw(transform); 

		}break;
		
		case MIRULOGO:
		{
			WEGraphics::SetTexture(MiruLogo);
			WEMtx44::MakeTransform(transform, 40.0f, 40.0f,
                            0.0f,
                            0.0f, 0.0f,
                            0.0f);

			WEGraphics::Draw(transform); 
		}break;

		case HEADPAD:
		{
			WEGraphics::SetTexture(HeadPad);
			WEMtx44::MakeTransform(transform, 50.0f, 50.0f,
                            0.0f,
                            0.0f, 5.0f,
                            0.0f);

			WEGraphics::Draw(transform); 

			WEGraphics::SetTexture(Reco);
			WEMtx44::MakeTransform(transform, 64.0f, 8.0f,
                            0.0f,
                            0.0f, -22.0f,
                            0.0f);

			WEGraphics::Draw(transform); 
		}break;

		case END:
		{
			WEStateManager::SetNextState(STATE_MAIN_MENU);
		}
	}

	WEGraphics::EndDraw();

	PictureTime += dt;
	if(PictureTime >= 0.0f && PictureTime < 0.63f * EACH_PICTURE_TIME)
	{
		Alpha += 300 * dt;
		if(Alpha >= 255.0f)
		{
			Alpha = 255.0f;
		}
	}
	else if(PictureTime >= 0.63f * EACH_PICTURE_TIME)
	{

		Alpha -= 400 * dt;
		if(Alpha <= 0.0f)
		{
			Alpha = 0.0f;
		}
	}

	if(PictureTime >= EACH_PICTURE_TIME)
	{
		PictureCount++;
		PictureTime = 0.0f;
		Alpha = 0.0f;
	}
}

void SplashShutdown()
{
}

void SplashUnload()
{
	WEGraphics::UnloadTexture(Blank);
	WEGraphics::UnloadTexture(DigipenLogo);
	WEGraphics::UnloadTexture(DigipenWriting);
	WEGraphics::UnloadTexture(Fmod);	
	WEGraphics::UnloadTexture(Powered);		
	WEGraphics::UnloadTexture(MiruLogo);
	WEGraphics::UnloadTexture(HeadPad);
	WEGraphics::UnloadTexture(Reco);
}