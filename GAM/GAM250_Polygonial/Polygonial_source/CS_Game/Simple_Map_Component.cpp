/**
 * @file Simple_Map_Component.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief Logic for map limitation
 * @version 0.1
 * @date 2019-03-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Simple_Map_Component.hpp"
#include "CS_Object.hpp"

 /**
  * @brief Construct a new Simple_Map_Component::Simple_Map_Component object
  *
  */
Simple_Map_Component::Simple_Map_Component()
	: CS_Component(CS_ComponentTypes::Background_Component)
{
}
/**
 * @brief Destroy the Simple_Map_Component::Simple_Map_Component() object
 *
 */
Simple_Map_Component::~Simple_Map_Component()
{
}
/**
 * @brief read data from json file
 *
 * @param jsonObject
 */
void Simple_Map_Component::ReadFromFile(CS_JsonObject& jsonObject)
{

}
/**
 * @brief write data to json file
 *
 * @param jsonObject
 */
void Simple_Map_Component::WriteToFile(CS_JsonObject& jsonObject)
{

}
/**
 * @brief initialize simplemap component
 *
 */
void Simple_Map_Component::Init()
{
	m_Parent->transform.SetDepth(-30);
}
/**
 * @brief Update Simple_Map_Component
 *
 * @param dt
 */
void Simple_Map_Component::Update(float dt)
{
	m_Parent->transform.SetDepth(-30);
}
/**
 * @brief Clone simplemap Component
 *
 * @return Simple_Map_Component*
 */
Simple_Map_Component* Simple_Map_Component::Clone() const
{
	return new Simple_Map_Component;
}
/**
 * @brief Show information about simplemap component on GUI
 *
 */
void Simple_Map_Component ::ShowGui()
{

}