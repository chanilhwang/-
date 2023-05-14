/**
 * @file Edit_Stage.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Stage editor.
 * @version 0.1
 * @date 2018-12-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "Edit_Stage.hpp"
#include "CS_Editor.hpp"
#include "CS_ArcheTypes.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_StageManager.hpp"
#include "CS_Input.hpp"
#include <sstream>
#include "CS_Object.hpp"
#include <Windows.h>
#include <iostream>

/**
 * @brief Destroy the Edit_Stage::Edit_Stage object
 * 
 */
Edit_Stage::~Edit_Stage()
{
}
/**
 * @brief Editor initialization.
 * 
 */
void Edit_Stage::Init()
{
        ShowCursor(true);
	press_check = false;
	std::stringstream file;
	file << CS_Editor::GetSelectedLevel();

	CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
        CS_Editor::EditStart(&camera);
}
/**
 * @brief Update the stage based on the input.
 * 
 * @param dt Delta time.
 */
void Edit_Stage::Update(float dt)
{
    CS_Editor::Update(dt);
    if (CS_Input::IsReleased(InputButtons::R) && CS_Input::IsPressed(InputButtons::Ctrl))
        CS_StageManager::Restart();
	if (CS_Input::IsPressed(InputButtons::O) && CS_Input::IsPressed(InputButtons::Ctrl))
		CS_StageManager::SetNextStage(CS_StageTypes::Before_GamePlay_Stage);
	if (CS_Input::IsPressed(InputButtons::F_5))
		CS_StageManager::SetNextStage(CS_StageTypes::Test_Stage);
}
/**
 * @brief Shut down the editor.
 * 
 */
void Edit_Stage::Shutdown()
{
    ShowCursor(false);
    CS_Editor::EditEnd();
    CS_ObjectManager::DestroyAllObjects(false);
}
