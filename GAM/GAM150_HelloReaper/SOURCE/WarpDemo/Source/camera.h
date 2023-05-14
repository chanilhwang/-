/******************************************************************************
FileName: camera.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef CAMERA_H

#include "hreaper_general.h"
class camera
{
public:
    point Pos;
    float ShakeTimer;
    float ShakeAmount;

    void Update(point BasePos, float dt);
    void SetShake(float _ShakeTimer, float _ShakeAmount);
};

#define CAMERA_H
#endif