/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Triangle.cpp
Purpose: To draw triangle with texture and modify textures in various way.
Language: c++
Platform: Window10 64bit / Studio 2017
Project: c.hwang_CS200_A4
Author: c.hwang
Creation date: 11/24/2018
- End Header ----------------------------------------------------------------*/

// Includes
#include "Triangle.hpp"

namespace CS200
{
  extern Texture					gTexMapList[TEXTURE_MAX];	// array of texture maps
  extern enum TextureType	gTexMapType;				// index to texture map	 array

  ////////////////////////////////////////////////////////////////////////////////
  /*!
      @brief  Renders the Triangle object to the input window
    @param	window
        Window to draw to

    @Notes	1. Note that the application discards clockwise oriented triangles.
        2. Use top-left rule as tie-breaking rule.
        3. Assume triangle vertices are real. Make adjustments for x-, r-, g-, and b-
          values at start and middle scan lines and for span limits.
        4. Linearly (not bi-linearly) interpolate vertex colors and texture coordinates.
          See Vertex2D.hpp for declaration of triangle vertex.
        5. After computing interpolated r, g, b, and texture coordinates u, v, compute
          pixel color using one of the following color combiner modes:
          Replace, Decal, Modulate, Gourard (that is, only vertex color interpolation
          or, no texture mapping).
        6. The user selected texture map is defined by variable gTexMapType of type enum TexType.
          Texture maps are stored in array gTexMapList[]. The texel color at texel coordinates
          (uint, vint) is accessed as follows: gTexMapList[gTexMapType].GetTexel(uint, vint) ...
          pSee Texture.hpp and Texture.cpp to for more information on reading and writing
          texel colors from or to texture map.
        7. The following texture functions must be implemented for the u and v
          texture coordinates: clamp, repeat, and mirrored repeat.
          Enumerated types TexFuncUMode and TexFuncVMode describe each of these 3 modes for
          the u and v coordinates, respectively.
        8. Note that mirrored repeat "flip-flops" the texture map. For instance, within
          texture coordinate range [0, 1], the texture map may be textured from left-
          to-right (or, from top-to-bottom). But, the mirrored repeat wrapping
          reorients the texture from right-to-left for texture coordinate range [1, 2],
          and then back again to left-to-right for coordinates [2, 3], and so on.
        9. Use "Test" in "Mode" menu to test your implementation.
          Test fills a 500 X 375 rectangle using 2 calls to your rasterizer. Notice
          that the right and bottom edges of the rectangle are not filled. Also. notice
          that there is no seam between the left and right triangles that together
          make up the rectangle.
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

  Color ComputeOutputColor(float r, float g, float b, float& u, float& v, Texture& tex, enum ColorCombineMode	ccMode,
																						enum TexFuncUMode		  uMode,
																						enum TexFuncVMode		  vMode	)
  {
	  float computed_u;
	  float computed_v;
	  switch (uMode)
	  {
	  case TEXFUNCU_CLAMP:
		  computed_u = max(min(u, 1.0), 0.0);
		  break;
	  case TEXFUNCU_REPEAT:
		  computed_u = u - floor(u);
		  break;
	  case TEXFUNCU_MIRROREDREPEAT:
		  bool IsFlipped = (((static_cast<int>(floor(u))) % 2 == 1));
		  if (IsFlipped)
		  {
			  computed_u = 1 - (u - floor(u));
		  }
		  else
		  {
			  computed_u = u - floor(u);
		  }
	  }

	  switch (vMode)
	  {
	  case TEXFUNCV_CLAMP:
		  computed_v = max(min(v, 1.0), 0.0);
		  break;
	  case TEXFUNCV_REPEAT:
		  computed_v = v - floor(v);
		  break;
	  case TEXFUNCV_MIRROREDREPEAT:
		  bool IsFlipped = (((static_cast<int>(floor(v))) % 2 == 1));
		  if (IsFlipped)
		  {
			  computed_v = 1 - (v - floor(v));
		  }
		  else
		  {
			  computed_v = v - floor(v);
		  }
	  }

	  int uINT = static_cast<int>(round(computed_u * tex.GetWidth() - 0.5f) + 0.5f);
	  int vINT = static_cast<int>(round(computed_v * tex.GetHeight() - 0.5f) + 0.5f);

	  Color texture_color = tex.GetTexel(uINT, vINT);
	  Color vertex_color(static_cast<const unsigned char>(floor(r)),
		  static_cast<const unsigned char>(floor(g)),
		  static_cast<const unsigned char>(floor(b)));
	  Color combined_color;
	  switch (ccMode)
	  {
	  case CC_REPLACE:
		  combined_color = texture_color;
		  break;
	  case CC_DECAL:
		  combined_color = { static_cast<const unsigned char>(vertex_color.GetR() + texture_color.GetA() / 255 * (texture_color.GetR() - vertex_color.GetR())),
							static_cast<const unsigned char>(vertex_color.GetG() + texture_color.GetA() / 255 * (texture_color.GetG() - vertex_color.GetG())),
							static_cast<const unsigned char>(vertex_color.GetB() + texture_color.GetA() / 255 * (texture_color.GetB() - vertex_color.GetB())) };
		  break;
	  case CC_MODULATE:
		  combined_color = { static_cast<const unsigned char>(texture_color.GetR() * vertex_color.GetR() / 255),
							static_cast<const unsigned char>(texture_color.GetG() * vertex_color.GetG() / 255),
							static_cast<const unsigned char>(texture_color.GetB() * vertex_color.GetB() / 255) };
		  break;
	  case CC_GOURARD:
		  combined_color = vertex_color;
		  break;
	  }

	  return combined_color;
  }

  float ComputeInterpolant(Point2D& vector1, Point2D& vector2)
  {
	  return vector1.ScalarTripleProduct(vector2);
  }

  void Triangle::
    Draw(const Window&			window,
  enum ColorCombineMode	ccMode,
  enum TexFuncUMode		  uMode,
  enum TexFuncVMode		  vMode) const
  {
	  Vertex2D topVertex;
	  Vertex2D middleVertex;
	  Vertex2D bottomVertex;

	  Texture& tex = gTexMapList[gTexMapType];
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
	  //top texture coord
	  float u_top = topVertex.GetU();
	  float v_top = topVertex.GetV();
	  //middle texture coord
	  float u_mid = middleVertex.GetU();
	  float v_mid = middleVertex.GetV();
	  //bottom texture coord
	  float u_bot = bottomVertex.GetU();
	  float v_bot = bottomVertex.GetV();

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
		  float u_left = u_top;
		  float v_left = v_top;

		  float x_right = x_top;
		  float r_right = r_top;
		  float g_right = g_top;
		  float b_right = b_top;
		  float u_right = u_top;
		  float v_right = v_top;

		  //If top and middle vertices are on same y-line
		  if (y_top == y_mid)
		  {
			  x_right = x_mid;
			  r_right = r_mid;
			  g_right = g_mid;
			  b_right = b_mid;
			  u_right = u_mid;
			  v_right = v_mid;
		  }
		  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //Decalaration that is used in SETPIXEL
		  float r;
		  float g;
		  float b;
		  float u;
		  float v;
		  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //Compute dA/dy and dA/dx
		  //For computing easily
		  float denominator = (x_mid - x_top)*(y_bot - y_top) - (x_bot - x_top)*(y_mid - y_top);

		  //Actual interpolants
		  float dr_dx = -1 * ((r_bot - r_top)*(y_mid - y_top) - (r_mid - r_top)*(y_bot - y_top)) / denominator;
		  float dg_dx = -1 * ((g_bot - g_top)*(y_mid - y_top) - (g_mid - g_top)*(y_bot - y_top)) / denominator;
		  float db_dx = -1 * ((b_bot - b_top)*(y_mid - y_top) - (b_mid - b_top)*(y_bot - y_top)) / denominator;
		  float du_dx = -1 * ((u_bot - u_top)*(y_mid - y_top) - (u_mid - u_top)*(y_bot - y_top)) / denominator;
		  float dv_dx = -1 * ((v_bot - v_top)*(y_mid - y_top) - (v_mid - v_top)*(y_bot - y_top)) / denominator;

		  float dr_dy = -1 * ((x_bot - x_top)*(r_mid - r_top) - (x_mid - x_top)*(r_bot - r_top)) / denominator;
		  float dg_dy = -1 * ((x_bot - x_top)*(g_mid - g_top) - (x_mid - x_top)*(g_bot - g_top)) / denominator;
		  float db_dy = -1 * ((x_bot - x_top)*(b_mid - b_top) - (x_mid - x_top)*(b_bot - b_top)) / denominator;
		  float du_dy = -1 * ((x_bot - x_top)*(u_mid - u_top) - (x_mid - x_top)*(u_bot - u_top)) / denominator;
		  float dv_dy = -1 * ((x_bot - x_top)*(v_mid - v_top) - (x_mid - x_top)*(v_bot - v_top)) / denominator;

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
			  u = u_left;
			  v = v_left;
			  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Span
			  for (int x = static_cast<int>(ceil(x_left));
				  x <= static_cast<int>(ceil(x_right)) - 1;
				  x++)
			  {
				  Color combined_color = ComputeOutputColor(r, g, b, u, v, tex, ccMode, uMode, vMode);

				  window.SetPixel(x, y, combined_color);
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				  //Incremental Calculation
				  r += dr_dx;
				  g += dg_dx;
				  b += db_dx;
				  u += du_dx;
				  v += dv_dx;
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  }
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incremental Calculation for y movement
			  r_left += dr_dy;
			  g_left += dg_dy;
			  b_left += db_dy;
			  u_left += du_dy;
			  v_left += dv_dy;

			  r_right += dr_dy;
			  g_right += dg_dy;
			  b_right += db_dy;
			  u_right += du_dy;
			  v_right += dv_dy;
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incrmental Calculation for x movement
			  x_left += left_inverse_slope;
			  r_left += left_inverse_slope * dr_dx;
			  g_left += left_inverse_slope * dg_dx;
			  b_left += left_inverse_slope * db_dx;
			  u_left += left_inverse_slope * du_dx;
			  v_left += left_inverse_slope * dv_dx;

			  x_right += right_inverse_slope;
			  r_right += right_inverse_slope * dr_dx;
			  g_right += right_inverse_slope * dg_dx;
			  b_right += right_inverse_slope * db_dx;
			  u_right += right_inverse_slope * du_dx;
			  v_right += right_inverse_slope * dv_dx;
			  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  }
		  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //Compute dA/dy and dA/dx
		  //For computing easily
		  denominator = (x_bot-x_mid)*(y_bot-y_top) - (x_bot - x_top)*(y_bot - y_mid);

		  //Actual interpolants
		  dr_dx = -1 * ((y_bot - y_mid) * (r_bot - r_top) - (y_bot - y_top) * (r_bot - r_mid)) / denominator;
		  dg_dx = -1 * ((y_bot - y_mid) * (g_bot - g_top) - (y_bot - y_top) * (g_bot - g_mid)) / denominator;
		  db_dx = -1 * ((y_bot - y_mid) * (b_bot - b_top) - (y_bot - y_top) * (b_bot - b_mid)) / denominator;
		  du_dx = -1 * ((y_bot - y_mid) * (u_bot - u_top) - (y_bot - y_top) * (u_bot - u_mid)) / denominator;
		  dv_dx = -1 * ((y_bot - y_mid) * (v_bot - v_top) - (y_bot - y_top) * (v_bot - v_mid)) / denominator;

		  dr_dy = -1 * ((x_bot - x_top) * (r_bot - r_mid) - (x_bot - x_mid) * (r_bot - r_top)) / denominator;
		  dg_dy = -1 * ((x_bot - x_top) * (g_bot - g_mid) - (x_bot - x_mid) * (g_bot - g_top)) / denominator;
		  db_dy = -1 * ((x_bot - x_top) * (b_bot - b_mid) - (x_bot - x_mid) * (b_bot - b_top)) / denominator;
		  du_dy = -1 * ((x_bot - x_top) * (u_bot - u_mid) - (x_bot - x_mid) * (u_bot - u_top)) / denominator;
		  dv_dy = -1 * ((x_bot - x_top) * (v_bot - v_mid) - (x_bot - x_mid) * (v_bot - v_top)) / denominator;

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
			  u = u_left;
			  v = v_left;
			  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  for (int x = static_cast<int>(ceil(x_left));
				  x <= static_cast<int>(ceil(x_right)) - 1;
				  x++)
			  {
				  Color combined_color = ComputeOutputColor(r, g, b, u, v, tex, ccMode, uMode, vMode);

				  window.SetPixel(x, y, combined_color);
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				  //Incremental Calculation
				  r += dr_dx;
				  g += dg_dx;
				  b += db_dx;
				  u += du_dx;
				  v += dv_dx;
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  }
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incremental Calculation for y movement
			  r_left += dr_dy;
			  g_left += dg_dy;
			  b_left += db_dy;
			  u_left += du_dy;
			  v_left += dv_dy;

			  r_right += dr_dy;
			  g_right += dg_dy;
			  b_right += db_dy;
			  u_right += du_dy;
			  v_right += dv_dy;
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incrmental Calculation for x movement
			  x_left += left_inverse_slope;
			  r_left += left_inverse_slope * dr_dx;
			  g_left += left_inverse_slope * dg_dx;
			  b_left += left_inverse_slope * db_dx;
			  u_left += left_inverse_slope * du_dx;
			  v_left += left_inverse_slope * dv_dx;

			  x_right += right_inverse_slope;
			  r_right += right_inverse_slope * dr_dx;
			  g_right += right_inverse_slope * dg_dx;
			  b_right += right_inverse_slope * db_dx;
			  u_right += right_inverse_slope * du_dx;
			  v_right += right_inverse_slope * dv_dx;
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

		  float u_left = u_top
			  , u_right = u_top;
		  float v_left = v_top
			  , v_right = v_top;

		  //If top and middle vertices are on same y-line
		  if (y_top == y_mid)
		  {
			  x_right = x_mid;
			  r_right = r_mid;
			  g_right = g_mid;
			  b_right = b_mid;
			  u_right = u_mid;
			  v_right = v_mid;
		  }
		  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //Decalaration that is used in SETPIXEL
		  float r;
		  float g;
		  float b;
		  float u;
		  float v;
		  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  //Compute dA/dy and dA/dx
		  //For computing easily
		  float denominator = (x_bot - x_top) * (y_mid - y_top) - (x_mid - x_top) * (y_bot - y_top);

		  //Actual interpolants
		  float dr_dx = -1 * ((r_mid - r_top)*(y_bot - y_top) - (r_bot - r_top)*(y_mid - y_top)) / denominator;
		  float dg_dx = -1 * ((g_mid - g_top)*(y_bot - y_top) - (g_bot - g_top)*(y_mid - y_top)) / denominator;
		  float db_dx = -1 * ((b_mid - b_top)*(y_bot - y_top) - (b_bot - b_top)*(y_mid - y_top)) / denominator;
		  float du_dx = -1 * ((u_mid - u_top)*(y_bot - y_top) - (u_bot - u_top)*(y_mid - y_top)) / denominator;
		  float dv_dx = -1 * ((v_mid - v_top)*(y_bot - y_top) - (v_bot - v_top)*(y_mid - y_top)) / denominator;

		  float dr_dy = -1 * ((x_mid - x_top)*(r_bot - r_top) - (x_bot - x_top)*(r_mid - r_top)) / denominator;
		  float dg_dy = -1 * ((x_mid - x_top)*(g_bot - g_top) - (x_bot - x_top)*(g_mid - g_top)) / denominator;
		  float db_dy = -1 * ((x_mid - x_top)*(b_bot - b_top) - (x_bot - x_top)*(b_mid - b_top)) / denominator;
		  float du_dy = -1 * ((x_mid - x_top)*(u_bot - u_top) - (x_bot - x_top)*(u_mid - u_top)) / denominator;
		  float dv_dy = -1 * ((x_mid - x_top)*(v_bot - v_top) - (x_bot - x_top)*(v_mid - v_top)) / denominator;

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
			  u = u_left;
			  v = v_left;
			  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  for (int x = static_cast<int>(ceil(x_left));
				  x <= static_cast<int>(ceil(x_right)) - 1;
				  x++)
			  {
				  Color combined_color = ComputeOutputColor(r, g, b, u, v, tex, ccMode, uMode, vMode);

				  window.SetPixel(x, y, combined_color);
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				  //Incremental Calculation
				  r += dr_dx;
				  g += dg_dx;
				  b += db_dx;
				  u += du_dx;
				  v += dv_dx;
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  }
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incremental Calculation for y movement
			  r_left += dr_dy;
			  g_left += dg_dy;
			  b_left += db_dy;
			  u_left += du_dy;
			  v_left += dv_dy;

			  r_right += dr_dy;
			  g_right += dg_dy;
			  b_right += db_dy;
			  u_right += du_dy;
			  v_right += dv_dy;
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incrmental Calculation for x movement
			  x_left += left_inverse_slope;
			  r_left += left_inverse_slope * dr_dx;
			  g_left += left_inverse_slope * dg_dx;
			  b_left += left_inverse_slope * db_dx;
			  u_left += left_inverse_slope * du_dx;
			  v_left += left_inverse_slope * dv_dx;

			  x_right += right_inverse_slope;
			  r_right += right_inverse_slope * dr_dx;
			  g_right += right_inverse_slope * dg_dx;
			  b_right += right_inverse_slope * db_dx;
			  u_right += right_inverse_slope * du_dx;
			  v_right += right_inverse_slope * dv_dx;
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
		  du_dx = -1 * ((y_bot - y_top) * (u_bot - u_mid) - (y_bot - y_mid) * (u_bot - u_top)) / denominator;
		  dv_dx = -1 * ((y_bot - y_top) * (v_bot - v_mid) - (y_bot - y_mid) * (v_bot - v_top)) / denominator;

		  dr_dy = -1 * ((x_bot - x_mid) * (r_bot - r_top) - (x_bot - x_top) * (r_bot - r_mid)) / denominator;
		  dg_dy = -1 * ((x_bot - x_mid) * (g_bot - g_top) - (x_bot - x_top) * (g_bot - g_mid)) / denominator;
		  db_dy = -1 * ((x_bot - x_mid) * (b_bot - b_top) - (x_bot - x_top) * (b_bot - b_mid)) / denominator;
		  du_dy = -1 * ((x_bot - x_mid) * (u_bot - u_top) - (x_bot - x_top) * (u_bot - u_mid)) / denominator;
		  dv_dy = -1 * ((x_bot - x_mid) * (v_bot - v_top) - (x_bot - x_top) * (v_bot - v_mid)) / denominator;

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
			  u = u_left;
			  v = v_left;
			  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  for (int x = static_cast<int>(ceil(x_left));
				  x <= static_cast<int>(ceil(x_right)) - 1;
				  x++)
			  {
				  Color combined_color = ComputeOutputColor(r, g, b, u, v, tex, ccMode, uMode, vMode);

				  window.SetPixel(x, y, combined_color);
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				  //Incremental Calculation
				  r += dr_dx;
				  g += dg_dx;
				  b += db_dx;
				  u += du_dx;
				  v += dv_dx;
				  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  }
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incremental Calculation for y movement
			  r_left += dr_dy;
			  g_left += dg_dy;
			  b_left += db_dy;
			  u_left += du_dy;
			  v_left += dv_dy;

			  r_right += dr_dy;
			  g_right += dg_dy;
			  b_right += db_dy;
			  u_right += du_dy;
			  v_right += dv_dy;
			  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			  //Incrmental Calculation for x movement
			  x_left += left_inverse_slope;
			  r_left += left_inverse_slope * dr_dx;
			  g_left += left_inverse_slope * dg_dx;
			  b_left += left_inverse_slope * db_dx;
			  u_left += left_inverse_slope * du_dx;
			  v_left += left_inverse_slope * dv_dx;

			  x_right += right_inverse_slope;
			  r_right += right_inverse_slope * dr_dx;
			  g_right += right_inverse_slope * dg_dx;
			  b_right += right_inverse_slope * db_dx;
			  u_right += right_inverse_slope * du_dx;
			  v_right += right_inverse_slope * dv_dx;
			  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		  }
	  }
  }	// namespace CS200

}