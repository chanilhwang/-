/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: CircularPillar.cpp
Purpose: draw, move, and test to collide with circular pillar
Language: c++
Platform: window10 64bit
Project: c.hwang_cs200_a5
Author: c.hwang
Creation date: <date on which you created this file>
- End Header ----------------------------------------------------------------*/

// Includes
#include "CircularPillar.hpp"
#include "PolygonalPillar.hpp"
#include "Point2D.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Renders a filled shape of type CircularPillar to input window
			using 8-way symmetry in conjunction with any one of the algorithms
			described in class.
	@param	window
			Window to draw to
	@Use	Use the following data members of class CircularPillar
			mCenPos: position of circular pillar center 
			mRadius: circular pillar radius
			mColor: constant color to fill circular pillar with

			Here's how you set a pixel: window.SetPixel(x, y, color);
*/
////////////////////////////////////////////////////////////////////////////////
void CircularPillar::Draw(const Window& window) const
{
	// @TODO: Scan-convert circle using 8-way symmetry in conjunction
	// with any one of the various circle rasterizers described in class
	int x_point = 0;
	int y_point = mRadius;
	int d = 1 - mRadius;
	int deltaE = 2 * x_point + 3;
	int deltaSE = -2 * mRadius + 5;
	
	for (int y = 0; y < y_point; y++)
	{
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + x_point, static_cast<int>(mCenPos.GetY()) + y, mColor);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - x_point, static_cast<int>(mCenPos.GetY()) + y, mColor);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + x_point, static_cast<int>(mCenPos.GetY()) - y, mColor);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - x_point, static_cast<int>(mCenPos.GetY()) - y, mColor);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + y, static_cast<int>(mCenPos.GetY()) + x_point, mColor);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - y, static_cast<int>(mCenPos.GetY()) + x_point, mColor);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) + y, static_cast<int>(mCenPos.GetY()) - x_point, mColor);
		window.SetPixel(static_cast<int>(mCenPos.GetX()) - y, static_cast<int>(mCenPos.GetY()) - x_point, mColor);
	}

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
		for (int y = 0; y < y_point; y++)
		{
			window.SetPixel(static_cast<int>(mCenPos.GetX()) + x_point, static_cast<int>(mCenPos.GetY()) + y, mColor);
			window.SetPixel(static_cast<int>(mCenPos.GetX()) - x_point, static_cast<int>(mCenPos.GetY()) + y, mColor);
			window.SetPixel(static_cast<int>(mCenPos.GetX()) + x_point, static_cast<int>(mCenPos.GetY()) - y, mColor);
			window.SetPixel(static_cast<int>(mCenPos.GetX()) - x_point, static_cast<int>(mCenPos.GetY()) - y, mColor);
			window.SetPixel(static_cast<int>(mCenPos.GetX()) + y, static_cast<int>(mCenPos.GetY()) + x_point, mColor);
			window.SetPixel(static_cast<int>(mCenPos.GetX()) - y, static_cast<int>(mCenPos.GetY()) + x_point, mColor);
			window.SetPixel(static_cast<int>(mCenPos.GetX()) + y, static_cast<int>(mCenPos.GetY()) - x_point, mColor);
			window.SetPixel(static_cast<int>(mCenPos.GetX()) - y, static_cast<int>(mCenPos.GetY()) - x_point, mColor);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief  Displace circular pillar to new location.
	@param	const Vector2D& vec
			Displacement vector
	@return	None
*/
////////////////////////////////////////////////////////////////////////////////
void CircularPillar::Move(const Vector2D& vec)
{
	// @TODO: Update geometric center, axis-aligned bounding box, and
	// bounding circle of circular pillar shape ...
	mCenPos += vec;
	mBC.Move(vec);
	mAABB.Move(vec);
}

////////////////////////////////////////////////////////////////////////////////
/*!
	@brief  Intersection test between an arbitrary point and CircularPillar shape.
	@param	const Point2D& pt
			Arbitrary point
	@return	true if point inside, that is, either in interior or on circular
			pillar boundary; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool CircularPillar::TestPillarVsPoint(const Point2D& pt) const
{
	// @TODO: Return true if point pt in interior or boundary of circular pillar;
	// false otherwise

	if((mCenPos - pt) * (mCenPos - pt) <= mRadius * mRadius)
	{
		return true;
	}
	return false;
}

/***************** NOTHING MORE TO IMPLEMENT BELOW ****************************/

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Auxiliary CircularPillar constructor using center and radius
	@param	const Point2D&	center
			shape center point - also, circular pillar center
	@param	int				radius
			Radius of circular pillar
	@param	const Color&	clr
			Solid color used to scan-convert circular pillar
*/
////////////////////////////////////////////////////////////////////////////////
CircularPillar::CircularPillar(	const Point2D& center, int radius, const Color& clr)
	: mCenPos(center), mRadius(radius), mColor(clr) 
{
	// build BC and AABB
	float r = static_cast<float>(radius);
	Point2D bPos[4];
	bPos[0].SetPosition(center.GetX()-r, center.GetY());
	bPos[1].SetPosition(center.GetX(), center.GetY()+r);
	bPos[2].SetPosition(center.GetX()+r, center.GetY());
	bPos[3].SetPosition(center.GetX(), center.GetY()-r);

	mAABB.Compute(bPos, 4);
	mBC.Compute(bPos, 4);
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Auxiliary CircularPillar constructor using center and point offset
			from center. Radius is distance between center and offset points.
	@param	const Point2D&	center
			shape center point - also, circular pillar center
	@param	const Point2D&	offset
			Coordinates of point on boundary of circular pillar
	@param	const Color&	clr
			Solid color for scan-converting circular pillar
*/
////////////////////////////////////////////////////////////////////////////////
CircularPillar::CircularPillar(	const Point2D& center, const Point2D& offset, const Color& clr)
	: mCenPos(center), mColor(clr)
{
	// build BC and AABB
	float r	= offset.Distance(center);
	mRadius	= static_cast<int>(r + 0.5f);

	Point2D bPos[4];
	bPos[0].SetPosition(center.GetX()-r, center.GetY());
	bPos[1].SetPosition(center.GetX(), center.GetY()+r);
	bPos[2].SetPosition(center.GetX()+r, center.GetY());
	bPos[3].SetPosition(center.GetX(), center.GetY()-r);

	mAABB.Compute(bPos, 4);
	mBC.Compute(bPos, 4);
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Since this function is a member of class CircularPillar, only
			the BC is used in intersection tests. A series of hierarchical
			intersection tests are performed to test for intersection between
			BC and other pillar objects. Note that in this application, there
			are two types of pillars: circular and polygonal. Also note that
			each pillar type uses both a BC and an AABB.
			The series of hierarchical tests are performed as follows:
			First, BC vs. BC test is performed. If it fails, there is no
			intersection.
			If the pillar pointed to argument is also a circular pillar, 
			intersection tests are concluded.
			However, if the pillar is polygonal, then BC vs. AABB test is
			performed. This is further followed by BC vs. polygon edge tests.
	@param	const Pillar* ps
			Run-time type identification allows ps to point to either a
			CircularPillar or PolygonalPillar object
	@return	bool
			True if circular pillar pointed to by the this pointer
			is intersecting with pillar pointed to by ps; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool CircularPillar::TestPillar(const Pillar* ps) const
{
	// CP == Circular pillar
	// PP == Polygonal pillar
	// Note that *this is CP while *ps may be either CP or PP ...

	// Test 1: this->BC vs. ps->BC
	// Return false for non-intersection
	BC bc = this->GetBC();
	if (bc.TestBCvsBC(ps->GetBC()) == false) return false;

	// If *ps is CP, we are done (since *this is a CP)
	// Return true for intersection
	if (const CircularPillar *pcp = dynamic_cast<const CircularPillar *>(ps))
		return true;

	// Note that *ps can now only be a PP ...

	// Test 2: this->AABB vs. ps->AABB
	// Return false for non-intersection
	AABB aabb = this->GetAABB();
	if (aabb.TestAABBVsAABB(ps->GetAABB()) == false) return false;

	// Test 3: test CP for intersection with PP's edges
	// Return true if CP intersects with PP; false otherwise
	return (ps->TestPillarVsBC(bc));
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded output operator <<
	@param	std::ostream& os
			Reference to an ostream object
	@param	const CircularPillar& p
			Reference to Color type
	@return	object of type ostream reference with value equivalent to the 
			ostream object against which the output operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const CircularPillar& cp)
{
	os << cp.mCenPos;
	os << cp.mRadius << "\n";
	os << cp.mColor;
	return os;
}

}	// namespace CS200