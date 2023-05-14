/******************************************************************************/
/*!
\file   vertex2d.hpp
\par    Purpose: Declare class, attributes and functions for 2d vertex.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\par    Course: CS200
\par    Assignment 4
\date   28/10/2014
*/
/******************************************************************************/

#ifndef VERTEX2D_HPP_
#define VERTEX2D_HPP_

#include <iostream>
#include "point2d.hpp"
#include "color.hpp"


/******************************************************************************/
/*!
\class  Vertex2D
\brief
        Simple vertex class that contains position and color.
*/
/******************************************************************************/
class Vertex2D
{
    /*  Overloaded input & output operators */
    friend std::ostream& operator<<(std::ostream&, const Vertex2D&);
    friend std::istream& operator>>(std::istream&, Vertex2D&);

public:
    /*  No need for a Vertex2D copy constructor.
    No need for a Vertex2D overloaded assignment operator.
    No need for a Vertex2D destructor */

    /*  Default constructor */
    Vertex2D(	float x			= 0.f, 
                float y			= 0.f, 
                unsigned char r	= 0,
                unsigned char g = 0,
                unsigned char b = 0,
                unsigned char a = 255
            ) : mPos(x, y), mClr(r, g, b, a) { }
    
    /*  Auxiliary constructor */
    Vertex2D(const Point2D& p, const Color& c) : mPos(p), mClr(c) { }
    Vertex2D(float x, float y, const Color& c) : mPos(x, y), mClr(c) { }

    /*  Auxiliary functions */
    void			SetPosition(const Point2D& p)	{ mPos = p; }
    void			SetPosition(float x, float y)	{ mPos.SetX(x); mPos.SetY(y); }
    void			SetColor(const Color& c)		{ mClr	=	c; }

    const Point2D&	GetPosition() const				{ return mPos; }
    Point2D&		GetPosition()					{ return mPos; }
    const Color&	GetColor() const				{ return mClr; }
    Color&			GetColor()						{ return mClr; }

private:
    Point2D mPos;	// position of vertex
    Color	mClr;	// color of vertex
};

#endif