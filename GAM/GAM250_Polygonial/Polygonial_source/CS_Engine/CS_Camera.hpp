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
#pragma once
#include "glm/glm.hpp"

class CS_Camera
{
public:
	using Point = glm::vec2;
	using Vector = glm::vec2;

	CS_Camera() = default;

	Point GetCenter() const;
        float GetCenter_x() const;
        float GetCenter_y() const;
	float GetTop_y() const;
	float GetBot_y() const;
        float GetRight_x() const;
        float GetLeft_x() const;
	void SetCenter(Point camera_center);
        void SetCenter_x(float x);
        void SetCenter_y(float y);
	Vector GetUp() const;
	Vector GetRight() const;

	void ResetUp(Vector camera_up = { 0, 1 });

	void MoveUp(float distance);
	void MoveRight(float distance);
	void Rotate(float angle_radians);

	void SetZoom(float newZoom);
	float GetZoom(void) const;

	glm::mat3 CameraToNDC() const;
	glm::mat3 CameraToWorld() const;
	glm::mat3 WorldToCamera() const;

	void SetVelocity(glm::vec2 vel);
	glm::vec2 GetVelocity() const;
	void Screenshake(float dt);
	void FadeIn(float dt);
	static void SetShakeOn();
	static void FadeInOn();
	void Update(float dt);
private:
	Point center{};
	Vector up{ 0, 1 };
	Vector right{ 1, 0 };
	glm::vec2 velocity{0,0};
	float zoom = 1.f;
};
