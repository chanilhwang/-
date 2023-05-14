/**
 * @file Background_NO_Adv_Component.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For background component
 * @version 0.1
 * @date 2018-12-14
 *
 * @copyright Copyright (c) 2018
 *
 */
#include "Background_NO_Adv_Component.hpp"
#include "../Dependencies/imgui/imgui.h"
#include "CS_Graphics.hpp"
#include "CS_StageManager.hpp"
#include "CS_Object.hpp"
Background_NO_Adv_Component::Background_NO_Adv_Component()
	:CS_Component(CS_ComponentTypes::Background_NO_Adv_Component)
{
}

Background_NO_Adv_Component::~Background_NO_Adv_Component()
{
}

void Background_NO_Adv_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
}

void Background_NO_Adv_Component::WriteToFile(CS_JsonObject & jsonObject)
{
}

void Background_NO_Adv_Component::Init()
{
}

void Background_NO_Adv_Component::Update(float dt)
{
	WindowSize = CS_Graphics::GetDisplayAreaSize();
	glm::vec2 camPos = CS_StageManager::GetCameraOfCurrentLevel()->GetCenter();
	m_Parent->transform.SetScale(WindowSize);
	m_Parent->transform.SetTranslation(camPos);
}

Background_NO_Adv_Component * Background_NO_Adv_Component::Clone() const
{
	return new Background_NO_Adv_Component;
}

void Background_NO_Adv_Component::ShowGui()
{
	if (ImGui::TreeNode("Background_NO_Adv_Component"))
	{
		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}
