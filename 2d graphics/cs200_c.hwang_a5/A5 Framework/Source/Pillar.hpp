////////////////////////////////////////////////////////////////////////////////
/*!
	@file	Pillar.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef PILLAR_HPP_
#define PILLAR_HPP_

#include "Window.hpp"
#include "AABB.hpp"
#include "BC.hpp"

// Includes

namespace CS200
{

// forward declaration ...
class PolygonalPillar;

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	Pillar
	@brief	Simple 2D class to describe the common characterstics of 
			solid obstacles in a pinball simulator  
*/
////////////////////////////////////////////////////////////////////////////////
class Pillar
{
public:
	Pillar();
	Pillar(const Pillar&);
	Pillar& operator=(const Pillar&);

	// Virtual functions ... 
	virtual			~Pillar();
	virtual void	Draw(const Window&) const						=	0;
	virtual void	Move(const Vector2D&)							=	0;

	// Derived pillar object vs. other derived pillar objects
	virtual bool	TestPillar(const Pillar*) const 				=	0;
	// Derived pillar object vs. point (Point in derived pillar object test
	virtual bool	TestPillarVsPoint(const Point2D&) const 		=	0;
	// Derived pillar object vs. bounding circle test
	// Default behaviour for derived objects is to report no intersection
	virtual bool	TestPillarVsBC(const BC&) const					{ return false; }
	// Derived pillar object vs. polygonal pillar (a specific derived object) test
	// Default behaviour for derived objects is to report no intersection
	virtual bool	TestPillarVsPP(const PolygonalPillar&) const	{ return false; }

	// Auxiliary functions ...

	// Draw bounding area boundary shapes ...
	void			DrawBC(const Window& window) const				{ mBC.Draw(window); }
	void			DrawAABB(const Window& window) const			{ mAABB.Draw(window); }

	// Retrieval of bounding area data ...
	const BC&		GetBC() const									{ return mBC; }
	BC				GetBC()											{ return mBC; }
	const AABB&		GetAABB() const									{ return mAABB; }
	AABB			GetAABB()										{ return mAABB; }

protected:
	BC			mBC;		// Bounding circle enclosing solid pillar
	AABB		mAABB;		// Axis-aligned box enclosing solid pillar
};	// class Pillar

}	// namespace CS200

#endif	// PILLAR_HPP_