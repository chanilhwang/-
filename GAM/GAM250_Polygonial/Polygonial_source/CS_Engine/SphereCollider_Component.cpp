/**
 * @file SphereCollider_Component.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For SphereCollider component.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "SphereCollider_Component.hpp"
#include "AABB_BoxCollider_Component.hpp"
#include "CS_Object.hpp"
#include "CS_Physics.hpp"
#include <iostream>
#include "OBB_BoxCollider_Component.hpp"
#include "imgui.h"

SphereCollider_Component::SphereCollider_Component()
:CS_ColliderComponent(CS_ComponentTypes::SphereCollider_Component,CS_ColliderType::Sphere)
{
    //collider_index = 1;
}

SphereCollider_Component::~SphereCollider_Component()
{
    CS_Physics::RemoveColliderComponent(this);
}

void SphereCollider_Component::ReadFromFile(CS_JsonObject& jsonObject)
{
	
}

void SphereCollider_Component::WriteToFile(CS_JsonObject & jsonObject)
{
	
}

void SphereCollider_Component::Init()
{
    auto scale = m_Parent->transform.GetScale();
    m_radius = (scale.x > scale.y) ? scale.x/2 : scale.y/2;
}

void SphereCollider_Component::Update(float dt)
{
    if(m_Parent->transform.WasScaleChanged)
    {
        auto scale = m_Parent->transform.GetScale();
        m_radius = (scale.x > scale.y) ? scale.x / 2 : scale.y / 2;
    }
}

SphereCollider_Component * SphereCollider_Component::Clone() const
{
    SphereCollider_Component* temp = new SphereCollider_Component;
    temp->m_radius = m_radius;
	CS_Physics::AddColliderComponent(temp);
    return temp;
}

void SphereCollider_Component::TestCollision(const CS_ColliderComponent * pOther)
{
    bool is_colliding = false;
    switch (pOther->m_colliderType)
    {
    case CS_ColliderType::Sphere:
    {
        const SphereCollider_Component * ptr_S = static_cast<const SphereCollider_Component*>(pOther);

        is_colliding = CS_IntersectCheck::Circle_Circle(m_Parent->transform.GetTranslation()
            , m_radius, pOther->GetParent()->transform.GetTranslation(), ptr_S->m_radius);
        break;
    }
    case CS_ColliderType::AABB_Box:
    {
        const AABB_BoxCollider_Component * ptr_B = static_cast<const AABB_BoxCollider_Component*>(pOther);

        is_colliding = CS_IntersectCheck::AABB_Box_Circle(ptr_B->Right_Top, ptr_B->Left_Bot
            , m_Parent->transform.GetTranslation(), m_radius);

        break;
    }
    case CS_ColliderType::OBB_Box:
        {
        const OBB_BoxCollider_Component* ptr_OBB_B = static_cast<const OBB_BoxCollider_Component*>(pOther);
        is_colliding = CS_IntersectCheck::OBB_Box_Circle(ptr_OBB_B->Right_Top, ptr_OBB_B->Left_Bot, m_Parent->transform.GetTranslation(), m_radius);
        }

    default:
        return;
    }
    if(is_colliding)
    {
		CS_Physics::AddCollisionPair
		({ m_Parent->my_id,m_Parent->m_name, m_Parent },
			{ pOther->GetParent()->my_id,pOther->GetParent()->m_name, pOther->GetParent() });
		
    }
}

void SphereCollider_Component::ShowGui()
{
	if (ImGui::TreeNode("SphereCollider_Component"))
	{
		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}

