/**
 * @file GamePlay1_Stage.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief GamePlay level
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "GamePlay1_Stage.hpp"
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
/**
 * @brief Destroy the GamePlay_Stage::GamePlay_Stage object
 * 
 */
GamePlay1_Stage::~GamePlay1_Stage()
{
    
}
/**
 * @brief Initialize the game level based on the level JSON file.
 * 
 */
void GamePlay1_Stage::Init()
{
    std::stringstream file;
    file << "GamePlayLevels/";
    file <<"Level"<<CS_StageManager::GetGameData().level<<".json";
    CS_ObjectManager::LoadObjectFromFile(file.str().c_str());

    //CS_SoundManager::SetSoundMode(CS_SoundTypes::BGM, CS_SoundMode::Loop_Normal);
    //CS_SoundManager::PlaySound(CS_SoundTypes::BGM);

	/*if(CS_StageManager::GetGameData().PlayerWeapon == "")
		CS_StageManager::PauseAndSetNextStage(CS_StageTypes::Before_GamePlay_Stage, true);*/
    indi_timer = 0.f;
    is_indi_removed = false;
	IsItemCreated = false;
        camera_logic.init(&camera);
        camera.SetZoom(0.6f);
        Cursor = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        if (Cursor)
        {
            Cursor->gfx.SetTextureHandle("Textures/cursor.png");
            Cursor->gfx.ishud = true;
        }
        CS_SoundManager::SetSoundMode(CS_SoundTypes::BGM, CS_SoundMode::Loop_Normal);
        CS_SoundManager::PlaySound(CS_SoundTypes::BGM);

	CS_Object* right_wall = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Blue_Particle);
	CS_Object* left_wall = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Blue_Particle);
	CS_Object* upper_wall = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Blue_Particle);
	CS_Object* below_wall = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::Blue_Particle);

	right_wall->transform.SetScale(glm::vec2(10.f, 2.5f * CS_Graphics::GetDisplayAreaSize().y));
	left_wall->transform.SetScale(glm::vec2(10.f, 2.5f * CS_Graphics::GetDisplayAreaSize().y));
	upper_wall->transform.SetScale(glm::vec2(2.5f * CS_Graphics::GetDisplayAreaSize().x, 10.f));
	below_wall->transform.SetScale(glm::vec2(2.5f * CS_Graphics::GetDisplayAreaSize().x, 10.f));

	right_wall->transform.SetTranslation(glm::vec2(2.5f * CS_Graphics::GetDisplayAreaSize().x / 2.0f + 5.f, 0.f));
	left_wall->transform.SetTranslation(glm::vec2(-2.5f * CS_Graphics::GetDisplayAreaSize().x / 2.0f + 5.f, 0.f));
	upper_wall->transform.SetTranslation(glm::vec2(0.f, 2.5f * CS_Graphics::GetDisplayAreaSize().y / 2.0f + 5.f));
	below_wall->transform.SetTranslation(glm::vec2(0.f, -2.5f * CS_Graphics::GetDisplayAreaSize().y / 2.0f + 5.f));

        color_indicator = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        color_indicator->transform.SetTranslation(glm::vec2(0.f, 500.f));
        color_indicator->transform.SetScale(glm::vec2(650.f, 150.f));
        color_indicator->gfx.SetTextureHandle("Textures/colorchange_indicator.png");
        if(CS_StageManager::GetGameData().PlayerWeapon == "Pistol")
        {
            charge_indicator = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
            charge_indicator->transform.SetTranslation(glm::vec2(0.f, -500.f));
            charge_indicator->transform.SetScale(glm::vec2(500, 400));
            charge_indicator->gfx.SetTextureHandle("Textures/charge_indicator.png");
        }
        if (CS_StageManager::GetGameData().PlayerWeapon == "Shotgun")
        {
            reload_indicator = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
            reload_indicator->transform.SetTranslation(glm::vec2(0.f, -500.f));
            reload_indicator->transform.SetScale(glm::vec2(500, 400));
            reload_indicator->gfx.SetTextureHandle("Textures/reload_indicator.png");
        }
        if (CS_StageManager::GetGameData().PlayerWeapon == "Gatling")
        {
            preheat_indicator = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
            preheat_indicator->transform.SetTranslation(glm::vec2(-300.f, -500.f));
            preheat_indicator->transform.SetScale(glm::vec2(500, 400));
            preheat_indicator->gfx.SetTextureHandle("Textures/preheat_indicator.png");

            overheat_indicator = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
            overheat_indicator->transform.SetScale(glm::vec2(650.f, 150.f));
            overheat_indicator->transform.SetTranslation(glm::vec2(300.f, -500.f));
            overheat_indicator->gfx.SetTextureHandle("Textures/overhead_indicator.png");
        }

}
/**
 * @brief Update based on the inputs.
 * 
 * @param dt Delta time.
 */
void GamePlay1_Stage::Update(float dt)
{
    
    if(is_indi_removed == false)
    {
        indi_timer += dt;
        if (indi_timer > 4.f)
        {
            color_indicator->gfx.SetTextureHandle("Textures/Invisible.png");
            if (CS_StageManager::GetGameData().PlayerWeapon == "Pistol")
            {
                charge_indicator->gfx.SetTextureHandle("Textures/Invisible.png");
            }
            if (CS_StageManager::GetGameData().PlayerWeapon == "Shotgun")
            {
                reload_indicator->gfx.SetTextureHandle("Textures/Invisible.png");
            }
            if (CS_StageManager::GetGameData().PlayerWeapon == "Gatling")
            {
                preheat_indicator->gfx.SetTextureHandle("Textures/Invisible.png");
                overheat_indicator->gfx.SetTextureHandle("Textures/Invisible.png");
            }
            is_indi_removed = true;
        }
    }
   
    camera_logic.camera_tracing(dt, &camera);
    if(CS_Input::IsTriggered(InputButtons::Escape))
    { 
        CS_StageManager::SetNextStage(CS_StageTypes::Before_GamePlay_Stage);
	CS_SoundManager::GetChannel(CS_SoundTypes::BGM)->setPaused(true);
    }
    if (CS_Input::IsTriggered(InputButtons::N))
    {
        CS_SoundManager::GetChannel(CS_SoundTypes::BGM)->setPaused(true);
    }
    Cursor->transform.SetTranslation(CS_Input::GetMouseWorldPos()); 
    ParticleGenerator::MakeBackGroundParticle(20, CS_ArcheTypes::White_Particle);
}
/**
 * @brief Shut down current level.
 * 
 */
void GamePlay1_Stage::Shutdown()
{
    camera.SetCenter_x(0);
    CS_ObjectManager::DestroyAllObjects(false);
    CS_SoundManager::StopSound(CS_SoundTypes::BGM);
}