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
#include "Triangle.hpp"

namespace CS200
{

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Determines if triangle vertices mVertex[0], mVertex[1], mVertex[2] are
			counter-clockwise ordered
	@param	None
	@return	bool
			Return true if vertices are counter-clockwise; otherwise false
		
	@Notes	1. Unlike class notes, in this assignment, triangle vertices are
			defined in device coordinates with X-axis oriented right and
			Y-axis oriented down.
*/
////////////////////////////////////////////////////////////////////////////////
bool Triangle::IsCounterClockwise() const
{
	float x1 = GetVertex(1).GetPosition().GetX() - GetVertex(0).GetPosition().GetX();
	float y1 = GetVertex(1).GetPosition().GetY() - GetVertex(0).GetPosition().GetY();
	float x2 = GetVertex(2).GetPosition().GetX() - GetVertex(0).GetPosition().GetX();
	float y2 = GetVertex(2).GetPosition().GetY() - GetVertex(0).GetPosition().GetY();
	float determinant = x1 * y2 - y1 * x2;
	if(determinant < 0)
	{
		return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////
/*!
    @brief  Renders Triangle object to input window
	@param	window
			Window to draw to
	@Notes	1. Discard clockwise oriented triangles.
			2. Use top-left fill rule to find interior pixel coordinates.
			3. Implement sub-pixel sampling for starting and ending scan-lines
			   and starting and ending x-values of a span.
			4. Linearly interpolate vertex colors to compute pixel color.
*/
////////////////////////////////////////////////////////////////////////////////

bool IsTriangleLeft(Point2D& top, Point2D& mid, Point2D& bot)
{
	float slopetobottom = (top.GetX() - bot.GetX()) / (top.GetY() - bot.GetY());
	float slopetomiddle = (top.GetX() - mid.GetX()) / (top.GetY() - mid.GetY());
	if (top.GetX() == mid.GetX())
	{
		return true;
	}
	else if (slopetobottom >= slopetomiddle)
	{

		return true;
	}
	else
	{
		return false;
	}
}

void Triangle::Draw(const Window& window) const
{
	// @TODO: Scan-convert triangle using linear interpolation of color attributes.
	// Ensure that the top-left fill rule and sub-pixel sampling is implemented.
	Vertex2D topVertex;
	Vertex2D middleVertex;
	Vertex2D bottomVertex;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //find out top, bottom, middle vertices
	if (mVertex[0].GetPosition().GetY() > mVertex[1].GetPosition().GetY())
	{
		topVertex = mVertex[1];
		if (topVertex.GetPosition().GetY() > mVertex[2].GetPosition().GetY())
		{
			topVertex = mVertex[2];
			if (mVertex[0].GetPosition().GetY() > mVertex[1].GetPosition().GetY())
			{
				middleVertex = mVertex[1];
				bottomVertex = mVertex[0];
			}
			else
			{
				middleVertex = mVertex[0];
				bottomVertex = mVertex[1];
			}
		}
		else
		{
			if (mVertex[0].GetPosition().GetY() > mVertex[2].GetPosition().GetY())
			{
				middleVertex = mVertex[2];
				bottomVertex = mVertex[0];
			}
			else
			{
				middleVertex = mVertex[0];
				bottomVertex = mVertex[2];
			}
		}
	}
	else
	{
		topVertex = mVertex[0];
		if (topVertex.GetPosition().GetY() > mVertex[2].GetPosition().GetY())
		{
			topVertex = mVertex[2];
			if (mVertex[0].GetPosition().GetY() > mVertex[1].GetPosition().GetY())
			{
				middleVertex = mVertex[1];
				bottomVertex = mVertex[0];
			}
			else
			{
				middleVertex = mVertex[0];
				bottomVertex = mVertex[1];
			}
		}
		else
		{
			if (mVertex[1].GetPosition().GetY() > mVertex[2].GetPosition().GetY())
			{
				middleVertex = mVertex[2];
				bottomVertex = mVertex[1];
			}
			else
			{
				middleVertex = mVertex[1];
				bottomVertex = mVertex[2];
			}
		}
	}

	if (topVertex.GetPosition().GetY() == middleVertex.GetPosition().GetY())
	{
		if (topVertex.GetPosition().GetX() > middleVertex.GetPosition().GetX())
		{
			Vertex2D temp = topVertex;
			topVertex = middleVertex;
			middleVertex = temp;
		}
	}
	if (bottomVertex.GetPosition().GetY() == middleVertex.GetPosition().GetY())
	{
		if (middleVertex.GetPosition().GetY() > bottomVertex.GetPosition().GetY())
		{
			Vertex2D temp = bottomVertex;
			bottomVertex = middleVertex;
			middleVertex = temp;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //Declarations for initial values
		  //topVertex
	float y_top = topVertex.GetPosition().GetY();
	float x_top = topVertex.GetPosition().GetX();
	//middleVertex
	float y_mid = middleVertex.GetPosition().GetY();
	float x_mid = middleVertex.GetPosition().GetX();
	//bottomVertex
	float y_bot = bottomVertex.GetPosition().GetY();
	float x_bot = bottomVertex.GetPosition().GetX();
	//top Color
	float r_top = topVertex.GetColor().GetR();
	float g_top = topVertex.GetColor().GetG();
	float b_top = topVertex.GetColor().GetB();
	//middle Color
	float r_mid = middleVertex.GetColor().GetR();
	float g_mid = middleVertex.GetColor().GetG();
	float b_mid = middleVertex.GetColor().GetB();
	//bottom Color
	float r_bot = bottomVertex.GetColor().GetR();
	float g_bot = bottomVertex.GetColor().GetG();
	float b_bot = bottomVertex.GetColor().GetB();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //left triangle
	if (IsTriangleLeft(topVertex.GetPosition(), middleVertex.GetPosition(), bottomVertex.GetPosition()))
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Decalaration for left edge and right edge values
		//Initial values are the values that top, mid, and bot vertices are on different y-line
		float x_left = x_top;
		float r_left = r_top;
		float g_left = g_top;
		float b_left = b_top;

		float x_right = x_top;
		float r_right = r_top;
		float g_right = g_top;
		float b_right = b_top;

		//If top and middle vertices are on same y-line
		if (y_top == y_mid)
		{
			x_right = x_mid;
			r_right = r_mid;
			g_right = g_mid;
			b_right = b_mid;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Decalaration that is used in SETPIXEL
		float r;
		float g;
		float b;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Compute dA/dy and dA/dx
		//For computing easily
		float denominator = (x_mid - x_top)*(y_bot - y_top) - (x_bot - x_top)*(y_mid - y_top);

		//Actual interpolants
		float dr_dx = -1 * ((r_bot - r_top)*(y_mid - y_top) - (r_mid - r_top)*(y_bot - y_top)) / denominator;
		float dg_dx = -1 * ((g_bot - g_top)*(y_mid - y_top) - (g_mid - g_top)*(y_bot - y_top)) / denominator;
		float db_dx = -1 * ((b_bot - b_top)*(y_mid - y_top) - (b_mid - b_top)*(y_bot - y_top)) / denominator;

		float dr_dy = -1 * ((x_bot - x_top)*(r_mid - r_top) - (x_mid - x_top)*(r_bot - r_top)) / denominator;
		float dg_dy = -1 * ((x_bot - x_top)*(g_mid - g_top) - (x_mid - x_top)*(g_bot - g_top)) / denominator;
		float db_dy = -1 * ((x_bot - x_top)*(b_mid - b_top) - (x_mid - x_top)*(b_bot - b_top)) / denominator;

		float left_inverse_slope = (x_mid - x_top) / (y_mid - y_top);
		float right_inverse_slope = (x_bot - x_top) / (y_bot - y_top);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//top half
		for (int y = static_cast<int>(ceil(y_top));
			y <= static_cast<int>(ceil(y_mid)) - 1;
			y++)
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			r = r_left;
			g = g_left;
			b = b_left;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Span
			for (int x = static_cast<int>(ceil(x_left));
				x <= static_cast<int>(ceil(x_right)) - 1;
				x++)
			{
				Color color(static_cast<int>(floor(r)), static_cast<int>(floor(g)), static_cast<int>(floor(b)));

				window.SetPixel(x, y, color);
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Incremental Calculation
				r += dr_dx;
				g += dg_dx;
				b += db_dx;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incremental Calculation for y movement
			r_left += dr_dy;
			g_left += dg_dy;
			b_left += db_dy;

			r_right += dr_dy;
			g_right += dg_dy;
			b_right += db_dy;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incrmental Calculation for x movement
			x_left += left_inverse_slope;
			r_left += left_inverse_slope * dr_dx;
			g_left += left_inverse_slope * dg_dx;
			b_left += left_inverse_slope * db_dx;

			x_right += right_inverse_slope;
			r_right += right_inverse_slope * dr_dx;
			g_right += right_inverse_slope * dg_dx;
			b_right += right_inverse_slope * db_dx;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Compute dA/dy and dA/dx
		//For computing easily
		denominator = (x_bot - x_mid)*(y_bot - y_top) - (x_bot - x_top)*(y_bot - y_mid);

		//Actual interpolants
		dr_dx = -1 * ((y_bot - y_mid) * (r_bot - r_top) - (y_bot - y_top) * (r_bot - r_mid)) / denominator;
		dg_dx = -1 * ((y_bot - y_mid) * (g_bot - g_top) - (y_bot - y_top) * (g_bot - g_mid)) / denominator;
		db_dx = -1 * ((y_bot - y_mid) * (b_bot - b_top) - (y_bot - y_top) * (b_bot - b_mid)) / denominator;

		dr_dy = -1 * ((x_bot - x_top) * (r_bot - r_mid) - (x_bot - x_mid) * (r_bot - r_top)) / denominator;
		dg_dy = -1 * ((x_bot - x_top) * (g_bot - g_mid) - (x_bot - x_mid) * (g_bot - g_top)) / denominator;
		db_dy = -1 * ((x_bot - x_top) * (b_bot - b_mid) - (x_bot - x_mid) * (b_bot - b_top)) / denominator;

		left_inverse_slope = (x_bot - x_mid) / (y_bot - y_mid);
		right_inverse_slope = (x_bot - x_top) / (y_bot - y_top);

		if (y_top == y_mid)
		{
			left_inverse_slope = (x_top - x_bot) / (y_top - y_bot);
			right_inverse_slope = (x_mid - x_bot) / (y_mid - y_bot);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//bottom half
		for (int y = static_cast<int>(ceil(y_mid));
			y <= static_cast<int>(ceil(y_bot)) - 1;
			y++)
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			r = r_left;
			g = g_left;
			b = b_left;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (int x = static_cast<int>(ceil(x_left));
				x <= static_cast<int>(ceil(x_right)) - 1;
				x++)
			{
				Color color(static_cast<int>(floor(r)), static_cast<int>(floor(g)), static_cast<int>(floor(b)));

				window.SetPixel(x, y, color);
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Incremental Calculation
				r += dr_dx;
				g += dg_dx;
				b += db_dx;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incremental Calculation for y movement
			r_left += dr_dy;
			g_left += dg_dy;
			b_left += db_dy;

			r_right += dr_dy;
			g_right += dg_dy;
			b_right += db_dy;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incrmental Calculation for x movement
			x_left += left_inverse_slope;
			r_left += left_inverse_slope * dr_dx;
			g_left += left_inverse_slope * dg_dx;
			b_left += left_inverse_slope * db_dx;

			x_right += right_inverse_slope;
			r_right += right_inverse_slope * dr_dx;
			g_right += right_inverse_slope * dg_dx;
			b_right += right_inverse_slope * db_dx;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //right triangle
	else
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Decalaration for left edge and right edge values
		//Initial values are the values that top, mid, and bot vertices are on different y-line
		float r_left = r_top;
		float g_left = g_top;
		float b_left = b_top;

		float r_right = r_top;
		float g_right = g_top;
		float b_right = b_top;

		float x_left = x_top
			, x_right = x_top;

		//If top and middle vertices are on same y-line
		if (y_top == y_mid)
		{
			x_right = x_mid;
			r_right = r_mid;
			g_right = g_mid;
			b_right = b_mid;
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Decalaration that is used in SETPIXEL
		float r;
		float g;
		float b;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Compute dA/dy and dA/dx
		//For computing easily
		float denominator = (x_bot - x_top) * (y_mid - y_top) - (x_mid - x_top) * (y_bot - y_top);

		//Actual interpolants
		float dr_dx = -1 * ((r_mid - r_top)*(y_bot - y_top) - (r_bot - r_top)*(y_mid - y_top)) / denominator;
		float dg_dx = -1 * ((g_mid - g_top)*(y_bot - y_top) - (g_bot - g_top)*(y_mid - y_top)) / denominator;
		float db_dx = -1 * ((b_mid - b_top)*(y_bot - y_top) - (b_bot - b_top)*(y_mid - y_top)) / denominator;

		float dr_dy = -1 * ((x_mid - x_top)*(r_bot - r_top) - (x_bot - x_top)*(r_mid - r_top)) / denominator;
		float dg_dy = -1 * ((x_mid - x_top)*(g_bot - g_top) - (x_bot - x_top)*(g_mid - g_top)) / denominator;
		float db_dy = -1 * ((x_mid - x_top)*(b_bot - b_top) - (x_bot - x_top)*(b_mid - b_top)) / denominator;

		float left_inverse_slope = (x_bot - x_top) / (y_bot - y_top);
		float right_inverse_slope = (x_mid - x_top) / (y_mid - y_top);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//top half
		for (int y = static_cast<int>(ceil(y_top));
			y <= static_cast<int>(ceil(y_mid)) - 1;
			y++)
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			r = r_left;
			g = g_left;
			b = b_left;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (int x = static_cast<int>(ceil(x_left));
				x <= static_cast<int>(ceil(x_right)) - 1;
				x++)
			{
				Color color(static_cast<int>(floor(r)), static_cast<int>(floor(g)), static_cast<int>(floor(b)));

				window.SetPixel(x, y, color);
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Incremental Calculation
				r += dr_dx;
				g += dg_dx;
				b += db_dx;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incremental Calculation for y movement
			r_left += dr_dy;
			g_left += dg_dy;
			b_left += db_dy;

			r_right += dr_dy;
			g_right += dg_dy;
			b_right += db_dy;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incrmental Calculation for x movement
			x_left += left_inverse_slope;
			r_left += left_inverse_slope * dr_dx;
			g_left += left_inverse_slope * dg_dx;
			b_left += left_inverse_slope * db_dx;

			x_right += right_inverse_slope;
			r_right += right_inverse_slope * dr_dx;
			g_right += right_inverse_slope * dg_dx;
			b_right += right_inverse_slope * db_dx;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//Compute dA/dy and dA/dx
		//For computing easily
		denominator = (x_bot - x_top) * (y_bot - y_mid) - (x_bot - x_mid) * (y_bot - y_top);

		//Actual interpolants
		dr_dx = -1 * ((y_bot - y_top) * (r_bot - r_mid) - (y_bot - y_mid) * (r_bot - r_top)) / denominator;
		dg_dx = -1 * ((y_bot - y_top) * (g_bot - g_mid) - (y_bot - y_mid) * (g_bot - g_top)) / denominator;
		db_dx = -1 * ((y_bot - y_top) * (b_bot - b_mid) - (y_bot - y_mid) * (b_bot - b_top)) / denominator;

		dr_dy = -1 * ((x_bot - x_mid) * (r_bot - r_top) - (x_bot - x_top) * (r_bot - r_mid)) / denominator;
		dg_dy = -1 * ((x_bot - x_mid) * (g_bot - g_top) - (x_bot - x_top) * (g_bot - g_mid)) / denominator;
		db_dy = -1 * ((x_bot - x_mid) * (b_bot - b_top) - (x_bot - x_top) * (b_bot - b_mid)) / denominator;

		left_inverse_slope = (x_bot - x_top) / (y_bot - y_top);
		right_inverse_slope = (x_bot - x_mid) / (y_bot - y_mid);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//bottom half
		for (int y = static_cast<int>(ceil(y_mid));
			y <= static_cast<int>(ceil(y_bot)) - 1;
			y++)
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			r = r_left;
			g = g_left;
			b = b_left;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (int x = static_cast<int>(ceil(x_left));
				x <= static_cast<int>(ceil(x_right));
				x++)
			{
				Color color(static_cast<int>(floor(r)), static_cast<int>(floor(g)), static_cast<int>(floor(b)));

				window.SetPixel(x, y, color);
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//Incremental Calculation
				r += dr_dx;
				g += dg_dx;
				b += db_dx;
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incremental Calculation for y movement
			r_left += dr_dy;
			g_left += dg_dy;
			b_left += db_dy;

			r_right += dr_dy;
			g_right += dg_dy;
			b_right += db_dy;
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//Incrmental Calculation for x movement
			x_left += left_inverse_slope;
			r_left += left_inverse_slope * dr_dx;
			g_left += left_inverse_slope * dg_dx;
			b_left += left_inverse_slope * db_dx;

			x_right += right_inverse_slope;
			r_right += right_inverse_slope * dr_dx;
			g_right += right_inverse_slope * dg_dx;
			b_right += right_inverse_slope * db_dx;
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
	}
}

}	// namespace CS200
