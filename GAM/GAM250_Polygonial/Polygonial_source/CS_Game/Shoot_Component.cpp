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
#include "Shoot_Component.hpp"
#include "CS_JsonManager.hpp"
#include "CS_Object.hpp"
#include "CS_ObjectManager.hpp"
#include "../Dependencies/imgui/imgui.h"
#include "Bullet_Component.hpp"
#include "Enemy_Component.hpp"
#include "CS_Math.hpp"

Shoot_Component::Shoot_Component() : CS_Component(CS_ComponentTypes::Shoot_Component)
{
	range = 900.f;
	delay = 0.6f;
	target_name = "Player";
}
void Shoot_Component::ReadFromFile(CS_JsonObject& jsonObject)
{
	jsonObject.ReadData("Target_Name", target_name);
	jsonObject.ReadData("Bullet_Speed", bullet_info.speed);
	jsonObject.ReadData("Bullet_size", bullet_info.size);
	jsonObject.ReadData("Bullet_Damage", bullet_info.damage);
	jsonObject.ReadData("Bullet_Duration", bullet_info.duration);
	jsonObject.ReadData("delay", delay);
	jsonObject.ReadData("range", range);
}
void Shoot_Component::WriteToFile(CS_JsonObject& jsonObject)
{
	jsonObject.WriteData("Target_Name", target_name);
	jsonObject.WriteData("Bullet_Speed", bullet_info.speed);
	jsonObject.WriteData("Bullet_size", bullet_info.size);
	jsonObject.WriteData("Bullet_Damage", bullet_info.damage);
	jsonObject.WriteData("Bullet_Duration", bullet_info.duration);
	jsonObject.WriteData("delay", delay);
	jsonObject.WriteData("range", range);
}
void Shoot_Component::ShowGui()
{
	if (ImGui::TreeNode("Shoot_Component"))
	{
		char text[20];
		std::copy(target_name.begin(), target_name.end(), text);
		text[target_name.length()] = '\0';
		ImGui::InputText("Target_Name", text, 20);
		target_name = text;
		ImGui::DragFloat2("Bullet_Size", &bullet_info.size.x);
		ImGui::DragFloat("Bullet_Speed", &bullet_info.speed);
		ImGui::DragInt("Bullet_Damage", &bullet_info.damage);
		ImGui::DragFloat("Bullet_Duration", &bullet_info.duration);
		ImGui::DragFloat("Delay", &delay);
		ImGui::DragFloat("range", &range);

		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}
void Shoot_Component::Init()
{
	WhatToChase = CS_ObjectManager::GetFirstObjectByName(target_name);
	timer = 0.0f;
}
Shoot_Component::~Shoot_Component()
{

}
bool Shoot_Component::CheckRange() const
{
	const glm::vec2& enemypos = WhatToChase->transform.GetTranslation();
	const glm::vec2& mypos = m_Parent->transform.GetTranslation();
	glm::vec2 posdiff = enemypos - mypos;
	return posdiff.x * posdiff.x + posdiff.y * posdiff.y
		< range * range;
}
void Shoot_Component::Update(float dt)
{
	if (WhatToChase == nullptr) {
		WhatToChase = CS_ObjectManager::GetFirstObjectByName(target_name);
	}
	if (CheckRange()) {
		if (!shootmultiple)
			Shoot(dt);
		else
			Multishot(dt);
	}
}

void Shoot_Component::Shoot(float dt)
{
	if (timer > delay) {
		glm::vec2 vec = WhatToChase->transform.GetTranslation();
		glm::vec2 myvec = m_Parent->transform.GetTranslation();
		vec = glm::normalize(vec - myvec);
		MakeBullet(vec, bullet_info.size);
		timer = 0.0f;
	}
	else {
		timer += dt;
	}
}

void Shoot_Component::Multishot(float dt)
{
	if (timer > delay)
	{
		glm::vec2 vec = WhatToChase->transform.GetTranslation();
		glm::vec2 myvec = m_Parent->transform.GetTranslation();
		vec = glm::normalize(vec - myvec);
		MakeBullet(vec, bullet_info.size);

		glm::vec3 VecWith3 = { vec.x, vec.y, 0 };
		glm::mat3 mat = affine2D::build_rotation(170.f);
		VecWith3 = mat * VecWith3;
		glm::vec2 new_vec = { VecWith3.x, VecWith3.y };
		MakeBullet(new_vec, bullet_info.size);

		VecWith3 = { vec.x, vec.y, 0 };
		mat = affine2D::build_rotation(-170.f);
		VecWith3 = mat * VecWith3;
		new_vec = { VecWith3.x, VecWith3.y };
		MakeBullet(new_vec, bullet_info.size);
		timer = 0.0f;
	}
	else
		timer += dt;
}

Color Shoot_Component::GetColor() const
{
	Enemy_Component* comp;
	m_Parent->GetComponent(CS_ComponentTypes::Enemy_Component, comp);
	return comp->GetColor();
}

void Shoot_Component::MakeBullet(glm::vec2 direction, glm::vec2 size)
{
	glm::vec2 velocity = direction * bullet_info.speed;
	CS_Object* bullet;
	if(GetColor() == White)
		bullet = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::White_Bullet);
	else if(GetColor() == Black)
		bullet = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Black_Bullet);
	Bullet_Component* bullet_comp;
	bullet->GetComponent(CS_ComponentTypes::Bullet_Component, bullet_comp);
	bullet->transform.SetTranslation(m_Parent->transform.GetTranslation());
	bullet->transform.SetScale(size);
	bullet->transform.SetRotation(glm::atan(velocity.y / velocity.x));
	bullet->transform.SetDepth(0.5f);
	bullet->physics.setDamping(1.0f);
	bullet->physics.setVelocity(velocity);
	bullet->m_name = "Enemy_Bullet";
	bullet_comp->Set_Color(GetColor());
	bullet_comp->Set_Damage(bullet_info.damage);
	bullet_comp->Set_Duration(bullet_info.duration);

	CS_ColliderComponent* collider;
	bullet->GetComponent(CS_ComponentTypes::SphereCollider_Component, collider);
}

CS_Component* Shoot_Component::Clone() const
{
	Shoot_Component* temp = new Shoot_Component;
	return temp;
}

void Shoot_Component::SetMultiShot(bool method)
{
	shootmultiple = method;
}
