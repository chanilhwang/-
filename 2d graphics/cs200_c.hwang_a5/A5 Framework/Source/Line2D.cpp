/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: <put file name here>
Purpose: <explain the contents of this file>
Language: <specify language and compiler>
Platform: <specify compiler version, hardware requirements, operating systems>
Project: <specify student login, class, and assignment. For example: if foo
is in class CS 200 and this
 file is part of assignment 3, then write: foo_CS200_3>
Author: <provide your name, student login, and student id>
Creation date: <date on which you created this file>
- End Header ----------------------------------------------------------------*/

// Includes
#include "Line2D.hpp"

namespace CS200
{

inline float FABS(float f) { return (f < 0.f) ? -f : f; }

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Routine for drawing a line from start vertex mStartVtx to end
			vertex mEndVtx using the DDA algorithm
    @param  window
            The window to draw to
	@Use	Use the following data members of class Line2D
			mStartVtx: start vertex
			mEndVtx: end vertex
*/
////////////////////////////////////////////////////////////////////////////////
void Line2D::DrawDDA(const Window& window) const
{
	float x0 = GetStartVertex().GetPosition().GetX();
	float y0 = GetStartVertex().GetPosition().GetY();
	float x1 = GetEndVertex().GetPosition().GetX();
	float y1 = GetEndVertex().GetPosition().GetY();
	Color I = mStartVtx.GetColor();

	float dx = x1 - x0, dy = y1 - y0;

	//m = 0
	if (x1 == x0)
	{
		float x = x0;
		int y = static_cast<int>(round(y0));
		int sym_y = static_cast<int>(round(y1));
		int ystep = (dy >= 0) ? 1 : -1;
		int del_y = static_cast<int>(round(y1) - y);
		del_y = (del_y >= 0) ? del_y : -del_y;
		if (del_y % 2 == 0)
		{
			del_y = del_y / 2;
			while (del_y--)
			{
				y += ystep;
				sym_y -= ystep;
				window.SetPixel(static_cast<int>(x), y, I);
				window.SetPixel(static_cast<int>(x), sym_y, I);
			}
		}
		else
		{
			del_y = del_y / 2;
			while (del_y--)
			{
				y += ystep;
				sym_y -= ystep;
				window.SetPixel(static_cast<int>(x), y, I);
				window.SetPixel(static_cast<int>(x), sym_y, I);
			}
			y += ystep;
			window.SetPixel(static_cast<int>(x), y, I);
		}
	}
	//m > 1
	else if (abs(dy) > abs(dx))
	{
		float rm = dx / dy, b = y0 - x0 / rm;
		int y = static_cast<int>(round(y0));
		int sym_y = static_cast<int>((round(y1)));
		int del_y = static_cast<int>(round(y1) - y);
		int ystep = (dy >= 0) ? 1 : -1;
		del_y = (del_y >= 0) ? del_y : -del_y;
		float x = (y - b)*rm + 0.5f;
		float sym_x = (sym_y - b)*rm + 0.5f;
		rm = (rm >= 0.f) ? rm : -rm;
		float xstep = (dx >= 0.f) ? rm : -rm;
		window.SetPixel(static_cast<int>(x), y, I);
		if (del_y % 2 == 0)
		{
			del_y = del_y / 2;
			while (del_y--)
			{
				x += xstep;
				y += ystep;
				sym_x -= xstep;
				sym_y -= ystep;
				window.SetPixel(static_cast<int>(x), y, I);
				window.SetPixel(static_cast<int>(sym_x), sym_y, I);
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
				window.SetPixel(static_cast<int>(x), y, I);
				window.SetPixel(static_cast<int>(sym_x), sym_y, I);
			}
			x += xstep;
			y += ystep;
			window.SetPixel(static_cast<int>(x), y, I);
		}
	}

	//0 <= |m| <= 1
	else if (abs(dy) <= abs(dx))
	{
		float m = dy / dx, b = y0 - m * x0;
		int x = static_cast<int>(round(x0));
		int sym_x = static_cast<int>(round(x1));
		int del_x = static_cast<int>(round(x1) - x);
		int xstep = (dx >= 0) ? 1 : -1;
		del_x = (del_x >= 0) ? del_x : -del_x;
		float y = m * static_cast<float>(x) + b + 0.5f;
		float sym_y = m * static_cast<float>(sym_x) + b + 0.5f;
		m = (m >= 0.f) ? m : -m;
		float ystep = (dy >= 0.f) ? m : -m;
		window.SetPixel(x, static_cast<int>(y), I);
		if (del_x % 2 == 0)
		{
			del_x = del_x / 2;
			while (del_x--)
			{
				x += xstep;
				y += ystep;
				sym_x -= xstep;
				sym_y -= ystep;
				window.SetPixel(x, static_cast<int>(y), I);
				window.SetPixel(sym_x, static_cast<int>(sym_y), I);
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
				window.SetPixel(x, static_cast<int>(y), I);
				window.SetPixel(sym_x, static_cast<int>(sym_y), I);
			}
			x += xstep;
			y += ystep;
			window.SetPixel(x, static_cast<int>(y), I);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Routine for drawing a line from start vertex mStartVtx to end
			vertex mEndVtx using the Midpoint/Bresenham algorithm
    @param  window
            The window to draw to
	@Use	Use the following data members of class Line2D
			mStartVtx: start vertex
			mEndVtx: end vertex
			Commented code below shows how to draw a horizontal line ...
*/
////////////////////////////////////////////////////////////////////////////////
void Line2D::DrawMidpoint(const Window& window) const
{
	int x0 = static_cast<int>(round(GetStartVertex().GetPosition().GetX()));
	int y0 = static_cast<int>(round(GetStartVertex().GetPosition().GetY()));
	int x1 = static_cast<int>(round(GetEndVertex().GetPosition().GetX()));
	int y1 = static_cast<int>(round(GetEndVertex().GetPosition().GetY()));
	Color I = mStartVtx.GetColor();

	int dx = x1 - x0, dy = y1 - y0, x = x0, y = y0, sym_x = x1, sym_y = y1;
	int ystep = (dy >= 0) ? 1 : -1;
	int xstep = (dx >= 0) ? 1 : -1;
	dx = (dx >= 0) ? dx : -dx;
	dy = (dy >= 0) ? dy : -dy;

	//m > 1
	if (abs(dy) > abs(dx))
	{
		window.SetPixel(x, y, I);
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
				window.SetPixel(x, y, I);
				window.SetPixel(sym_x, sym_y, I);
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
				window.SetPixel(x, y, I);
				window.SetPixel(sym_x, sym_y, I);
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
			window.SetPixel(x, y, I);
		}
	}
	//0 <= |m| <= 1
	else if (abs(dy) <= abs(dx))
	{
		window.SetPixel(x, y, I);
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
				window.SetPixel(x, y, I);
				window.SetPixel(sym_x, sym_y, I);
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
				window.SetPixel(x, y, I);
				window.SetPixel(sym_x, sym_y, I);
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////////////
/*
			Nothing more to implement below
*/
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Intersection test between an arbitrary point and 2d line segment.
	@param	const Point2D& pt
			Arbitrary point
	@return	true if point is on line, that is, if absolute distance of point from line
			is a smaller value; false otherwise
*/
////////////////////////////////////////////////////////////////////////////////
bool Line2D::TestLineVsPoint(const Point2D& pt) const
{
	float width	=	5.f;

	Point2D p0 = mStartVtx.GetPosition(), p1 = mEndVtx.GetPosition();
	Point2D e(p1 - p0);

	if (e.MagnitudeSq() < 1.f) return false;

	Point2D n(-e.GetY(), e.GetX());
	n.Normalize();
	float dist = n*pt - n*p0;
	dist = (dist >= 0.f) ? dist : -dist;
	if (dist > width) return false;

	Point2D	dn(n.GetX()*width, n.GetY()*width);
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

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Overloaded output operator <<
	@param	std::ostream& os
			Reference to an ostream object
	@param	const Line2D& l
			Reference to Line2D type
	@return	object of type ostream reference with value equivalent to the 
			ostream object against which the output operator is applied
*/
////////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream& os, const Line2D& ls)
{
	os << ls.mStartVtx;
	os << ls.mEndVtx;

	return os;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Displace line segment to new location.
	@param	const Vector2D& vec
			Displacement vector
	@return	None
*/
////////////////////////////////////////////////////////////////////////////////
void Line2D::Move(const Vector2D& vec)
{
	mStartVtx.SetPosition(mStartVtx.GetPosition() + vec);
	mEndVtx.SetPosition(mEndVtx.GetPosition() + vec);
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Computes run of line segment.
	@param	None
	@return	Difference between X coordinates of end and start points
*/
////////////////////////////////////////////////////////////////////////////////
float Line2D::GetDeltaX() const
{
	return (mEndVtx.GetPosition().GetX() - mStartVtx.GetPosition().GetX());
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Computes rise of line segment.
	@param	None
	@return	Difference between Y coordinates of end and start points
*/
////////////////////////////////////////////////////////////////////////////////
float Line2D::GetDeltaY() const
{
	return (mEndVtx.GetPosition().GetY() - mStartVtx.GetPosition().GetY());
}

}   // namespace CS200