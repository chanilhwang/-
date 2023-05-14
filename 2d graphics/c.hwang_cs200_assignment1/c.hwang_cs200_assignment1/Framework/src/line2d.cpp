/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: line2d.cpp
Purpose: line class implementation
Language: c++
Platform: visual studio 2017, windows 10 64-bit
Project: c.hwang_CS200_a1
Author: hwang chan il, c.hwang, tktlssla803@gmail.com
Creation date: 10/4/2018
- End Header ----------------------------------------------------------------*/

// Includes
#include "line2d.hpp"


/******************************************************************************/
/*!
\fn     void Line2D::DrawDDA(Graphics &gl) const
\brief
        Routine for drawing a line from start vertex mStartVtx to end
        vertex mEndVtx using the DDA algorithm.

        Drawing a pixel at integer coord (x, y) is done by calling
        gl.SetPix(x, y, color);
        in which color can be obtained from either endpoint, e.g.
        Color color = mStartVtx.GetColor();

\param  gl
        Reference to graphics instance, for access to offscreen buffer.
*/
/******************************************************************************/
void Line2D::DrawDDA(Graphics &gl) const
{
	float x0 = GetStartVertex().GetPosition().GetX();
	float y0 = GetStartVertex().GetPosition().GetY();
	float x1 = GetEndVertex().GetPosition().GetX();
	float y1 = GetEndVertex().GetPosition().GetY();
	Color I = mStartVtx.GetColor();

	float dx = x1 - x0, dy = y1 - y0;

	if(x1 == x0)
	{
		float x = x0;
		int y = round(y0);
		int sym_y = round(y1);
		int ystep = (dy >= 0) ? 1 : -1;
		int del_y = round(y1) - y;
		del_y = (del_y >= 0) ? del_y : -del_y;
		if (del_y % 2 == 0)
		{
			del_y = del_y / 2;
			while (del_y--)
			{
				y += ystep;
				sym_y -= ystep;
				gl.SetPix((int)x, y, I);
				gl.SetPix((int)x, sym_y, I);
			}
		}
		else
		{
			del_y = del_y / 2;
			while (del_y--)
			{
				y += ystep;
				sym_y -= ystep;
				gl.SetPix((int)x, y, I);
				gl.SetPix((int)x, sym_y, I);
			}
			y += ystep;
			gl.SetPix((int)x, y, I);
		}
	}
	//m > 1
	else if (abs(dy) > abs(dx))
	{
		float rm = dx / dy, b = y0 - x0 / rm;
		int y = round(y0);
		int sym_y = round(y1);
		int del_y = round(y1) - y;
		int ystep = (dy >= 0) ? 1 : -1;
		del_y = (del_y >= 0) ? del_y : -del_y;
		float x = (y - b)*rm + 0.5f;
		float sym_x = (sym_y - b)*rm + 0.5f;
		rm = (rm >= 0.f) ? rm : -rm;
		float xstep = (dx >= 0.f) ? rm : -rm;
		gl.SetPix((int)x, y, I);
		if (del_y % 2 == 0)
		{
			del_y = del_y / 2;
			while (del_y--)
			{
				x += xstep;
				y += ystep;
				sym_x -= xstep;
				sym_y -= ystep;
				gl.SetPix((int)x, y, I);
				gl.SetPix((int)sym_x, sym_y, I);
			}
		}
		else
		{
			del_y = del_y / 2;
			while (del_y--)
			{
				x += xstep;
				y += ystep;
				sym_x -= xstep;
				sym_y -= ystep;
				gl.SetPix((int)x, y, I);
				gl.SetPix((int)sym_x, sym_y, I);
			}
			x += xstep;
			y += ystep;
			gl.SetPix((int)x, y, I);
		}
	}
	
	//0 <= |m| <= 1
	else if (abs(dy) <= abs(dx))
	{
		float m = dy / dx, b = y0 - m * x0;
		int x = round(x0);
		int sym_x = round(x1);
		int del_x = round(x1) - x;
		int xstep = (dx >= 0) ? 1 : -1;
		del_x = (del_x >= 0) ? del_x : -del_x;
		float y = m * (float)x + b + 0.5f;
		float sym_y = m * (float)sym_x+ b + 0.5f;
		m = (m >= 0.f) ? m : -m;
		float ystep = (dy >= 0.f) ? m : -m;
		gl.SetPix(x, (int)y, I);
		if (del_x % 2 == 0)
		{
			del_x = del_x / 2;
			while (del_x--)
			{
				x += xstep;
				y += ystep;
				sym_x -= xstep;
				sym_y -= ystep;
				gl.SetPix((int)x, y, I);
				gl.SetPix((int)sym_x, sym_y, I);
			}
		}
		else
		{
			del_x = del_x / 2;
			while (del_x--)
			{
				x += xstep;
				y += ystep;
				sym_x -= xstep;
				sym_y -= ystep;
				gl.SetPix((int)x, y, I);
				gl.SetPix((int)sym_x, sym_y, I);
			}
			x += xstep;
			y += ystep;
			gl.SetPix((int)x, y, I);
		}
	}
}


/******************************************************************************/
/*!
\fn     void Line2D::DrawMidpoint(Graphics &gl) const
\brief
        Routine for drawing a line from start vertex mStartVtx to end
        vertex mEndVtx using the Midpoint/Bresenham algorithm.

        Drawing a pixel at integer coord (x, y) is done by calling
        gl.SetPix(x, y, color);
        in which color can be obtained from either endpoint, e.g.
        Color color = mStartVtx.GetColor();

\param  gl
        Reference to graphics instance, for access to offscreen buffer.
*/
/******************************************************************************/
void Line2D::DrawMidpoint(Graphics &gl) const
{
	int x0 = round(GetStartVertex().GetPosition().GetX());
	int y0 = round(GetStartVertex().GetPosition().GetY());
	int x1 = round(GetEndVertex().GetPosition().GetX());
	int y1 = round(GetEndVertex().GetPosition().GetY());
	Color I = mStartVtx.GetColor();

	int dx = x1 - x0, dy = y1 - y0, x = x0, y = y0, sym_x = x1, sym_y = y1;
	int ystep = (dy >= 0) ? 1 : -1;
	int xstep = (dx >= 0) ? 1 : -1;
	dx = (dx >= 0) ? dx : -dx;
	dy = (dy >= 0) ? dy : -dy;

	//m > 1
	if (abs(dy) > abs(dx))
	{
		gl.SetPix(x, y, I);
		int d = 2 * dx - dy, dN = 2 * dx, dNE = 2 * (dx - dy);
		if (dy % 2 == 0)
		{
			dy = dy / 2;
			while (dy--)
			{
				y += ystep;
				sym_y -= ystep;
				if (d >= 0)
				{
					x += xstep;
					sym_x -= xstep;
					d += dNE;
				}
				else
				{
					d += dN;
				}
				gl.SetPix(x, y, I);
				gl.SetPix(sym_x, sym_y, I);
			}
		}
		else
		{
			dy = dy / 2;
			while (dy--)
			{
				y += ystep;
				sym_y -= ystep;
				if (d >= 0)
				{
					x += xstep;
					sym_x -= xstep;
					d += dNE;
				}
				else
				{
					d += dN;
				}
				gl.SetPix(x, y, I);
				gl.SetPix(sym_x, sym_y, I);
			}
			y += ystep;
			sym_y -= ystep;
			if (d >= 0)
			{
				x += xstep;
				sym_x -= xstep;
				d += dNE;
			}
			else
			{
				d += dN;
			}
			gl.SetPix(x, y, I);
		}
	}
	//0 <= |m| <= 1
	else if (abs(dy) <= abs(dx))
	{
		gl.SetPix(x, y, I);
		int d = 2 * dy - dx, dE = 2 * dy, dNE = 2 * (dy - dx);
		if (dx % 2 == 0)
		{
			dx = dx / 2;
			while (dx--)
			{
				x += xstep;
				sym_x -= xstep;
				if (d >= 0)
				{
					y += ystep;
					sym_y -= ystep;
					d += dNE;
				}
				else
				{
					d += dE;
				}
				gl.SetPix(x, y, I);
				gl.SetPix(sym_x, sym_y, I);
			}
		}
		else
		{
			dx = dx / 2;
			while (dx--)
			{
				x += xstep;
				sym_x -= xstep;
				if (d >= 0)
				{
					y += ystep;
					sym_y -= ystep;
					d += dNE;
				}
				else
				{
					d += dE;
				}
				gl.SetPix(x, y, I);
				gl.SetPix(sym_x, sym_y, I);
			}
		}
	}
}



/******************************************************************************/
/*
            Nothing more to implement below
*/
/******************************************************************************/

/******************************************************************************/
/*!
\fn     bool Line2D::TestLineVsPoint(const Point2D& pt) const
\brief
        Intersection test between an arbitrary point and 2d line segment.

\param  pt
        Reference to the point to be checked against.
*/
/******************************************************************************/
bool Line2D::TestLineVsPoint(const Point2D& pt) const
{
    float width    =    5.f;

    Point2D p0 = mStartVtx.GetPosition(), p1 = mEndVtx.GetPosition();
    Point2D e(p1 - p0);

    if (e.MagnitudeSq() < 1.f) return false;

    Point2D n(-e.GetY(), e.GetX());
    n.Normalize();
    float dist = n*pt - n*p0;
    dist = (dist >= 0.f) ? dist : -dist;
    if (dist > width) return false;

    Point2D    dn(n.GetX()*width, n.GetY()*width);
    Point2D v3 = p0 - dn;
    Point2D l(n.GetX()*2.f*width, n.GetY()*2.f*width);
    l.Normalize();
    Point2D n1(-l.GetY(), l.GetX());
    dist = n1*pt - n1*v3;
    if (dist > 0.f) return false;

    Point2D v1 = p1 + dn;
    l.SetX(-l.GetX()); l.SetY(-l.GetY());
    Point2D n3(-l.GetY(), l.GetX());
    dist = n3*pt - n3*v1;
    if (dist > 0.f) return false;
    
    return true;
}


/******************************************************************************/
/*!
\fn     std::ostream& operator<<(std::ostream& os, const Line2D& ls)
\brief
        Overloaded output operator <<

\param  os
        Reference to an ostream object.
\param  ls
        Reference to Line2D object.
*/
/******************************************************************************/
std::ostream& operator<<(std::ostream& os, const Line2D& ls)
{
    os << ls.mStartVtx;
    os << ls.mEndVtx;

    return os;
}


/******************************************************************************/
/*!
\fn     void Line2D::Move(const Vector2D& vec)
\brief
        Displace line segment to new location.

\param  vec
        Displacement vector.
*/
/******************************************************************************/
void Line2D::Move(const Vector2D& vec)
{
    mStartVtx.SetPosition(mStartVtx.GetPosition() + vec);
    mEndVtx.SetPosition(mEndVtx.GetPosition() + vec);
}


/******************************************************************************/
/*!
\fn     float Line2D::GetDeltaX() const
\brief
        Computes run of line segment.
\return
        Difference between X coordinates of end and start points
*/
/******************************************************************************/
float Line2D::GetDeltaX() const
{
    return (mEndVtx.GetPosition().GetX() - mStartVtx.GetPosition().GetX());
}


/******************************************************************************/
/*!
\fn     float Line2D::GetDeltaY() const
\brief
        Computes rise of line segment.
\return
        Difference between Y coordinates of end and start points
*/
/******************************************************************************/
float Line2D::GetDeltaY() const
{
    return (mEndVtx.GetPosition().GetY() - mStartVtx.GetPosition().GetY());
}

