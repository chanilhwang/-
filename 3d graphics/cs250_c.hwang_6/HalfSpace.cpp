/*
HalfSpace.cpp
-Name : Hwang Chan IL
-Assignment Number : 6
-Course Name : CS250
-term : Spring 2019
*/
#include "HalfSpace.h"

HalfSpace::HalfSpace(const Vector& m, const Point& C)
	: Hcoord(m.x, m.y, m.z, -(m.x * C.x + m.y * C.y + m.z * C.z))
{}

HalfSpace::HalfSpace(const Point& A, const Point& B, const Point& C, const Point& P)
{
	Vector m = cross(B - A, B - C);
	if (dot(m, P - A) > 0)
	{
		m = -m;
	}
	x = m.x;
	y = m.y;
	z = m.z;
	w = -(m.x * C.x + m.y * C.y + m.z * C.z);
}

Polyhedron::Polyhedron(bool cube)
{
	if (cube)
	{
		half_spaces.push_back(HalfSpace(0, 0, 1, -1));
		half_spaces.push_back(HalfSpace(0, 0, -1, -1));
		half_spaces.push_back(HalfSpace(0, 1, 0, -1));
		half_spaces.push_back(HalfSpace(0, -1, 0, -1));
		half_spaces.push_back(HalfSpace(1, 0, 0, -1));
		half_spaces.push_back(HalfSpace(-1, 0, 0, -1));
	}
}

Polyhedron::Polyhedron(float fov, float a, float N, float F)
{
	const float& near_width = N * tan(fov / 2);
	const float& near_height = near_width * a;

	half_spaces.push_back(HalfSpace(0, 0, 1, N));
	half_spaces.push_back(HalfSpace(0, 0, -1, -F));
	half_spaces.push_back(HalfSpace(-N, 0, near_width, 0));
	half_spaces.push_back(HalfSpace(N, 0, near_width, 0));
	half_spaces.push_back(HalfSpace(0, -N, near_height, 0));
	half_spaces.push_back(HalfSpace(0, N, near_height, 0));
}

bool contains(const Polyhedron & polyhedron, const Hcoord & P)
{
	if (P.w <= 0)
		return false;
	for (auto& half_space : polyhedron.half_spaces)
	{
		if (dot(half_space, P) > 0)
			return false;
	}		
	return true;
}
