/**
 * @file Test_Stage.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Testing stage.
 * @version 0.1
 * @date 2018-12-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "Test_Stage.hpp"
#include "CS_Input.hpp"
#include "CS_StageTypes.hpp"
#include "CS_StageManager.hpp"
#include "CS_Editor.hpp"
#include <sstream>
#include "CS_ObjectManager.hpp"
#include <iostream>
/**
 * @brief Destroy the Test_Stage::Test_Stage object
 * 
 */
Test_Stage::~Test_Stage()
{
}
/**
 * @brief Initialize this stage.
 * 
 */
void Test_Stage::Init()
{
	std::stringstream file;
	file << CS_Editor::GetSelectedLevel();
	CS_ObjectManager::LoadObjectFromFile(file.str().c_str());
        mouse_cursor = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        if (mouse_cursor)
        {
            mouse_cursor->gfx.SetTextureHandle("Textures/cursor.png");
            mouse_cursor->gfx.ishud = true;
        }

	//if (CS_StageManager::GetGameData().PlayerWeapon == "")
	//	CS_StageManager::PauseAndSetNextStage(CS_StageTypes::Before_GamePlay_Stage, true);
        camera_logic.init(&camera);
        camera.SetZoom(0.6f);
}
/**
 * @brief Update this stage based on the inputs.
 * 
 * @param dt Delta time.
 */
void Test_Stage::Update(float dt)
{
    camera_logic.camera_tracing(dt, &camera);
    bool ctrlkey = CS_Input::IsPressed(InputButtons::Ctrl);
    
    if (!ctrlkey)
    {
        if (CS_Input::IsReleased(InputButtons::Mouse_Middle))
            camera.SetZoom(1.f);
    }
    if (ctrlkey)
    {
        float new_zoom = camera.GetZoom() + (CS_Input::GetMouseWheelScrollAmount() * 0.001f);
        if (new_zoom > 4.0)
            new_zoom = 4.f;
        if (new_zoom < 0.1f)
            new_zoom = 0.1f;
        camera.SetZoom(new_zoom);
    }
	if (CS_Input::IsPressed(InputButtons::Escape))
	{
		CS_StageManager::SetNextStage(CS_StageTypes::Edit_Stage);
	}
    if (mouse_cursor)
        mouse_cursor->transform.SetTranslation(CS_Input::GetMouseWorldPos());
}
/**
 * @brief Shut down this level.
 * 
 */
void Test_Stage::Shutdown()
{
	CS_ObjectManager::DestroyAllObjects(true);
}