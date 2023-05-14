/**
 * @file Test_Stage.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Testing stage.
 * @version 0.1
 * @date 2018-12-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include "CS_Stages.hpp"
#include "Camera_Logic.hpp"
#include "CS_Object.hpp"

class Test_Stage :public CS_Stage
{
public:
	~Test_Stage() override;
	void Init() override;
	void Update(float dt) override;
	void Shutdown() override;
private:
    Camera_Logic camera_logic;
    CS_Object* mouse_cursor;
};
