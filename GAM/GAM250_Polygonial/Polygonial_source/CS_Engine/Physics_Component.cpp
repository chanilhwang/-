/**
 * @file Physics_Component.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For physics management.
 * @version 0.1
 * @date 2018-12-14
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include "Physics_Component.hpp"
#include "CS_ComponentTypes.hpp"
#include "CS_Object.hpp"
#include "CS_JsonManager.hpp"
#include "imgui.h"

Physics_Component::Physics_Component():CS_Component(CS_ComponentTypes::Physics_Component)
{
}

Physics_Component & Physics_Component::operator=(const Physics_Component & right)
{
    inverseMass = right.inverseMass;
    damping = right.damping;
    velocity = right.velocity;
    forceAccum = right.forceAccum;
    acceleration = right.acceleration;
    return *this;
}

Physics_Component::~Physics_Component()
{
}

void Physics_Component::ReadFromFile(CS_JsonObject & jsonObject)
{
	jsonObject.ReadData("inverseMass", inverseMass);
	jsonObject.ReadData("damping", damping);
	jsonObject.ReadData("velocity", velocity);
	jsonObject.ReadData("forceAccum", forceAccum);
	jsonObject.ReadData("acceleration", acceleration);
}

void Physics_Component::WriteToFile(CS_JsonObject& jsonObject)
{
}


void Physics_Component::Update(float dt)
{
	integrate(dt);
}

void Physics_Component::ShowGui()
{
	if (ImGui::TreeNode("Physics_Component"))
	{
		ImGui::DragFloat("inverseMass", &inverseMass);
		ImGui::DragFloat("damping", &damping);
		ImGui::DragFloat2("velocity", &velocity.x);
		ImGui::DragFloat2("forceAccum", &forceAccum.x);
		ImGui::DragFloat2("acceleration", &acceleration.x);
		ImGui::TreePop();
	}
}


void Physics_Component::integrate(float duration)
{
	// We don't integrate things with zero mass.
	if (inverseMass <= 0.0f) return;

	assert(duration > 0.0);

	// Update linear position.
	auto pos = m_Parent->transform.GetTranslation();
	pos += velocity * duration;
	m_Parent->transform.SetTranslation(pos);
	// Work out the acceleration from the force
	glm::vec2 resultAcc = acceleration;
	resultAcc += forceAccum * inverseMass;
	// Update linear velocity from the acceleration.
	velocity += resultAcc * duration;
	// Impose drag.
	velocity *= glm::pow(damping, duration);

	// Clear the forces.
	clearAccumulator();
}

void Physics_Component::setMass(const float mass)
{
	assert(mass != 0);
	inverseMass = 1.f / mass;
}

float Physics_Component::getMass() const
{
	if (inverseMass == 0) {
		return FLOATMAX;
	}
	else {
		return 1.f / inverseMass;
	}
}

void Physics_Component::setInverseMass(const float inverseMass)
{
	Physics_Component::inverseMass = inverseMass;
}

float Physics_Component::getInverseMass() const
{
	return inverseMass;
}

bool Physics_Component::hasFiniteMass() const
{
	return inverseMass >= 0.0f;
}

void Physics_Component::setDamping(const float damping)
{
	Physics_Component::damping = damping;
}

float Physics_Component::getDamping() const
{
	return damping;
}

void Physics_Component::setVelocity(const glm::vec2 & velocity)
{
	Physics_Component::velocity = velocity;
}

void Physics_Component::setVelocity_X(float x)
{
	velocity.x = x;
}

void Physics_Component::setVelocity_Y(float y)
{
	velocity.y = y;
}


glm::vec2 Physics_Component::getVelocity() const
{
	return velocity;
}

void Physics_Component::setAcceleration(const glm::vec2 & acceleration)
{
	Physics_Component::acceleration = acceleration;
}

void Physics_Component::setAcceleration_X(float x)
{
	acceleration.x = x;
}

void Physics_Component::setAcceleration_Y(float y)
{
	acceleration.y = y;
}

glm::vec2 Physics_Component::getAcceleration() const
{
	return acceleration;
}

void Physics_Component::clearAccumulator()
{
	forceAccum.x = 0.f;
	forceAccum.y = 0.f;
}

void Physics_Component::addForce(const glm::vec2 & force)
{
	forceAccum += force;
}

void Physics_Component::addForce_X(float x)
{
	forceAccum.x += x;
}

void Physics_Component::addForce_Y(float y)
{
	forceAccum.y += y;
}
