/******************************************************************************
FileName: bullet.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef BULLET_H

#include "hreaper_general.h"

class bullet : public object
{
public:
    int Bulletid;
    float MaxLife;
    float RemainingLife;
    int Damage;

    void Initialize(float BulletWidth, float BulletHeight, int Inputid, float LifeTime);
    void Activate(point InitPos, float InputdX, float InputdY);
    void Move(float dt);
    bool CollideCheck(object *Target);
};

#define BULLET_H
#endif