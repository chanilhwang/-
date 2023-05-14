/**
 * @file Bomb_Component.cpp
 * @author Sungmin Moon
 * @email elphior0\@gmail.com
 * @brief Bomb controls.
 * @version 0.1
 * @date
 *
 * @copyright Copyright (c) 2018
 *
 */
#include "Bomb_Component.hpp"
#include "CS_Object.hpp"
#include "SphereCollider_Component.hpp"
#include "CS_ObjectManager.hpp"
#include "../Dependencies/imgui/imgui.h"

Bomb_Component::Bomb_Component() :CS_Component(CS_ComponentTypes::Bomb_Component)
{
}

void Bomb_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
}

void Bomb_Component::WriteToFile(CS_JsonObject & jsonObject)
{
}

void Bomb_Component::Init()
{
	m_Parent->m_name = "Bomb";
}

Bomb_Component::~Bomb_Component()
{
}

void Bomb_Component::Update(float dt)
{
	if (m_Parent->is_outside_screen())
		m_Parent->is_dead = true;

	if (CS_Physics::IsThereCollidingWithName(m_Parent->my_id, "Bullet")
		|| CS_Physics::IsThereCollidingWithName(m_Parent->my_id, "Enemy"))
	{
		//explosion = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Explosion);
		//explosion->gfx.SetTextureHandle(animation);
		//explosion->gfx.SetAnimation(4, 1, 0.25f);
		//explosion->transform.SetTranslation(m_Parent->transform.GetTranslation());
		//explosion->transform.SetScale(m_Parent->transform.GetScale() * 5.f);
		m_Parent->is_dead = true;
	}
}

Bomb_Component * Bomb_Component::Clone() const
{
	return new Bomb_Component;
}

void Bomb_Component::ShowGui()
{
	if (ImGui::TreeNode("Bomb_Component"))
	{
		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}
