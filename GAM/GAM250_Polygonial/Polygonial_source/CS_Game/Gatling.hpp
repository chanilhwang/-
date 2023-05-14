/**
 * @file Gatling.cpp
 * @author ChanIl Hwang, ChunMook.kim
 * @email tktlssla803@gmail.com
 * @brief Logic for gatling
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "Gun.hpp"
#include "CS_Object.hpp"

struct gatling_info {
	BulletInfo bullet_info;
	int bullet_num = 3;
	float delay = 0.07f;
	float distance_between_bullets = 100.0f;
};

class Player_Component;
class Gatling : public Gun
{
public:
	Gatling(gatling_info info);
	void Shoot(Needed_Player_Info player_info) override;
        void After_Shoot(Needed_Player_Info player_info) override;


        void Cursor_update(glm::vec2 mouse_position) override;
	int bullet_number;
	float distance_between_bullets;
private:
    int pre_heat;
    int over_heat;
    int heat_complete;
    int heat_limit;
    bool heat_ready;
    bool cool_down;
    bool sound_bool;
    bool cool_down_sound_bool;
    bool is_rest;
    float slow_speed;
    float UI_rotate_speed;
    float UI_rotate_max_speed;
    CS_Object* player;
    Player_Component* player_component;
    std::string target_name;
    CS_Object* MG_UI;
};