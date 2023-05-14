/*
CameraRender2D.cpp
-Name : Hwang Chan IL
-Assignment Number : 4
-Course Name : CS250
-term : Spring 2019
*/
#include "CameraRender2D.h"
#include "Projection.h"

CameraRender2D::CameraRender2D(Render & r)
	:render(r)
{
	Camera cam;
	world2camera = WorldToCamera(cam);
	camera2ndc = CameraToNDC(cam);
}

CameraRender2D::~CameraRender2D(void)
{
}

void CameraRender2D::SetCamera(const Camera & cam)
{
	world2camera = WorldToCamera(cam);
	camera2ndc = CameraToNDC(cam);
}

void CameraRender2D::DisplayEdges(Mesh & m, const Affine & A, const Vector & color)
{
	cam_vertices.clear();
	ndc_vertices.clear();

	Affine m2c = world2camera * A;
	for (int i = 0; i < m.VertexCount(); i++)
	{
		cam_vertices.push_back( m2c * m.GetVertex(i));
		Hcoord result = camera2ndc * cam_vertices[i];
		result = (1 / result.w) * result;
		ndc_vertices.push_back(result);
	}
	render.SetColor(color);
	for (int i = 0; i < m.EdgeCount(); i++)
	{
		const Mesh::Edge& targetEdge = m.GetEdge(i);
		const Point& P = cam_vertices[targetEdge.index1];
		const Point& Q = cam_vertices[targetEdge.index2];
		if (P.z < 0 && Q.z < 0)
		{
			render.DrawLine(ndc_vertices[targetEdge.index1], ndc_vertices[targetEdge.index2]);
		}
	}
}

void CameraRender2D::DisplayFaces(Mesh & m, const Affine & A, const Vector & color)
{
	const Point Eye(0, 0, 0);
	const Vector Light(0, 0, 1);

	cam_vertices.clear();
	ndc_vertices.clear();

	Affine m2c = world2camera * A;
	for (int i = 0; i < m.VertexCount(); i++)
	{
		cam_vertices.push_back(m2c * m.GetVertex(i));
		Hcoord result = camera2ndc * cam_vertices[i];
		result = (1 / result.w) * result;
		ndc_vertices.push_back(result);
	}
	for (int i = 0; i < m.FaceCount(); i++)
	{
		const Mesh::Face& targetFace = m.GetFace(i);
		const Point& P = cam_vertices[targetFace.index1];
		const Point& Q = cam_vertices[targetFace.index2];
		const Point& R = cam_vertices[targetFace.index3];
		if (P.z < 0 && Q.z < 0 && R.z < 0)
		{
			Vector normal = cross(Q - P, R - P);
			if (dot(normal, Eye - P) > 0)
			{
				float mu = abs(dot(normal, Light)) / (abs(normal) * abs(Light));
				render.SetColor(mu * color);
				render.FillTriangle(ndc_vertices[targetFace.index1], ndc_vertices[targetFace.index2], ndc_vertices[targetFace.index3]);
			}
		}
	}
}
