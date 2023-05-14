/******************************************************************************
FileName: ui.cpp
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "ui.h"

#include "WEMtx44.h"
#include "WEGraphics.h"

namespace
{
}

void ui::Initialize(int _MaxPlayerShield, int _MaxPlayerEnergy)
{
	MaxPlayerEnergy = _MaxPlayerEnergy;
	MaxPlayerHealth = _MaxPlayerShield;
}

void ui::UpdateUI(player *Player)
{
    PlayerHealth = Player->Health;
    PlayerEnergy = Player->Energy;

}

void ui::DrawUI(int BlankTexture)
{
    WEMtx44 UITransform;

	//Drawing part
    WEGraphics::SetTexture(BlankTexture);
	WEGraphics::SetToOrtho();

	for (int Index = 0;
		Index < MaxPlayerHealth;
		Index++)
	{
		WEGraphics::SetTextureColor(0xff808080);
		WEMtx44::MakeTransform(UITransform, UIHealthSize.X * 0.5f, UIHealthSize.Y * 0.5f,
			0.0f,
			UIPosition.X + UIEnergySize.X,
			UIPosition.Y - UIHealthSize.Y * 1.1f * Index,
			0.0f);
		WEGraphics::Draw(UITransform);
	}

	//Draw Health
	for (int Index = 0; Index < PlayerHealth; Index++)
	{
		WEGraphics::SetTextureColor(0xff6654fe);
		WEMtx44::MakeTransform(UITransform, UIHealthSize.X, UIHealthSize.Y,
			0.0f,
			UIPosition.X + UIEnergySize.X,
			UIPosition.Y - UIHealthSize.Y * 1.1f * Index,
			1.0f);
		WEGraphics::Draw(UITransform);
	}

	for (int Index = 0;
		Index < MaxPlayerEnergy;
		Index++)
	{
		WEGraphics::SetTextureColor(0xff808080);
		WEMtx44::MakeTransform(UITransform, UIEnergySize.X * 0.5f, UIEnergySize.Y * 0.5f,
			0.0f,
			UIPosition.X,
			UIPosition.Y - UIEnergySize.Y * 1.1f * Index,
			0.0f);
		WEGraphics::Draw(UITransform);

	}
	//Draw Energy : 1.1f is distance between UIs
	for (int Index = 0; 
         Index < PlayerEnergy;
         Index++)
	{
        WEGraphics::SetTextureColor(0xffb8ff65);
        WEMtx44::MakeTransform(UITransform, UIEnergySize.X, UIEnergySize.Y,
            0.0f,
            UIPosition.X,
			UIPosition.Y - UIEnergySize.Y * 1.1f * Index,
            1.0f);
        WEGraphics::Draw(UITransform);
	}

	WEGraphics::SetToPerspective();
}