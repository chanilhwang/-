#pragma once
#include"CS_ColliderComponent.hpp"

class SphereCollider_Component;
class OBB_BoxCollider_Component;
class AABB_BoxCollider_Component : public CS_ColliderComponent
{
    friend SphereCollider_Component;
    friend OBB_BoxCollider_Component;
public:  
    AABB_BoxCollider_Component();
    ~AABB_BoxCollider_Component() override;
    void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
    void Init() override;
    void Update(float dt) override;
    AABB_BoxCollider_Component* Clone() const override;
    void TestCollision(const CS_ColliderComponent* pOther) override;
	void ShowGui() override;

private:
    glm::vec2 Right_Top{};
    glm::vec2 Left_Bot{};
};
