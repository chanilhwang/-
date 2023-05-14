/******************************************************************************/
/*!
\file   triangle.hpp
\par    Purpose: Declare class, attributes and functions for triangle drawing.
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

#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include "vertex2d.hpp"

/******************************************************************************/
/*!
\class  Triangle
\brief
        Simple triangle class.
*/
/******************************************************************************/
class Triangle
{
public:
    /*  No need for default constructor, copy constructor, and destructor
        No need for overloaded assignment operator. */

    /*  Auxiliary constructor */
    Triangle(const Vertex2D& v0, const Vertex2D& v1, const Vertex2D& v2)
    {
        mVertex[0] = v0;
        mVertex[1] = v1;
        mVertex[2] = v2;
    }

    bool IsCounterClockwise() const;
    void Draw(unsigned char *frameBuffer) const;

    const Vertex2D& GetVertex(unsigned index) const				{ return mVertex[index]; }
    Vertex2D		GetVertex(unsigned index)					{ return mVertex[index]; }
    void			SetVertex(const Vertex2D& v, unsigned index){ mVertex[index] = v; }

private:
    Vertex2D	mVertex[3];
};


#endif