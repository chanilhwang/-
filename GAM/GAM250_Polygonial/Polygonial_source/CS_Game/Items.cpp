/**
 * @file Items.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Item class
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include <algorithm>
#include "Items.hpp"

bool Item::operator==(const Item& rhs) const
{
	return (this->item_name == rhs.item_name);
}

void Inventory::AddItem(Item item)
{
	items.push_back(item);
}

void Inventory::RemoveItem(Item item)
{
	auto cond = std::find(items.begin(), items.end(), item);
	if (cond == items.end())
		return;
	items.erase(cond);
}

void Inventory::Init()
{
	items.clear();
}