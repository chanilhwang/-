////////////////////////////////////////////////////////////////////////////////
/*!
	@file	PolygonalPillar.hpp
	@author	pghali@digipen.edu, sang.le@digipen.edu
	@date	July 27, 2014
	@par	CS200 Framework
	@par	Copyright 2005-2014 DigiPen Institute of Technology
*/
////////////////////////////////////////////////////////////////////////////////

// Guard
#ifndef POLYGONALPILLAR_HPP_
#define POLYGONALPILLAR_HPP_

// Includes
#include "Window.hpp"
#include "Vertex2D.hpp"
#include "Edge2D.hpp"
#include "Pillar.hpp"
#include <iostream>

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
	@class	PolygonalPillar
	@brief	Simple class for counter-clockwise oriented convex polygons described
			in device coordinates.
*/
////////////////////////////////////////////////////////////////////////////////
class PolygonalPillar : public Pillar
{
	friend std::ostream& operator<<(std::ostream&, const PolygonalPillar&);
public:
	PolygonalPillar() : mPtrVertexArray(0), mVertexCount(0), mPtrEdgeArray(0) { }
	PolygonalPillar(const PolygonalPillar& poly);
	PolygonalPillar(const CS200::Vertex2D* pVtx, int numVtx);
	~PolygonalPillar()	{ delete [] mPtrVertexArray; delete [] mPtrEdgeArray; }

	PolygonalPillar& operator=(const PolygonalPillar &);

	// virtual functions ...
	void	Draw(const Window&) const;
	void	Move(const Vector2D&);
	Edge2D* GetEdges() const { return mPtrEdgeArray; }
	int GetVertexCount() const { return mVertexCount; }
	Vertex2D* GetVertexes() const { return mPtrVertexArray; }

	// Polygonal pillar vs. Other pillars
	bool	TestPillar(const Pillar*) const;			
	// Polygonal Pillar vs. Point test (point in polygon test)
	bool	TestPillarVsPoint(const Point2D&) const;
	// Polygonal Pillar vs. Bounding Circle test (polygon-circle intersection test)
	bool	TestPillarVsBC(const BC&) const;
	// Polygonal Pillar vs. Polygon Pillar test (Separation Axis Theorem)
	bool	TestPillarVsPP(const PolygonalPillar&) const;
	
	// Auxiliary functions ...
	bool	IsConvex() const;

private:
	int			mVertexCount;		// vertex count of n-gon
	Vertex2D*	mPtrVertexArray;	// dynamically generated array of vertices of n-gon
	Edge2D*		mPtrEdgeArray;		// dynamically generated array of point-normal equations of n-gon's edges
	
};	// class PolygonalPillar

}	// namespace CS200

#endif	// POLYGONALPILLAR_HPP_