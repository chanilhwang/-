/**
 * @file Clear_Stage.cpp
 * @author ChunMook kim
 * @email kimmistral@naver.com
 * @brief Clear level
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Clear_Stage.hpp"
#include "CS_ObjectManager.hpp"
#include <iostream>
#include<sstream>
#include"CS_StageManager.hpp"
#include "CS_Input.hpp"

void Clear_Stage::Init()
{
    std::stringstream file;
    file << "OtherLevels/";
    file << "Clear_Stage.json";
    CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
}

void Clear_Stage::Update(float dt)
{
    if (CS_Input::IsTriggered(InputButtons::R))
    {
        CS_StageManager::SetNextStage(CS_StageTypes::GamePlay1_Stage);
        CS_Input::Rest();
    }
    if (CS_Input::IsTriggered(InputButtons::Escape))
    {
        CS_StageManager::SetNextStage(CS_StageTypes::Menu_Stage);
        CS_Input::Rest();
    }
}

void Clear_Stage::Shutdown()
{
    CS_ObjectManager::DestroyAllObjects(false);
}
