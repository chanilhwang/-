/**
 * @file OBB_BoxCollider_Component.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For OBB collider
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "AABB_BoxCollider_Component.hpp"
#include "CS_Object.hpp"
#include "CS_Physics.hpp"
#include "SphereCollider_Component.hpp"
#include "CS_Math.hpp"
#include "OBB_BoxCollider_Component.hpp"
#include <iostream>
#include "imgui.h"

OBB_BoxCollider_Component::OBB_BoxCollider_Component()
    :CS_ColliderComponent(CS_ComponentTypes::OBB_BoxCollider_Component, CS_ColliderType::OBB_Box)
{

}

OBB_BoxCollider_Component::~OBB_BoxCollider_Component()
{
    CS_Physics::RemoveColliderComponent(this);
}
void OBB_BoxCollider_Component::Init()
{
    half_width_height.x = m_Parent->transform.GetScale().x / 2;
    half_width_height.y = m_Parent->transform.GetScale().y / 2;

    Right = { 1, 0 };
    Right = mat2x2::build_rotation(m_Parent->transform.GetRotation())*Right;

    Up = { 0, 1 };
    Up = mat2x2::build_rotation(m_Parent->transform.GetRotation())*Up;

    glm::vec3 points[2] = {   { 0.5, 0.5, 1 } ,{ -0.5, -0.5, 1 } };

    auto mat = m_Parent->transform.GetModelToWorld();
    for (auto& point : points)
        point = mat * point;

    Right_Top = { points[0].x, points[0].y };
    Left_Bot = { points[1].x, points[1].y };

}
void OBB_BoxCollider_Component::ReadFromFile(CS_JsonObject& jsonObject)
{
	
}

void OBB_BoxCollider_Component::WriteToFile(CS_JsonObject & jsonObject)
{
	
}

void OBB_BoxCollider_Component::Update(float dt)
{
    if (m_Parent->transform.WasRotationChanged || m_Parent->transform.WasTranslationChanged)
    {
        half_width_height.x = m_Parent->transform.GetScale().x / 2;
        half_width_height.y = m_Parent->transform.GetScale().y / 2;

        Right = { 1, 0 };
        Right = mat2x2::build_rotation(m_Parent->transform.GetRotation())*Right;

        Up = { 0, 1 };
        Up = mat2x2::build_rotation(m_Parent->transform.GetRotation())*Up;

        glm::vec3 points[2] = { { 0.5, 0.5, 1 } ,{ -0.5, -0.5, 1 } };

        auto mat = m_Parent->transform.GetModelToWorld();
        for (auto& point : points)
            point = mat * point;

        Right_Top = { points[0].x, points[0].y };
        Left_Bot = { points[1].x, points[1].y };
    }
}

CS_ColliderComponent* OBB_BoxCollider_Component::Clone() const
{
    OBB_BoxCollider_Component* temp = new OBB_BoxCollider_Component;
    temp->half_width_height = half_width_height;
    temp->Right = Right;
    temp->Up = Up;
	CS_Physics::AddColliderComponent(temp);
    return temp;
}

void OBB_BoxCollider_Component::TestCollision(const CS_ColliderComponent* pOther)
{
    bool is_colliding = false;
    switch (pOther->m_colliderType)
    {
        case CS_ColliderType::OBB_Box:
        {
            const OBB_BoxCollider_Component* ptr_OBB_B = static_cast<const OBB_BoxCollider_Component*>(pOther);
            is_colliding = CS_IntersectCheck::OBB_Box_OBB_Box(m_Parent->transform.GetTranslation(), Up, Right, half_width_height,
                ptr_OBB_B->GetParent()->transform.GetTranslation(), ptr_OBB_B->Up, ptr_OBB_B->Right, ptr_OBB_B->half_width_height);
            break;
        }
        case CS_ColliderType::AABB_Box:
        {
            const AABB_BoxCollider_Component* ptr_B = static_cast<const AABB_BoxCollider_Component*>(pOther);
            is_colliding = CS_IntersectCheck::OBB_Box_AABB_Box(Right_Top, Left_Bot, ptr_B->Right_Top, ptr_B->Left_Bot);
            break;
        }
        case CS_ColliderType::Sphere:
        {
            const SphereCollider_Component* ptr_S = static_cast<const SphereCollider_Component*>(pOther);
            is_colliding = CS_IntersectCheck::OBB_Box_Circle(Right_Top, Left_Bot, ptr_S->GetParent()->transform.GetTranslation(), ptr_S->m_radius);
            break;
        }
    }

    if (is_colliding)
    {
		CS_Physics::AddCollisionPair
		({ m_Parent->my_id,m_Parent->m_name, m_Parent },
			{ pOther->GetParent()->my_id,pOther->GetParent()->m_name, pOther->GetParent() });
    }
}

void OBB_BoxCollider_Component::ShowGui()
{
	if (ImGui::TreeNode("OBB_BoxCollider_Component"))
	{
		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}


