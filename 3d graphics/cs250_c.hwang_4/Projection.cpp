/*
Projection.cpp
-Name : Hwang Chan IL
-Assignment Number : 4
-Course Name : CS250
-term : Spring 2019
*/
#include "Projection.h"

Affine CameraToWorld(const Camera& cam)
{
	return Affine{ cam.Right(), cam.Up(), cam.Back(), cam.Eye() };
}

Affine WorldToCamera(const Camera& cam)
{
	return inverse(CameraToWorld(cam));
}

Matrix CameraToNDC(const Camera& cam)
{
	const float& W = cam.ViewportGeometry().x;
	const float& H = cam.ViewportGeometry().y;
	const float& D = cam.ViewportGeometry().z;
	const float& N = cam.NearDistance();
	const float& F = cam.FarDistance();

	Matrix result;
	result.row[0] = Hcoord(2 * D / W, 0, 0, 0);
	result.row[1] = Hcoord(0, 2 * D / H, 0, 0);
	result.row[2] = Hcoord(0, 0, (N + F) / (N - F), 2 * N * F / (N - F));
	result.row[3] = Hcoord(0, 0, -1.f, 0);

	return result;
}