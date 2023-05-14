/**
 * @file Lose_Stage.cpp
 * @author ChunMook kim
 * @email kimmistral@naver.com
 * @brief Lose Stage
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Lose_Stage.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_StageManager.hpp"
#include "CS_Input.hpp"
#include <iostream>
#include <sstream>
void Lose_Stage::Init()
{
    std::stringstream file;
    file << "OtherLevels/";
    file << "Lose_Stage.json";
    CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
}

void Lose_Stage::Update(float dt)
{
    if (CS_Input::IsTriggered(InputButtons::R))
    {
        CS_StageManager::SetNextStage(CS_StageTypes::GamePlay1_Stage);
        CS_Input::Rest();
    }
    if (CS_Input::IsTriggered(InputButtons::Escape))
    {
        CS_StageManager::SetNextStage(CS_StageTypes::Before_GamePlay_Stage);
        CS_Input::Rest();
    }
}

void Lose_Stage::Shutdown()
{
    CS_ObjectManager::DestroyAllObjects(false);
}
