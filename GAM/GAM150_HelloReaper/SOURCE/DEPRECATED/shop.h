/******************************************************************************
FileName: shop.h
Project: Hello Reaper
Author: Chanil, Hwang (c.hwang)
All content © 2017 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
#ifndef SHOP_H

#include "hreaper_general.h"
#include "level_info.h"

class shop
{
public:
	level_info ShopLevel;
};

void ShopLoad();
void ShopInit();
void ShopUpdate(float dt);
void ShopShutdown();
void ShopUnload();

#define SHOP_H
#endif