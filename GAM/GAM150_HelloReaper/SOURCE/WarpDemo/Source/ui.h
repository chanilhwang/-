/******************************************************************************
FileName: ui.h
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef UI_H

#include "player.h"
#include "WEApplication.h"

class ui
{
public:
    int PlayerHealth;
    int PlayerShield;
    int PlayerEnergy;
    int PlayerWeapon;

	int MaxPlayerHealth;
	int MaxPlayerEnergy;

	bool IsFirstTimeToUpdate = true;
    
	point UIEnergySize = { WEApplication::GetHeight() * 0.05f ,
		WEApplication::GetHeight() * 0.05f };
	point UIHealthSize = { WEApplication::GetHeight() * 0.05f,
		WEApplication::GetHeight() * 0.05f };

	//position of energy
	point UIPosition = { WEApplication::GetWidth() * 0.25f,
		WEApplication::GetHeight() * 0.7f };
    
	void Initialize(int MaxPlayerShield, int MaxPlayerEnergy);
    void UpdateUI(player *Player);
    void DrawUI(int BlankTexture);
};

#define UI_H
#endif