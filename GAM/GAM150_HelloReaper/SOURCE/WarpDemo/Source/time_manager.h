/******************************************************************************
FileName: time_manager.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#ifndef TIME_MANAGER_H

class time_manager
{
public:
    //How much will the time slow down
    //Half Slow : SlowDownFactor = 0.5f
    float SlowDownFactor;
    //How long for will the time slow down
    float SlowDownLength;

    time_manager(float _SlowDownFactor, float _SlowDownLength);
    float DoSlowmotion(float dt);
    bool CheckRemainingTime();
};
#define TIME_MANAGER_H
#endif