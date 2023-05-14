/**
 * @file Before_GamePlay_Stage.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Weapon-Selection stage
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Stages.hpp"
class CS_Object;
class Player_Component;
class Before_GamePlay_Stage :public CS_Stage
{
public:
	~Before_GamePlay_Stage() override;
	void Init() override;
	void Update(float dt) override;
	void Shutdown() override;

private:
    CS_Object* object_array[3];
    CS_Object* selected_object;
    CS_Object* selecter;
    int button_count;
};