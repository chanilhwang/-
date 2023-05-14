/**
 * @file Pistol.hpp
 * @author ChanIl Hwang, ChunMook.kim
 * @email tktlssla803@gmail.com
 * @brief Logic for pistol
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once

#include "Gun.hpp"

struct pistol_info {
	BulletInfo bullet_info;
	float delay = 0.7f;
};

class Pistol : public Gun
{
public:
	Pistol(pistol_info info);
	void Shoot(Needed_Player_Info player_info) override;
        void After_Shoot(Needed_Player_Info player_info) override;
        void Cursor_update(glm::vec2 mouse_position) override;
private:
    int charge_factor;
    int min_damage;
    int max_damege;
    float min_speed;
    float max_speed;
    float min_scale;
    float max_scale;
    float initial_scale;
    float initial_speed;
    int initial_damege;
    int sound_bool;

    float pistol_timer;
};