/*
Camera.cpp
-Name : Hwang Chan IL
-Assignment Number : 3
-Course Name : CS250
-term : Spring 2019
*/

#include "Camera.h"

Camera::Camera(void)
	: eye(Point(0, 0, 0)), right(Vector(1, 0, 0)), back(Vector(0, 0, 1)),
	up(Vector(0, 1, 0)), near(static_cast<float>(0.1)), far(10), distance(5), width(5), height(5)
{
}

Camera::Camera(const Point& E, const Vector& look, const Vector& vp,
	float fov, float aspect, float near, float far)
	:eye(E), near(near), far(far)
{
	back = (-1 / abs(look)) * look;
	Vector crossVec = cross(look, vp);
	right = (1 / abs(crossVec)) * crossVec;
	up = cross(back, right);
	distance = (near + far) / 2;
	width = 2 * distance * tan(fov / 2);
	height = width / aspect;
}

Point Camera::Eye(void) const
{
	return eye;
}

Vector Camera::Right(void) const
{
	return right;
}

Vector Camera::Up(void) const
{
	return up;
}

Vector Camera::Back(void) const
{
	return back;
}

Vector Camera::ViewportGeometry(void) const
{
	return Vector(width, height, distance);
}

float Camera::NearDistance(void) const
{
	return near;
}

float Camera::FarDistance(void) const
{
	return far;
}

Camera& Camera::Zoom(float factor)
{
	width *= factor;
	height *= factor;

	return *this;
}

Camera& Camera::Forward(float distance_increment)
{
	eye = eye - distance_increment * back;

	return *this;
}

Camera& Camera::Yaw(float angle)
{
	Affine AffineAboutUp = rotate(angle, up);
	right = AffineAboutUp * right;
	back = AffineAboutUp * back;

	return *this;
}

Camera& Camera::Pitch(float angle)
{
	Affine AffineAboutRight = rotate(angle, right);
	up = AffineAboutRight * up;
	back = AffineAboutRight * back;

	return *this;
}

Camera& Camera::Roll(float angle)
{
	Affine AffineAboutUp = rotate(angle, back);
	right = AffineAboutUp * right;
	up = AffineAboutUp * up;

	return *this;
}