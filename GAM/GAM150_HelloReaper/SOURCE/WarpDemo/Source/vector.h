/******************************************************************************
FileName: vector.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef VECTOR_H

#include "hreaper_general.h"
#include "math.h"

point UnitVectorCalculation(point &Actor, point &Target)
{
    point Result = {0.0f, 0.0f};
    float Distance = (float)sqrt((Target.X - Actor.X) * (Target.X - Actor.X) + 
                                (Target.Y - Actor.Y) * (Target.Y - Actor.Y));
    if(Distance >1.0f)
    {
        Result = {(Target.X - Actor.X) / Distance, 
                    (Target.Y - Actor.Y) / Distance};
    }
    return Result;
}

#define VECTOR_H
#endif