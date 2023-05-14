/*
SimpleRender.cpp
-Name : Hwang Chan IL
-Assignment Number : 3
-Course Name : CS250
-term : Spring 2019
*/

#include "SimpleRender.h"

SimpleRender::SimpleRender(Render &r)
	:render(r)
{
	PersProj[0] = Hcoord(1, 0, 0, 0);
	PersProj[1]	= Hcoord(0, 1, 0, 0),
	PersProj[2]	= Hcoord(0, 0, static_cast<float>(1.0 / 11), static_cast<float>(-5.0 / 11)),
	PersProj[3]	= Hcoord(0, 0, static_cast<float>(-2.0 / 11), static_cast<float>(10.0 / 11));
}

SimpleRender::~SimpleRender(void)
{
}

void SimpleRender::DisplayEdges(Mesh& m, const Affine& A, const Vector& color)
{
	proj_vertices.clear();

	Matrix m2p = PersProj * A;
	for (int i = 0; i < m.VertexCount(); i++)
	{
		Hcoord result = m2p * m.GetVertex(i);
		result = (1 / result.w) * result;
		proj_vertices.push_back(result);
	}
	for (int i = 0; i < m.EdgeCount(); i++)
	{
		const Mesh::Edge& targetEdge = m.GetEdge(i);
		const Point& P = proj_vertices[targetEdge.index1];
		const Point& Q = proj_vertices[targetEdge.index2];
		render.SetColor(color);
		render.DrawLine(P, Q);
	}
}

void SimpleRender::DisplayFaces(Mesh& m, const Affine& A, const Vector& color)
{
	Vector light(0, 0, 1);
	Point Eye(0, 0, 5);

	world_vertices.clear();
	proj_vertices.clear();

	for (int i = 0; i < m.VertexCount(); i++)
	{
		world_vertices.push_back(A * m.GetVertex(i));
		Hcoord result = PersProj * world_vertices[i];
		result = (1 / result.w) * result;
		proj_vertices.push_back(result);
	}
	for (int i = 0; i < m.FaceCount(); i++)
	{
		const Mesh::Face& targetFace = m.GetFace(i);
		const Point& P = world_vertices[targetFace.index1];
		const Point& Q = world_vertices[targetFace.index2];
		const Point& R = world_vertices[targetFace.index3];
		Vector normal = cross(Q - P, R - P);
		if (dot(normal, Eye - P) > 0)
		{
			float mu = abs(dot(normal, light)) / (abs(normal) * abs(light));
			render.SetColor(mu * color);
			render.FillTriangle(proj_vertices[targetFace.index1], proj_vertices[targetFace.index2], proj_vertices[targetFace.index3]);
		}
	}
}