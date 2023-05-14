/**
 * @file Bullet_Component.cpp
 * @author Sungmin Moon / ChanIl Hwang
 * @email elphior0\@gmail.com / tktlssla803@gmail.com
 * @brief Bullet controls.
 * @version 0.1
 * @date 2018-12-10
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#include "Bullet_Component.hpp"
#include "Player_Component.hpp"
#include "CS_Object.hpp"
#include "SphereCollider_Component.hpp"
#include "CS_Input.hpp"
#include "CS_ObjectManager.hpp"
#include "../Dependencies/imgui/imgui.h"
/**
 * @brief Construct a new Bullet_Component::Bullet_Component object
 * 
 */
Bullet_Component::Bullet_Component() :CS_Component(CS_ComponentTypes::Bullet_Component)
{
}
/**
 * @brief Destroy the Bullet_Component::Bullet_Component object
 *
 */
Bullet_Component::~Bullet_Component()
{
}
void Bullet_Component::ReadFromFile(CS_JsonObject& jsonObject)
{

}

void Bullet_Component::WriteToFile(CS_JsonObject& jsonObject)
{

}
CS_Component* Bullet_Component::Clone() const
{
	Bullet_Component* temp = new Bullet_Component;
	return temp;
}
void Bullet_Component::ShowGui()
{

}
/**
 * @brief Texture and timer initialization.
 * 
 */
void Bullet_Component::Init()
{
}
/**
 * @brief Up
 * 
 * @param dt 
 */
void Bullet_Component::Update(float dt)
{
	if (IsDurationEnded(dt))
		TerminateThis();
	//CheckCollideAndInteract(TargetObject);
}

bool Bullet_Component::IsDurationEnded(float dt)
{
	duration -= dt;
	return (duration <= 0);
}

void Bullet_Component::CheckCollideAndInteract(std::string Object)
{
	if (CS_Physics::IsThereAnyColliding(m_Parent->my_id))
	{
		m_Parent->is_dead = true;
		//TerminateThis();
	}
}

void Bullet_Component::TerminateThis()
{
	m_Parent->is_dead = true;
}

int Bullet_Component::Get_Damage() const
{
	return dmg.Get_damage();
}

void Bullet_Component::Set_Damage(int new_dmg)
{
		dmg.Set_damage(new_dmg);
}

Color Bullet_Component::Get_Color() const
{
	return color;
}

void Bullet_Component::Set_Color(Color color)
{
	this->color = color;
}

void Bullet_Component::Set_Duration(float duration)
{
	this->duration = duration;
}

void Bullet_Component::SetTarget(std::string target)
{
	TargetObject = target;
}
