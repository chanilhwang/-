/**
 * @file Main_Menu_Stage.hpp
 * @author ChunMook kim
 * @email kimmistral@naver.com
 * @brief Main_Menu_level header
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content � 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Stages.hpp"

class Main_Menu_Stage :public CS_Stage
{
public:
    void Init() override;
    void Update(float dt) override;
    void Shutdown() override;

private:

};

