/**
 * @file CS_EditMouseSelectAABB.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For transform component
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "CS_EditMouseSelectAABB.hpp"
#include "CS_Object.hpp"
#include "CS_Input.hpp"
#include <iostream>

void CS_EditMouseSelectAABB::SetParent(CS_Object * pParent)
{
    m_Parent = pParent;
}

void CS_EditMouseSelectAABB::Update()
{
	CheckMouseIn();
}

bool CS_EditMouseSelectAABB::IsMouseIn()
{
	return isMouseIn;
}

void CS_EditMouseSelectAABB::CheckMouseIn()
{
	//compute by model way
	glm::vec2 temp=(m_Parent->gfx.ishud)?CS_Input::GetMouseHUDPos():CS_Input::GetMouseWorldPos();

	glm::vec3 modelMousePos{ temp.x,temp.y,1.f };
	modelMousePos = m_Parent->transform.GetWorldToModel()*modelMousePos;
	glm::vec2 min = m_Parent->gfx.mesh.GetMin();
	glm::vec2 max = m_Parent->gfx.mesh.GetMax();
	isMouseIn=min.x < modelMousePos.x&&modelMousePos.x < max.x;
	isMouseIn&= min.y < modelMousePos.y&&modelMousePos.y < max.y;
}
