/**
 * @file Gun.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Logic for guns
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include <map>
#include "../Dependencies/Math/glm/glm.hpp"
#include "Color.hpp"
#include "CS_SoundManager.hpp"
#include "Items.hpp"
#include "Bullet_Info.hpp"

struct Needed_Player_Info
{
	glm::vec2 player_pos;
	Color color;
};

class Gun
{
public:
	Gun(BulletInfo bullet_info, float delay);
	~Gun();

	//shooting part
	virtual void Shoot(Needed_Player_Info player_info) = 0;
        virtual void After_Shoot(Needed_Player_Info player_info) = 0;
        virtual void Cursor_update(glm::vec2 mouse_position) = 0;
	glm::vec2 CalculateVectorBetweenPlayerAndGun(glm::vec2 player_pos);
	void MakeBullet(glm::vec2 position, glm::vec2 direction, Color color);
	void UpdateDelay(float dt);
	bool CheckDelay();

	//item part
	void AddItem(Item item);
	void UpdateInventory(Item& item);
	void InitInventory();
	//void Visualize();

	BulletInfo bullet_info;
	float delay;
	float timer;
	Inventory inventory;
	std::string gun_name;
};