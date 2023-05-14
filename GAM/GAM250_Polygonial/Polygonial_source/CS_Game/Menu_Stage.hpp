/**
 * @file Menu_Stage.hpp
 * @author Chunmook Kim
 * @email kimmistral\@naver.com
 * @brief Level to display main menu.
 * @version 0.1
 * @date 2018-12-10
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include "CS_Stages.hpp"


class CS_Object;
class Menu_Stage :public CS_Stage
{
public:
	void Init() override;
	void Update(float dt) override;
	void Shutdown() override;
private:
    CS_Object* object_array[3];
    CS_Object* selected_object;
    CS_Object* selector[2];
    int button_count;
};
