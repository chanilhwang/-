/**
 * @file Shoot_Component.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Shooting Logic for enemies
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once

#include "CS_Components.hpp"
#include "../Dependencies/Math/glm/glm.hpp"
#include "Color.hpp"
#include "Bullet_Info.hpp"

class Shoot_Component : public CS_Component
{
public:
	Shoot_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	~Shoot_Component() override;
	void Update(float dt) override;
	void ShowGui() override;
	CS_Component* Clone() const override;
	void SetMultiShot(bool method);

private:
	bool CheckRange() const;
	void Shoot(float dt);
	void Multishot(float dt);
	void MakeBullet(glm::vec2 velocity, glm::vec2 size);
	Color GetColor() const;

	std::string target_name;
	BulletInfo bullet_info;
	float delay;
	float range;

	CS_Object* WhatToChase;
	float timer;
	bool shootmultiple;
};