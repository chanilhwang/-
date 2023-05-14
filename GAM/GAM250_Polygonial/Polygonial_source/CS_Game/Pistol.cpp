/**
 * @file Pistol.cpp
 * @author ChanIl Hwang, ChunMook.kim
 * @email tktlssla803@gmail.com
 * @brief Logic for pistol
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Pistol.hpp"
#include "CS_Input.hpp"
#include "CS_ObjectManager.hpp"
#include "CS_Particle.hpp"

Pistol::Pistol(pistol_info info)
	: Gun(info.bullet_info, info.delay)
{
	gun_name = "pistol";
        charge_factor = 0;
        min_damage = 2;
        max_damege = 10;
        min_speed = 800;
        max_speed = 2000;
        min_scale = 40;
        max_scale = 80;
        initial_damege = min_damage;
        initial_scale = min_scale;
        initial_speed = min_speed;
        sound_bool = true;
        pistol_timer = 0;
}

void Pistol::Cursor_update(glm::vec2 mouse_position)
{
}

void Pistol::Shoot(Needed_Player_Info player_info)
{
	/*if (CheckDelay())
	{
		CS_SoundManager::PlaySound(CS_SoundTypes::Pistol);
		glm::vec2 Vec = CalculateVectorBetweenPlayerAndGun(player_info.player_pos);
		MakeBullet(player_info.player_pos, Vec, player_info.color);
		timer = 0;
	}*/
    if(timer > delay)
    {
        pistol_timer += 0.015f;
        if (sound_bool)
        {
            CS_SoundManager::PlaySound(CS_SoundTypes::Pistol_charge);
            sound_bool = false;
        }
        bullet_info.speed = 100.f;
        ++min_speed;
        ++min_scale;
        if (charge_factor < 61)
            ++charge_factor;
        CS_ArcheTypes archetype = CS_ArcheTypes::Particle;
        ParticleGenerator::MakeChargingParticles(1, archetype,
            player_info.player_pos,
            glm::vec2(-200.f, 200.f) - 3.f * glm::vec2(-charge_factor, charge_factor),
            1.0f);

        if (charge_factor > 30) {
            //change to red
        }

        if (CS_Input::IsReleased(InputButtons::Mouse_Left) && charge_factor > 0)
        {
            pistol_timer = 0;
            sound_bool = true;
            CS_SoundManager::StopSound(CS_SoundTypes::Pistol_charge);
            CS_SoundManager::PlaySound(CS_SoundTypes::Pistol);
            if (charge_factor > 60)
            {
                bullet_info.speed = max_speed;
                bullet_info.damage = max_damege;
                bullet_info.size = glm::vec2(max_scale);
            }
            else
            {
                bullet_info.size = glm::vec2(min_scale);
                bullet_info.speed = min_speed;
                bullet_info.damage = min_damage;
            }
            glm::vec2 Vec = CalculateVectorBetweenPlayerAndGun(player_info.player_pos);
            MakeBullet(player_info.player_pos, Vec, player_info.color);
            charge_factor = 0;
            min_damage = initial_damege;
            min_scale = initial_scale;
            min_speed = initial_speed;
            bullet_info.size = glm::vec2(initial_scale);
            timer = 0;
        }
    }
  
    
}

void Pistol::After_Shoot(Needed_Player_Info player_info)
{

}
