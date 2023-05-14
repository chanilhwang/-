/******************************************************************************
FileName: hreaper_intrinsics.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef HREAPER_INTRINSICS_H

#include "hreaper_general.h"
#include <cmath>

inline uint
SafeTruncateUInt(uint Value)
{
    Assert(Value <= 0xFFFFFFFF);
    uint Result = (uint)Value;
    return(Result);
}

inline int
RoundFloatToInt(float Real32)
{
    int Result = (int)(Real32 + 0.5f);
    return(Result);
}

inline uint
RoundFloatToUInt(float Real32)
{
    uint Result = (uint)(Real32 + 0.5f);
    return(Result);
}

inline unsigned char
RoundFloatToUChar(float Real32)
{
    unsigned char Result = (unsigned char)(Real32 + 0.5f);
    return(Result);
}

inline int
TruncateFloatToInt(float Real32)
{
    int Result = (int)Real32;
    return(Result);
}

#define HREAPER_INTRINSICS_H
#endif