/**
 * @file Gatling.cpp
 * @author ChanIl Hwang, ChunMook Kim
 * @email tktlssla803@gmail.com
 * @brief Logic for gatling
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Gatling.hpp"
#include "CS_Input.hpp"
#include "CS_ObjectManager.hpp"
#include "Player_Component.hpp"
#include "CS_Timer_Frame.hpp"

Gatling::Gatling(gatling_info info)
	:Gun(info.bullet_info, info.delay), bullet_number(info.bullet_num), distance_between_bullets(info.distance_between_bullets)
{
        UI_rotate_max_speed = 100.f;
        UI_rotate_speed = -10.0f;
        bullet_info.speed = 2000;
        bullet_info.size = glm::vec2(60.f, 45.f);
	gun_name = "gatling";
        heat_ready = false;
        cool_down = false;
        sound_bool = true;
        cool_down_sound_bool = true;
        pre_heat = 0;
        over_heat = 0;
        heat_complete = 70;
        slow_speed = 150.f;
		distance_between_bullets = 30.f;
        heat_limit = 200;
		bullet_number = 0;
        player = CS_ObjectManager::GetFirstObjectByName("Player");
        if(player)
            player->GetComponent(CS_ComponentTypes::Player_Component, player_component);
        MG_UI = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        MG_UI->transform.SetScale(glm::vec2(100, 100));
        MG_UI->gfx.SetTextureHandle("Textures/MG_charger.png");
}


void Gatling::Cursor_update(glm::vec2 mouse_position)
{
    MG_UI->transform.SetRotation(UI_rotate_speed);
    MG_UI->transform.SetTranslation({ mouse_position.x, mouse_position.y});
}

void Gatling::Shoot(Needed_Player_Info player_info)
{
    while (player == nullptr)
    {
        if (player == nullptr) {
            player = CS_ObjectManager::GetFirstObjectByName("Player");
        }
    }
    if(player)
        player->GetComponent(CS_ComponentTypes::Player_Component, player_component);

    if(heat_ready)
    {
        UI_rotate_speed -= 0.2f;
        CS_SoundManager::StopSound(CS_SoundTypes::MG_cooldown);
        if (CheckDelay())
        {
            CS_SoundManager::PlaySound(CS_SoundTypes::MG_shot);

            glm::vec2 Vec = CalculateVectorBetweenPlayerAndGun(player_info.player_pos);
			if (bullet_number % 2 == 0)
			{
				MakeBullet(player_info.player_pos + glm::vec2(-Vec.y, Vec.x) * distance_between_bullets, Vec, player_info.color);
				bullet_number++;
			}
			else
			{
                                MakeBullet(player_info.player_pos - glm::vec2(-Vec.y, Vec.x) *distance_between_bullets, Vec, player_info.color);
				bullet_number++;
                        }
            
            timer = 0;
        }
        if(player)
            player_component->SetSpeed(slow_speed);

        ++over_heat;
        if(over_heat > heat_limit)
        {
            if (cool_down_sound_bool)
            {
                CS_SoundManager::PlaySound(CS_SoundTypes::MG_shotend);
                CS_SoundManager::PlaySound(CS_SoundTypes::MG_cooldown);
                sound_bool = false;
                cool_down_sound_bool = false;
            }
            heat_ready = false;
            pre_heat = 0;
            over_heat = 0;
        }
        cool_down_sound_bool = true;
    }
    else
    {
        if(sound_bool && (over_heat == 0))
        {
            CS_SoundManager::PlaySound(CS_SoundTypes::machinegun_spin);
            sound_bool = false;
        }
        UI_rotate_speed-=0.1f;
        if (player)
            player_component->SetSpeed(400.f);
        ++pre_heat;
    }
    if(pre_heat > heat_complete)
    {
        CS_SoundManager::StopSound(CS_SoundTypes::machinegun_spin);
        sound_bool = true;
        heat_ready = true;
    }
    if(CS_Input::IsReleased(InputButtons::Mouse_Left) && pre_heat > 0 && player_component)
    {
        UI_rotate_speed = -10.0f;
        CS_SoundManager::StopSound(CS_SoundTypes::machinegun_spin);
        player_component->SetSpeed(player_component->original_speed);
        pre_heat = 0;
        over_heat = 0;
        sound_bool = true;
        heat_ready = false;
    }
}

void Gatling::After_Shoot(Needed_Player_Info player_info)
{

}
