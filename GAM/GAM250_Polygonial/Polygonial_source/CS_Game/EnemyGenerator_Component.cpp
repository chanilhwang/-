/**
 * @file EnemyGenerator_Component.cpp
 * @author Sungmin Moon
 * @email elphior0@gmail.com
 * @brief Enemy generator logic
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */

#include "EnemyGenerator_Component.hpp"
#include "CS_Color.hpp"
#include "CS_Mesh.hpp"
#include "../Dependencies/imgui/imgui.h"
#include "CS_StageManager.hpp"
#include "CS_JsonManager.hpp"
#include "Chase_Component.hpp"
#include "Shoot_Component.hpp"
#include "Strafe_Component.hpp"
#include "CS_Input.hpp"
#include "GenEffect_Component.hpp"

EnemyGenerator_Component::EnemyGenerator_Component() : CS_Component(CS_ComponentTypes::EnemyGenerator_Component)
{
	wave_num = 5;
	wave_info.resize(5);
}

void EnemyGenerator_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
	jsonObject.ReadData("# of Waves", wave_num);
	wave_info.resize(wave_num);
	for (int i = 0; i < wave_num; i++)
	{
		std::string wave = "#" + std::to_string(i);
		int helper;
		jsonObject.ReadData("Randomize Wave " + wave, helper);
		if (helper == 0)
			wave_info[i].randomize = false;
		else if (helper == 1)
			wave_info[i].randomize = true;

		jsonObject.ReadData("Enemy Count " + wave, wave_info[i].num_enemies);
		jsonObject.ReadData("Gentimer " + wave, wave_info[i].gentimer);
		jsonObject.ReadData("Time till Next Wave " + wave, wave_info[i].time_until_next_wave);

		std::string str;
		std::string counter;
		if (!wave_info[i].randomize)
		{
			wave_info[i].color_type.resize(wave_info[i].num_enemies);
			wave_info[i].shape_type.resize(wave_info[i].num_enemies);
			for (int j = 0; j < wave_info[i].num_enemies; j++)
			{
				str = "Color ";
				counter = "-" + std::to_string(j);
				str += wave + counter;
				jsonObject.ReadData(str, helper);
				wave_info[i].color_type[j] = (Color)helper;

				str = "Shape ";
				str += wave + counter;
				jsonObject.ReadData(str, helper);
				wave_info[i].shape_type[j] = (Shape)helper;
			}
		}
	}
}

void EnemyGenerator_Component::WriteToFile(CS_JsonObject & jsonObject)
{	
	jsonObject.WriteData("# of Waves", wave_num);
	for (int i = 0; i < wave_num; i++)
	{
		std::string wave = "#" + std::to_string(i);
		jsonObject.WriteData("Randomize Wave " + wave, static_cast<int>(wave_info[i].randomize));
		jsonObject.WriteData("Enemy Count " + wave, wave_info[i].num_enemies);
		jsonObject.WriteData("Gentimer " + wave, wave_info[i].gentimer);
		jsonObject.WriteData("Time till Next Wave " + wave, wave_info[i].time_until_next_wave);

		std::string str;
		std::string counter;
		if (!wave_info[i].randomize)
		{
			for (int j = 0; j < wave_info[i].num_enemies; j++)
			{
				str = "Color ";
				counter = "-" + std::to_string(j);
				str += wave + counter;
				jsonObject.WriteData(str, static_cast<int>(wave_info[i].color_type[j]));

				str = "Shape ";
				str += wave + counter;
				jsonObject.WriteData(str, static_cast<int>(wave_info[i].shape_type[j]));
			}
		}
	}
}

void EnemyGenerator_Component::Init()
{
	timer = 0.f;
	cam = CS_StageManager::GetCameraOfCurrentLevel();
	enemy_counter = 0;
	position.resize(4);
	wave_counter = 0;
	current_wave = &wave_info[wave_counter];
	InfiniteMode = false;
	GameFinished = false;
}

EnemyGenerator_Component::~EnemyGenerator_Component()
{
}

void EnemyGenerator_Component::Update(float dt)
{
	// Cheat code to test randomized infinite waves
	
	if (CS_Input::IsTriggered(InputButtons::M))
	{
		
		InfiniteMode = true;
		infinite.randomize = true;
		infinite.num_enemies = 10;
		infinite.gentimer = 1.f;
		infinite.time_until_next_wave = 5.f;
		current_wave = &infinite;
		wave_counter = 0;
		
		
		/*
		GameFinished = true;
		game_end.randomize = false;
		game_end.num_enemies = 0;
		game_end.gentimer = 0.f;
		game_end.time_until_next_wave = 3.f;
		current_wave = &game_end;
		timer = 0.f;
		current_wave->gentimer = 3.f;
		*/

		//timer = 0.f;
		//current_wave = &wave_info[wave_num - 1];
	}
	
	position.at(0) = { cam->GetLeft_x(), cam->GetTop_y() + reposition };
	position.at(1) = { cam->GetRight_x() + reposition, cam->GetBot_y() };
	position.at(2) = { cam->GetLeft_x(), cam->GetBot_y() - reposition };
	position.at(3) = { cam->GetLeft_x() - reposition, cam->GetBot_y() };
	timer += dt;

	if (enemy_counter < current_wave->num_enemies)
	{
		if (timer >= current_wave->gentimer)
		{
			if (current_wave->randomize || InfiniteMode)
				RandomlyGenerateEnemy(dt);
			else
			{
				shape = current_wave->shape_type[enemy_counter];
				color = current_wave->color_type[enemy_counter];
				GenerateSetEnemy(shape, color, dt);
			}
			timer = 0;
			enemy_counter++;
		}
	}
	else
	{
		if (InfiniteMode)
		{
			wave_counter++;
			if (wave_counter == 3)
			{
				if (infinite.gentimer > 0.5f)
					infinite.gentimer -= 0.2f;
			}
			enemy_counter = 0;
			infinite.num_enemies++;
			timer = 0.f;
		}/*
		else if (GameFinished)
		{
			if (timer >= current_wave->time_until_next_wave)
				CS_StageManager::SetNextStage(CS_StageTypes::Clear_Stage);
		}*/
		else if (wave_counter < wave_num - 1)
		{
			if (timer >= current_wave->time_until_next_wave)
			{
				timer = 0.f;
				enemy_counter = 0;
				wave_counter++;
				current_wave = &wave_info[wave_counter];
			}
		}
		else
		{
			
			InfiniteMode = true;
			infinite.randomize = true;
			infinite.num_enemies = 10;
			infinite.gentimer = 1.f;
			infinite.time_until_next_wave = 5.f;
			current_wave = &infinite;
			wave_counter = 0;
			

			// Move to Clear Stage
			/*
		         if (CS_ObjectManager::GetFirstObjectByName("enemy") == nullptr)
			{
				// Move to clear
				GameFinished = true;
				game_end.randomize = false;
				game_end.num_enemies = 0;
				game_end.gentimer = 0.f;
				game_end.time_until_next_wave = 3.f;
				current_wave = &game_end;
				timer = 0.f;
				current_wave->gentimer = 3.f;
			}
                    */
		}
	}

}

EnemyGenerator_Component * EnemyGenerator_Component::Clone() const
{
	return new EnemyGenerator_Component;
}

void EnemyGenerator_Component::ShowGui()
{
	if (ImGui::TreeNode("EnemyGenerator_Component"))
	{
		ImGui::DragInt("# of Waves", &wave_num);
		wave_info.resize(wave_num);
		for (int wave_index = 0; wave_index < wave_num; wave_index++)
		{
			std::string wave = "Wave #";
			wave += std::to_string(wave_index);
			if (ImGui::TreeNode(wave.c_str()))
			{
				ImGui::Checkbox("Randomize?", &wave_info[wave_index].randomize);
				ImGui::DragInt("# of Enemies", &wave_info[wave_index].num_enemies);
				ImGui::DragFloat("Gentimer", &wave_info[wave_index].gentimer);
				ImGui::DragFloat("Time Until Next Wave", &wave_info[wave_index].time_until_next_wave);
				if (!wave_info[wave_index].randomize)
				{
					wave_info[wave_index].color_type.resize(wave_info[wave_index].num_enemies);
					wave_info[wave_index].shape_type.resize(wave_info[wave_index].num_enemies);
					for (int i = 0; i < wave_info[wave_index].num_enemies; i++)
					{
						int selected_color = wave_info[wave_index].color_type[i];
						const char* color_list[Color_Count] = { "Null", "Black", "White" };
						std::string list = "Color " + std::to_string(i);
						ImGui::ListBox(list.c_str(), &selected_color, color_list, Color_Count, -1);
						if (selected_color == 0)
							wave_info[wave_index].color_type[i] = Null_Color;
						else if (selected_color == 1)
							wave_info[wave_index].color_type[i] = Black;
						else if (selected_color == 2)
							wave_info[wave_index].color_type[i] = White;

						int selected_shape = wave_info[wave_index].shape_type[i];
						const char* shape_list[Shape_Count] = { "Null", "Triangle", "Rectangle", "Pentagon" };
						list = "Shape " + std::to_string(i);
						ImGui::ListBox(list.c_str(), &selected_shape, shape_list, Shape_Count, -1);
						if (selected_shape == 0)
							wave_info[wave_index].shape_type[i] = Null_Shape;
						else if (selected_shape == 1)
							wave_info[wave_index].shape_type[i] = Triangle;
						else if (selected_shape == 2)
							wave_info[wave_index].shape_type[i] = Rectangle;
						else if (selected_shape == 3)
							wave_info[wave_index].shape_type[i] = Pentagon;
					}
				}
				ImGui::TreePop();
			}
		}
		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}

int EnemyGenerator_Component::RandomDice(int max)
{
	return (rand() % max);
}

glm::vec2 EnemyGenerator_Component::RandomizePosition()
{
	int random = RandomDice(4);
	glm::vec2 enemy_position = position[random];
	if (random == 0 || random == 2)
	{
		int position_randomizer = rand() % (int)(cam->GetRight_x() - cam->GetLeft_x());
		enemy_position.x += (float)position_randomizer;
	}
	else
	{
		int position_randomizer = rand() % (int)(cam->GetTop_y() - cam->GetBot_y());
		enemy_position.y += (float)position_randomizer;
	}
	return enemy_position;
}

void EnemyGenerator_Component::RandomlyGenerateEnemy(float dt)
{
	glm::vec2 enemy_position = RandomizePosition();
	shape = static_cast<Shape>(rand() % (Shape::Shape_Count - 1) + 1);
	color = static_cast<Color>(rand() % (Color::Color_Count - 1) + 1);

	CS_Object* spawn = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::GenEffect);
	spawn->transform.SetTranslation(enemy_position);
	if (shape == Triangle)
		CS_SoundManager::PlaySound(CS_SoundTypes::GenSound);
	else if (shape == Rectangle)
		CS_SoundManager::PlaySound(CS_SoundTypes::RectangleGen);
	else if (shape == Pentagon)
		CS_SoundManager::PlaySound(CS_SoundTypes::PentagonGen);
	GenEffect_Component* geneffect;
	spawn->GetComponent(CS_ComponentTypes::GenEffect_Component, geneffect);
	geneffect->SetupTexture(shape, color);
}

void EnemyGenerator_Component::GenerateSetEnemy(Shape given_shape, Color given_color, float dt)
{
	glm::vec2 enemy_position = RandomizePosition();

	shape = given_shape;
	color = given_color;

	if (given_shape == Null_Shape)
		shape = static_cast<Shape>(rand() % (Shape::Shape_Count - 1) + 1);
	if (given_color == Null_Color)
		color = static_cast<Color>(rand() % (Color::Color_Count - 1) + 1);



	CS_Object* spawn = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::GenEffect);
	spawn->transform.SetTranslation(enemy_position);
	if (shape == Triangle)
		CS_SoundManager::PlaySound(CS_SoundTypes::GenSound);
	else if (shape == Rectangle)
		CS_SoundManager::PlaySound(CS_SoundTypes::RectangleGen);
	else if (shape == Pentagon)
		CS_SoundManager::PlaySound(CS_SoundTypes::PentagonGen);
	GenEffect_Component* geneffect;
	spawn->GetComponent(CS_ComponentTypes::GenEffect_Component, geneffect);
	geneffect->SetupTexture(shape, color);
}