/******************************************************************************
FileName: camera.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "camera.h"

#include "WERandom.h"

void camera::Update(point BasePos, float dt)
{
    Pos = BasePos;
    if(ShakeTimer >= 0)
    {
        Pos.X += WERandom::GetFloat(-(ShakeAmount + 2.0f * ShakeTimer), ShakeAmount + 2.0f * ShakeTimer);
        Pos.Y += WERandom::GetFloat(-(ShakeAmount + 2.0f * ShakeTimer), ShakeAmount + 2.0f * ShakeTimer);        
        ShakeTimer -= dt;
    }
    else
    {
        Pos = BasePos; 
    }
}

void camera::SetShake(float _ShakeTimer, float _ShakeAmount)
{
    ShakeTimer = _ShakeTimer;
    ShakeAmount = _ShakeAmount;
}