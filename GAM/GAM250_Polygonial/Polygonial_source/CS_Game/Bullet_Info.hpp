/**
 * @file Bullet_info.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Variable used for bullet
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "../Dependencies/Math/glm/glm.hpp"

struct BulletInfo
{
	BulletInfo() {
		speed = 1000.f;
		duration = 2.f;
		size = { 40,20 };
		damage = 1;
	}
	BulletInfo(float speed, float duration, glm::vec2 size, int damage) {
		this->speed = speed;
		this->duration = duration;
		this->size = size;
		this->damage = damage;
	}
	float speed = 1000.f;
	float duration = 2.0f;
	glm::vec2 size = { 40, 20 };
	int damage = 1;
};