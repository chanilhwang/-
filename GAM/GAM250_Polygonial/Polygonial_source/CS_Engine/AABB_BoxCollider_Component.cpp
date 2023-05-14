/**
 * @file AABB_BoxCollider_Component.cpp
 * @author Chunmook Kim
 * @email kimmistral\@naver.com
 * @brief Component for AABB collision check.
 * @version 0.1
 * @date 2018-12-10
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
/**
 * @brief Construct a new aabb boxcollider component::aabb boxcollider component object
 * 
 */
AABB_BoxCollider_Component::AABB_BoxCollider_Component()
    :CS_ColliderComponent(CS_ComponentTypes::AABB_BoxCollider_Component, CS_ColliderType::AABB_Box)
{
}
/**
 * @brief Destroy the aabb boxcollider component::aabb boxcollider component object
 * 
 */
AABB_BoxCollider_Component::~AABB_BoxCollider_Component()
{
    CS_Physics::RemoveColliderComponent(this);
}
/**
 * @brief Read initialization data from the JSON file.
 * 
 * @param jsonObject The JSON file to read from.
 */
void AABB_BoxCollider_Component::ReadFromFile(CS_JsonObject& jsonObject)
{
	
}
/**
 * @brief Write data into the JSON file.
 * 
 * @param jsonObject The JSON file to write into.
 */
void AABB_BoxCollider_Component::WriteToFile(CS_JsonObject& jsonObject)
{
	
}

/**
 * @brief Initialize four vertexes.
 * 
 */
void AABB_BoxCollider_Component::Init()
{
    glm::vec3 points[4] = { { -0.5, 0.5, 1 },  { 0.5, 0.5, 1 } ,
                            { -0.5, -0.5, 1 },  { 0.5, -0.5, 1 } };

    auto mat = m_Parent->transform.GetModelToWorld();
    for(auto& point: points)
       point = mat * point;
   Left_Bot = { points[0].x,points[0].y };
   Right_Top = { points[0].x,points[0].y };
   for (auto& point : points)
   {
       if(point.x<Left_Bot.x)
       {
           Left_Bot.x = point.x;
       }
       if (point.y < Left_Bot.y)
       {
           Left_Bot.y = point.y;
       }
       if (point.x > Right_Top.x)
       {
           Right_Top.x = point.x;
       }
       if (point.y > Right_Top.y)
       {
           Right_Top.y = point.y;
       }
   }
   
}
/**
 * @brief Update with collision check.
 * 
 * @param dt Delta time.
 */
void AABB_BoxCollider_Component::Update(float dt)
{
    if(m_Parent->transform.WasScaleChanged|| m_Parent->transform.WasRotationChanged || m_Parent->transform.WasTranslationChanged)
    {
        glm::vec3 points[4] = { { -0.5, 0.5, 1 },  { 0.5, 0.5, 1 } ,
                          { -0.5, -0.5, 1 },  { 0.5, -0.5, 1 } };

        auto mat = m_Parent->transform.GetModelToWorld();
        for (auto& point : points)
            point = mat * point;
        Left_Bot = { points[0].x,points[0].y };
        Right_Top = { points[0].x,points[0].y };
        for (auto& point : points)
        {
            if (point.x < Left_Bot.x)
            {
                Left_Bot.x = point.x;
            }
            if (point.y < Left_Bot.y)
            {
                Left_Bot.y = point.y;
            }
            if (point.x > Right_Top.x)
            {
                Right_Top.x = point.x;
            }
            if (point.y > Right_Top.y)
            {
                Right_Top.y = point.y;
            }
        }
    }
}
/**
 * @brief Make a clone of this component.
 * 
 * @return AABB_BoxCollider_Component* Clone of this component.
 */
AABB_BoxCollider_Component * AABB_BoxCollider_Component::Clone() const
{
    AABB_BoxCollider_Component *temp = new AABB_BoxCollider_Component;
    temp->Left_Bot = Left_Bot;
    temp->Right_Top = Right_Top;
	CS_Physics::AddColliderComponent(temp);
    return temp;
}
/**
 * @brief Test collision with other collider components.
 * 
 * @param pOther Other collision component to test with.
 */
void AABB_BoxCollider_Component::TestCollision(const CS_ColliderComponent* pOther)
{

    bool is_colliding = false;
    switch(pOther->m_colliderType)
    {
        case CS_ColliderType::Sphere:
            {
                const SphereCollider_Component* ptr_S = static_cast<const SphereCollider_Component*>(pOther);
                is_colliding = CS_IntersectCheck::AABB_Box_Circle(Right_Top, Left_Bot, pOther->GetParent()->transform.GetTranslation(), ptr_S->m_radius);
                break;
            }
        case CS_ColliderType::AABB_Box:
            {
                const AABB_BoxCollider_Component* ptr_B = static_cast<const AABB_BoxCollider_Component*>(pOther);
                is_colliding = CS_IntersectCheck::AABB_Box_AABB_Box(Right_Top, Left_Bot, ptr_B->Right_Top, ptr_B->Left_Bot);
                break;
            }
        case CS_ColliderType::OBB_Box:
            {
                const OBB_BoxCollider_Component* ptr_OBB_B = static_cast<const OBB_BoxCollider_Component*>(pOther);
                is_colliding = CS_IntersectCheck::OBB_Box_AABB_Box(Right_Top, Left_Bot, ptr_OBB_B->Right_Top, ptr_OBB_B->Left_Bot);
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
/**
 * @brief For imgui button usage.
 * 
 */
void AABB_BoxCollider_Component::ShowGui()
{
	if (ImGui::TreeNode("AABB_BoxCollider_Component"))
	{
		is_Dead = ImGui::Button("remove");
		ImGui::TreePop();
	}
}
