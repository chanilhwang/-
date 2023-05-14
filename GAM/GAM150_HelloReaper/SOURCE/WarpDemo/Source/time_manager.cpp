/******************************************************************************
FileName: time_manager.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "time_manager.h"

time_manager::time_manager(float _SlowDownFactor, float _SlowDownLength)
{
    SlowDownFactor = _SlowDownFactor;
    SlowDownLength = _SlowDownLength;
}
float time_manager::DoSlowmotion(float dt)
{
    dt *= SlowDownFactor;
    
    return dt;
}

bool time_manager::CheckRemainingTime(void)
{
    if(SlowDownLength > 0.0f)
        return true;
    else
        return false;
}
