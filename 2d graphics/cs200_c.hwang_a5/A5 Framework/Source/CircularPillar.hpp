////////////////////////////////////////////////////////////////////////////////
/*!
	@file	CircularPillar.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef CIRCULARPILLAR_HPP_
#define CIRCULARPILLAR_HPP_

// Includes
#include "Window.hpp"
#include "Point2D.hpp"
#include "Color.hpp"
#include "Pillar.hpp"
#include <iostream>

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	CircularPillar
	@brief	Simple CircularPillar class
			Note: Base class Pillar declares center of CircularPillar
*/
////////////////////////////////////////////////////////////////////////////////
class CircularPillar : public Pillar
{
	friend std::ostream& operator<<(std::ostream&, const CircularPillar &);

public:
	CircularPillar() : mCenPos(0.f, 0.f), mRadius(0) { }
	// No need for a CircularPillar copy constructor
	// No need for a CircularPillar assignment operator
	// No need for a CircularPillar destructor

	// Auxiliary constructors
	CircularPillar(	const Point2D&		center,
					int					radius,
					const Color&		clr);
	CircularPillar(	const Point2D&		center,
					const Point2D&		offset,
					const Color&		clr); 

	// virtual functions ...
	void	Draw(const Window& window) const;
	void	Move(const Vector2D& pt);

	// Circular pillar vs. Other pillars
	bool	TestPillar(const Pillar*) const;
	// Circular Pillar vs. Point test (point in circle test)
	bool	TestPillarVsPoint(const Point2D&) const;

private:
	Point2D		mCenPos;	// geometric center of pillar
	int			mRadius;
	Color		mColor;
};	// class CircularPillar

}	// namespace CS200

#endif	// CIRCULARPILLAR_HPP_