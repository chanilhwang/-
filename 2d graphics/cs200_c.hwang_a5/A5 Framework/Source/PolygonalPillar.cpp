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
#include "PolygonalPillar.hpp"
#include "CircularPillar.hpp"
#include "Triangle.hpp"
#include <cmath>

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Renders PolygonalPillar object to input window
	@param	const Window& window
			Window to render to
	@Notes	1. Discard non-convex polygons.
			2. Render convex polygon by implicitly splitting it into triangles.
*/
////////////////////////////////////////////////////////////////////////////////
void PolygonalPillar::Draw(const Window& window) const
{
	// @TODO: Scan-convert counter-clockwise oriented convex polygon ...
	for (int i = 0; i < mVertexCount - 2; i++)
	{
		Triangle tri(mPtrVertexArray[0], mPtrVertexArray[i + 1], mPtrVertexArray[i + 2]);
		tri.Draw(window);
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Determines if counter-clockwise ordered set of n vertices
			define a convex n-gon.
	@param	none
	@return	true if counter-clockwise ordered vertex set defines a convex n-gon;
			false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool PolygonalPillar::IsConvex() const
{
	// @TODO: Return true if polygon is convex, false otherwise
	for (int i = 0; i < mVertexCount; ++i)
	{
		Point2D vertex1 = mPtrVertexArray[i % mVertexCount].GetPosition();
		Point2D vertex2 = mPtrVertexArray[(i + 1) % mVertexCount].GetPosition();
		Point2D vertex3 = mPtrVertexArray[(i + 2) % mVertexCount].GetPosition();

		float dx1 = vertex2.GetX() - vertex1.GetX();
		float dy1 = vertex2.GetY() - vertex1.GetY();
		float dx2 = vertex3.GetX() - vertex2.GetX();
		float dy2 = vertex3.GetY() - vertex2.GetY();

		if (dx1 * dy2 - dy1 * dx2 > 0)
			return false;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Displace convex polygon to new location.
	@param	const Vector2D& vec
			Displacement vector
	@return	None
*/
////////////////////////////////////////////////////////////////////////////////
void PolygonalPillar::Move(const Vector2D& vec)
{
	// @TODO:
	// Translate polygon by displacement vector disp
	// Update position coordinates, geometric center, edge data,
	// bounding circle and axis-aligned bounding box of shape ...
	for (int i = 0; i < mVertexCount; i++)
	{
		mPtrVertexArray[i].SetPosition(mPtrVertexArray[i].GetPosition() + vec);
		mPtrEdgeArray[i].SetEndPoint(mPtrEdgeArray[i].GetEndPoint() + vec);
		mPtrEdgeArray[i].SetStartPoint(mPtrEdgeArray[i].GetStartPoint() + vec);
	}
	mBC.Move(vec);
	mAABB.Move(vec);
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Intersection test between an arbitrary point and counter-clockwise
			oriented convex polygon.
	@param	const Point2D& pt
			Arbitrary point
	@return	true if point is inside, that is, either in the interior or on
			the convex polygon boundary; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool PolygonalPillar::TestPillarVsPoint(const Point2D& pt) const 
{
	// @TODO: Return true if point pt is in counter-clockwise oriented polygon;
	// false otherwise
	for (int i = 0; i < mVertexCount; ++i)
	{
		Vector2D starttopoint = pt - mPtrEdgeArray[i].GetStartPoint();

		if (mPtrEdgeArray[i].GetNml().GetX() * starttopoint.GetX() + mPtrEdgeArray[i].GetNml().GetY() * starttopoint.GetY() < 0)
		{
			return false;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  BC vs. convex polygon pillar (PP) test
			The test proceeds in this manner:
			for each edge E of PP
				if BC contains either start or end point of E, there is intersection
				otherwise if BC is in outside half-plane of E, there is no intersection
				otherwise if BC is in inside half-plane of E, continue to next edge
				otherwise if BC is intersected by (finite) E, there is intersection
			if BC is in inside half-plane of all edges of PP, there is intersection
			otherwise, there is no intersection

			Or see page 7 from lecture notes
	@param	const AABB& aabb
			AABB to be tested against BC bc
	@param	const BC& bc
			BC to be tested against AABB
	@return	bool
			True if BC is intersecting with AABB; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
float dotProduct(Vector2D v1, Vector2D v2)
{
	return v1.GetX() * v2.GetX() + v1.GetY() * v2.GetY();
}

float GetShortestDistance(const Edge2D& edge, const Point2D& pt)
{
	Vector2D starttopoint = pt - edge.GetStartPoint();
	Vector2D starttoend = edge.GetEndPoint() - edge.GetStartPoint();

	float determinant = dotProduct(starttopoint, starttoend);
	if(determinant < 0)
	{
		return dotProduct(starttopoint, starttopoint);
	}
	else if (determinant > dotProduct(starttoend, starttoend))
	{
		Vector2D endtopoint = pt - edge.GetEndPoint();
		return dotProduct(endtopoint, endtopoint);
	}
	else
	{
		float s = dotProduct(starttopoint, starttoend);
		float segment = dotProduct(starttoend, starttoend);
		float k = dotProduct(starttopoint, starttopoint);
		return dotProduct(starttopoint, starttopoint) - ((s*s) / (segment));
	}
}

bool PolygonalPillar::TestPillarVsBC(const BC& bc) const
{
	// @TODO: BC vs. convex polygon test.
	// See notes in function header for additional information
	if (TestPillarVsPoint(bc.mCenPos))
	{
		return true;
	}
	else
	{
		float shortestdistance = GetShortestDistance(mPtrEdgeArray[0], bc.mCenPos);
		for (int i = 1; i < mVertexCount; i++)
		{
			float value = GetShortestDistance(mPtrEdgeArray[i], bc.mCenPos);
			if (shortestdistance > value)
			{
				shortestdistance = value;
			}
		}
		if (shortestdistance < bc.mRadius * bc.mRadius)
		{	
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Convex polygonal pillar (PP) vs. convex polygonal pillar test.
			Simplest manner to implement this test is to use the
			Separation Axis Theorem (SAT).
			For pictures and information about the SAT method, see pages 14-23
			from lecture notes.
	@param	const PolygonalPillar& pp
			PolygonalPillar object to test against
	@return	bool
			True if the polygonal pillars are intersecting; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
void ComputeProjInterval(const PolygonalPillar& polygon, const Vector2D& normal, float& min, float &max)
{
	min = max = dotProduct(polygon.GetVertexes()[0].GetPosition(), normal);
	for (int i = 1; i < polygon.GetVertexCount(); i++)
	{
		float value = dotProduct(polygon.GetVertexes()[i].GetPosition(), normal);
		if (value < min)
		{
			min = value;
		}
		else if (value > max)
		{
			max = value;
		}
	}
}

bool PolygonalPillar::TestPillarVsPP(const PolygonalPillar& rpp) const
{
	// @TODO: Intersection test between two polygonal pillars using the
	// Separation Axis Theorem
	float min0;
	float max0;
	float min1;
	float max1;

	//rpp
	for (int i = 0; i < mVertexCount; i++)
	{
		Vector2D edgenormal = mPtrEdgeArray[i].GetNml();
		ComputeProjInterval(rpp, edgenormal, min0, max0);
		ComputeProjInterval(*this, edgenormal, min1, max1);
		if (max0 < min1 || max1 < min0)
		{
			return false;
		}
	}

	//my poloygon
	for (int i = 0; i < rpp.mVertexCount; i++)
	{
		Vector2D edgenormal = rpp.mPtrEdgeArray[i].GetNml();
		ComputeProjInterval(rpp, rpp.mPtrEdgeArray[i].GetNml(), min0, max0);
		ComputeProjInterval(*this, rpp.mPtrEdgeArray[i].GetNml(), min1, max1);
		if (max0 < min1 || max1 < min0)
		{
			return false;
		}
	}

	return true;
}

/***************** NOTHING MORE TO IMPLEMENT BELOW ****************************/

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Since this function is a member of class PolygonalPillar, the
			BC, AABB, and individual edges of the convex polygonal pillar (PP)
			are used in the following hierarchical intersection tests. Note
			that in this application, there are two types of pillars: circular
			and polygonal. Also note that each pillar type uses both a BC and an
			AABB. The hierarchical tests are performed as follows: First,
			BC vs. BC test is performed. If it fails, there is no intersection.
			If the pillar pointed to by parameter ps is a circular pillar (CP),
			AABB (of PP) vs. BC (of CP) test is performed. If there
			is no intersection, testing concludes and we are done. However, 
			if there is an intersection, the individual edges of PP are tested
			against BC of CP.
			On the other hand, if the pillar pointed to by parameter ps
			is also a PP, AABB vs. AABB test is performed.
			If there is an intersection, the more refined Separation Axis Theorem
			test is performed between the two PPs.
	@param	const Pillar *ps
			Run-time type identification allows ps to point to either a
			CircularPillar or PolygonalPillar object
	@return	bool
			True if polygonal pillar pointed to by the this pointer
			is intersecting with pillar pointed to by argument ps; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool PolygonalPillar::TestPillar(const Pillar* ps) const
{
	// @TODO: Hierarchical intersection tests between polygonal pillar 
	// and other circular and polygonal pillars in the following order:
	// 1. this->BC vs. ps->BC
	// this->BC refers to the BC of object invoking this virtual function
	// which we know is a polygonal pillar.
	// ps->BC refers to the BC of the object pointed to by parameter ps
	// may be pointing to either a circular or polygonal pillar. The correct
	// object is resolved using runtime type identification by C++
	// If this test fails, we are done - there is no collision
	// otherwise, there may be a collision
	BC	bc	=	this->GetBC();
	if (bc.TestBCvsBC(ps->GetBC()) == false)
		return false;

	// 2. Next, perform this->BC vs. ps->AABB test
	if (CS200::TestBCVsAABB(this->GetBC(), ps->GetAABB()) == false)
		return false;

	// 3. If test 2 fails, again there may or may not be an intersection.
	// Perform this->AABB vs. ps->AABB test
	AABB	aabb	=	this->GetAABB();
	if (aabb.TestAABBVsAABB(ps->GetAABB()) == false)
		return false;

	// 4. If test 3 fails, perform this->polygon vs. ps->BC test
	// only if the object is a circular pillar
	if (const CircularPillar *pcp = dynamic_cast<const CircularPillar *>(ps))
		return TestPillarVsBC(pcp->GetBC());

	// 5. If test 4 fails, perform this->pillar vs. ps->pillar test
	if (const PolygonalPillar *ppp = dynamic_cast<const PolygonalPillar *>(ps))
		return ppp->TestPillarVsPP(*this);

	return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  PolygonalPillar copy constructor
	@param	const PolygonalPillar& rhs
			Input PolygonalPillar
*/
////////////////////////////////////////////////////////////////////////////////
PolygonalPillar::PolygonalPillar(const PolygonalPillar& rhs)
{
	if(this != &rhs)
	{
		mAABB	=	rhs.mAABB;
		mBC		=	rhs.mBC;

		mVertexCount	=	rhs.mVertexCount;
		mPtrVertexArray	=	new Vertex2D [mVertexCount];
		mPtrEdgeArray	=	new Edge2D [mVertexCount];
		for(int i = 0; i < mVertexCount; ++i)
		{
            mPtrVertexArray[i]	=	rhs.mPtrVertexArray[i];
			mPtrEdgeArray[i]	=	rhs.mPtrEdgeArray[i];
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Auxiliary PolygonalPillar constructor
	@param	const CS200::Vertex2D* pVtx
			Pointer to first element of array of counter-clockwise oriented
			convex polygon vertices
	@param	int numVtx
			Total number of vertices in array from first argument
*/
////////////////////////////////////////////////////////////////////////////////
PolygonalPillar::PolygonalPillar(const CS200::Vertex2D* pVtx, int numVtx)
{
	mVertexCount	=	numVtx;
	mPtrVertexArray	=	new Vertex2D [mVertexCount];
	mPtrEdgeArray	=	new Edge2D [mVertexCount];
	Point2D	*pPos	=	new Point2D [mVertexCount];
	for (int i = 0; i < mVertexCount; ++i)
	{
		mPtrVertexArray[i]	=	pVtx[i];

		mPtrEdgeArray[i].SetStartPoint(pVtx[i].GetPosition());
		mPtrEdgeArray[i].SetEndPoint(pVtx[(i+1)%mVertexCount].GetPosition());
		mPtrEdgeArray[i].ComputePointNormalEquation();

		pPos[i]		=	pVtx[i].GetPosition();
	}

	mAABB.Compute(pPos, mVertexCount);
	mBC.Compute(pPos, mVertexCount);

	delete [] pPos;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  PolygonalPillar assignment operator
	@param	const PolygonalPillar& rhs
			Input PolygonalPillar
*/
////////////////////////////////////////////////////////////////////////////////
PolygonalPillar& PolygonalPillar::operator=(const PolygonalPillar& rhs)
{
	if(this != &rhs)
	{
		mAABB	=	rhs.mAABB;
		mBC		=	rhs.mBC;

		mVertexCount	=	rhs.mVertexCount;

		delete [] mPtrVertexArray;
		delete [] mPtrEdgeArray;
		mPtrVertexArray	=	new Vertex2D [mVertexCount];
		mPtrEdgeArray	=	new Edge2D [mVertexCount];
		for(int i = 0; i < mVertexCount; ++i)
		{
			mPtrVertexArray[i]	=	rhs.mPtrVertexArray[i];
			mPtrEdgeArray[i]	=	rhs.mPtrEdgeArray[i];
		}
	}

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded output operator <<
	@param	std::ostream& os
			Reference to an ostream object
	@param	const PolygonalPillar& pp
			Reference to PolygonalPillar type
	@return	object of type ostream reference with value equivalent to the 
			ostream object against which the output operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const PolygonalPillar& pp)
{
	os << pp.mVertexCount << "\n";
	for (int i = 0; i < pp.mVertexCount; ++i)
		os << pp.mPtrVertexArray[i];

	return os;
}

}	// namespace CS200
