/**
 * @file Player_Component.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Player Component
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "SphereCollider_Component.hpp"
#include "CS_Components.hpp"
#include "Enemy_Component.hpp"
#include "HP.h"
#include "Gun_List.hpp"
#include "Camera_Logic.hpp"

class Player_Component :public CS_Component
{
public:
	Player_Component();
	~Player_Component() override;
    void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
    void Init() override;
    void Update(float dt) override;
    Player_Component* Clone() const override;
	void ShowGui() override;

	Color GetColor();
        void SetSpeed(float speed);
        float GetSpeed();
	void SelectGun(std::string gun_name);
	void InitGun();
        void DeleteGun();
	Needed_Player_Info MakePlayerInfo() const;
    float original_speed;
	Camera_Logic* camera = nullptr;

private:
	void PlayerInput();
	void CheckCollideAndGetDamaged(float dt);
	void Die(float dt);
	void Rotate(float dieing_rotation_radian);

	float dieing_time;
	float dieing_rotation_radian;
	float dieing_reducing_scale = 0.99f;

	float invincibility_timer;
	float invincibility;
	float speed;
	bool invincible_mode;

	std::string White_Texture;
	std::string Black_Texture;

	pistol_info pistol_info;
	shotgun_info shotgun_info;
	gatling_info gatling_info;

	HP hp;
	Gun* gun;

	Color color;

        bool up, down, left, right;

        bool sound_bool;
        bool only_once;

        CS_Object* hp_object;
        float accumulate_dt;
        float avoid_timer;
        float avoid_delay;
};