/******************************************************************************/
/*!
\file   linebox2d.cpp
\par    Purpose: Box shape class implementation
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Prasanna Ghali, Sang Le
\par    email: pghali@digipen.edu, sang.le@digipen.edu
\par    Course: CS200
\date   12/9/2014
*/
/******************************************************************************/

// Includes
#include "box2d.hpp"
#include "line2d.hpp"


const float    Box2D::EPSILON       =    0.1f;
const float    Box2D::sHalfWidth    =    3.0f;

/******************************************************************************/
/*!
\fn     void Box2D::Draw(Graphics &gl, const Color& clr) const
\brief  Draws boundary of square box.
\param  gl
        Graphics instance for access to offscreen framebuffer.
\param  clr
        Reference to color of the box.
*/
/******************************************************************************/
void Box2D::Draw(Graphics &gl, const Color& clr) const
{
    Vertex2D vtx0(mMin, clr);
    Vertex2D vtx1(mMin.GetX(), mMax.GetY(), clr);
    Vertex2D vtx2(mMax, clr);
    Vertex2D vtx3(mMax.GetX(), mMin.GetY(), clr);

    Line2D line[]    =
    {
        Line2D(vtx0, vtx1),
        Line2D(vtx1, vtx2),
        Line2D(vtx2, vtx3),
        Line2D(vtx3, vtx0),
    };

    for (int i = 0; i < 4; ++i)
    {
        line[i].DrawDDA(gl);
    }
}


/******************************************************************************/
/*!
\fn     bool Box2D::TestBox2DVsPoint(const Point2D& p) const
\brief  2d square Box vs. point test.
\param  p
        Point to be tested.
\return True if point is intersecting with box; false otherwise.
*/
/******************************************************************************/
bool Box2D::TestBox2DVsPoint(const Point2D& p) const
{
    return (p.GetX() >= mMin.GetX() && p.GetX() <= mMax.GetX() && 
            p.GetY() >= mMin.GetY() && p.GetY() <= mMax.GetY()) ? true : false;
}


/******************************************************************************/
/*!
\fn     void LineBox2D::ComputeIntersection(const Point2D& p0, 
                                            const Point2D& p1, 
                                            Point2D& inter) const
\brief  Compute an edit square box at each end point of a line segment.
\param  p0
        Start point of line segment.
\param  p1
        End point of line segment.
\param  inter
        Intersection point.
*/
/******************************************************************************/
void LineBox2D::ComputeIntersection(const Point2D& p0, const Point2D& p1, Point2D& inter) const
{
    // compute Box2D's for both end points
    float    x0     =    p0.GetX(), y0  = p0.GetY();
    float    x1     =    p1.GetX(), y1  = p1.GetY();
    float    dx     =    x1 - x0, dy    = y1 - y0;
    float    adx    =    (dx < 0.f) ? -dx : dx, ady = (dy < 0.f) ? -dy : dy;
    float    xi, yi;

    if (adx <= Box2D::EPSILON)
    {
        xi = x0;
        yi = (dy > 0.f) ? (y0+Box2D::sHalfWidth) : (y0-Box2D::sHalfWidth);
    }
    else if (ady <= Box2D::EPSILON)
    {
        yi = y0;
        xi = (dx > 0.f) ? (x0+Box2D::sHalfWidth) : (x0-Box2D::sHalfWidth);
    }
    else
    {
        float m     =    dy/dx;
        float am    =    (m > 0.f) ? m : -m;
        float b     =    y0 - m*x0;

        if (am >= 1.f)
        {
            yi = (dy > 0.f) ? (y0+Box2D::sHalfWidth) : (y0-Box2D::sHalfWidth);
            xi = (yi - b)/m;
        }
        else
        {
            xi = (dx > 0.f) ? (x0+Box2D::sHalfWidth) : (x0-Box2D::sHalfWidth);
            yi = m*xi + b;
        }
    }

    inter.SetX(xi); inter.SetY(yi);

#if 0
    Box2D st(x0 + (x0-xi), y0 + (y0-yi), Box2D::sHalfWidth, Box2D::sHalfWidth);
    Box2D en(x1 - (x0-xi), y1 - (y0-yi), Box2D::sHalfWidth, Box2D::sHalfWidth);

    mStart = st;
    mEnd = en;
#endif
}


/******************************************************************************/
/*!
\fn     void LineBox2D::ComputeBox(const Point2D& p0, const Point2D& p1)
\brief  Compute an edit square box at each end point of a line segment.
\param  p0
        Start point of line segment.
\param  p1
        End point of line segment.
*/
/******************************************************************************/
void LineBox2D::ComputeBox(const Point2D& p0, const Point2D& p1)
{
    Point2D inter;
    ComputeIntersection(p0, p1, inter);

    Point2D diff = p0 - inter;
    Point2D disp = p0 + diff;
    Box2D st(disp.GetX(), disp.GetY(), Box2D::sHalfWidth, Box2D::sHalfWidth);
    disp = p1 - diff;
    Box2D en(disp.GetX(), disp.GetY(), Box2D::sHalfWidth, Box2D::sHalfWidth);

    mStart = st;
    mEnd = en;    
}


/******************************************************************************/
/*!
\fn     void LineBox2D::ComputeStartBox(const Point2D& p0, const Point2D& p1)
\brief  Compute an edit square box only for start point of line segment.
\param  p0
        Start point of line segment.
\param  p1
        End point of line segment.
*/
/******************************************************************************/
void LineBox2D::ComputeStartBox(const Point2D& p0, const Point2D& p1)
{
    Point2D inter;
    ComputeIntersection(p0, p1, inter);
    Point2D diff = p0 - inter;
    Point2D disp = p0 + diff;
    Box2D st(disp.GetX(), disp.GetY(), Box2D::sHalfWidth, Box2D::sHalfWidth);
    mStart = st;
}


/******************************************************************************/
/*!
\fn     void LineBox2D::ComputeEndBox(const Point2D& p0, const Point2D& p1)
\brief  Compute an edit square box only for end point of line segment.
\param  p0
        Start point of line segment.
\param  p1
        End point of line segment.
*/
/******************************************************************************/
void LineBox2D::ComputeEndBox(const Point2D& p0, const Point2D& p1)
{
    Point2D inter;
    ComputeIntersection(p0, p1, inter);

    Point2D diff = p0 - inter;
    Point2D disp = p1 - diff;
    Box2D en(disp.GetX(), disp.GetY(), Box2D::sHalfWidth, Box2D::sHalfWidth);
    mEnd = en;    
}

