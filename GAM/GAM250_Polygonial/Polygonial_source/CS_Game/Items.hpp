/**
 * @file Items.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Item class
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include <map>
#include <vector>
#include <string>
#include "../Dependencies/Math/glm/glm.hpp"

class Item
{
public:
	std::string item_name;
	int increased_dmg;
	int increased_barrel;
	int increased_hp;
	float decreased_delay;
	float increased_lifetime;
	float increased_speed;
	glm::vec2 increased_size;

	bool operator==(const Item& rhs) const;
};

class Inventory
{
public:
	void AddItem(Item item);
	void RemoveItem(Item item);
	void Init();

private:
	std::vector<Item> items;
};