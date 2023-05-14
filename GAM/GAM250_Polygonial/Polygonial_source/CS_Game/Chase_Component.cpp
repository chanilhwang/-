/**
 * @file Chase_Component.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Chasing Logic for enemies
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include <string>
#include "Chase_Component.hpp"
#include "CS_JsonManager.hpp"
#include "CS_Object.hpp"
#include "CS_ObjectManager.hpp"
#include "../Dependencies/imgui/imgui.h"
#include <iostream>

Chase_Component::Chase_Component() : CS_Component(CS_ComponentTypes::Chase_Component)
{
	target_name = "Player";
}
Chase_Component::~Chase_Component()
{

}
void Chase_Component::ReadFromFile(CS_JsonObject& jsonObject)
{
	jsonObject.ReadData("Target_Name", target_name);
	jsonObject.ReadData("Speed", speed);
	jsonObject.ReadData("Delay", delay);
}
void Chase_Component::WriteToFile(CS_JsonObject& jsonObject)
{
	jsonObject.WriteData("Target_Name", target_name);
	jsonObject.WriteData("Speed", speed);
	jsonObject.WriteData("Delay", delay);
}
void Chase_Component::ShowGui()
{
	if (ImGui::TreeNode("Chase_Component"))
	{
		static char text[20];
		std::copy(target_name.begin(), target_name.end(), text);
		text[target_name.length()] = '\0';
		ImGui::InputText("Target_Name", text, 20);
		target_name = text;
		ImGui::DragFloat("Speed", &speed);
		ImGui::DragFloat("Delay", &delay);

		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}
void Chase_Component::SetSpeed(float new_speed)
{
	speed = new_speed;
}
void Chase_Component::SetAcceleration(bool set_acceleration)
{
	accelerate = set_acceleration;
}
void Chase_Component::SetDelay(float delay)
{
	this->delay = delay;
}
void Chase_Component::Init()
{
	WhatToChase = CS_ObjectManager::GetFirstObjectByName(target_name);
	timer = 0.0f;
	m_Parent->physics.setDamping(1.0f);
	speed = 100.f;
}
void Chase_Component::Update(float dt)
{
	if (WhatToChase == nullptr) {
		WhatToChase = CS_ObjectManager::GetFirstObjectByName(target_name);
	}
	Chase(dt);
}
void Chase_Component::Chase(float dt)
{
	glm::vec2 vec = WhatToChase->transform.GetTranslation();
	glm::vec2 myvec = m_Parent->transform.GetTranslation();
	vec = glm::normalize(vec - myvec) * speed;
	m_Parent->physics.setVelocity(vec);
	if (accelerate)
	{
		if (speed < max_speed)
			speed += 25.f * dt;
	}
}
CS_Component* Chase_Component::Clone() const
{
	Chase_Component *temp = new Chase_Component;
	return temp;
}
