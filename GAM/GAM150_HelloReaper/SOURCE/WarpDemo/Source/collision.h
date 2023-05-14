/******************************************************************************
FileName: collision.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef COLLISION_H
#include "hreaper_general.h"

//Rectangle to Rectangle//
static
bool CollisionRR(object *TargetA, object *TargetB)
{
    float LeftA = TargetA->Pos.X - 0.5f * TargetA->Width;
    float RightA = TargetA->Pos.X + 0.5f * TargetA->Width;
    float TopA = TargetA->Pos.Y + 0.5f * TargetA->Height;
    float BottomA = TargetA->Pos.Y - 0.5f * TargetA->Height;

    float LeftB = TargetB->Pos.X - 0.5f * TargetB->Width;
    float RightB = TargetB->Pos.X + 0.5f * TargetB->Width;
    float TopB = TargetB->Pos.Y + 0.5f * TargetB->Height;
    float BottomB = TargetB->Pos.Y - 0.5f * TargetB->Height;

    if(LeftA > RightB || LeftB > RightA || TopA < BottomB || TopB < BottomA)
        return false;
    else
        return true;
}

//(g.lee)Basic Raycast function.
//It does it's job certainly well, but not that efficeint - or maybe not.
//I really don't know!
static
bool RayCastR(const point &Start, const point &Vector,
              object &Target)
{
    float Left = Target.Pos.X - Target.Width;
    float Right = Target.Pos.X + Target.Width;
    float Top = Target.Pos.Y + Target.Height;
    float Bottom = Target.Pos.Y - Target.Height;
    
    if(((Vector.Y * (Left - Start.X) - Vector.X * (Top - Start.Y) < 0) &&
        (Vector.Y * (Left - Start.X) - Vector.X * (Bottom - Start.Y) < 0) &&
        (Vector.Y * (Right - Start.X) - Vector.X * (Top - Start.Y) < 0) &&
        (Vector.Y * (Right - Start.X) - Vector.X * (Bottom - Start.Y) < 0)) ||
       ((Vector.Y * (Left - Start.X) - Vector.X * (Top - Start.Y) > 0) &&
        (Vector.Y * (Left - Start.X) - Vector.X * (Bottom - Start.Y) > 0) &&
        (Vector.Y * (Right - Start.X) - Vector.X * (Top - Start.Y) > 0) &&
        (Vector.Y * (Right - Start.X) - Vector.X * (Bottom - Start.Y) > 0)))
    {
        return false;
    }
    else
	{
		if(Vector.X >= 0.0f && Vector.Y >= 0.0f)
		{
			if(Start.Y >= Top || Start.X >= Right)
				return false;
		}
		else if(Vector.X >= 0.0f && Vector.Y < 0.0f)
		{
			if(Start.Y < Bottom || Start.X >= Right)
				return false;
		}
		else if(Vector.X < 0.0f && Vector.Y >= 0.0f)
		{
			if(Start.Y >= Top || Start.X < Left)
				return false;
		}
		else
		{
			if(Start.Y < Bottom || Start.X < Left)
				return false;
		}
        return true;
	}
}

#define COLLISION_H
#endif

//Circle to Circle//
/*
bool32 CollisionCC(object *TargetA, object *TargetB)
{
    bool32 Result = ((TargetB->Pos.X - TargetA->Pos.X) * (TargetB->Pos.X - TargetA->Pos.X) + 
                    (TargetB->Pos.Y - TargetA->Pos.Y) * (TargetB->Pos.Y - TargetA->Pos.Y)) <= 
                    ((TargetA->Rad + TargetB->Rad) * (TargetA->Rad + TargetB->Rad));
    return Result;
}
*/