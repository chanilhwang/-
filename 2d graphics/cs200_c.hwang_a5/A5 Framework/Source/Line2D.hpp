////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Line2D.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef LINE2D_HPP_
#define LINE2D_HPP_

// Includes
#include "Window.hpp"
#include "Vertex2D.hpp"

namespace CS200
{
////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Line2D
	@brief	Simple 2D line class 
*/
////////////////////////////////////////////////////////////////////////////////
class Line2D
{
	friend std::ostream& operator<<(std::ostream&, const Line2D&);
	friend std::istream& operator>>(std::istream&, Line2D&);

public:

	// No need for a Line2D default constructor
	// No need for a Line2D copy constructor
	// No need for a Line2D overloaded assignment operator
	// No need for a Line2D destructor

	// Auxiliary constructor ...
	Line2D(const Vertex2D& start, const Vertex2D& end) : mStartVtx(start), mEndVtx(end) { }

	// Auxiliary functions ...
	void SetStartVertex(const Vertex2D& startVtx)	{ mStartVtx	= startVtx; }
	void SetEndVertex(const Vertex2D& endVtx)		{ mEndVtx	= endVtx; }

	const Vertex2D&	GetStartVertex() const			{ return mStartVtx; }
	const Vertex2D&	GetEndVertex() const			{ return mEndVtx; }

	Vertex2D&		GetStartVertex()				{ return mStartVtx; }
	Vertex2D&		GetEndVertex()					{ return mEndVtx; }

	const Point2D&	GetStartVtxPosition() const		{ return mStartVtx.GetPosition(); }
	const Point2D&	GetEndVtxPosition() const		{ return mEndVtx.GetPosition(); }
	void SetStartVtxPosition(const Point2D& pos)	{ mStartVtx.SetPosition(pos); }
	void SetEndVtxPosition(const Point2D& pos)		{ mEndVtx.SetPosition(pos); }

	float			GetDeltaX() const;
	float			GetDeltaY() const;

	// Displace line's end points ...
	void			Move(const Vector2D& vec);

	// Line scan conversion functions ...
	void			DrawDDA(const Window&) const;
	void			DrawMidpoint(const Window&) const;

	// Is point on or close to line?
	bool			TestLineVsPoint(const Point2D&) const;

protected:
	Vertex2D	mStartVtx;
	Vertex2D	mEndVtx;
};

}	// namespace CS200

#endif	// LINE2D_HPP_