/**
 * @file Chase_Component.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Chasing Logic for enemies
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Components.hpp"

class Chase_Component :public CS_Component
{
public:
	Chase_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	~Chase_Component() override;
	void Update(float dt) override;
	CS_Component* Clone() const override;
	void ShowGui() override;
	void SetSpeed(float new_speed);
	void SetAcceleration(bool set_acceleration);
	void SetDelay(float delay);
private:
	void Chase(float dt);
	float max_speed = 500.f;
	std::string target_name;
	float speed;
	float delay;
	bool accelerate;
	float timer;
	CS_Object* WhatToChase;
};