/******************************************************************************
FileName: bullet.cpp
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#include "bullet.h"
#include "collision.h"

//For the values that are not going to be changed.//
void bullet::Initialize(float BulletWidth, float BulletHeight, int Inputid, float LifeTime)
{
    Width = BulletWidth;
    Height = BulletHeight;
    Pos = {0.0f, 0.0f};
    id = 0;
    Bulletid = Inputid;
    MaxLife = LifeTime;

    Visible = false;
}

//For the values that are going to be changed//
void bullet::Activate(point InitPos, 
                      float InputdX, float InputdY)
{
    Pos = InitPos;
    Visible = true;
    dX = InputdX;
    dY = InputdY;
    RemainingLife = MaxLife;
}

void bullet::Move(float dt)
{
    if(Visible)
    {
        Pos.X += (1.0f / MaxLife) * dX * dt;
        Pos.Y += (1.0f / MaxLife) * dY * dt;
        RemainingLife -= dt;
        if(RemainingLife <= 0)
        {
            Visible = false;
        }
    }
}

bool bullet::CollideCheck(object *Target)
{
    bool Collided = false;
    if(Visible)
    {
        int idCheck = Target->id * Bulletid;

        if(idCheck < 0 && Target->Visible && !Target->Invincible)
        {
            if(CollisionRR(this, Target))
            {
                Target->Health -= 1;
                Collided = true;
                Visible = false;       
            }
            else
            {
                //logging;
            }
        }
    }
        
    return Collided;
}