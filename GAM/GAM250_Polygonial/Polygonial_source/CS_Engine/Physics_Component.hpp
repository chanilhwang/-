/**
 * @file Physics_Component.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief For physics management.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 * 
 */
#pragma once
#include "CS_Components.hpp"
#include <glm/vec2.hpp>
#define FLOATMAX 3.402823466e+38F
class CS_Object;
class Physics_Component :public CS_Component
{
	friend CS_Object;
public:
	Physics_Component();
        Physics_Component& operator=(const Physics_Component& right);
	~Physics_Component() override;
        void ReadFromFile(CS_JsonObject& jsonObject) override;
	void WriteToFile(CS_JsonObject& jsonObject) override;
	void Update(float dt) override;
        Physics_Component* Clone() const override { return nullptr; }//this is included in object so we shouldn't clone
		void ShowGui() override;
protected:
	float inverseMass=0.1f;

	float damping = 0.f;

	glm::vec2 velocity{};

	glm::vec2 forceAccum{};

	glm::vec2 acceleration{};
private:
	void integrate(float duration);
public:
	void setMass(const float mass);
	float getMass() const;
	void setInverseMass(const float inverseMass);
	float getInverseMass() const;
	bool hasFiniteMass() const;
	void setDamping(const float damping);
	float getDamping() const;
	void setVelocity(const glm::vec2 &velocity);
	void setVelocity_X(float x);
	void setVelocity_Y(float y);
	glm::vec2 getVelocity() const;
	void setAcceleration(const glm::vec2 &acceleration);
	void setAcceleration_X(float x);
	void setAcceleration_Y(float y);
	glm::vec2 getAcceleration() const;
	void clearAccumulator();
	void addForce(const glm::vec2 &force);
	void addForce_X(float x);
	void addForce_Y(float y);
};
