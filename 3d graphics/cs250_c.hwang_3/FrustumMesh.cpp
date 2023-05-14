/*
Camera.cpp
-Name : Hwang Chan IL
-Assignment Number : 3
-Course Name : CS250
-term : Spring 2019
*/

#include "FrustumMesh.h"

const Mesh::Edge FrustumMesh::edges[16] = { Edge(0,1), Edge(0,2), Edge(0,3), Edge(0,4),
											Edge(1,5), Edge(2,6), Edge(3,7), Edge(4,8),
											Edge(1,2), Edge(2,4), Edge(4,3), Edge(3,1),
											Edge(5,6), Edge(6,8), Edge(8,7), Edge(7,5) };

const Mesh::Face FrustumMesh::faces[12] = { Face(1,4,2),Face(1,3,4), Face(1,6,5), Face(1, 2, 6),
											Face(5,6,8), Face(5,8,7), Face(2,4,6), Face(4,8,6),
											Face(1,5,3), Face(3,5,7), Face(3,8,4), Face(3,7,8) };

FrustumMesh::FrustumMesh(float fov, float a, float n, float f)
{
	float tangentHalfOffov = tan(fov / 2);
	float nearX = tangentHalfOffov * n;
	float nearY = nearX / a;
	float farX = tangentHalfOffov * f;
	float farY = farX / a;
	vertices[0] = Point(0, 0, 0);
	vertices[1] = Point(-nearX, nearY, -n);
	vertices[2] = Point(nearX, nearY, -n);
	vertices[3] = Point(-nearX, -nearY, -n);
	vertices[4] = Point(nearX, -nearY, -n);
	vertices[5] = Point(-farX, farY, -f);
	vertices[6] = Point(farX, farY, -f);
	vertices[7] = Point(-farX, -farY, -f);
	vertices[8] = Point(farX, -farY, -f);
	center = Point(0, 0, -(n + f) / 2);
	dimensions = Vector(nearX + farX, nearY + farY, f-n);
}

int FrustumMesh::VertexCount(void)
{
	return 9;
}

Point FrustumMesh::GetVertex(int i)
{
	return vertices[i];
}

Vector FrustumMesh::Dimensions(void)
{
	return dimensions;
}

Point FrustumMesh::Center(void)
{
	return center;
}

int FrustumMesh::FaceCount(void)
{
	return 12;
}

Mesh::Face FrustumMesh::GetFace(int i)
{
	return faces[i];
}

int FrustumMesh::EdgeCount(void)
{
	return 16;
}

Mesh::Edge FrustumMesh::GetEdge(int i)
{
	return edges[i];
}
