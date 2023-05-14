/**
 * @file Edit_Stage.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Stage editor.
 * @version 0.1
 * @date 2018-12-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include "CS_Stages.hpp"

class CS_Object;

class Edit_Stage :public CS_Stage
{
public:
    ~Edit_Stage() override;
    void Init() override;
    void Update(float dt) override;
    void Shutdown() override;
private:
    bool press_check;
};
