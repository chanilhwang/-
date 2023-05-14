/**
 * @file OBB_BoxCollider_Component.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For OBB collider
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include <glm/vec2.hpp>
#include "CS_ColliderComponent.hpp"

class AABB_BoxCollider_Component;
class SphereCollider_Component;
class OBB_BoxCollider_Component : public CS_ColliderComponent
{
    friend AABB_BoxCollider_Component;
    friend SphereCollider_Component;
public:
    void Init() override;
    OBB_BoxCollider_Component();
    ~OBB_BoxCollider_Component() override;
    void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
    void Update(float dt) override;
    CS_ColliderComponent* Clone() const override;
    void TestCollision(const CS_ColliderComponent* pOther) override;
    void ShowGui() override;

private:
    glm::vec2 half_width_height{};
    glm::vec2 Right_Top{};
    glm::vec2 Left_Bot{};
    glm::vec2 Right{};//unit_vector->rotation need to change based on rotation 
    glm::vec2 Up{};//unit_vector->rotation need to change based on rotation 
};
