/******************************************************************************/
/*!
\file   point2d.hpp
\par    Purpose: Point class header.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   12/9/2014
*/
/******************************************************************************/

// Guard
#ifndef POINT2D_HPP_
#define POINT2D_HPP_

// Includes
#include <iostream>


/****************************************************************************/
/*!
\class  Point2D

\brief  Simple 2D point class.
*/
/****************************************************************************/
typedef class Point2D Vector2D;

class Point2D
{
    friend std::ostream& operator<<(std::ostream&, const Point2D&);
    friend std::istream& operator>>(std::istream&, Point2D&);

public:
    Point2D(const float x = 0.f, const float y = 0.f) : mX(x), mY(y) { }
    // No need for a Point2D copy constructor
    // No need for a Point2D overloaded assignment operator
    // No need for a Point2D destructor

    // operator overloading
    Point2D     operator+(const Point2D& r) const   { return Point2D(mX+r.mX, mY+r.mY); }
    Point2D     operator+(float f) const            { return Point2D(mX+f, mY+f); }
    Point2D&    operator+=(const Point2D& r)        { mX+=r.mX, mY+=r.mY; return *this; }
    Point2D&    operator+=(float f)                 { mX+=f; mY+=f; return *this; }
    Point2D     operator-(const Point2D& r) const   { return Point2D(mX-r.mX, mY-r.mY); }
    Point2D     operator-(float f) const            { return Point2D(mX-f, mY-f); }
    Point2D&    operator-=(const Point2D& r)        { mX-=r.mX; mY-=r.mY; return *this; }
    Point2D&    operator-=(float f)                 { mX-=f; mY-=f; return *this; }
    Point2D     operator/(float f) const            { return Point2D(mX/f, mY/f); }
    Point2D&    operator/=(float f)                 { mX/=f; mY/=f; return *this; }
    float       operator*(const Point2D  r) const   { return (mX*r.mX + mY*r.mY); }

    // Auxiliary functions
    //  v1=(x1, y1, 0), v2=(x2, y2, 0), k = (0, 0, 1)
    //    scalar triple product: (v1 x v2) . (0, 0, 1)
    float   ScalarTripleProduct(const Point2D& r) const { return (mX*r.mY - r.mX*mY); }
    float   MagnitudeSq() const                         { return (mX*mX + mY*mY); }
    float   Magnitude() const                           { return sqrtf(MagnitudeSq()); }
    void    Normalize()                                 { float m = 1.f/Magnitude(); mX*=m; mY*=m; }
    float   DistanceSq(const Point2D& r) const          { return ((r.mX-mX)*(r.mX-mX) + (r.mY-mY)*(r.mY-mY)); }
    float   Distance(const Point2D& r) const            { return sqrtf(DistanceSq(r)); }
    float   GetX() const                                { return mX; }
    float   GetY() const                                { return mY; }
    void    SetX(const float x)                         { mX    =    x; }
    void    SetY(const float y)                         { mY    =    y; }
    void    SetPosition(float x, float y)               { SetX(x); SetY(y); }

private:
    float mX, mY;
};    // class Point2D

#endif    // POINT2D_HPP_