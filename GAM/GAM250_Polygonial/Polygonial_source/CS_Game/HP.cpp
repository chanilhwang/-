/**
 * @file HP.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief file for hp
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "HP.h"


HP::HP() : Max_HP(3)
{
    current_HP = Max_HP;
}

int HP::Get_Max_HP() const
{
    return Max_HP;
}

int HP::Get_Current_HP() const
{
    return current_HP;
}

void HP::Damaged(int dmg)
{
	current_HP -= dmg;
        
}

void HP::Set_Max_HP(int max_hp)
{
	Max_HP = max_hp;
	current_HP = Max_HP;
}

void HP::FullFill(void)
{
	current_HP = Max_HP;
}