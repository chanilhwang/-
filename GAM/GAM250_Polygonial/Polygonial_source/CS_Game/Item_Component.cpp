/**
 * @file Item_Component.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Item Component
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Item_Component.hpp"
#include "CS_Physics.hpp"
#include "../Dependencies/imgui/imgui.h"
#include "CS_ObjectManager.hpp"
#include "CS_JsonManager.hpp"

Item_Component::Item_Component() :CS_Component(CS_ComponentTypes::Item_Component)
{

}

void Item_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
	jsonObject.ReadData("Item_Name", item_name);
	jsonObject.ReadData("Increased_dmg", increased_dmg);
	jsonObject.ReadData("Increased_barrel", increased_barrel);
	jsonObject.ReadData("Increased_hp", increased_hp);
	jsonObject.ReadData("Increased_lifetime", increased_lifetime);
	jsonObject.ReadData("Increased_speed", increased_speed);
	jsonObject.ReadData("decreased_delay", decreased_delay);
}

void Item_Component::WriteToFile(CS_JsonObject & jsonObject)
{
	jsonObject.WriteData("Item_Name", item_name);
	jsonObject.WriteData("Increased_dmg", increased_dmg);
	jsonObject.WriteData("Increased_barrel", increased_barrel);
	jsonObject.WriteData("Increased_hp", increased_hp);
	jsonObject.WriteData("Increased_lifetime", increased_lifetime);
	jsonObject.WriteData("Increased_speed", increased_speed);
	jsonObject.WriteData("decreased_delay", decreased_delay);
}

void Item_Component::Init()
{

}

Item_Component::~Item_Component()
{

}

void Item_Component::Update(float dt)
{

}

Item_Component * Item_Component::Clone() const
{
	return new Item_Component;
}

void Item_Component::ShowGui()
{
	if (ImGui::TreeNode("Item_Component"))
	{
		static char text[20];
		std::copy(item_name.begin(), item_name.end(), text);
		text[item_name.length()] = '\0';
		ImGui::InputText("Item_Name", text, 20);
		item_name = text;

		ImGui::DragInt("Increased_Damage", &increased_dmg);
		ImGui::DragInt("Increased_barrel", &increased_barrel);
		ImGui::DragInt("Increased_hp", &increased_hp);
		ImGui::DragFloat("Decreased_Delay", &decreased_delay);
		ImGui::DragFloat("bullet_lifetime", &increased_lifetime);
		ImGui::DragFloat("Decreased_speed", &increased_speed);

		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
} 