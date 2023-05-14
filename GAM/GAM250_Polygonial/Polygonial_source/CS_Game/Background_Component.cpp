/**
 * @file Background_Component.cpp
 * @author Chunmook Kim
 * @email seojunwoo72@gmail.com
 * @brief Component for BackGround component 
 * @version 0.1
 * @date 2018-12-13
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#include "Background_Component.hpp"
#include "CS_Graphics.hpp"
#include "CS_Object.hpp"
#include "CS_StageManager.hpp"
#include "../Dependencies/imgui/imgui.h"

/**
 * @brief Construct a new Background_Component::Background_Component object
 * 
 */
Background_Component::Background_Component():CS_Component(CS_ComponentTypes::Background_Component)
{
}
/**
 * @brief Destroy the Background_Component::Background_Component object
 * 
 */
Background_Component::~Background_Component()
{
}

/**
 * @brief read data from json file
 * 
 * @param jsonObject 
 */
void Background_Component::ReadFromFile(CS_JsonObject& jsonObject)
{
}

/**
 * @brief write data to json file
 * 
 * @param jsonObject 
 */
void Background_Component::WriteToFile(CS_JsonObject & jsonObject)
{
}

/**
 * @brief initialize background component
 * 
 */
void Background_Component::Init()
{
	WindowSize = CS_Graphics::GetDisplayAreaSize();
	m_Parent->transform.SetScale(WindowSize);
}

/**
 * @brief Update background_Component
 * 
 * @param dt 
 */
void Background_Component::Update(float dt)
{
	//WindowSize = CS_Graphics::GetDisplayAreaSize();
	
	/*glm::vec2 camPos = CS_StageManager::GetCameraOfCurrentLevel()->GetCenter();
	glm::vec2 camMove = CS_StageManager::GetCameraOfCurrentLevel()->GetVelocity()*dt;
	m_Parent->transform.SetScale(WindowSize * 1.1f);
	m_Parent->transform.SetTranslation(camPos);
	float ToMeshMove = (0.5f / WindowSize.x) * camMove.x;
	CurrentAdvence += ToMeshMove;
	if (CurrentAdvence >= MaxAdvence)
		CurrentAdvence = 0.f;
	m_Parent->gfx.mesh.ComputeMatrix({ CurrentAdvence,0.f }, 0.5f, 1.f);*/
}

/**
 * @brief Clone background Component
 * 
 * @return Background_Component* 
 */
Background_Component * Background_Component::Clone() const
{
	return new Background_Component;
}

/**
 * @brief Show information about background component on GUI
 * 
 */
void Background_Component::ShowGui()
{
	if (ImGui::TreeNode("Background_Component"))
	{
		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}
