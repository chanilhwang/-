/******************************************************************************/
/*!
\file   vertex2d.cpp
\par    Purpose: Vertex class implementation.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   12/9/2014
*/
/******************************************************************************/
// Includes
#include "vertex2d.hpp"


/******************************************************************************/
/*!
\fn     std::ostream& operator<<(std::ostream& os, const Vertex2D& v)
\brief
        Overloaded output operator <<

\param  os
        Reference to an ostream object.
\param  v
        Reference to vertex.
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
\fn     std::istream& operator>>(std::istream& is, Vertex2D& v)
\brief
        Overloaded output operator >>

\param  is
        Reference to an istream object.
\param  v
        Reference to vertex.
*/
/******************************************************************************/
std::istream& operator>>(std::istream& is, Vertex2D& v)
{
    Point2D pos;
    Color    clr;

    is >> pos;
    is >> clr;

    v.SetPosition(pos);
    v.SetColor(clr);

    return is;
}

