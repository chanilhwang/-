/**
 * @file CS_Particle.hpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief header file for particle 
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#pragma once
#include <string>
#include "../Dependencies/Math/glm/glm.hpp"
#include "CS_Object.hpp"

struct Particle {
	CS_Object* pObject;
	float life;
	float scale_multiplier = 0.9f;
	float alpha_multiplier = 1.0f;
};

class ParticleGenerator {
public:
	static void MakeParticles(int amount, CS_ArcheTypes arche,
		glm::vec2 init_pos,
		float starting_degree, float ending_degree,
		float min_velocity, float max_velocity,
		float life);
	static void MakeParticles(int amount, CS_ArcheTypes arche,
		glm::vec2 init_pos,
		glm::vec2 vector,
		float speed,
		float life);
	static void MakeChargingParticles(int amount, CS_ArcheTypes arche,
		glm::vec2 object,
		glm::vec2 offset,
		float life);
	static void MakeBackGroundParticle(int amount, CS_ArcheTypes arche);
	void Update(float dt);
};