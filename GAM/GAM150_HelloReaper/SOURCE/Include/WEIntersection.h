/******************************************************************************/
/*!
\file   WEIntersection.h
\author Matt Casanova
\par    email: mcasanov\@digipen.edu
\par    GAM150
\par    Simple 2D Game Engine
\date   2012/11/26

Prototypes for intersection and Distance tests

*/
/******************************************************************************/
#ifndef WE_INTERSECTION_H
#define WE_INTERSECTION_H

#include "WEVec2.h"

//!Functions to test the distance between two objects
namespace WEDistance
{
//!Tests Distance between a point and  a circle
float PointCircle(const WEVec2& point, const WEVec2& circleCenter, 
  float radius);
//!Tests Distance between a point and a rectangle
float PointRect(const WEVec2& point, const WEVec2& rectCenter, 
  float width, float height);
//!Tests Distance between a point and a line segment
float PointLine(const WEVec2& point, const WEVec2& lineStart,
  const WEVec2& lineEnd);
//!Tests Distance between two circles
float CircleCircle(const WEVec2& circleCenter0, float radius0,
  const WEVec2& circleCenter1, float radius1);
//!Tests Distance between a circle and a rectangle
float CircleRect(const WEVec2& circleCenter, float radius,
  const WEVec2& rectCenter, float width,
  float height);
//!Tests Distance between a circle and a line segment
float CircleLine(const WEVec2& circleCenter, float radius,
  const WEVec2& lineStart,
  const WEVec2& lineEnd);
//!Tests Distance between two rectangles
float RectRect(const WEVec2& rectCenter0, float width0, float height0,
  const WEVec2& rectCenter1,
  float width1, float height1);
}//end namespace Distance


//! Functions to test if two objects are simply intersecting
namespace WEIntersection
{
//! Test if a point intersects a circle
bool PointCircle(const WEVec2& point, const WEVec2& circleCenter, 
  float radius);
//! Tests if a point intersects a rectangle
bool PointRect(const WEVec2& point, const WEVec2& rectCenter,
  float width, float height);
//! Test if two circles intersect
bool CircleCircle(const WEVec2& circleCenter0, float radius0,
  const WEVec2& circleCenter1, float radius1);
//! Test if a circle interects with a rectangle
bool CircleRect(const WEVec2& circleCenter, float radius,
  const WEVec2& rectCenter, float width, float height);
//! Test if a circle and a line intersect
bool CircleLine(const WEVec2& circleCenter, float radius,
  const WEVec2& lineStart, const WEVec2& lineEnd);
//! Tests if two rectangles intersect
bool RectRect(const WEVec2& rectCenter0, float width0, float height0,
  const WEVec2& rectCenter1, float width1, float height1);
}//end namespace WEIntersection





#endif