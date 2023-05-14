/**
 * @file CS_ColliderComponent.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For collision detection.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_Components.hpp"
#include <glm/glm.hpp>
#include "CS_Physics.hpp"
#include "CS_Editor.hpp"
#include <iostream>
#include "CS_JsonManager.hpp"

enum class CS_ColliderType
{
    Sphere,
    AABB_Box,
    OBB_Box,
    INVAILD
};

class CS_ColliderComponent:public CS_Component
{
public:
	CS_ColliderComponent(CS_ComponentTypes m_component_type,CS_ColliderType m_colliderType)
    :CS_Component(m_component_type),m_colliderType(m_colliderType){}
	~CS_ColliderComponent() override{};
        void ReadFromFile(CS_JsonObject& jsonObject) override{};
		void WriteToFile(CS_JsonObject& jsonObject) override {};
	void Update(float dt) override{};
        CS_ColliderComponent* Clone() const override{return nullptr;}
	virtual void TestCollision(const CS_ColliderComponent* pOther){};
        const CS_ColliderType m_colliderType;
		void ShowGui() override{}
		void SetGhost(bool toSet) { is_Ghost = toSet; }
	mutable bool is_Ghost = false;
};
namespace CS_IntersectCheck
{
    bool Circle_Circle(const glm::vec2& circleCenter0, float radius0, const glm::vec2& circleCenter1, float radius1);
    bool AABB_Box_Circle(const glm::vec2& Right_Top, const glm::vec2& Left_Bot, const glm::vec2& circleCenter, float radius);
    bool AABB_Box_AABB_Box(const glm::vec2& Right_Top1, const glm::vec2& Left_Bot1, const glm::vec2& Right_Top2, const glm::vec2& Left_Bot2);
    bool OBB_Box_OBB_Box(const glm::vec2 center1, const glm::vec2 Up1, const glm::vec2 Right1, const glm::vec2 half_lengths1,
        const glm::vec2 center2, const glm::vec2 Up2, const glm::vec2 Right2, const glm::vec2 half_lengths2);
    bool OBB_Box_AABB_Box(const glm::vec2& Right_Top1, const glm::vec2& Left_Bot1, const glm::vec2& Right_Top2, const glm::vec2& Left_Bot2);
    bool OBB_Box_Circle(const glm::vec2& Right_Top, const glm::vec2& Left_Bot, const glm::vec2& circleCenter, float radius);
}