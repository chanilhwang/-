/**
 * @file Splash_Stage.hpp
 * @author ChunMook kim
 * @email kimmistral@naver.com
 * @brief Splash level
 * @version 0.1
 * @date 2019-03-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include "CS_Stages.hpp"

class Splash_Stage :public CS_Stage
{
public:
    void Init() override;
    void Update(float dt) override;
    void Shutdown() override;
private:
    float logo_timer;
};

