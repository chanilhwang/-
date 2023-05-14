////////////////////////////////////////////////////////////////////////////////
/*!
	@file	AABB.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef AABB_HPP_
#define AABB_HPP_

// Includes
#include "Window.hpp"
#include "Point2D.hpp"
#include "BC.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	AABB
	@brief	2D Axis-aligned bounding box class to bound
			a solid pillar's extreme points along the X- and Y-axes.
*/
////////////////////////////////////////////////////////////////////////////////
class AABB
{
public:
	AABB() { }
	// Auxiliary constructor
	AABB(const Point2D& minPos, const Point2D& maxPos) : 
			mMinPos(minPos), mMaxPos(maxPos), mCenPos((mMinPos+mMaxPos)/2.f) { }

	// No need for a AABB copy constructor
	// No need for a AABB overloaded assignment operator
	// No need for a AABB destructor

	// Auxiliary functions ...
	void		Draw(const Window&) const;

	void		Compute(const Point2D*, int);
	void		Move(const Vector2D&);

	// AABB vs. Point test
	bool		TestAABBVsPoint(const Point2D&);
	// AABB vs. AABB test
	bool		TestAABBVsAABB(const AABB&);
	// AABB vs. BC (point in lozenge) test
	friend bool	TestBCVsAABB(const BC&, const AABB&);	

	// Auxiliary functions ...
	void		Expand(float r) { mMinPos -= r; mMaxPos += r; }

public:
	Point2D		mMinPos;	// Top-left point with minimum x and y values
	Point2D		mMaxPos;	// Bottom-right point with maximum x and y values
	Point2D		mCenPos;	// Midpoint of diagonal from mMinPos and mMaxPos
};

}	// namespace CS200


#endif	// AABB_HPP_