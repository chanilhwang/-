/******************************************************************************/
/*!
\file   vertex2d.cpp
\par    Purpose: Implement functions for 2d vertex.
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

#include "vertex2d.hpp"


/******************************************************************************/
/*!
\brief
        Overloaded output operator <<
\param  std::ostream& os
        Reference to an ostream object
\param  const Vertex2D& v
        Reference to Vertex2D type
\return
        Object of type ostream reference with value equivalent to the 
            ostream object against which the output operator is applied
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Vertex2D& v)
{
    os << v.mPos;
    os << v.mClr;

    return os;
}


/******************************************************************************/
/*!
\brief
        Overloaded input operator >>
\param  std::istream& is
        Reference to an istream object
\param  const Vertex2D& v
        Reference to Vertex2D type
\return
        Object of type istream reference with value equivalent to the 
            istream object against which input operator is applied
*/
/******************************************************************************/
std::istream& operator>>(std::istream& is, Vertex2D& v)
{
    Point2D pos;
    Color	clr;

    is >> pos;
    is >> clr;

    v.SetPosition(pos);
    v.SetColor(clr);

    return is;
}

