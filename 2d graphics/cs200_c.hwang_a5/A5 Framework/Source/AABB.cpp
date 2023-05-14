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
#include "AABB.hpp"
#include "Line2D.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Construct a 2d axis-aligned bounding box for a pillar with a set
			of points pPos[numPoints].
	@param	const Point2D* pPos
			Pointer to first element of array of elements each of type Point2D
	@param	int numPoints
			Number of elements in array of points
*/
////////////////////////////////////////////////////////////////////////////////
void AABB::Compute(const Point2D* pPos, int numPoints)
{
	// @TODO: Compute top-left (minimum), bottom-right (maximum)
	// corners, and center of axis-aligned bounding box.
	mMaxPos = pPos[0];
	mMinPos = pPos[0];

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
		else if(mMinPos.GetY() > pPos[i].GetY())
		{
			mMinPos.SetY(pPos[i].GetY());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Displace axis-aligned bounding box by vector disp to new location.
	@param	const Vector2D& vec
			Displacement vector
	@return	None
*/
////////////////////////////////////////////////////////////////////////////////
void AABB::Move(const Vector2D& vec)
{
	// @TODO: Update top-left, bottom-right, and center positions
	// of axis-aligned bounding box
	mCenPos += vec;
	mMinPos += vec;
	mMaxPos += vec;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  2d AABB vs. point test.
	@param	const Point2D& p
			Point to be tested
	@return	bool
			True if point is intersecting with aabb; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool AABB::TestAABBVsPoint(const Point2D& p)
{
	// @TODO: Return true if point p inside or on AABB;
	// false otherwise	
	if (p.GetX() <= mMinPos.GetX() &&
		p.GetX() >= mMaxPos.GetX() &&
		p.GetY() <= mMinPos.GetY() &&
		p.GetY() >= mMaxPos.GetY())
	{
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  2d AABB vs. 2d AABB test
	@param	const AABB& aabb
			AABB to test against
	@return	bool
			Return true if AABB's intersect; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool AABB::TestAABBVsAABB(const AABB& aabb)
{
	// @TODO: Return true if AABB's overlap along both X- and
	// Y-axes; false otherwise
	if ((mMaxPos.GetX() < aabb.mMinPos.GetX()) ||
		(aabb.mMaxPos.GetX() < mMinPos.GetX()) ||
		(mMaxPos.GetY() < aabb.mMinPos.GetY()) ||
		(aabb.mMaxPos.GetY() < mMinPos.GetY()))
	{
		return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Draws boundary of axis-aligned bounding box
	@param	None
	@return	None
*/
////////////////////////////////////////////////////////////////////////////////
void AABB::Draw(const Window& window) const
{
	static Color sRedClr(255, 0, 0, 255);

	// @TODO: Draw boundary of axis-aligned bounding box in red color
	int leftx = static_cast<int>(mMinPos.GetX());
	int rightx = static_cast<int>(mMaxPos.GetX());
	int topy = static_cast<int>(mMaxPos.GetY());
	int bottomy = static_cast<int>(mMinPos.GetY());

	for (int x = static_cast<int>(mMinPos.GetX()); x <= static_cast<int>(mMaxPos.GetX()); ++x)
	{
		window.SetPixel(x, topy, sRedClr);
		window.SetPixel(x, bottomy, sRedClr);
	}
	for (int y = static_cast<int>(mMinPos.GetY()); y <= static_cast<int>(mMaxPos.GetY()); ++y)
	{
		window.SetPixel(leftx, y, sRedClr);
		window.SetPixel(rightx, y, sRedClr);
	}
}

}	// namespace CS200
