/**
 * @file Shotgun.cpp
 * @author ChanIl Hwang, ChunMook.kim
 * @email tktlssla803@gmail.com
 * @brief Logic for shotgun
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Shotgun.hpp"
#include "CS_Math.hpp"
#include "CS_ObjectManager.hpp"
#define PI 3.14159265359f
Shotgun::Shotgun(shotgun_info info)
	:Gun(info.bullet_info, info.delay), angle(info.angle), bullet_number(info.bullet_num)
{   
	gun_name = "shotgun";
        player = CS_ObjectManager::GetFirstObjectByName("Player");
        maximun_bullet_count = 3;
        bullet_count = maximun_bullet_count;
        reload_timer = 0;
        is_fired = false;
        blinking_togle = false;
        bullet_UI[0] = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        bullet_UI[1] = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        bullet_UI[2] = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        for(int i = 0; i < 3; ++i)
        {
            UI_offset[i] = 22.f * (1 + i);
            bullet_UI[i]->transform.SetScale(glm::vec2(60, 60));
            bullet_UI[i]->gfx.SetTextureHandle("Textures/Shotgun_bullet.png");
        }
        empty_UI = CS_ObjectManager::CreateObjectWithArcheType(CS_ArcheTypes::NewObject);
        empty_UI->transform.SetScale(glm::vec2(60, 60));
        empty_UI->gfx.SetTextureHandle("Textures/Invisible.png");
        blinking_timer = 0;
        blinking_delay = 0.5f;
}

void Shotgun::Cursor_update(glm::vec2 mouse_position)
{
    for (int i = 0; i < 3; ++i)
    {
        bullet_UI[i]->transform.SetTranslation({ mouse_position.x + 100, mouse_position.y + UI_offset[i] });
    }
    empty_UI->transform.SetTranslation({mouse_position.x + 100, mouse_position.y + 22});
    if(bullet_count == 0)
    {
        if (blinking_togle)
        {
            empty_UI->gfx.SetTextureHandle("Textures/mouse_blink_1.png");
        }
        else
        {
            empty_UI->gfx.SetTextureHandle("Textures/mouse_blink_2.png");
        }
    }
    else
    {
        empty_UI->gfx.SetTextureHandle("Textures/Invisible.png");
    }
    
}

bool Shotgun::Check_reload()
{
    return timer > delay;
}

void Shotgun::Shoot(Needed_Player_Info player_info)
{
    while(player == nullptr)
    {
        if (player == nullptr) {
            player = CS_ObjectManager::GetFirstObjectByName("Player");
        }
    }
    if(bullet_count == 0)
    {      
        
        if(Check_reload())
        {
            CS_SoundManager::PlaySound(CS_SoundTypes::Warning);
            timer = 0;
        }
    }
    if (CheckDelay() && bullet_count)
    {
        empty_UI->gfx.SetTextureHandle("Textures/Invisible.png");
        --bullet_count;
        bullet_UI[bullet_count]->gfx.SetTextureHandle("Textures/Invisible.png");
        CS_SoundManager::PlaySound(CS_SoundTypes::Shotgun);
        glm::vec2 Vec = CalculateVectorBetweenPlayerAndGun(player_info.player_pos);
       /* player->physics.setVelocity_X(-8000 * Vec.x);
        player->physics.setVelocity_Y(-8000 * Vec.y);*/
        for(int bullet_index = 0; bullet_index < 12; ++bullet_index)
        {
            int random_value = rand();
            angle = (random_value % static_cast<int>(30) - 15) * PI / 180.f;
            glm::vec3 VecWith3 = { Vec.x, Vec.y, 0 };
            glm::mat3 mat = affine2D::build_rotation(angle);
            VecWith3 = mat * VecWith3;
            glm::vec2 New_Vec = { VecWith3.x, VecWith3.y };
            bullet_info.speed = random_value % 600 + 800.f;
            float bullet_scalar = random_value % 60 + 20.f;
            bullet_info.size = glm::vec2(bullet_scalar);
            MakeBullet(player_info.player_pos + Vec, New_Vec, player_info.color);
        }
        
        timer = 0;
    }
    else if(bullet_count == 0)
    {
        blinking_timer += 0.03f;
        if(blinking_timer > blinking_delay)
        {
            if(blinking_togle)
            {
                blinking_togle = false;
            }
            else
            {
                blinking_togle = true;
            }
            blinking_timer = 0;
        }
    }
    /*수-도 코드*/
    /*
     * 0. Input 인식 -> 이 함수에 도달했다는 것은 이미 left click가 인식 되었다는것 
     * 1. CalculateVectorBetweenPlayerAndGun함수를 이용해, 마우스와 플레이어 사이의 벡터 구하기
     * 2. 해당 벡터를 기준으로 90도를 설정
     * 3. 총 10개의 총알을 for문을 통해 생성
     * 4. 각도는 CalculateVectorBetweenPlayerAndGun에다 90도 안팎으로 +-를 통해 조절
     * 5. 발사 
     */
}

void Shotgun::After_Shoot(Needed_Player_Info player_info)
{
    if (CheckDelay() && (bullet_count < maximun_bullet_count))
    {
        for(unsigned int bullet_index = 0; bullet_index < maximun_bullet_count; ++bullet_index)
            bullet_UI[bullet_index]->gfx.SetTextureHandle("Textures/Shotgun_bullet.png");
        bullet_count = 3;
        CS_SoundManager::PlaySound(CS_SoundTypes::Shotgun_reload);
        timer = 0;
    }
}
