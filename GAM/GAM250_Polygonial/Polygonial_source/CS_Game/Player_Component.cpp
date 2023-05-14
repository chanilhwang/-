/**
 * @file Player_Component.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Player Component
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include <exception>
#include "Player_Component.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_Input.hpp"
#include <glm/glm.hpp>
#include "../Dependencies/imgui/imgui.h"
#include "CS_Physics.hpp"
#include "CS_StageManager.hpp"
#include "CS_Camera.hpp"
#include "Bullet_Component.hpp"
#include "CS_JsonManager.hpp"
#include "Gun_List.hpp"
#include "Enemy_Component.hpp"

/**
 * @brief Construct a new Player_Component::Player_Component object
 * 
 */
Player_Component::Player_Component() 
	: CS_Component(CS_ComponentTypes::Player_Component)
{
	invincibility = 0.005f;
	speed = 400.f;
	original_speed = speed;
	invincibility_timer = 0.0f;
	invincible_mode = false;
	color = White;
	gun = nullptr;
        sound_bool = true;
        
}
/**
 * @brief Destroy the Player_Component::Player_Component object
 * 
 */
Player_Component::~Player_Component()
{
    only_once = true;
}
/**
 * @brief Read initialization data from the JSON file.
 * 
 * @param jsonObject The JSON file to read from.
 */
void Player_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
	jsonObject.ReadData("invincibility", invincibility);
	jsonObject.ReadData("speed", speed);
	std::string Invincible_mode_str;
	jsonObject.ReadData("invincible_mode", Invincible_mode_str);
	if (Invincible_mode_str == "true")
	{
		invincible_mode = true;
	}
	else if (Invincible_mode_str == "false")
	{
		invincible_mode = false;
	}
	int max_health;
	jsonObject.ReadData("hp", max_health);
	hp.Set_Max_HP(max_health);

	jsonObject.ReadData("Pistol_Speed", pistol_info.bullet_info.speed);
	jsonObject.ReadData("Pistol_Bullet_Size", pistol_info.bullet_info.size);
	jsonObject.ReadData("Pistol_Damage", pistol_info.bullet_info.damage);
	jsonObject.ReadData("Pistol_Duration", pistol_info.bullet_info.duration);

	jsonObject.ReadData("Shotgun_Speed", shotgun_info.bullet_info.speed);
	jsonObject.ReadData("Shotgun_Bullet_Size", shotgun_info.bullet_info.size);
	jsonObject.ReadData("Shotgun_Damage", shotgun_info.bullet_info.damage);
	jsonObject.ReadData("Shotgun_Duration", shotgun_info.bullet_info.duration);
	jsonObject.ReadData("Shotgun_Angle", shotgun_info.angle);
	jsonObject.ReadData("Shotgun_Bullet_Number", shotgun_info.bullet_num);

	jsonObject.ReadData("Gatling_Speed", gatling_info.bullet_info.speed);
	jsonObject.ReadData("Gatling_Bullet_Size", gatling_info.bullet_info.size);
	jsonObject.ReadData("Gatling_Damage", gatling_info.bullet_info.damage);
	jsonObject.ReadData("Gatling_Duration", gatling_info.bullet_info.duration);
	jsonObject.ReadData("Gatling_Bullet_Number", gatling_info.bullet_num);
	jsonObject.ReadData("Gatling_Distance_Between_Bullets", gatling_info.distance_between_bullets);

	jsonObject.ReadData("Dieing_Time", dieing_time);
	jsonObject.ReadData("Dieing_Rotation_Radian", dieing_rotation_radian);
	jsonObject.ReadData("Dieing_Reducing_Scale", dieing_reducing_scale);
}
/**
 * @brief Write data into the JSON file.
 * 
 * @param jsonObject The JSON file to write into.
 */
void Player_Component::WriteToFile(CS_JsonObject & jsonObject)
{
	jsonObject.WriteData("invincibility", invincibility);
	jsonObject.WriteData("speed", speed);
	std::string Invincible_mode_str;
	if (invincible_mode)
	{
		Invincible_mode_str == "true";
	}
	else
	{
		Invincible_mode_str == "false";
	}
	jsonObject.WriteData("invincible_mode", Invincible_mode_str);
	int max_hp = hp.Get_Max_HP();
	jsonObject.WriteData("hp", max_hp);

	jsonObject.WriteData("Pistol_Speed", pistol_info.bullet_info.speed);
	jsonObject.WriteData("Pistol_Bullet_Size", pistol_info.bullet_info.size);
	jsonObject.WriteData("Pistol_Damage", pistol_info.bullet_info.damage);
	jsonObject.WriteData("Pistol_Duration", pistol_info.bullet_info.duration);

	jsonObject.WriteData("Shotgun_Speed", shotgun_info.bullet_info.speed);
	jsonObject.WriteData("Shotgun_Bullet_Size", shotgun_info.bullet_info.size);
	jsonObject.WriteData("Shotgun_Damage", shotgun_info.bullet_info.damage);
	jsonObject.WriteData("Shotgun_Duration", shotgun_info.bullet_info.duration);
	jsonObject.WriteData("Shotgun_Angle", shotgun_info.angle);
	jsonObject.WriteData("Shotgun_Bullet_Number", shotgun_info.bullet_num);

	jsonObject.WriteData("Gatling_Speed", gatling_info.bullet_info.speed);
	jsonObject.WriteData("Gatling_Bullet_Size", gatling_info.bullet_info.size);
	jsonObject.WriteData("Gatling_Damage", gatling_info.bullet_info.damage);
	jsonObject.WriteData("Gatling_Duration", gatling_info.bullet_info.duration);
	jsonObject.WriteData("Gatling_Bullet_Number", gatling_info.bullet_num);
	jsonObject.WriteData("Gatling_Distance_Between_Bullets", gatling_info.distance_between_bullets);

	jsonObject.WriteData("Dieing_Time", dieing_time);
	jsonObject.WriteData("Dieing_Rotation_Radian", dieing_rotation_radian);
	jsonObject.WriteData("Dieing_Reducing_Scale", dieing_reducing_scale);
}
/**
 * @brief Initialize.
 * 
 */
void Player_Component::Init()
{
	m_Parent->transform.SetTranslation(glm::vec2(0, 0));
        only_once = true;
        sound_bool = true;
        accumulate_dt = 0;
	dieing_time = 3.0f;
	hp.FullFill();
        m_Parent->m_name = "Player";
	invincibility_timer = 0.f;
	color = Black;
        m_Parent->gfx.SetTextureHandle("Textures/Black_Arrow.png");
	InitGun();

        hp_object = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        hp_object->gfx.SetTextureHandle("Textures/hp_3.png");
        hp_object->transform.SetScale(glm::vec2(200, 200));
        only_once = false;
        avoid_timer = 0;
        avoid_delay = 0.5f;
        /*gun = nullptr;*/

}
/**
 * @brief Update player based on the input.
 * 
 * @param dt Delta time.
 */
void Player_Component::Update(float dt)
{
        //accumulate_dt -= dt;
	glm::vec2 mouse_pos = CS_Input::GetMouseWorldPos();
	glm::vec2 last_pos = m_Parent->transform.GetTranslation();
	glm::vec2 rotation_vector = glm::normalize(mouse_pos - last_pos);
	m_Parent->transform.SetRotationWithVector(rotation_vector);
        gun->Cursor_update(mouse_pos);
	/*if (camera == nullptr)
		return;*/
	if (hp.Get_Current_HP() <= 0)
	{
		Die(dt);
	}
	else
	{
		if (!invincible_mode)
			CheckCollideAndGetDamaged(dt);
		if(CS_Input::IsAnyPressed())
			PlayerInput();
		gun->UpdateDelay(dt);
	}
        //hp_object->transform.SetRotation(accumulate_dt);
        hp_object->transform.SetTranslation(m_Parent->transform.GetTranslation() + glm::vec2(0, -10));

		if (m_Parent->transform.GetTranslation() != glm::vec2(0, 0))
			m_Parent->physics.setVelocity(m_Parent->physics.getVelocity() + 0.1f * speed * glm::normalize(glm::vec2(0, 0) - m_Parent->transform.GetTranslation()));

}
/**
 * @brief Make a clone of this component.
 * 
 * @return Player_Component* Clone of this component.
 */
Player_Component * Player_Component::Clone() const
{
    Player_Component *temp = new Player_Component;
    return temp;
}
/**
 * @brief For imgui button usage.
 * 
 */
void Player_Component::ShowGui()
{
	if (ImGui::TreeNode("Player_Component"))
	{
		ImGui::DragFloat("Invincibility Time", &invincibility);
		ImGui::DragFloat("Speed", &speed);
		int helper = hp.Get_Max_HP();
		ImGui::DragInt("Max_Health", &helper);
		hp.Set_Max_HP(helper);
		ImGui::Checkbox("Invincibility_mode", &invincible_mode);

		if (ImGui::TreeNode("Pistol"))
		{
			ImGui::DragFloat("Speed", &pistol_info.bullet_info.speed);
			ImGui::DragFloat2("Bullet_Size", &pistol_info.bullet_info.size.x);
			ImGui::DragInt("Damage", &pistol_info.bullet_info.damage);
			ImGui::DragFloat("Duration", &pistol_info.bullet_info.duration);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("ShotGun"))
		{
			ImGui::DragFloat("Speed", &shotgun_info.bullet_info.speed);
			ImGui::DragFloat2("Bullet_Size", &shotgun_info.bullet_info.size.x);
			ImGui::DragInt("Damage", &shotgun_info.bullet_info.damage);
			ImGui::DragFloat("Duration", &shotgun_info.bullet_info.duration);
			ImGui::DragFloat("Angle", &shotgun_info.angle);
			ImGui::DragInt("Bullet_Number", &shotgun_info.bullet_num);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Gatling"))
		{
			ImGui::DragFloat("Speed", &gatling_info.bullet_info.speed);
			ImGui::DragFloat2("Bullet_Size", &gatling_info.bullet_info.size.x);
			ImGui::DragInt("Damage", &gatling_info.bullet_info.damage);
			ImGui::DragFloat("Duration", &gatling_info.bullet_info.duration);
			ImGui::DragInt("Bullet_Number", &gatling_info.bullet_num);
			ImGui::DragFloat("Distance_Between_Bullets", &gatling_info.distance_between_bullets);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Dieing_Motion"))
		{
			ImGui::DragFloat("time", &dieing_time);
			ImGui::DragFloat("rotation radian", &dieing_rotation_radian);
			ImGui::DragFloat("reducing scale", &dieing_reducing_scale);

			ImGui::TreePop();
		}

		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}

Color Player_Component::GetColor()
{
	return color;
}

void Player_Component::PlayerInput()
{
	//map limitation
	//constexpr float mapwidth = 100.f;
	//glm::vec2 mapsize = camera->GetMapSize();
	////movement
	//auto& physics = m_Parent->physics;
	//const auto& translation = m_Parent->transform.GetTranslation();
	//if (-mapsize.x / 2 + mapwidth >= translation.x) {}
	//else if (CS_Input::IsPressed(InputButtons::A))
	//	physics.setVelocity_X(physics.getVelocity().x - speed);
	//else
	//	physics.setVelocity_X(physics.getVelocity().x);

	//if (mapsize.x / 2 - mapwidth <= translation.x) {}
	//else if (CS_Input::IsPressed(InputButtons::D))
	//	physics.setVelocity_X(physics.getVelocity().x + speed);
	//else
	//	physics.setVelocity_X(physics.getVelocity().x);

	//if (mapsize.y / 2  - mapwidth <= translation.y) {}
	//else if (CS_Input::IsPressed(InputButtons::W))
	//	physics.setVelocity_Y(physics.getVelocity().y + speed);
	//else
	//	physics.setVelocity_Y(physics.getVelocity().y);

	//if (-mapsize.y / 2 + mapwidth >= translation.y) {}
	//else if (CS_Input::IsPressed(InputButtons::S))
	//	physics.setVelocity_Y(physics.getVelocity().y - speed);
	//else
	//	physics.setVelocity_Y(physics.getVelocity().y);

    /*const float speed = 400.f;*/
    glm::vec2 movement = { 0.f,0.f };

    if (CS_Input::IsPressed(InputButtons::A))
        left = true;
    else
        left = false;

    if (CS_Input::IsPressed(InputButtons::D))
        right = true;
    else
        right = false;

    if (CS_Input::IsPressed(InputButtons::W))
        up = true;
    else
        up = false;

    if (CS_Input::IsPressed(InputButtons::S))
        down = true;
    else
        down = false;

    if (left)
    {
        movement.x = -1;

    }
    if (right)
    {
        movement.x = 1;

    }
    if (up)
    {
        movement.y = 1;
    }

    if (down)
    {
        movement.y = -1;
    }
	glm::vec2 normalized;
	if (movement.x != 0 || movement.y != 0)
		normalized = glm::normalize(movement);
	else
		normalized = movement;
    m_Parent->physics.setVelocity(normalized * speed);
	//invincible mode
	if (CS_Input::IsPressed(InputButtons::Ctrl) && CS_Input::IsPressed(InputButtons::I))
	{
		invincible_mode = true;
	}

	//Change Color
	if (CS_Input::IsTriggered(InputButtons::Space))
	{
		if (color == Black)
		{
			color = White;
			m_Parent->gfx.SetTextureHandle("Textures/White_Arrow.png");
		}
		else
		{
			color = Black;
			m_Parent->gfx.SetTextureHandle("Textures/Black_Arrow.png");
		}
	}

	//Use Weapons
	if (CS_Input::IsPressed(InputButtons::Mouse_Left))
	{
		gun->Shoot(MakePlayerInfo());
	}

        if (CS_Input::IsPressed(InputButtons::Mouse_Right))
        {
            gun->After_Shoot(MakePlayerInfo());
        }

	if (CS_Input::IsTriggered(InputButtons::_1))
	{
		delete gun;
		gun = new Pistol(pistol_info);
	}
	if (CS_Input::IsTriggered(InputButtons::_2))
	{
		delete gun;
		gun = new Shotgun(shotgun_info);
	}
	if (CS_Input::IsTriggered(InputButtons::_3))
	{
		delete gun;
		gun = new Gatling(gatling_info);
	}

}

void Player_Component::CheckCollideAndGetDamaged(float dt)
{
	CS_ColliderComponent* mycollider;
	m_Parent->GetComponent(CS_ComponentTypes::SphereCollider_Component, mycollider);
        avoid_timer += dt;
	if (mycollider->is_Ghost)
	{
		if (invincibility_timer > invincibility)
		{
			mycollider->is_Ghost = false;
			invincibility_timer = 0.f;
		}
		invincibility_timer += dt;
	}
	else
	{
		if (!CS_Physics::IsThereAnyColliding(m_Parent->my_id))
			return;
		auto* CollidedObject = CS_Physics::GetCollidedObjectWithName(m_Parent->my_id, "Enemy");
		if (CollidedObject == nullptr)
			CollidedObject = CS_Physics::GetCollidedObjectWithName(m_Parent->my_id, "Enemy_Bullet");
		else
		{
			Enemy_Component* enemy;
			CollidedObject->GetComponent(CS_ComponentTypes::Enemy_Component, enemy);
			//Color Check
                        CS_SoundManager::PlaySound(CS_SoundTypes::PlayerHit);
			hp.Damaged(enemy->GetDamage());
                        if(hp.Get_Current_HP() == 2)
                            hp_object->gfx.SetTextureHandle("Textures/hp_2.png");
                        if (hp.Get_Current_HP() == 1)
                            hp_object->gfx.SetTextureHandle("Textures/hp_1.png");
			mycollider->is_Ghost = true;
			invincibility_timer = 0.f;
			enemy->IsCollidedPlayer = true;
			CS_Camera::SetShakeOn();
			return;
		}
		if (CollidedObject != nullptr)
		{
			Bullet_Component* bullet;
			CollidedObject->GetComponent(CS_ComponentTypes::Bullet_Component, bullet);
			if (color == bullet->Get_Color()) {
                            CS_SoundManager::PlaySound(CS_SoundTypes::PlayerHit);
				hp.Damaged(bullet->Get_Damage());
                                if (hp.Get_Current_HP() == 2)
                                    hp_object->gfx.SetTextureHandle("Textures/hp_2.png");
                                if (hp.Get_Current_HP() == 1)
                                    hp_object->gfx.SetTextureHandle("Textures/hp_1.png");
				CollidedObject->is_dead = true;
				mycollider->is_Ghost = true;
				invincibility_timer = 0.f;
				CS_Camera::SetShakeOn();
			}
                        else
                        {
                            if (avoid_timer > avoid_delay)
                            {
                                CS_SoundManager::PlaySound(CS_SoundTypes::Back);
                                avoid_timer = 0;
                            }

                        }
		}
               
	}
}

void Player_Component::SelectGun(std::string gun_name)
{
	if (gun_name == "pistol")
		gun = new Pistol(pistol_info);
	else if (gun_name == "shotgun")
		gun = new Shotgun(shotgun_info);
	else if (gun_name == "gatling")
		gun = new Gatling(gatling_info);
	else
		throw std::exception("Unvalid gun_name");
}

void Player_Component::InitGun()
{
	if(gun != nullptr)
		delete gun;
	if (CS_StageManager::GetGameData().PlayerWeapon == "Pistol") {
		gun = new Pistol(pistol_info);
	}
	else if (CS_StageManager::GetGameData().PlayerWeapon == "Gatling") {
		gun = new Gatling(gatling_info);
	}
	else if (CS_StageManager::GetGameData().PlayerWeapon == "Shotgun") {
		gun = new Shotgun(shotgun_info);
	}
}


void Player_Component::DeleteGun()
{
    if(gun)
    {
        delete gun;
        gun = nullptr;
    }
}

Needed_Player_Info Player_Component::MakePlayerInfo() const
{
	return { m_Parent->transform.GetTranslation(), color };
}

void Player_Component::Die(float dt)
{
    CS_SoundManager::StopSound(CS_SoundTypes::MG_cooldown);
    CS_SoundManager::StopSound(CS_SoundTypes::machinegun_spin);
    CS_ObjectManager::DestoryObject(hp_object);
        if(sound_bool)
        {
            CS_SoundManager::PlaySound(CS_SoundTypes::Explosion1);
            sound_bool = false;
        }
       
	static bool IsParticleCreated = false;
	if (!IsParticleCreated) {
		CS_ArcheTypes archetype = CS_ArcheTypes::Particle;
		ParticleGenerator::MakeParticles(60,
			archetype,
			m_Parent->transform.GetTranslation(),
			0, 360,
			2500, 3000,
			1.3f);
		IsParticleCreated = true;
	}
	else if (dieing_time > 0) {
		Rotate(dieing_rotation_radian);
		m_Parent->transform.SetScale(m_Parent->transform.GetScale() * dieing_reducing_scale);
		dieing_time -= dt;
	}
	else {
		is_Dead = true;
		IsParticleCreated = true;
                CS_StageManager::SetNextStage(CS_StageTypes::Lose_Stage);
		//Set game over stage
	}
        
}

void Player_Component::Rotate(float dieing_rotation_radian)
{
	const float& rot = m_Parent->transform.GetRotation();
	m_Parent->transform.SetRotation(rot + dieing_rotation_radian);
}

void Player_Component::SetSpeed(float speed)
{
	this->speed = speed;
}

float Player_Component::GetSpeed()
{
    return speed;
}
