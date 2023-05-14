/**
 * @file Strafe_Component.hpp
 * @author Sungmin Moon
 * @email elphior0@gmail.com
 * @brief Enemy strafe pattern logic
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */

#pragma once
#include "CS_Components.hpp"
#include "CS_Math.hpp"
class Strafe_Component :public CS_Component
{
public:
	Strafe_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	~Strafe_Component() override;
	void Update(float dt) override;
	CS_Component* Clone() const override;
	void ShowGui() override;
	void SetSpeed(float new_speed);
	void SetDelay(float new_delay);
	void SetRange(float s_range, float c_range);

	void Strafe(float dt);
	int CheckRange();

	void CircularMovement();
	void Chase();
	void KeepDistance();

private:
	std::string target_name;
	float speed;
	float delay;
	float shooting_range, chasing_range;
	float timer;
	CS_Object* WhatToChase;
};