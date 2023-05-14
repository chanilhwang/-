/**
 * @file Splash_Stage.cpp
 * @author ChunMook kim
 * @email kimmistral@naver.com
 * @brief Splash level
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Splash_Stage.hpp"
#include "CS_ObjectManager.hpp"
#include <iostream>
#include <sstream>
#include "CS_JsonManager.hpp"
#include "CS_Input.hpp"
#include "CS_StageManager.hpp"

void Splash_Stage::Init()
{
    logo_timer = 0;
    std::stringstream file;
    file << "OtherLevels/";
    file << "Splash_Stage.json";
    CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
}

void Splash_Stage::Update(float dt)
{
    logo_timer += dt;
    bool time_over = logo_timer > 6.0f;
    if (CS_Input::IsAnyPressed() || time_over)
    {
        CS_StageManager::SetNextStage(CS_StageTypes::Menu_Stage);
        CS_Input::Rest();
    }
}

void Splash_Stage::Shutdown()
{
    CS_ObjectManager::DestroyAllObjects(false);
}
