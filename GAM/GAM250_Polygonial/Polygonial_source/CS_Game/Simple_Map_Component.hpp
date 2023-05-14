/**
 * @file Simple_Map_Component.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Logic for map limitation
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Components.hpp"
#include <glm/vec2.hpp>

class Simple_Map_Component : public CS_Component
{
public:
	Simple_Map_Component();
	~Simple_Map_Component() override;
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	void Update(float dt) override;
	Simple_Map_Component* Clone() const override;
	void ShowGui() override;
private:
	float radius;
	glm::vec2 center;
	
};