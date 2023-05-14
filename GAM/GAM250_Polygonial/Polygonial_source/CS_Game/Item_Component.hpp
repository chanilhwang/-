/**
 * @file Item_Component.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Item Component
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include <glm/glm.hpp>
#include "CS_Components.hpp"
#include "Items.hpp"

class Item_Component :public CS_Component, public Item
{
public:
	Item_Component();
	void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Init() override;
	~Item_Component() override;
	void Update(float dt) override;
	Item_Component* Clone() const override;
	void ShowGui() override;

private:
};