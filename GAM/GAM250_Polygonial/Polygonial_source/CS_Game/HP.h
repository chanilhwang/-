/**
 * @file HP.h
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief file for hp
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once

#include "Color.hpp"

class HP
{
public:
    HP();
    int Get_Max_HP(void) const;
    int Get_Current_HP(void) const;
	void Set_Max_HP(int max_hp);
	void FullFill(void);
    void Damaged(int dmg);

private:
    int Max_HP;
    int current_HP;
};