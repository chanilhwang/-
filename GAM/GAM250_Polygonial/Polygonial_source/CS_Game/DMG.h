/**
 * @file DMG.h
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief file for dmg
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once

#include "Color.hpp"

class DMG
{
public:
	DMG();
    int Get_damage(void) const;
    void Set_damage(int new_damage);

private:
    int damage;
};
