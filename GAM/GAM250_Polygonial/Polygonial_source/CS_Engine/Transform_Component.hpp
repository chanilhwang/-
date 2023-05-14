#pragma once
#include <glm/glm.hpp>
#include "CS_Components.hpp"
class Physics_Component;
class CS_Object;
class Transform_Component:public CS_Component
{
	friend CS_Object;
public:
    Transform_Component():CS_Component(CS_ComponentTypes::Transform_Component){}
    Transform_Component& operator=(const Transform_Component& right);
    void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Update(float dt)override;

    Transform_Component* Clone() const override { return nullptr; }//this is included in object so we shouldn't clone
	void ShowGui() override;
	glm::mat3 GetModelToWorld() const;
    glm::mat3 GetWorldToModel() const;
    float CalculateWorldDepth() const;
    float GetDepth() const;
    void SetDepth(float new_depth);

    glm::vec2 GetTranslation() const;
    void SetTranslation(const glm::vec2& new_translation);
    float GetTranslation_x(void) const;
    float GetTranslation_y(void) const;
    void SetTranslation_x(float new_x);
    void SetTranslation_y(float new_y);

    glm::vec2 GetScale() const;
    void SetScale(const glm::vec2& new_scale);
    float GetScale_x(void) const;
    float GetScale_y(void) const;
    void SetScale_x(float new_x);
    void SetScale_y(float new_y);

    float GetRotation() const;
    void SetRotation(float new_rotation);
	void SetRotationWithVector(const glm::vec2& vec);

    const Transform_Component* GetTransformParent() const;
    void SetTransformParent(const Transform_Component* const transform_parent);
	mutable bool WasTranslationChanged = true;
	mutable bool WasScaleChanged = true;
	mutable bool WasRotationChanged = true;
	mutable bool WasDepthChanged = true;
private:
    glm::vec2 translation{};
    glm::vec2 scale{ 1.0f, 1.0f };
    float rotation = 0.0f;
    float depth = 0.5f;
    const Transform_Component* parent = nullptr;
	glm::mat3 currentAffine2D;
};

