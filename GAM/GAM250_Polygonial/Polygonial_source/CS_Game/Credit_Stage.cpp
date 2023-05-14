/**
 * @file Credit_Stage.cpp
 * @author ChunMook kim
 * @email kimmistral@naver.com
 * @brief credit level
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Credit_Stage.hpp"
#include <iostream>
#include "CS_ObjectManager.hpp"
#include <sstream>
#include "CS_Input.hpp"
#include "CS_StageManager.hpp"
void Credit_Stage::Init()
{
    std::stringstream file;
    file << "OtherLevels/";
    file << "Credit_Stage.json";
    CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
}

void Credit_Stage::Update(float dt)
{
    if (CS_Input::IsAnyTriggered())
    {
        CS_StageManager::SetNextStage(CS_StageTypes::Menu_Stage);
        CS_Input::Rest();
    }
}

void Credit_Stage::Shutdown()
{
    CS_ObjectManager::DestroyAllObjects(false);
}
