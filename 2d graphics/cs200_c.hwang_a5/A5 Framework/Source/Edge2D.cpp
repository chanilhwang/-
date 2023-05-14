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
#include "Edge2D.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Computes the normalized outward normal and orthogonal distance from
			origin to edge. This function assumes the start and end points of edge
			are previously been set to appropriate position coordinates.
	@param	None
	@return None
*/
////////////////////////////////////////////////////////////////////////////////
void Edge2D::ComputePointNormalEquation()
{
	// @TODO: Point-normal equation of directed edge from start point to end point.
	mNormalVec.SetX(mEndPoint.GetY() - mStartPoint.GetY());
	mNormalVec.SetY(mStartPoint.GetX() - mEndPoint.GetX());
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Auxiliary Edge2D constructor that uses two points defining a counter-
	clockwise oriented directed edge. The normalized outward normal of the directed
	edge is computed. Finally, the orthogonal distance from the origin to the line
	is computed.
	@param	const Point2D& stPt
			Start point of edge
	@param	const Point2D& endPt
			End point of edge
	Notes	1. This application is implemented in the device frame rather than
			the world frame. The class notes discuss intersection calculations
			in the world frame.
			In the world frame, if e = (ex, ey) is the displacement vector of
			the directed edge from stPt to endPt. The outward normal to this
			directed edge is obtained by rotating displacement vector e through
			an angle of -90 about the Z-axis. Therefore, the outward normal is
			computed as n = ((ey, -ex).
			In device frame, the Y-axis is inverted in comparison with the world
			frame Y-axis. The device frame outward normal to the directed edge
			with displacement from the start to end point is n = (-ey, ex).
*/
////////////////////////////////////////////////////////////////////////////////
Edge2D::Edge2D(const Point2D& stPt, const Point2D& endPt) : mStartPoint(stPt), mEndPoint(endPt)
{
	ComputePointNormalEquation();
}

}	// namespace CS200
