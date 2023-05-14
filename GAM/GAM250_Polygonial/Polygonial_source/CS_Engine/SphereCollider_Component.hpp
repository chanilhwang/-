/**
 * @file SphereCollider_Component.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For SphereCollider component.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_ColliderComponent.hpp"
class AABB_BoxCollider_Component;
class OBB_BoxCollider_Component;
class SphereCollider_Component:public CS_ColliderComponent
{
        friend AABB_BoxCollider_Component;
        friend OBB_BoxCollider_Component;
public:
	SphereCollider_Component();
	~SphereCollider_Component() override;
        void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
        void Init() override;
	void Update(float dt) override;
        SphereCollider_Component* Clone() const override;
	void TestCollision(const CS_ColliderComponent* pOther) override;
	void ShowGui() override;
private:
	float m_radius;
};

