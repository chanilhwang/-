/**
 * @file DMG.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief file for dmg
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "DMG.h"

DMG::DMG()
	:damage(1)
{}

int DMG::Get_damage() const
{
    return damage;
}

void DMG::Set_damage(int new_damage)
{
	damage = new_damage;
}
