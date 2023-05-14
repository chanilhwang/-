/******************************************************************************
FileName: hreaper_general.h
Project: Hello Reaper
Author: Gyuhyeon, Lee (g.lee)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef HREAPER_GENERAL_H

//Disable useless warnings
#pragma warning (disable : 4100)
#pragma warning (disable : 4505)
#pragma warning (disable : 4244)
#pragma warning (disable : 4239)

//typedefine
#define internal static 
#define local_persist static 
#define global_variable static
typedef unsigned int uint;
#if 1
#define Assert(Expression) if(!(Expression)) {*(int *)0 = 0;}
#else
#define Assert(Expression)
#endif

class color
{
public:
    float Red;
    float Green;
    float Blue;
    float Alpha;
};

class point
{
public:
	float X;
	float Y;
	
	point operator +(const point &ref)
	{
		point Result = {X + ref.X, Y + ref.Y};

		return Result;
	}
	point operator -(const point &ref)
	{
		point Result = {X - ref.X, Y - ref.Y};

		return Result;
	}
};

class point_int
{
public:
	int X;
	int Y;
};

class object
{
public:
	point Pos;
	
	float Width;
	float Height;
	bool Visible;

	float dX;
	float dY;

	int id;
	int Health;
	bool Invincible;

	int CurrentTxt;
};

#define HREAPER_GENERAL_H
#endif HREAPER_GENERAL_H