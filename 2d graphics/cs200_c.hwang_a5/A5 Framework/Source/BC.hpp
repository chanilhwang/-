////////////////////////////////////////////////////////////////////////////////
/*!
	@file	BC.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef BC_HPP_
#define BC_HPP_

// Includes
#include "Window.hpp"
#include "Point2D.hpp"

namespace CS200
{

// forward declarations for friend function declarations ...
class AABB;

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	BC
	@brief	A bounding circle class to completely enclose a shape
*/
////////////////////////////////////////////////////////////////////////////////
class BC
{
public:
	BC() : mCenPos(0.f, 0.f), mRadius(0.f) { }
	BC(const Point2D& center, float radius) : mCenPos(center), mRadius(radius) { }

	// No need for a BC copy constructor
	// No need for a BC overloaded assignment operator
	// No need for a BC destructor

	// Auxiliary functions ...
	void		Draw(const Window&) const;

	void		Compute(const Point2D*, int);
	void		Move(const Vector2D&);

	// BC vs. Point test
	bool		TestBCVsPoint(const Point2D&) const;
	// BC vs. BC test
	bool		TestBCvsBC(const BC&) const;
	// BC vs. AABB (point in lozenge) test
	friend bool	TestBCVsAABB(const BC&, const AABB&);

public:
	Point2D	mCenPos;
	float	mRadius;
};

}	// namespace CS200


#endif	// AABB2D_HPP_