/**
 * @file Quit_Check_Stage.hpp
 * @author Chunmook Kim
 * @email kimmistral\@naver.com
 * @brief Level to check quit menu.
 * @version 0.1
 * @date 2018-12-10
 *
 * @copyright Copyright (c) 2018
 *
 */
#include "Quit_Check_Stage.hpp"
#include "CS_StageManager.hpp"
#include "CS_Input.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_JsonManager.hpp"
#include "CS_SoundManager.hpp"
#include <iostream>
#include "CS_Physics.hpp"
#include "CS_SoundManager.hpp"

void Quit_Check_Stage::Init()
{
	std::stringstream file;
	file << "OtherLevels/";
	file << "Quit_Check_Stage.json";
	CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
	button_count = 0;
}

void Quit_Check_Stage::Update(float dt)
{
	if (object_array[0] == nullptr)
	{
		object_array[0] = CS_ObjectManager::GetFirstObjectByName("yes_Button");
	}
	if (object_array[1] == nullptr)
	{
		object_array[1] = CS_ObjectManager::GetFirstObjectByName("no_Button");
	}

	if (selector[0] == nullptr)
	{
		selector[0] = CS_ObjectManager::GetFirstObjectByName("selector_1");
	}
	if (selector[1] == nullptr)
	{
		selector[1] = CS_ObjectManager::GetFirstObjectByName("selector_2");
	}

	if (CS_Input::IsTriggered(InputButtons::A))
	{
		CS_SoundManager::PlaySound(CS_SoundTypes::Back);

		if (button_count == 1)
			button_count = 0;
		else
			++button_count;
	}
	if (CS_Input::IsTriggered(InputButtons::D))
	{
		CS_SoundManager::PlaySound(CS_SoundTypes::Back);
		if (button_count == 0)
			button_count = 1;
		else
			--button_count;
	}
	glm::vec2 current_object_position = object_array[button_count]->transform.GetTranslation();
	selector[0]->transform.SetTranslation(glm::vec2(current_object_position.x - 200, current_object_position.y));
	selector[1]->transform.SetTranslation(glm::vec2(current_object_position.x + 200, current_object_position.y));
	if (CS_Input::IsTriggered(InputButtons::Space))
	{
		CS_SoundManager::PlaySound(CS_SoundTypes::Select);
		if (object_array[button_count]->m_name == "yes_Button")
		{
			CS_StageManager::Quit();
		}
		if (object_array[button_count]->m_name == "no_Button")
		{
			CS_StageManager::SetNextStage(CS_StageTypes::Menu_Stage);
		}
	}

	ParticleGenerator::MakeBackGroundParticle(20, CS_ArcheTypes::Yellow_Particle);
}

void Quit_Check_Stage::Shutdown()
{
	CS_ObjectManager::DestroyAllObjects(false);
}
