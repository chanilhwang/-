/******************************************************************************
FileName: ending.cpp
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
#include "WEMath.h"
#include "WERandom.h"

#include "fmod_general.h"
#include "hreaper_general.h"
#include "ending.h"
#include <cstdio>
namespace
{
    int EndingTxt;
    fmod_sound_system PlayBGM;
    WEGameData *GameData;
}
void EndingLoad()
{
    GameData = WEStateManager::GetGameData();

    EndingTxt= WEGraphics::LoadTexture("Textures\\ending.tga");
}
void EndingInit()
{
    PlayBGM.MakeSound("Sounds\\BGM\\peace.mp3", true);	
    PlayBGM.PlaySound();
	WEGraphics::SetBackgroundColor(0.098039f, 0.094117f, 0.113725f);
}
void EndingUpdate(float dt)
{
    WEGraphics::StartDraw();
	WEMtx44 transform;

    WEGraphics::SetTextureColor(0xffffffff);
    WEGraphics::SetTexture(EndingTxt);
    WEMtx44::MakeTransform(transform, 70.0f, 70.0f,
                            0.0f,
                            0.0f, 0.0f,
                            0.0f);

	WEGraphics::Draw(transform);

    char Text[30];
    sprintf_s(Text, "Total Clear Time : %.3f", GameData->PlayTime);
    WEGraphics::SetTextureColor(0xFFFFFFff);
    WEGraphics::WriteText(Text, -35.0f, -45.0f);


    WEGraphics::Draw(transform);

    WEGraphics::EndDraw();
}
void EndingShutdown()
{

}
void EndingUnload()
{
    WEGraphics::UnloadTexture(EndingTxt);
}