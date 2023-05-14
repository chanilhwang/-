/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: triangle.cpp
Purpose: Implement functions for triangle drawing.
Language: C++
Platform: Visual Studio 2017, Windows 10 64-bit
Project: c.hwang_cs200_a4
Author: c.hwang / Hwang Chan Il
Creation date: 11/05/2018
- End Header ----------------------------------------------------------------*/


#include "triangle.hpp"
#include "graphics.hpp"


/******************************************************************************/
/*!
\fn     IsCounterClockwise
\brief
        Check whether a triangle is counter-clockwise in device frame.
\param  unsigned char key
        Pressed key.
\param  int x
        Current x-position of the mouse cursor.
\param  int y
        Current y-position of the mouse cursor (from top).
\return
        True if counter-clockwise, false otherwise.
*/
/******************************************************************************/
bool Triangle::IsCounterClockwise() const
{
	float x1 = GetVertex(1).GetPosition().GetX() - GetVertex(0).GetPosition().GetX();
	float y1 = GetVertex(1).GetPosition().GetY() - GetVertex(0).GetPosition().GetY();
	float x2 = GetVertex(2).GetPosition().GetX() - GetVertex(0).GetPosition().GetX();
	float y2 = GetVertex(2).GetPosition().GetY() - GetVertex(0).GetPosition().GetY();

	float determinant = x1 * y2 - y1 * x2;

	if (determinant < 0)
	{
		return true;
	}

    return false;
}


/******************************************************************************/
/*!
\fn     Draw
\brief
        Draw a triangle in device-frame coordinate using top-left tie-breaking
        rule. If this rule is implemented correctly, the right and bottom edges
        of the two testing triangles should not be filled. Also notice the clear
        demarcation of edge pixels between those two triangles.
        Check by pressing TAB to toggle between draw mode and test mode.
\param  unsigned char *frameBuffer
        The framebuffer texture whose pixel colors are set for drawing.
*/
/******************************************************************************/
void Triangle::Draw(unsigned char *frameBuffer) const
{
	Vertex2D topVertex = GetVertex(0);
	Vertex2D middleVertex = GetVertex(1);
	Vertex2D bottomVertex = GetVertex(2);

	//find out top, bottom, middle vertices
	if (topVertex.GetPosition().GetY() > middleVertex.GetPosition().GetY())
	{
		Vertex2D temp;
		temp = topVertex;
		topVertex = middleVertex;
		middleVertex = temp;
	}
	if (topVertex.GetPosition().GetY() > bottomVertex.GetPosition().GetY())
	{
		Vertex2D temp;
		temp = topVertex;
		topVertex = bottomVertex;
		bottomVertex = temp;
	}
	if (middleVertex.GetPosition().GetY() > bottomVertex.GetPosition().GetY())
	{
		Vertex2D temp;
		temp = middleVertex;
		middleVertex = bottomVertex;
		bottomVertex = temp;
	}

	//topVertex
	float yt = topVertex.GetPosition().GetY();
	float xt = topVertex.GetPosition().GetX();
	//middleVertex
	float ym = middleVertex.GetPosition().GetY();
	float xm = middleVertex.GetPosition().GetX();
	//bottomVertex
	float yb = bottomVertex.GetPosition().GetY();
	float xb = bottomVertex.GetPosition().GetX();
	//top Color
	float rt = topVertex.GetColor().GetR();
	float gt = topVertex.GetColor().GetG();
	float bt = topVertex.GetColor().GetB();
	//middle Color
	float rm = middleVertex.GetColor().GetR();
	float gm = middleVertex.GetColor().GetG();
	float bm = middleVertex.GetColor().GetB();
	//bottom Color
	float rb = bottomVertex.GetColor().GetR();
	float gb = bottomVertex.GetColor().GetG();
	float bb = bottomVertex.GetColor().GetB();

	//left triangle
	if (xm <= xb)
	{
		//Initializing part
		//If there are both top half and bottom half
		float rl = rt;
		float gl = gt;
		float bl = bt;

		float rr = rt;
		float gr = gt;
		float br = bt;

		float xl = xt
			, xr = xt;

		//If top vertex and middle vertex is on same span,
		if (yt == ym)
		{
			//left one is top, and right one is middle
			if (xt < xm)
			{
				xl = xt;
				rl = rt;
				gl = gt;
				bl = bt;

				xr = xm;
				rr = rm;
				gr = gm;
				br = bm;
			}
			else
			{
				xl = xm;
				rl = rm;
				gl = gm;
				bl = bm;

				xr = xt;
				rr = rt;
				gr = gt;
				br = bt;
			}
		}

		float drl = (rt - rm) / (yt - ym);
		float dgl = (gt - gm) / (yt - ym);
		float dbl = (bt - bm) / (yt - ym);
		float drr = (rt - rb) / (yt - yb);
		float dgr = (gt - gb) / (yt - yb);
		float dbr = (bt - bb) / (yt - yb);

		float dxl = (xt - xm) / (yt - ym);
		float dxr = (xt - xb) / (yt - yb);

		//top half
		for (int y = ceil(yt); y <= ceil(ym) - 1 ; y++)
		{
			float drs = (rr - rl) / (xr - xl);
			float dgs = (gr - gl) / (xr - xl);
			float dbs = (br - bl) / (xr - xl);

			float r = rl;
			float g = gl;
			float b = bl;

			for (int x = ceil(xl);
				x <= ceil(xr) - 1;
				x++)
			{
				r += drs;
				g += dgs;
				b += dbs;

				Color color(floor(r), floor(g), floor(b));
				SetPixel(frameBuffer, x, y, color);
			}

			rl += drl;
			rr += drr;
			gl += dgl;
			gr += dgr;
			bl += dbl;
			br += dbr;

			xl += dxl;
			xr += dxr;
		}

		drl = (rb - rm) / (yb - ym);
		dbl = (bb - bm) / (yb - ym);
		dgl = (gb - gm) / (yb - ym);
		dxl = (xm - xb) / (ym - yb);
		
		//bottom half
		for (int y = ceil(ym); y <= ceil(yb) - 1; y++)
		{
			float drs = (rr - rl) / (xr - xl);
			float dgs = (gr - gl) / (xr - xl);
			float dbs = (br - bl) / (xr - xl);

			float r = rl;
			float g = gl;
			float b = bl;

			for (int x = ceil(xl);
				x <= ceil(xr) - 1;
				x++)
			{
				r += drs;
				g += dgs;
				b += dbs;

				Color color(floor(r), floor(g), floor(b));
				SetPixel(frameBuffer, x, y, color);
			}
			rl += drl;
			rr += drr;
			gl += dgl;
			gr += dgr;
			bl += dbl;
			br += dbr;
			xl += dxl;
			xr += dxr;
		}
	}
	//right triangle
	else
	{
		//Initializing part
		//If there are both top half and bottom half
		float rl = rt;
		float gl = gt;
		float bl = bt;

		float rr = rt;
		float gr = gt;
		float br = bt;

		float xl = xt
			, xr = xt;

		//If top vertex and middle vertex is on same span,
		if (yt == ym)
		{
			//left one is top, and right one is middle
			if (xt < xm)
			{
				xl = xt;
				rl = rt;
				gl = gt;
				bl = bt;

				xr = xm;
				rr = rm;
				gr = gm;
				br = bm;
			}
			else
			{
				xl = xm;
				rl = rm;
				gl = gm;
				bl = bm;

				xr = xt;
				rr = rt;
				gr = gt;
				br = bt;
			}
		}

		float drl = (rt - rb) / (yt - yb);
		float dgl = (gt - gb) / (yt - yb);
		float dbl = (bt - bb) / (yt - yb);
		float drr = (rt - rm) / (yt - ym);
		float dgr = (gt - gm) / (yt - ym);
		float dbr = (bt - bm) / (yt - ym);

		float dxl = (xt - xb) / (yt - yb);
		float dxr = (xt - xm) / (yt - ym);

		//top half
		for (int y = ceil(yt); y <= ceil(ym) - 1; y++)
		{
			float drs = (rr - rl) / (xr - xl);
			float dgs = (gr - gl) / (xr - xl);
			float dbs = (br - bl) / (xr - xl);

			float r = rl;
			float g = gl;
			float b = bl;

			for (int x = ceil(xl);
				x <= ceil(xr) - 1;
				x++)
			{
				r += drs;
				g += dgs;
				b += dbs;

				Color color(floor(r), floor(g), floor(b));
				SetPixel(frameBuffer, x, y, color);
			}

			rl += drl;
			rr += drr;
			gl += dgl;
			gr += dgr;
			bl += dbl;
			br += dbr;

			xl += dxl;
			xr += dxr;
		}

		drr = (rb - rm) / (yb - ym);
		dbr = (bb - bm) / (yb - ym);
		dgr = (gb - gm) / (yb - ym);
		dxr = (xm - xb) / (ym - yb);

		//bottom half
		for (int y = ceil(ym); y <= ceil(yb) - 1; y++)
		{
			float drs = (rr - rl) / (xr - xl);
			float dgs = (gr - gl) / (xr - xl);
			float dbs = (br - bl) / (xr - xl);

			float r = rl;
			float g = gl;
			float b = bl;

			for (int x = ceil(xl);
				x <= ceil(xr) - 1;
				x++)
			{
				r += drs;
				g += dgs;
				b += dbs;

				Color color(floor(r), floor(g), floor(b));
				SetPixel(frameBuffer, x, y, color);
			}
			rl += drl;
			rr += drr;
			gl += dgl;
			gr += dgr;
			bl += dbl;
			br += dbr;
			xl += dxl;
			xr += dxr;
		}
	}
}

