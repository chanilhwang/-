/**
 * @file Shotgun.hpp
 * @author ChanIl Hwang, ChunMook.kim
 * @email tktlssla803@gmail.com
 * @brief Logic for shotgun
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once

#include "Gun.hpp"
#include "CS_Object.hpp"

struct shotgun_info {
	BulletInfo bullet_info;
	float angle = 0.5f;
	float delay = 0.8f;
	int bullet_num = 20;
};

class Shotgun : public Gun
{
public:
	Shotgun(shotgun_info info);
	void Shoot(Needed_Player_Info player_info) override;

        void After_Shoot(Needed_Player_Info player_info) override;
        void Cursor_update(glm::vec2 mouse_position) override;
        bool Check_reload();
	//radian
	float angle;
	int bullet_number;
        bool is_fired;
        unsigned int bullet_count;
        unsigned int maximun_bullet_count;
private:
    float UI_offset[3];
    CS_Object* bullet_UI[3];
    CS_Object* empty_UI;
    CS_Object* player;
    std::string target_name;
    float reload_timer;
    float reload_delay;
    float blinking_timer;
    float blinking_delay;
    bool blinking_togle;
    float sound_timer;
};