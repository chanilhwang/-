////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Edge2D.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef EDGE2D_HPP_
#define EDGE2D_HPP_

#include "Point2D.hpp"
#include <cmath>

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Edge2D
	@brief	Simple vertex class describing a counter-clockwise ordered directed
	edges in device frame: start point, end point, normalized normal, and
	orthogonal distance from origin to edge
*/
////////////////////////////////////////////////////////////////////////////////
class Edge2D
{
public:
	Edge2D() { }
	// No need for a Edge2D copy constructor
	// No need for a Edge2D overloaded assignment operator
	// No need for a Edge2D destructor

	// Auxiliary constructor for directed edge, in device frame, from
	// start point stPt to end point endPt 
	Edge2D(const Point2D& stPt, const Point2D& endPt);

	// Auxiliary functions
	void				SetStartPoint(const Point2D& stPt)	{ mStartPoint = stPt; }
	void				SetEndPoint(const Point2D& endPt)	{ mEndPoint = endPt; }
	const Point2D &		GetStartPoint() const				{ return mStartPoint; }
	Point2D &			GetStartPoint()						{ return mStartPoint; }
	const Point2D &		GetEndPoint() const					{ return mEndPoint; }
	Point2D &			GetEndPoint()						{ return mEndPoint; }
	const Vector2D &	GetNml() const						{ return mNormalVec; }
	Vector2D &			GetNml()							{ return mNormalVec; }

	Vector2D			GetEdgeVector() const				{ return (mEndPoint-mStartPoint); }
	void				ComputePointNormalEquation();

private:
	Point2D		mStartPoint;	// start point of directed edge
	Point2D		mEndPoint;		// end point of directed edge
	Vector2D	mNormalVec;		// normalized outward normal to edge from mStartPoint and mEndPoint
	float		mOrthoDistance;	// orthogonal distance from origin to edge
};

}

#endif // EDGE2D_HPP_
