/******************************************************************************/
/*!
\file   linebox2d.hpp
\par    Purpose: Box shape class header
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   12/9/2014
*/
/******************************************************************************/
// Guard
#ifndef BOX_HPP_
#define BOX_HPP_

// Includes
#include "graphics.hpp"
#include "color.hpp"
#include "point2d.hpp"


/****************************************************************************/
/*!
\class  Box2D
        
\brief  Simple square box shape, used for two bounding square boxes at
        each end point of line segment.
*/
/****************************************************************************/
class Box2D
{
public:
    static const float    EPSILON;
    static const float    sHalfWidth;

    Box2D() { }
    // Auxiliary constructor
    Box2D(float centerX, float centerY, float halfWidth, float halfHeight) :
        mMin(centerX-halfWidth, centerY-halfHeight),
        mMax(centerX+halfWidth, centerY+halfHeight),
        mCenter(centerX, centerY) { }
    // Auxiliary constructor
    Box2D(const Point2D& center, float halfWidth, float halfHeight) : 
        mMin(center.GetX()-halfWidth, center.GetY()-halfHeight),
        mMax(center.GetX()+halfWidth, center.GetY()+halfHeight),
        mCenter(center) { }

    // No need for a Box2D copy constructor
    // No need for a Box2D overloaded assignment operator
    // No need for a Box2D destructor

    // Auxiliary functions ...
    void            Move(const Vector2D& v) { mMin += v; mMax += v; mCenter += v; }
    void            Draw(Graphics &gl, const Color&) const;

    const Point2D&  GetCenter()    const       { return mCenter; }
    Point2D&        GetCenter()                { return mCenter; }
    
    // Square box vs. Point test
    bool    TestBox2DVsPoint(const Point2D&) const;

private:
    Point2D    mMin;        // Top-left point of box
    Point2D    mMax;        // Bottom-right point of box
    Point2D    mCenter;    // Midpoint of diagonal from _mMin and _mMax
};  // class Box2D


/****************************************************************************/
/*!
\class  LineBox2D
        
\brief  Encapsulates two bounding square boxes at each end point of line segment.
*/
/****************************************************************************/
class LineBox2D
{
public:
    LineBox2D() { }
    // Auxiliary constructor
    LineBox2D(const Box2D& start, const Box2D& end) : mStart(start), mEnd(end) { }

    // Auxiliary functions ...
    void            ComputeIntersection(const Point2D&, const Point2D&, Point2D&) const;
    void            ComputeBox(const Point2D&, const Point2D&);
    void            ComputeStartBox(const Point2D&, const Point2D&);
    void            ComputeEndBox(const Point2D&, const Point2D&);

    void            MoveStart(const Vector2D& v)    { mStart.Move(v); }
    void            MoveEnd(const Vector2D& v)        { mEnd.Move(v); }
    void            Move(const Vector2D& v)            { mStart.Move(v); mEnd.Move(v); }
    void            DrawStart(Graphics &gl, const Color& c) { mStart.Draw(gl, c); }
    void            DrawEnd(Graphics &gl, const Color& c) { mEnd.Draw(gl, c); }
    void            Draw(Graphics &gl, const Color& c)
                                { DrawStart(gl, c); DrawEnd(gl, c); }

    const Box2D&    GetStartBox() const     { return mStart; }
    Box2D&          GetStartBox()           { return mStart; }
    const Box2D&    GetEndBox() const       { return mEnd; }
    Box2D&          GetEndBox()             { return mEnd; }
    void            SetStartBox(Box2D& bb)  { mStart = bb; }
    void            SetEndBox(Box2D& bb)    { mEnd = bb; }

private:
    Box2D    mStart;
    Box2D    mEnd;
};  // class LineBox2D

#endif    // BOX_HPP_