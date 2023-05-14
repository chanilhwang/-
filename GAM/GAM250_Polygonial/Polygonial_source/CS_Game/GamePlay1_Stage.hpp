/**
 * @file GamePlay1_Stage.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief GamePlay level
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Stages.hpp"
#include "Simple_Map_Component.hpp"
#include "EnemyGenerator_Component.hpp"
#include "Camera_Logic.hpp"

class Player_Component;
class GamePlay1_Stage :public CS_Stage
{
public:
    ~GamePlay1_Stage() override;
    void Init() override;
    void Update(float dt) override;
    void Shutdown() override;

private:
	EnemyGenerator_Component* enemy_gen;
	CS_Object* items[4];
        CS_Object* Cursor;
        CS_Object* color_indicator;
        CS_Object* reload_indicator;
        CS_Object* preheat_indicator;
        CS_Object* overheat_indicator;
        CS_Object* charge_indicator;
	bool IsItemCreated = false;
        Camera_Logic camera_logic;
        float indi_timer;
        bool is_indi_removed;
};