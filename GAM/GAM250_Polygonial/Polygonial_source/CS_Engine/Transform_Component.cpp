/**
 * @file Transform_Component.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For transform component
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Transform_Component.hpp"
#include "CS_Math.hpp"
#include "CS_JsonManager.hpp"
#include "imgui.h"
#include "CS_Editor.hpp"

constexpr float RADIAN_LIMIT = 6.28319f;

Transform_Component& Transform_Component::operator=(const Transform_Component& right)
{
    translation = right.translation;
    rotation = right.rotation;
    scale = right.scale;
    depth = right.depth;
    parent = right.parent;
    return *this;
}

void Transform_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
    jsonObject.ReadData("translation", translation);
    jsonObject.ReadData("rotation", rotation);
    jsonObject.ReadData("scale", scale);
    jsonObject.ReadData("depth", depth);
}

void Transform_Component::WriteToFile(CS_JsonObject& jsonObject)
{
	jsonObject.WriteData("translation", translation);
	jsonObject.WriteData("rotation", rotation);
	jsonObject.WriteData("scale", scale);
	jsonObject.WriteData("depth", depth);
}

void Transform_Component::Update(float /*dt*/)
{
	if (WasTranslationChanged || WasRotationChanged || WasScaleChanged || WasDepthChanged|| CS_Editor::IsEditModeOn())
	{
		currentAffine2D = affine2D::build_translation(translation)* affine2D::build_rotation(rotation)* affine2D::build_scale(scale);
		WasTranslationChanged = WasRotationChanged = WasScaleChanged = WasDepthChanged = false;
	}
}

void Transform_Component::ShowGui()
{
	if (ImGui::TreeNode("Transform_Component"))
	{
		ImGui::DragFloat2("translation", &translation.x);
		ImGui::DragFloat("rotation", &rotation);
		ImGui::DragFloat2("scale", &scale.x);
		ImGui::DragFloat("depth", &depth);
		ImGui::TreePop();
	}
}

glm::mat3 Transform_Component::GetModelToWorld() const
{
    if (parent == nullptr)
        return currentAffine2D;
    return parent->GetModelToWorld()*currentAffine2D;
}

glm::mat3 Transform_Component::GetWorldToModel() const
{
	
    auto current = glm::inverse(currentAffine2D);
    if (parent == nullptr)
        return current;
    return current * (parent->GetWorldToModel());
}

float Transform_Component::CalculateWorldDepth() const
{
    if (parent == nullptr)
        return depth;
    return depth + parent->GetDepth();
}

float Transform_Component::GetDepth() const
{
    return depth;
}

void Transform_Component::SetDepth(float new_depth)
{
	WasDepthChanged = true;
    depth = new_depth;
}

glm::vec2 Transform_Component::GetTranslation() const
{
    return translation;
}

void Transform_Component::SetTranslation(const glm::vec2 & new_translation)
{
	WasTranslationChanged = true;
        translation = new_translation;
}


float Transform_Component::GetTranslation_x() const
{
    return translation.x;
}

float Transform_Component::GetTranslation_y() const
{
    return translation.y;
}

void Transform_Component::SetTranslation_x(float new_x)
{
    WasTranslationChanged = true;
    translation.x = new_x;
}

void Transform_Component::SetTranslation_y(float new_y)
{
    WasTranslationChanged = true;
    translation.y = new_y;
}

glm::vec2 Transform_Component::GetScale() const
{
    return scale;
}


float Transform_Component::GetScale_x() const
{
    return scale.x;
}

float Transform_Component::GetScale_y() const
{
    return scale.y;
}

void Transform_Component::SetScale_x(float new_x)
{
    WasScaleChanged = true;
    scale.x = new_x;
}

void Transform_Component::SetScale_y(float new_y)
{
    WasScaleChanged = true;
    scale.x = new_y;
}

void Transform_Component::SetScale(const glm::vec2 & new_scale)
{
	WasScaleChanged = true;
    scale = new_scale;
}

float Transform_Component::GetRotation() const
{
    return rotation;
}

void Transform_Component::SetRotation(float new_rotation)
{
	WasRotationChanged = true;
	if (new_rotation > RADIAN_LIMIT)
		new_rotation -= RADIAN_LIMIT;
	else if (new_rotation < -RADIAN_LIMIT)
		new_rotation += RADIAN_LIMIT;
    rotation = new_rotation;
}

void Transform_Component::SetRotationWithVector(const glm::vec2 & vec)
{
	WasRotationChanged = true;
	//static const glm::vec2 originVec = { 1.f,0.f };
	//float cosR=glm::dot(originVec, vec)/(1*glm::length(vec));
	//rotation = glm::acos(cosR);
	rotation=glm::atan(vec.y, vec.x);
}

const Transform_Component * Transform_Component::GetTransformParent() const
{
    return parent;
}

void Transform_Component::SetTransformParent(const Transform_Component * const transform_parent)
{
    parent = transform_parent;
}
