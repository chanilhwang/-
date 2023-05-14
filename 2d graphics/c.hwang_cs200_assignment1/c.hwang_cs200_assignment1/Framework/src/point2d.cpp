/******************************************************************************/
/*!
\file   point2d.cpp
\par    Purpose: Point class implementation.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   12/9/2014
*/
/******************************************************************************/

// Includes
#include "point2d.hpp"


/******************************************************************************/
/*!
\fn     std::ostream& operator<<(std::ostream& os, const Point2D& p)
\brief
        Overloaded output operator <<

\param  os
        Reference to an ostream object.
\param  p
        Reference to Point2D object.
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Point2D& p)
{
    os << p.mX << " " << p.mY  << "\n";
    return os;
}


/******************************************************************************/
/*!
\fn     std::istream& operator>>(std::istream& is, Point2D& p)
\brief
        Overloaded output operator >>

\param  is
        Reference to an istream object.
\param  p
        Reference to Point2D object.
*/
/******************************************************************************/
std::istream& operator>>(std::istream& is, Point2D& p)
{
    char str[100];
    is.getline(str, 100);

    float fx, fy;
    sscanf_s(str, " %f %f", &fx, &fy);
    p.SetPosition(fx, fy);

    return is;
}

