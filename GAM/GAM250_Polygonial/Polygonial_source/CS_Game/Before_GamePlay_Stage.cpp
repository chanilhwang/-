/**
 * @file Before_GamePlay_Stage.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Weapon-Selection stage
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Before_GamePlay_Stage.hpp"
#include "CS_Object.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_StageManager.hpp"
#include "Player_Component.hpp"
#include "Enemy_Component.hpp"
#include "CS_Input.hpp"
#include <sstream>
#include "CS_JsonManager.hpp"
#include "CS_GUI.hpp"
#include <iostream>
#include "CS_SoundManager.hpp"
/**
 * @brief Destroy the GamePlay_Stage::GamePlay_Stage object
 *
 */
Before_GamePlay_Stage::~Before_GamePlay_Stage()
{

}
/**
 * @brief Initialize the game level based on the level JSON file.
 *
 */
void Before_GamePlay_Stage::Init()
{;
	std::stringstream file;
	file << "OtherLevels/";
	file << "Before_GamePlay.json";
	CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
        selecter = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        selecter->gfx.SetTextureHandle("Textures/weapon_selector.png");
        selecter->transform.SetScale(glm::vec2(60.f, 60.f));
}
/**
 * @brief Update based on the inputs.
 *
 * @param dt Delta time.
 */
void Before_GamePlay_Stage::Update(float dt)
{
    if (CS_Input::IsTriggered(InputButtons::Escape))
    {
        CS_StageManager::SetNextStage(CS_StageTypes::Menu_Stage);
    }
    if (object_array[0] == nullptr)
    {
        object_array[0] = CS_ObjectManager::GetFirstObjectByName("Pistol");
    }
    if (object_array[1] == nullptr)
    {
        object_array[1] = CS_ObjectManager::GetFirstObjectByName("Shotgun");
    }
    if (object_array[2] == nullptr)
    {
        object_array[2] = CS_ObjectManager::GetFirstObjectByName("Gatling");
    }

    if (CS_Input::IsTriggered(InputButtons::A))
    {
        CS_SoundManager::PlaySound(CS_SoundTypes::Back);
        if (button_count == 0)
            button_count = 2;
        else
            --button_count;
    }
    if (CS_Input::IsTriggered(InputButtons::D))
    {
        CS_SoundManager::PlaySound(CS_SoundTypes::Back);
        if (button_count == 2)
            button_count = 0;
        else
            ++button_count;
    }
    glm::vec2 selected_object_position = object_array[button_count]->transform.GetTranslation();
    selecter->transform.SetTranslation(glm::vec2(selected_object_position.x, selected_object_position.y + 340));

    if (CS_Input::IsTriggered(InputButtons::Space))
    {
        CS_SoundManager::PlaySound(CS_SoundTypes::Select);
        if (object_array[button_count]->m_name == "Pistol")
        {
            CS_StageManager::GetGameData().PlayerWeapon = "Pistol";
            CS_StageManager::SetNextStage(CS_StageTypes::GamePlay1_Stage);
        }
        if (object_array[button_count]->m_name == "Shotgun")
        {
            CS_StageManager::GetGameData().PlayerWeapon = "Shotgun";
            CS_StageManager::SetNextStage(CS_StageTypes::GamePlay1_Stage);
        }
        if (object_array[button_count]->m_name == "Gatling")
        {
            CS_StageManager::GetGameData().PlayerWeapon = "Gatling";
            CS_StageManager::SetNextStage(CS_StageTypes::GamePlay1_Stage);
        }
    }

}
/**
 * @brief Shut down current level.
 *
 */
void Before_GamePlay_Stage::Shutdown()
{
	camera.SetCenter_x(0);
	CS_ObjectManager::DestroyAllObjects(true);
	CS_SoundManager::StopSound(CS_SoundTypes::BGM);
}