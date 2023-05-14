/******************************************************************************
FileName: main_menu.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "WEInput.h"
#include "WEStateManager.h"
#include "WEApplication.h"
#include "WEGameData.h"
#include "WEMtx44.h"
#include "WEGraphics.h"
#include "WEVec2.h"

#include "main_menu.h"
#include "states_enum.h"
#include "hreaper_intrinsics.h"

#include "hreaper_general.h"
#include "draw_texture.h"
#include "tile_info.h"
#include "particle.h"

//For the FMOD
#include "fmod_general.h"

#include <windows.h>
#include <stdio.h>

namespace
{

    point_int Selector;

	//To know how many words player selected
	//and output only those words
	int SelectedNum;

	bool Selected;
	bool Backed;

	int MenuNum;
	int SubMenuNum;
	int SubSubMenuNum;

	bool StickLock;

	int InputX;
	int InputY;

	//Textures
	int BlankTxt;
	int MainScreenLogo;
	int Credit;

	//Main Menu Writings
	int StartWriting;
	int OptionWriting;
	int CreditWriting;
	int QuitWriting;
	int MusicVolumeWriting;
	int SFXVolumeWriting;
	int MiruLogo;
	int ResolutionWriting;
	int FullScreenWriting;
	int UncheckedBox;
	int CheckedBox;
	int FmodTxt;
	int FmodCopyright;
	emitter Emitter;

	fmod_sound_system PlayBGM;
	fmod_sound_system PlaySFX;

	//Pointer to the Game Data.
	WEGameData *GameData;

	enum
	{
		MENU_BASE, MENU_START, MENU_OPTION, MENU_CREDIT, MENU_QUIT
	};

	#define SCREEN_NUMBER 9

	int ScreenWidth[SCREEN_NUMBER] = {800, 1024, 1280, 
									1366, 1400, 1440,
									1600, 1600, 1920};
	int ScreenHeight[SCREEN_NUMBER] = {600, 768, 720, 
									768, 1050, 900, 
									900, 1024, 1080};
}

void MainMenuLoad()
{
   GameData = WEStateManager::GetGameData();
    BlankTxt = WEGraphics::LoadTexture("Textures\\Blank.tga");
	MainScreenLogo = WEGraphics::LoadTexture("Textures\\MainMenu\\MainScreenLogo.tga");
	Credit = WEGraphics::LoadTexture("Textures\\MainMenu\\credit.tga");
	StartWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\start.tga");
	OptionWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\option.tga");
	CreditWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\credit_writing.tga");
	QuitWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\quit.tga");
	MusicVolumeWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\music_volume.tga");
	SFXVolumeWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\sfx_volume.tga");
	MiruLogo = WEGraphics::LoadTexture("Textures\\Splash\\miru_logo.tga");
	ResolutionWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\resolution.tga");
	FullScreenWriting = WEGraphics::LoadTexture("Textures\\MainMenu\\FullScreen.tga");
	UncheckedBox = WEGraphics::LoadTexture("Textures\\MainMenu\\checkbox_unchecked.tga");
	CheckedBox = WEGraphics::LoadTexture("Textures\\MainMenu\\checkbox_checked.tga");
	FmodTxt = WEGraphics::LoadTexture("Textures\\Splash\\fmod_logo.tga");
	FmodCopyright = WEGraphics::LoadTexture("Textures\\Splash\\fmod_copyright.tga");
}

void MainMenuInit()
{
	GameData = WEStateManager::GetGameData();

	Selector = { 0 };
	Selected = false;
	Backed = false;
	MenuNum = 0;
	SubMenuNum = 0;
	SubSubMenuNum = 0;
	SelectedNum = 0;
	InputX = 0;
	InputY = 0;
	StickLock = false;

	MenuNum = MENU_BASE;
	//Particle
	Emitter.Initialize(20);
	//Sound
	PlayBGM.MakeSound("Sounds\\BGM\\MainMenuTheme.mp3", true);
	PlayBGM.PlaySound();	
	PlaySFX.MakeSound("Sounds\\SFX\\select.mp3", false);		
	//BackGround
	WEGraphics::SetBackgroundColor(0.098039f, 0.094117f, 0.113725f);
}

void WordBoundCheck(point_int &_Selector, int dX, int dY, tile_map *_WordTiles)
{
	if((_Selector.X + dX >= 0) && (_Selector.Y + dY>= 0) &&
		(_Selector.X + dX < _WordTiles->CountX) && 
		(_Selector.Y + dY < _WordTiles->CountY) &&
		(_Selector.X >= 0) && (_Selector.Y >= 0) && 
		(_Selector.X < _WordTiles->CountX) && 
		(_Selector.Y < _WordTiles->CountY))
    {
        _Selector.X += dX;
        _Selector.Y += dY;
    }
}

void MainMenuUpdate(float dt)
{

	WEMtx44 transform;
	WEVec2 LeftThumb = {0};
	if (WEInput::GamePadIsConnected())
		WEInput::GetLeftThumb(LeftThumb);

	WEVec2 AbsLeftThumb = LeftThumb;

	if(LeftThumb.x < 0.0f)
		AbsLeftThumb.x *= -1.0f;

	if(LeftThumb.y < 0.0f)
		AbsLeftThumb.y *= -1.0f;
		
	if (AbsLeftThumb.x < 0.35f && AbsLeftThumb.y < 0.35f)
		StickLock = false;

	InputX = 0;
	InputY = 0;
	if((!WEInput::IsRepeating(WE_W) && WEInput::IsTriggered(WE_W)) || 
		(!StickLock && LeftThumb.y >= 0.35f && AbsLeftThumb.x < AbsLeftThumb.y))
	{
		InputX = 0;
		InputY = -1;
		StickLock = true;
	}
    else if((!WEInput::IsRepeating(WE_S) && WEInput::IsTriggered(WE_S)) || 
			(!StickLock && LeftThumb.y <= -0.35f && AbsLeftThumb.x < AbsLeftThumb.y))
	{
		InputX = 0;
		InputY = 1;
		StickLock = true;
	}
    else if((!WEInput::IsRepeating(WE_A) && WEInput::IsTriggered(WE_A)) || 
			(!StickLock && LeftThumb.x <= -0.35f && AbsLeftThumb.y < AbsLeftThumb.x))
	{
		InputX = -1;
		InputY = 0;
		StickLock = true;
	}
    else if((!WEInput::IsRepeating(WE_D) && WEInput::IsTriggered(WE_D)) || 
			(!StickLock && LeftThumb.x >= 0.35f && AbsLeftThumb.y < AbsLeftThumb.x))
	{
		InputX = 1;
		InputY = 0;
		StickLock = true;
	}

	Emitter.UpdateParticles(dt);

	//Start Drawing

	if((!WEInput::IsRepeating(WE_SPACE) && WEInput::IsTriggered(WE_SPACE)) || 
		(!WEInput::IsRepeating(WE_GAMEPAD_A) && WEInput::IsTriggered(WE_GAMEPAD_A)))
	{
		Selected = true;
	}
	else
	{
		Selected = false;
	}
	
	if((!WEInput::IsRepeating(WE_ESCAPE) && WEInput::IsTriggered(WE_ESCAPE)) || 
		(!WEInput::IsRepeating(WE_GAMEPAD_B) && WEInput::IsTriggered(WE_GAMEPAD_B)))
	{
		Backed = true;
	}
	else
	{
		Backed = false;
	}
	WEGraphics::StartDraw();

	switch(MenuNum)
	{
		case MENU_BASE:
		{
			if(Selected)
			{
				switch(SubMenuNum)
				{
					case 0:
						MenuNum = MENU_START;
						Selector.X = 0;
						Selector.Y = 0;
						break;
					case 1:
						MenuNum = MENU_OPTION;
						break;
					case 2:
						MenuNum = MENU_CREDIT;
						break;
					case 3:
						MenuNum = MENU_QUIT;
				}
				SubSubMenuNum = 0;
				break;
			}
			SubMenuNum += InputY;
			if(SubMenuNum < 0)
			{
				SubMenuNum = 3;
			}

			if(SubMenuNum > 3)
			{
				SubMenuNum = 0;
			}

			WEGraphics::SetTextureColor((unsigned char)(255),
										(unsigned char)(255), 
										(unsigned char)(255), 
										(unsigned char)(255));

			WEGraphics::SetTexture(MainScreenLogo);
			WEMtx44::MakeTransform(transform, 50.0f, 50.0f,
                            0.0f,
                            -2.0f, 20.0f,
                            0.0f);
			WEGraphics::Draw(transform); 

			WEGraphics::SetTextureColor((unsigned char)(254),
										(unsigned char)(84), 
										(unsigned char)(102), 
										(unsigned char)(255));
			WEGraphics::SetTexture(BlankTxt);
			WEMtx44::MakeTransform(transform, 32.0f, 8.0f,
                            0.0f,
                            0.0f, -15.0f - SubMenuNum * 10.0f,
                            0.0f);

			WEGraphics::Draw(transform); 
			WEGraphics::SetTextureColor((unsigned char)(255),
										(unsigned char)(255), 
										(unsigned char)(255), 
										(unsigned char)(255));
			WEGraphics::SetTexture(StartWriting);
			WEMtx44::MakeTransform(transform, 24.0f, 6.0f,
                            0.0f,
                            0.2f, -15.0f,
                            0.0f);

			WEGraphics::Draw(transform);

			WEGraphics::SetTexture(OptionWriting);
			WEMtx44::MakeTransform(transform, 24.0f, 6.0f,
                            0.0f,
                            0.2f, -24.5f,
                            0.0f);

			WEGraphics::Draw(transform); 

			WEGraphics::SetTexture(CreditWriting);
			WEMtx44::MakeTransform(transform, 24.0f, 6.0f,
                            0.0f,
                            0.2f, -34.5f,
                            0.0f);

			WEGraphics::Draw(transform);

			WEGraphics::SetTexture(QuitWriting);
			WEMtx44::MakeTransform(transform, 24.0f, 6.0f,
                            0.0f,
                            0.2f, -44.5f,
                            0.0f);

			WEGraphics::Draw(transform); 
		}break;
		
		case MENU_START:
		{
			//Initialize Player Infos
			GameData->PlayerShield = 0;
			GameData->PlayerBullet = 0;
			GameData->PlayerEnergy = 5;
			GameData->PlayerUpgrades[7] = { 0 };
			GameData->CurrentLevelID = -1;
			GameData->PlayTime = 0.0f;
			WEStateManager::SetNextState(STATE_TUTORIAL);
		}break;

		case MENU_OPTION:
		{
			if(Backed)
			{
				MenuNum = MENU_BASE;
				break;
			}

			SubSubMenuNum += InputY;
			if(SubSubMenuNum < 0)
			{
				SubSubMenuNum = 3;
			}

			if(SubSubMenuNum > 3)
			{
				SubSubMenuNum = 0;
			}

			float *Volume = 0;

			switch(SubSubMenuNum)
			{
				//BGMVolume
				case 0:
				{
					Volume = &GameData->BGMVolume;	
					*Volume += InputX * 10.0f;

					if(*Volume <= 0.0f)
						*Volume = 0.0f;
					if(*Volume >= 100.0f)
						*Volume = 100.0f;	
				}break;

				//SFXVolume
				case 1:
				{
					Volume = &GameData->SFXVolume;		
					*Volume += InputX * 10.0f;

					if(*Volume <= 0.0f)
						*Volume = 0.0f;
					if(*Volume >= 100.0f)
						*Volume = 100.0f;
				}break;

				//Resolution
				case 2:
				{
					//Change the resolution only if the player tries to change it.
					if(InputX != 0)
					{
						int *Screen = &GameData->ScreenNumber;

						*Screen += InputX;
						if(*Screen > SCREEN_NUMBER - 1)
							*Screen = SCREEN_NUMBER - 1;
						if(*Screen < 0)
							*Screen = 0;
							
						WEApplication::ChangeResolution(ScreenWidth[GameData->ScreenNumber], 
														ScreenHeight[GameData->ScreenNumber]);
					}
				}break;

				//FullScreen
				case 3:
				{
					if(InputX != 0)
					{
						bool *IsFullScreen = &GameData->FullScreen;
						if(*IsFullScreen == true)
							*IsFullScreen = false;
						else
							*IsFullScreen = true;

						WEApplication::SetFullScreen(*IsFullScreen);
					}
				}break;
			}
			
			WEGraphics::SetTextureColor(0xFFFFFFFF); //white			
			WEGraphics::SetTexture(OptionWriting);	
			WEMtx44::MakeTransform(transform, 40.0f, 10.0f,
									0.0f,
									00.0f, 45.0f,
									0.0f);
			WEGraphics::Draw(transform);
			
			//Music Volume Writing
			if(SubSubMenuNum == 0)
				WEGraphics::SetTextureColor(0xffb8ff65);				
			else
				WEGraphics::SetTextureColor(0xFFFFFFFF); //white

			WEGraphics::SetTexture(MusicVolumeWriting);	
			WEMtx44::MakeTransform(transform, 48.0f, 6.0f,
									0.0f,
									0.0f, 30.0f,
									0.0f);
			WEGraphics::Draw(transform);

			//Music Volume Line
			WEGraphics::SetTexture(BlankTxt);			
			WEGraphics::SetTextureColor(0xFFFFFFFF); //white
			WEMtx44::MakeTransform(transform, 100.0f, 0.7f,
									0.0f,
									0.0f, 20.0f,
									0.0f);
			WEGraphics::Draw(transform);

			//SFX Volume Writing
			if(SubSubMenuNum == 1)
				WEGraphics::SetTextureColor(0xffb8ff65);				
			else
				WEGraphics::SetTextureColor(0xFFFFFFFF); //white
			WEGraphics::SetTexture(SFXVolumeWriting);	
			WEMtx44::MakeTransform(transform, 48.0f, 6.0f,
									0.0f,
									00.0f, 10.0f,
									0.0f);
			WEGraphics::Draw(transform);

			WEGraphics::SetTexture(BlankTxt);
			WEGraphics::SetTextureColor((unsigned char)(255),
										(unsigned char)(255), 
										(unsigned char)(255), 
										(unsigned char)(255));
			WEMtx44::MakeTransform(transform, 100.0f, 0.7f,
									0.0f,
									0.0f, 0.0f,
									0.0f);
			WEGraphics::Draw(transform);
			
			WEGraphics::SetTexture(BlankTxt);			
			if(SubSubMenuNum == 0)
				WEGraphics::SetTextureColor((unsigned char)(254),
											(unsigned char)(84), 
											(unsigned char)(102), 
											(unsigned char)(255));
			WEMtx44::MakeTransform(transform, 10.0f, 5.0f,
									0.0f,
									GameData->BGMVolume - 50.0f, 20.0f,
									0.0f);
			WEGraphics::Draw(transform);

			WEGraphics::SetTextureColor(0xFFFFFFFF);
			if(SubSubMenuNum == 1)
				WEGraphics::SetTextureColor((unsigned char)(254),
											(unsigned char)(84), 
											(unsigned char)(102), 
											(unsigned char)(255));
			WEMtx44::MakeTransform(transform, 10.0f, 5.0f,
									0.0f,
									GameData->SFXVolume - 50.0f, 0.0f,
									0.0f);
			WEGraphics::Draw(transform);

			//Resolution
			if(SubSubMenuNum == 2)
				WEGraphics::SetTextureColor(0xffb8ff65);				
			else
				WEGraphics::SetTextureColor(0xFFFFFFFF); //white
			WEGraphics::SetTexture(ResolutionWriting);			
			WEMtx44::MakeTransform(transform, 24.0f, 6.0f,
									0.0f,
									0.0f, -10.0f,
									0.0f);
			WEGraphics::Draw(transform);

			//Write Resolution
			char Text2[20];
			sprintf_s(Text2, "%d * %d", ScreenWidth[GameData->ScreenNumber], ScreenHeight[GameData->ScreenNumber]);
			WEGraphics::SetTextureColor(0xFFFFFFFF);
			WEGraphics::WriteText(Text2, -8.0f, -20.0f);

			if(SubSubMenuNum == 3)
				WEGraphics::SetTextureColor(0xffb8ff65);				
			else
				WEGraphics::SetTextureColor(0xFFFFFFFF); //white
			WEGraphics::SetTexture(FullScreenWriting);			
			WEMtx44::MakeTransform(transform, 24.0f, 6.0f,
									0.0f,
									0.0f, -30.0f,
									0.0f);
			WEGraphics::Draw(transform);

			if(GameData->FullScreen == true)
			{
				WEGraphics::SetTexture(CheckedBox);			
			}
			else
			{
				WEGraphics::SetTexture(UncheckedBox);
			}
			WEMtx44::MakeTransform(transform, 8.0f, 8.0f,
									0.0f,
									0.0f, -40.0f,
									0.0f);
			WEGraphics::Draw(transform);
		}break;

		case MENU_CREDIT:
		{
			if(Backed)
			{
				MenuNum = MENU_BASE;
				break;
			}
			WEGraphics::SetTextureColor(0xFFFFFFFF); //white
			
			WEGraphics::SetTexture(CreditWriting);	
			WEMtx44::MakeTransform(transform, 40.0f, 10.0f,
									0.0f,
									00.0f, 45.0f,
									0.0f);
			WEGraphics::Draw(transform);

			WEGraphics::SetTextureColor(0xFFFFFFFF); //white
			
			WEGraphics::SetTexture(Credit);	
			WEMtx44::MakeTransform(transform, 130.0f, 65.0f,
									0.0f,
									30.0f, -7.0f,
									0.0f);
			WEGraphics::Draw(transform);
			WEGraphics::SetTextureColor(0xFFFFFFFF); //white
			
			WEGraphics::SetTexture(MiruLogo);	
			WEMtx44::MakeTransform(transform, 45.0f, 45.0f,
									0.0f,
									-57.0f, 4.0f,
									0.0f);
			WEGraphics::Draw(transform);

			WEGraphics::SetTexture(FmodTxt);	
			WEMtx44::MakeTransform(transform, 40.0f, 10.0f,
									0.0f,
									-57.0f, -25.0f,
									0.0f);
			WEGraphics::Draw(transform);

			WEGraphics::SetTexture(FmodCopyright);	
			WEMtx44::MakeTransform(transform, 96.0f, 12.0f,
									0.0f,
									-23.5f, -40.0f,
									0.0f);
			WEGraphics::Draw(transform);
		}break;
		case MENU_QUIT:
		{
			WEStateManager::Quit();
		}
	} 
	for (int Index = 0; 
		 Index < Emitter.ParticleNum; 
		 ++Index)
    {
		particle* Particle = &Emitter.Particles[Index];

		if(Particle != 0)
		{
			WEGraphics::SetTextureColor((unsigned char)Particle->Color.Red, 
										(unsigned char)Particle->Color.Green, 
										(unsigned char)Particle->Color.Blue, 
										RoundFloatToUChar(Particle->Color.Alpha));
			WEGraphics::SetTexture(BlankTxt);
			WEMtx44::MakeTransform(transform, Particle->Scale.X, Particle->Scale.Y,
									0.0f,
									Particle->Pos.X, Particle->Pos.Y,
									0.0f);
			//Draw Object
			WEGraphics::Draw(transform);
		}
    }
	WEGraphics::EndDraw();

	//FMOD Process
	PlayBGM.channel->setVolume(0.008f * GameData->BGMVolume);
	PlayBGM.system->update();
	PlaySFX.channel->setVolume(0.008f * GameData->SFXVolume);
	PlaySFX.system->update();
	if(InputX != 0 || InputY != 0)
		PlaySFX.PlaySound();
}

void MainMenuShutdown()
{
	PlayBGM.system->close();
	PlayBGM.system->release();

	PlaySFX.system->close();
	PlaySFX.system->release();	
}

void MainMenuUnload()
{
	WEGraphics::UnloadTexture(BlankTxt);
	WEGraphics::UnloadTexture(MainScreenLogo);
	WEGraphics::UnloadTexture(Credit);
	WEGraphics::UnloadTexture(OptionWriting);
	WEGraphics::UnloadTexture(CreditWriting);
	WEGraphics::UnloadTexture(StartWriting);
	WEGraphics::UnloadTexture(QuitWriting);
	WEGraphics::UnloadTexture(MusicVolumeWriting);
	WEGraphics::UnloadTexture(SFXVolumeWriting);	
	WEGraphics::UnloadTexture(MiruLogo);
	WEGraphics::UnloadTexture(ResolutionWriting);
	WEGraphics::UnloadTexture(FullScreenWriting);
	WEGraphics::UnloadTexture(CheckedBox);
	WEGraphics::UnloadTexture(UncheckedBox);
	WEGraphics::UnloadTexture(FmodTxt);	
	WEGraphics::UnloadTexture(FmodCopyright);		
}