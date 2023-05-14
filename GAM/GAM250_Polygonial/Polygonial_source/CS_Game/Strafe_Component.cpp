/**
 * @file Strafe_Component.cpp
 * @author Sungmin Moon
 * @email elphior0@gmail.com
 * @brief Enemy strafe pattern logic
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */

#include "Strafe_Component.hpp"
#include "CS_JsonManager.hpp"
#include "CS_Object.hpp"
#include "CS_ObjectManager.hpp"
#include "../Dependencies/imgui/imgui.h"
#include <iostream>
#include <string>

#define BACK_OFF -1
#define SHOOTING_RANGE 0
#define CHASING_RANGE 1


Strafe_Component::Strafe_Component() : CS_Component(CS_ComponentTypes::Strafe_Component)
{
	target_name = "Player";
}

void Strafe_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
	jsonObject.ReadData("Target_Name", target_name);
	jsonObject.ReadData("Speed", speed);
	jsonObject.ReadData("Delay", delay);
	jsonObject.ReadData("Shooting Range", shooting_range);
	jsonObject.ReadData("Chasing Range", chasing_range);
}

void Strafe_Component::WriteToFile(CS_JsonObject & jsonObject)
{
	jsonObject.WriteData("Target_Name", target_name);
	jsonObject.WriteData("Speed", speed);
	jsonObject.WriteData("Delay", delay);
	jsonObject.WriteData("Shooting Range", shooting_range);
	jsonObject.WriteData("Chasing Range", chasing_range);
}

void Strafe_Component::Init()
{
	timer = 0.f;
	WhatToChase = CS_ObjectManager::GetFirstObjectByName(target_name);
}

Strafe_Component::~Strafe_Component()
{
}

void Strafe_Component::Update(float dt)
{
	if (WhatToChase == nullptr)
	{
		WhatToChase = CS_ObjectManager::GetFirstObjectByName(target_name);
	}
	else
	{
		Strafe(dt);
	}
}

CS_Component * Strafe_Component::Clone() const
{
	Strafe_Component* temp = new Strafe_Component;
	return temp;
}

void Strafe_Component::ShowGui()
{
	if (ImGui::TreeNode("Strafe_Component"))
	{
		char text[20];
		std::copy(target_name.begin(), target_name.end(), text);
		text[target_name.length()] = '\0';
		ImGui::InputText("Target_Name", text, 20);
		target_name = text;
		ImGui::DragFloat("Speed", &speed);
		ImGui::DragFloat("Delay", &delay);
		ImGui::DragFloat("Shooting Range", &shooting_range);
		ImGui::DragFloat("Chasing Range", &chasing_range);

		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}

void Strafe_Component::SetSpeed(float new_speed)
{
	speed = new_speed;
}

void Strafe_Component::SetDelay(float new_delay)
{
	delay = new_delay;
}

void Strafe_Component::SetRange(float s_range, float c_range)
{
	shooting_range = s_range;
	chasing_range = c_range;
}

void Strafe_Component::Strafe(float dt)
{
	if (WhatToChase != nullptr)
	{
		timer -= dt;
		if (timer < 0.f)
		{
			int status = CheckRange();
			if (status == BACK_OFF)
				KeepDistance();
			else if (status == SHOOTING_RANGE)
				CircularMovement();
			else
				Chase();

			timer = delay;
		}
	}
}

int Strafe_Component::CheckRange()
{
	const glm::vec2 enemypos = WhatToChase->transform.GetTranslation();
	const glm::vec2 mypos = m_Parent->transform.GetTranslation();
	glm::vec2 posdiff = enemypos - mypos;
	float distance = posdiff.x * posdiff.x + posdiff.y * posdiff.y;
	if (distance < shooting_range * shooting_range)
		return BACK_OFF;
	if (distance > chasing_range * chasing_range)
		return CHASING_RANGE;
	else 
		return SHOOTING_RANGE;
}

void Strafe_Component::CircularMovement()
{
	glm::vec2 vec = WhatToChase->transform.GetTranslation();
	glm::vec2 myvec = m_Parent->transform.GetTranslation();
	vec = glm::normalize(vec - myvec);
	glm::vec3 Vec3 = { vec.x, vec.y, 0.f };
	glm::mat3 rotation = affine2D::build_rotation(90.f);
	Vec3 = Vec3 * rotation;
	vec.x = Vec3.x * speed;
	vec.y = Vec3.y * speed;

	m_Parent->physics.setVelocity(vec);
}

void Strafe_Component::Chase()
{
	glm::vec2 vec = WhatToChase->transform.GetTranslation();
	glm::vec2 myvec = m_Parent->transform.GetTranslation();
	vec = glm::normalize(vec - myvec) * speed;

	m_Parent->physics.setVelocity(vec);
}

void Strafe_Component::KeepDistance()
{
	glm::vec2 vec = WhatToChase->transform.GetTranslation();
	glm::vec2 myvec = m_Parent->transform.GetTranslation();
	vec = glm::normalize(myvec - vec) * speed;

	m_Parent->physics.setVelocity(vec);
	
}
