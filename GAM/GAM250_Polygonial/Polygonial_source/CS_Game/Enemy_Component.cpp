/**
 * @file Enemy_Component.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Enemy Component
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Enemy_Component.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_Physics.hpp"
#include "../Dependencies/imgui/imgui.h"
#include "Bullet_Component.hpp"
#include "CS_JsonManager.hpp"
#include "CS_ResourceLists.hpp"
#include "CS_Particle.hpp"

/**
 * @brief Construct a new Enemy_Component::Enemy_Component object
 * 
 */
Enemy_Component::Enemy_Component() : CS_Component(CS_ComponentTypes::Enemy_Component)
{
	color = Color::Null_Color;
	shape = Shape::Null_Shape;
	StartUpMotionDuration = 0;
}
/**
 * @brief Read initialization data from the JSON file.
 * 
 * @param jsonObject The JSON file to read from.
 */
void Enemy_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
	int helper;
	jsonObject.ReadData("Max_Health", helper);
	hp.Set_Max_HP(helper);
	jsonObject.ReadData("Damage", helper);
	dmg.Set_damage(helper);
	std::string str;
	jsonObject.ReadData("Color", str);
	if (str == "White")
		color = White;
	else if (str == "Black")
		color = Black;
	jsonObject.ReadData("Shape", str);
	if (str == "Triangle")
		shape = Triangle;
	else if (str == "Rectangle")
		shape = Rectangle;
	else if (str == "Pentagon")
		shape = Pentagon;
}
/**
 * @brief Write data into the JSON file.
 * 
 * @param jsonObject The JSON file to write into.
 */
void Enemy_Component::WriteToFile(CS_JsonObject& jsonObject)
{

	jsonObject.WriteData("Max_Health", hp.Get_Max_HP());
	jsonObject.WriteData("Damage", dmg.Get_damage());

	if(color == Color::Null_Color)
		jsonObject.WriteData("Color", "NULL");
	else if (color == White)
		jsonObject.WriteData("Color", "White");
	else if (color == Black)
		jsonObject.WriteData("Color", "Black");

	if(shape == Shape::Null_Shape)
		jsonObject.WriteData("Shape", "NULL");
	else if (shape == Triangle)
		jsonObject.WriteData("Shape", "Triangle");
	else if (shape == Rectangle)
		jsonObject.WriteData("Shape", "Rectangle");
	else if (shape == Pentagon)
		jsonObject.WriteData("Shape", "Pentagon");
}
/**
 * @brief Speed initialization.
 * 
 */
void Enemy_Component::Init()
{
	m_Parent->m_name = "Enemy";
	SetupTexture();
	StartUp = true;
	StartUpMotionDuration = 2.0f;
}
/**
 * @brief Destroy the Enemy_Component::Enemy_Component object
 * 
 */
Enemy_Component::~Enemy_Component()
{
}
/**
 * @brief Keep tracing the player, and destroy itself
 * 		  when it gets out of the screen.
 * 
 * @param dt Delta time.
 */
void Enemy_Component::Update(float dt)
{
	CheckCollideAndGetDamaged();
	if (IsCollidedPlayer) {
		CollidedPlayerMotion(dt);
	}
	else if (IsGetDamaged) {
		GetDamagedMotion(dt);
	}
	else if (hp.Get_Current_HP() <= 0) {
		Die(dt);
	}
	else {
		Rotate(dt);
		MakeMovementAir(dt);
	}
}
/**
 * @brief Make a clone of this component.
 * 
 * @return Enemy_Component* Clone of this component.
 */
Enemy_Component * Enemy_Component::Clone() const
{
	return new Enemy_Component;
}
/**
 * @brief For imgui button usage.
 * 
 */
void Enemy_Component::ShowGui()
{
	if (ImGui::TreeNode("Enemy_Component"))
	{
		int helper = hp.Get_Max_HP();
		ImGui::DragInt("Health", &helper);
		hp.Set_Max_HP(helper);
		helper = dmg.Get_damage();
		ImGui::DragInt("Damage", &helper);
		dmg.Set_damage(helper);

		static int selected_color = color;
		static const char* color_list[Color::Color_Count] = { "Null", "White", "Black" };
		ImGui::ListBox("Color", &selected_color, color_list, Color::Color_Count, -1);
		if (selected_color == 0)
			color = Color::Null_Color;
		else if (selected_color == 1)
			color = White;
		else if (selected_color == 2)
			color = Black;

		static int selected_shape = shape;
		static const char* shape_list[Shape::Shape_Count] = { "Null", "Triangle", "Rectangle", "Pentagon" };
		ImGui::ListBox("Shape", &selected_shape, shape_list, Shape::Shape_Count, -1);
		if (selected_shape == 0)
			shape = Shape::Null_Shape;
		else if (selected_shape == 1)
			shape = Triangle;
		else if (selected_shape == 2)
			shape = Rectangle;
		else if(selected_shape == 3)
			shape = Pentagon;

		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}

void Enemy_Component::SetShape(Shape newShape)
{
	shape = newShape;
}

void Enemy_Component::SetColor(Color newColor)
{
	color = newColor;
}

void Enemy_Component::CheckCollideAndGetDamaged()
{
	if (!CS_Physics::IsThereAnyColliding(m_Parent->my_id))
		return;

	CS_Object* CollidedObject = CS_Physics::GetCollidedObjectWithName(m_Parent->my_id, "Player_Bullet");

	if (CollidedObject != nullptr)
	{
		Bullet_Component* bullet;
		CollidedObject->GetComponent(CS_ComponentTypes::Bullet_Component, bullet);
		if (bullet == nullptr)
			return;
		if (bullet->Get_Color() == color && !CollidedObject->is_dead) {
			if (bullet->Get_Color() == color)
			{
                            CS_SoundManager::PlaySound(CS_SoundTypes::EnemyHit);
				hp.Damaged(bullet->Get_Damage());
				CollidedObject->is_dead = true;
				if (!IsGetDamaged) {
					IsGetDamaged = true;
					timer = 0.3f;
				}
			}
		}
	}
}

void Enemy_Component::Rotate(float dt)
{
	const float& rot = m_Parent->transform.GetRotation();
	m_Parent->transform.SetRotation(rot + dt);
}

Color Enemy_Component::GetColor() const
{
	return color;
}

void Enemy_Component::Die(float dt)
{
	CS_ArcheTypes archetype;
	if (GetColor() == Black)
		archetype = CS_ArcheTypes::Black_Particle;
	else if (GetColor() == White)
		archetype = CS_ArcheTypes::White_Particle;
	ParticleGenerator::MakeParticles(15,
		archetype,
		m_Parent->transform.GetTranslation(),
		0, 360,
		1000, 1200,
		0.5f);
	ParticleGenerator::MakeParticles(15,
		archetype,
		m_Parent->transform.GetTranslation(),
		0, 360,
		500, 600,
		1.0f);
	ParticleGenerator::MakeParticles(4,
		archetype,
		m_Parent->transform.GetTranslation(),
		0, 360,
		250, 300,
		1.3f);
	m_Parent->is_dead = true;
}

void Enemy_Component::StartUpMotion(float dt)
{
	if (!StartUp)
		return;
	Rotate(dt);
}

int Enemy_Component::GetDamage() const
{
	return dmg.Get_damage();
}

void Enemy_Component::SetupTexture()
{
	if (shape && color)
	{
		m_Parent->transform.SetScale({ 100, 100 });
		if (shape == Triangle)
		{

			if (color == Black)
			{
				m_Parent->gfx.SetTextureHandle("Textures/Black_Triangle.png");
			}
			else if (color == White)
			{
				m_Parent->gfx.SetTextureHandle("Textures/White_Triangle.png");
			}
		}
		else if (shape == Rectangle)
		{
			if (color == Black)
			{
				m_Parent->gfx.SetTextureHandle("Textures/Black_Rectangle.png");
			}
			else if (color == White)
			{
				m_Parent->gfx.SetTextureHandle("Textures/White_Rectangle.png");
			}
		}
		else if (shape == Pentagon)
		{
			if (color == Black)
			{
				m_Parent->gfx.SetTextureHandle("Textures/Black_Pentagon.png");
			}
			else if (color == White)
			{
				m_Parent->gfx.SetTextureHandle("Textures/White_Pentagon.png");
			}
		}
	}
}

void Enemy_Component::MakeMovementAir(float dt)
{
	static float timer = 0;
	timer += dt;
	if (timer > 0.05f) {
		ParticleGenerator::MakeParticles(1,
			CS_ArcheTypes::Bubble,
			m_Parent->transform.GetTranslation() + glm::abs(m_Parent->transform.GetScale()) * -glm::normalize(m_Parent->physics.getVelocity()),
			-m_Parent->physics.getVelocity(),
			100.f,
			0.7f);
		timer -= 0.05f;
	}
}

void Enemy_Component::GetDamagedMotion(float dt) {
	if (timer < 0) {
		IsGetDamaged = false;
	}
	timer -= dt;
	if (timer >= 0.15f && timer <= 0.3f) {
		m_Parent->transform.SetScale(m_Parent->transform.GetScale() + dt * glm::vec2(1000, 1000));
	}
	else if (timer >= 0.0f && timer <= 0.15f) {
		m_Parent->transform.SetScale(m_Parent->transform.GetScale() - dt * glm::vec2(1000, 1000));
	}
}

void Enemy_Component::CollidedPlayerMotion(float dt) {
	CS_ArcheTypes arche = CS_ArcheTypes::Particle;
	if (color == Color::Black)
		ParticleGenerator::MakeParticles(4,
			CS_ArcheTypes::Particle,
			m_Parent->transform.GetTranslation(),
			-30, 30,
			300.f, 500.f,
			0.7f);
	else
		ParticleGenerator::MakeParticles(4,
			CS_ArcheTypes::White_Particle,
			m_Parent->transform.GetTranslation(),
			-30, 30,
			300.f, 500.f,
			0.7f);

	m_Parent->is_dead = true;
}