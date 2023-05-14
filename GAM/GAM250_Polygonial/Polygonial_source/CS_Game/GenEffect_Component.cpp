/**
 * @file GenEffect_Component.cpp
 * @author Sungmin Moon
 * @email elphior0@gmail.com
 * @brief Enemy generating effect logic
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */

#include "GenEffect_Component.hpp"
#include "Chase_Component.hpp"
#include "Shoot_Component.hpp"
#include "Strafe_Component.hpp"
#include "CS_SoundManager.hpp"

GenEffect_Component::GenEffect_Component() :CS_Component(CS_ComponentTypes::GenEffect_Component)
{
}

void GenEffect_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
}

void GenEffect_Component::WriteToFile(CS_JsonObject & jsonObject)
{
}

void GenEffect_Component::Init()
{
	timer = 0.f;
	size = { 300.f, 300.f };
}

GenEffect_Component::~GenEffect_Component()
{
}

void GenEffect_Component::Update(float dt)
{
	timer += dt;
	size -= (timer * 50.f);
	m_Parent->transform.SetScale(size);
	if (timer > life)
	{
		GenEnemy(current_shape, current_color);
		m_Parent->is_dead = true;
	}
}

GenEffect_Component * GenEffect_Component::Clone() const
{
	return new GenEffect_Component;
}

void GenEffect_Component::ShowGui()
{
}

void GenEffect_Component::SetupTexture(Shape shape, Color color)
{
	if (color == Black)
	{
		if (shape == Triangle)
		{
			m_Parent->gfx.SetTextureHandle("Textures/Appear_BlackTriangle.png");
		}
		else if (shape == Rectangle)
		{
			m_Parent->gfx.SetTextureHandle("Textures/Appear_BlackRectangle.png");
		}
		else if (shape == Pentagon)
		{
			m_Parent->gfx.SetTextureHandle("Textures/Appear_BlackPentagon.png");
		}
	}
	else if (color == White)
	{
		if (shape == Triangle)
		{
			m_Parent->gfx.SetTextureHandle("Textures/Appear_WhiteTriangle.png");
		}
		else if (shape == Rectangle)
		{
			m_Parent->gfx.SetTextureHandle("Textures/Appear_WhiteRectangle.png");
		}
		else if (shape == Pentagon)
		{
			m_Parent->gfx.SetTextureHandle("Textures/Appear_WhitePentagon.png");
		}
	}
	current_shape = shape;
	current_color = color;
}

void GenEffect_Component::GenEnemy(Shape shape, Color color)
{
	CS_Object* enemy;

	if (shape == Triangle)
		enemy = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Chasing_Enemy);
	else if (shape == Rectangle)
		enemy = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Shooting_Enemy);
	else
		enemy = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Strafing_Enemy);

	if (enemy != nullptr)
	{
		Enemy_Component* enemy_component;
		enemy->transform.SetTranslation(m_Parent->transform.GetTranslation());
		if (shape == Triangle)
		{
			Chase_Component* chase_component;
			enemy->GetComponent(CS_ComponentTypes::Chase_Component, chase_component);
			chase_component->SetSpeed(150.f);
			chase_component->SetAcceleration(true);
		}
		else if (shape == Rectangle)
		{
			Chase_Component* chase_component;
			enemy->GetComponent(CS_ComponentTypes::Chase_Component, chase_component);
			Shoot_Component* shoot_component;
			enemy->GetComponent(CS_ComponentTypes::Shoot_Component, shoot_component);

			chase_component->SetSpeed(125.f);
			chase_component->SetAcceleration(false);
			shoot_component->SetMultiShot(false);
		}
		else
		{
			Strafe_Component* strafe_component;
			enemy->GetComponent(CS_ComponentTypes::Strafe_Component, strafe_component);
			Shoot_Component* shoot_component;
			enemy->GetComponent(CS_ComponentTypes::Shoot_Component, shoot_component);
			strafe_component->SetSpeed(200.f);
			strafe_component->SetDelay(0.01f);
			strafe_component->SetRange(500.f, 700.f);
			shoot_component->SetMultiShot(false);
		}
		enemy->GetComponent(CS_ComponentTypes::Enemy_Component, enemy_component);
		enemy->transform.SetDepth(0.5f);
		enemy_component->SetShape(shape);
		enemy_component->SetColor(color);
		enemy_component->SetupTexture();
	}
}
