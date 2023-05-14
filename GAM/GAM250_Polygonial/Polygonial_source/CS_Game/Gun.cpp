/**
 * @file Gun.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Logic for guns
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Gun.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_Input.hpp"
#include "Bullet_Component.hpp"
#include "CS_Object.hpp"
#include "Pistol.hpp"
#include "Shotgun.hpp"
#include "Gatling.hpp"
#include "CS_ColliderComponent.hpp"

Gun::Gun(BulletInfo bullet_info, float delay)
	: bullet_info(bullet_info), delay(delay)
{
	timer = 0;
}

Gun::~Gun() {}

void Gun::MakeBullet(glm::vec2 position, glm::vec2 direction, Color color)
{
	glm::vec2 velocity = bullet_info.speed * direction;
	CS_Object* bullet;
	if (color == White)
		bullet = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::White_Bullet);
	else if (color == Black)
		bullet = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Black_Bullet);
	Bullet_Component* bullet_comp;
	bullet->GetComponent(CS_ComponentTypes::Bullet_Component, bullet_comp);
	bullet->transform.SetTranslation(glm::vec2(position.x, position.y));
	bullet->transform.SetScale(bullet_info.size);
	bullet->transform.SetRotation(glm::atan(direction.y / direction.x));
	bullet->transform.SetDepth(0.5f);
	bullet->physics.setDamping(1.0f);
	bullet->physics.setVelocity(velocity);
	bullet->m_name = "Player_Bullet";

	bullet_comp->Set_Color(color);

	bullet_comp->Set_Damage(bullet_info.damage);
	bullet_comp->Set_Duration(bullet_info.duration);
	bullet_comp->SetTarget("Enemy");

	CS_ColliderComponent* collider;
	bullet->GetComponent(CS_ComponentTypes::SphereCollider_Component, collider);
}

glm::vec2 Gun::CalculateVectorBetweenPlayerAndGun(glm::vec2 player_pos)
{
	glm::vec2 mousepos = CS_Input::GetMouseWorldPos();
	glm::vec2 bullet_vector = glm::normalize(mousepos - player_pos);

	return bullet_vector;
}

void Gun::UpdateDelay(float dt)
{
	if (timer > delay)
		return;
	else
		timer += dt;
}

bool Gun::CheckDelay()
{
	return timer > delay;
}

void Gun::AddItem(Item item)
{
	inventory.AddItem(item);
	UpdateInventory(item);
}

void Gun::UpdateInventory(Item& item)
{
	if (Pistol* test = dynamic_cast<Pistol*>(this))
	{
		test->bullet_info.damage += item.increased_dmg;
		test->bullet_info.duration += item.increased_lifetime;
		test->bullet_info.size += item.increased_size;
		test->bullet_info.speed += item.increased_speed;
		test->delay *= item.decreased_delay;
		return;
	}
	if (Shotgun* test = dynamic_cast<Shotgun*>(this))
	{
		test->bullet_info.damage += item.increased_dmg;
		test->bullet_info.size += item.increased_size;
		test->bullet_info.speed += item.increased_speed;
		test->delay *= item.decreased_delay;
		test->bullet_number += item.increased_barrel;
		return;
	}
	if (Gatling* test = dynamic_cast<Gatling*>(this))
	{
		test->bullet_info.damage += item.increased_dmg;
		test->bullet_info.size += item.increased_size;
		test->bullet_info.speed += item.increased_speed;
		test->delay *= item.decreased_delay;
		test->bullet_number += item.increased_barrel;
		return;
	}
}

void Gun::InitInventory()
{
	inventory.Init();
}

//void Gun::Visualize()
//{
//
//}