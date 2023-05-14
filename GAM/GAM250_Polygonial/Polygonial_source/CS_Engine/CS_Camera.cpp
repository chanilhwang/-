/**
 * @file CS_Camera.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Camera control and adjustments.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_Camera.hpp"
#include "CS_Math.hpp"
#include "CS_Graphics.hpp"

namespace {
	int shakecount = 0;
	float fadeintimer = 0.0f;
}

CS_Camera::Point CS_Camera::GetCenter() const
{
	return center;
}

float CS_Camera::GetCenter_x() const
{
        return center.x;
}

float CS_Camera::GetCenter_y() const
{
        return center.y;
}

float CS_Camera::GetTop_y() const
{
	glm::vec2 DisplaySize = CS_Graphics::GetDisplayAreaSize();
	return center.y + DisplaySize.y / 2;
}

float CS_Camera::GetBot_y() const
{
	glm::vec2 DisplaySize = CS_Graphics::GetDisplayAreaSize();
	return center.y - DisplaySize.y / 2;
}

float CS_Camera::GetRight_x() const
{
	glm::vec2 DisplaySize = CS_Graphics::GetDisplayAreaSize();
    return center.x + DisplaySize.x/2;
}

float CS_Camera::GetLeft_x() const
{
	glm::vec2 DisplaySize = CS_Graphics::GetDisplayAreaSize();
    return center.x - DisplaySize.x/2;
}

void CS_Camera::SetCenter(Point camera_center)
{
	center = camera_center;
}

void CS_Camera::SetCenter_x(float x)
{
        center.x = x;
}

void CS_Camera::SetCenter_y(float y)
{
        center.y = y;
}

CS_Camera::Vector CS_Camera::GetUp() const
{
	return up;
}
CS_Camera::Vector CS_Camera::GetRight() const
{
	return right;
}
void CS_Camera::ResetUp(Vector camera_up)
{
	up = camera_up;
	right = { camera_up.y, -camera_up.x };
}
void CS_Camera::MoveUp(float distance)
{
	glm::vec3 vector = CameraToWorld()* glm::vec3{ 0.f, distance, 0.f };
	center.x += vector.x;
	center.y += vector.y;
}
void CS_Camera::MoveRight(float distance)
{
	glm::vec3 vector = CameraToWorld()* glm::vec3{ distance, 0.f, 0.f };
	center.x += vector.x;
	center.y += vector.y;
}
void CS_Camera::Rotate(float angle_radians)
{
	glm::mat3 mat = affine2D::build_rotation(angle_radians);
	glm::vec3 Up = { up.x,up.y,0 };
	glm::vec3 Right = { right.x,right.y ,0 };
	Up = mat * Up;
	Right = mat * Right;
	up = { Up.x,Up.y };
	right = { Right.x,Right.y };
}
void CS_Camera::SetZoom(float newZoom)
{
	zoom = newZoom;
}
float CS_Camera::GetZoom(void) const
{
	return zoom;
}
glm::mat3 CS_Camera::CameraToNDC() const
{
	glm::mat3 ctoNDC(1.f);
	glm::vec2 DisplaySize = CS_Graphics::GetDisplayAreaSize();
	ctoNDC[0].x = 2.f*zoom / DisplaySize.x;
	ctoNDC[1].y = 2.f*zoom / DisplaySize.y;
	return ctoNDC;
}
glm::mat3 CS_Camera::CameraToWorld() const
{
	glm::mat3 ctow{ right.x,right.y,0,up.x,up.y,0,center.x,center.y,1 };
	return ctow;
}

glm::mat3 CS_Camera::WorldToCamera() const
{
	float uc = glm::dot(right, center), vc = glm::dot(up, center);
	glm::mat3 wtoc{ right.x,up.x,0,right.y,up.y,0,-uc,-vc,1 };
	return wtoc;
}

void CS_Camera::SetVelocity(glm::vec2 vel)
{
	velocity = vel;
}

glm::vec2 CS_Camera::GetVelocity() const
{
	return velocity;
}

void CS_Camera::Screenshake(float dt)
{
	glm::vec2 shakerange = glm::vec2(10,10);
	if (shakecount % 2 == 0) {
		SetCenter(GetCenter() - shakerange);
	}
	else {
		SetCenter(GetCenter() + shakerange);
	}
	shakecount--;
}

void CS_Camera::FadeIn(float dt) {
	if (fadeintimer < 0) {
		return;
	}
	//do fadein effects
}

void CS_Camera::SetShakeOn()
{
	//count always need to be even
	shakecount = 10;
}

void CS_Camera::FadeInOn()
{
	fadeintimer = 0.5f;
}

void CS_Camera::Update(float dt)
{

	if (shakecount > 0)
		Screenshake(dt);
	if (fadeintimer > 0)
		FadeIn(dt);

	center += velocity * dt;
}