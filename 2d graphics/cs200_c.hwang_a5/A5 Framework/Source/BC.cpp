/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: <put file name here>
Purpose: <explain the contents of this file>
Language: <specify language and compiler>
Platform: <specify compiler version, hardware requirements, operating systems>
Project: <specify student login, class, and assignment. For example: if foo
is in class CS 200 and this
 file is part of assignment 3, then write: foo_CS200_3>
Author: <provide your name, student login, and student id>
Creation date: <date on which you created this file>
- End Header ----------------------------------------------------------------*/

// Includes
#include "BC.hpp"
#include "Color.hpp"
#include "AABB.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Construct a bounding circle for pillar modeled by numPoints number
			of points stored in pPos[]
	@param	const Point2D* pPos
			Pointer to first element of array of elements each of type Point2D
	@param	int numPoints
			Number of elements in array of points
*/
////////////////////////////////////////////////////////////////////////////////
void BC::Compute(const Point2D* pPos, int numPoints)
{
	// @TODO: Compute bounding circle with tightest possible fit
	Point2D mMaxPos = pPos[0];
	Point2D mMinPos = pPos[0];

	for (int i = 1; i < numPoints; i++)
	{
		if (mMaxPos.GetX() < pPos[i].GetX())
		{
			mMaxPos.SetX(pPos[i].GetX());
		}
		else if (mMinPos.GetX() > pPos[i].GetX())
		{
			mMinPos.SetX(pPos[i].GetX());
		}

		if (mMaxPos.GetY() < pPos[i].GetY())
		{
			mMaxPos.SetY(pPos[i].GetY());
		}
		else if (mMinPos.GetY() > pPos[i].GetY())
		{
			mMinPos.SetY(pPos[i].GetY());
		}
	}

	mCenPos = (mMinPos + mMaxPos) / 2.f;
	
	mRadius = pPos[0].Distance(mCenPos);
	for (int i = 1; i < numPoints; i++)
	{
		if (mRadius * mRadius < pPos[i].DistanceSq(mCenPos))
			mRadius = pPos[i].Distance(mCenPos);
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Displace bounding circle to new location.
	@param	const Vector2D& disp
			Displacement vector
	@return	None
*/
////////////////////////////////////////////////////////////////////////////////
void BC::Move(const Vector2D& vec)
{
	// @TODO: Update bounding circle so that its center is displaced
	// by vector disp
	mCenPos += vec;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Point vs. BC test
	@param	const Point2D& p
			Point to be tested
	@return	bool
			True if point is intersecting with bounding circle; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool BC::TestBCVsPoint(const Point2D& p) const
{
	// @TODO: Return true if point p inside or on bounding circle;
	// false otherwise
	if ((mCenPos - p) * (mCenPos - p) <= mRadius * mRadius)
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  BC vs. BC test
	@param	const BC& bc
			BC to test against
	@return	bool
			Return true if BC's intersect; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool BC::TestBCvsBC(const BC& bc) const
{
	// @TODO: Return true if bounding circle's overlap; false otherwise
	if ((mCenPos - bc.mCenPos) * (mCenPos - bc.mCenPos) <= ((bc.mRadius + mRadius) * (bc.mRadius + mRadius)))
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  BC vs. AABB test

	@param	const BC& bc
			BC to be tested against AABB
	@param	const AABB& aabb
			AABB to be tested against BC bc
	@return	bool
			True if BC is intersecting with AABB; false otherwise
	@Notes	This a friend function to classes CS200::BC and CS200::AABB
*/
////////////////////////////////////////////////////////////////////////////////
bool TestBCVsAABB(const BC& bc, const AABB& aabb)
{
	// @TODO: Return true if BC bc is intersecting with AABB aabb;
	// otherwise return false.
	//first step
	if (bc.mCenPos.GetX() >= aabb.mMinPos.GetX() &&
		bc.mCenPos.GetX() <= aabb.mMaxPos.GetX() &&
		bc.mCenPos.GetY() >= aabb.mMinPos.GetY() &&
		bc.mCenPos.GetY() <= aabb.mMaxPos.GetY())
	{
		return true;
	}
	else
	{
		float closestX = bc.mCenPos.GetX();
		float closestY = bc.mCenPos.GetY();
		//second step
		if (bc.mCenPos.GetX() < aabb.mMinPos.GetX())
		{
			closestX = aabb.mMinPos.GetX();
		}
		else if (bc.mCenPos.GetX() > aabb.mMaxPos.GetX()) {
			closestX = aabb.mMaxPos.GetX();
		}
		if (bc.mCenPos.GetY() < aabb.mMinPos.GetY()) {
			closestY = aabb.mMinPos.GetY();
		}
		else if (bc.mCenPos.GetY() > aabb.mMaxPos.GetY()) {
			closestY = aabb.mMaxPos.GetY();
		}
		float distanceX = bc.mCenPos.GetX() - closestX;
		float distanceY = bc.mCenPos.GetY() - closestY;
		float distancesquaded = distanceX * distanceX + distanceY * distanceY;

		return (bc.mRadius * bc.mRadius > distancesquaded);
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Draws boundary of bounding circle
	@param	window
			Window to draw to
	@return	None
	@Use	Use the following data members of class BC
			mCenPos: position of BC center 
			mRadius: BC radius
*/
////////////////////////////////////////////////////////////////////////////////
void BC::Draw(const Window& window) const
{
	static Color redClr(255, 0, 0, 255);

	// @TODO: Draw boundary (not a filled circle) of BC in red color
	// @TODO: Scan-convert circle using 8-way symmetry in conjunction
	// with any one of the various circle rasterizers described in class
	int x_point = 0;
	int y_point = static_cast<int>(mRadius);
	int d = 1 - static_cast<int>(mRadius);
	int deltaE = 2 * x_point + 3;
	int deltaSE = -2 * static_cast<int>(mRadius) + 5;
	window.SetPixel(static_cast<int>(mCenPos.GetX()) + x_point, static_cast<int>(mCenPos.GetY()) + y_point, redClr);

	while (y_point > x_point)
	{
		if (d < 0)
		{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
		}
		else
		{
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;
			y_point--;
		}
		x_point++;
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + x_point, static_cast<int>(mCenPos.GetY()) + y_point, redClr);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - x_point, static_cast<int>(mCenPos.GetY()) + y_point, redClr);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + x_point, static_cast<int>(mCenPos.GetY()) - y_point, redClr);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - x_point, static_cast<int>(mCenPos.GetY()) - y_point, redClr);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + y_point, static_cast<int>(mCenPos.GetY()) + x_point, redClr);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - y_point, static_cast<int>(mCenPos.GetY()) + x_point, redClr);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + y_point, static_cast<int>(mCenPos.GetY()) - x_point, redClr);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - y_point, static_cast<int>(mCenPos.GetY()) - x_point, redClr);
	}
}

}	// namespace CS200
