/**
 * @file Menu_Stage.cpp
 * @author Chunmook Kim
 * @email kimmistral\@naver.com
 * @brief Level to display main menu.
 * @version 0.1
 * @date 2018-12-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "Menu_Stage.hpp"
#include "CS_StageManager.hpp"
#include "CS_Input.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_JsonManager.hpp"
#include "CS_SoundManager.hpp"
#include <iostream>
#include "CS_Physics.hpp"
#include "CS_SoundManager.hpp"
/**
 * @brief Initialize this level based on the JSON file.
 * 
 */
void Menu_Stage::Init()
{
    std::stringstream file;
    file << "OtherLevels/";
    file << "Menu_Stage.json";
    CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
    button_count = 0;
}
/**
 * @brief Get inputs and update this level.
 * 
 * @param dt Delta time.
 */
void Menu_Stage::Update(float dt)
{
    if(object_array[0] == nullptr)
    {
        object_array[0] = CS_ObjectManager::GetFirstObjectByName("Start_Button");
    }
    if (object_array[1] == nullptr)
    {
        object_array[1] = CS_ObjectManager::GetFirstObjectByName("Quit_Button");
    }
    if (object_array[2] == nullptr)
    {
        object_array[2] = CS_ObjectManager::GetFirstObjectByName("Credit_Button");
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
        
        if (button_count == 2)
            button_count = 0;
        else
            ++button_count;
    }
    if (CS_Input::IsTriggered(InputButtons::D))
    {
        CS_SoundManager::PlaySound(CS_SoundTypes::Back);
        if (button_count == 0)
            button_count = 2;
        else
            --button_count;
    }
    glm::vec2 current_object_position = object_array[button_count]->transform.GetTranslation();
    selector[0]->transform.SetTranslation(glm::vec2(current_object_position.x - 200, current_object_position.y));
    selector[1]->transform.SetTranslation(glm::vec2(current_object_position.x + 200, current_object_position.y));
    if (CS_Input::IsTriggered(InputButtons::Space))
    {
        CS_SoundManager::PlaySound(CS_SoundTypes::Select);
        if (object_array[button_count]->m_name == "Start_Button")
        {
            CS_StageManager::SetNextStage(CS_StageTypes::Before_GamePlay_Stage);
        }
        if (object_array[button_count]->m_name == "Quit_Button")
        {
            CS_StageManager::SetNextStage(CS_StageTypes::Quit_Check_Stage);
        }
        if (object_array[button_count]->m_name == "Credit_Button")
        {
            CS_StageManager::SetNextStage(CS_StageTypes::Credit_Stage);
        }
    }

	ParticleGenerator::MakeBackGroundParticle(20, CS_ArcheTypes::Yellow_Particle);
}
/**
 * @brief Shut down this level.
 * 
 */
void Menu_Stage::Shutdown()
{
    CS_ObjectManager::DestroyAllObjects(false);
}
