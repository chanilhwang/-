/**
 * @file CS_Particle.cpp
 * @author ChanIl Hwang
 * @email tktlssla803@gmail.com
 * @brief implementation file for particle
 * @version 0.1
 * @date 2019-05-10
 *
 * @All content © 2019 DigiPen (USA) Corporation, all rights reserved.
 *
 */
#include <vector>
#include <random>
#include "CS_Particle.hpp"
#include "CS_Graphics.hpp"
#include "CS_ObjectManager.hpp"
#include "../Dependencies/Math/glm/glm.hpp"

#define PI 3.14159265359f

namespace {
	std::vector<Particle> Particles;
}

void ParticleGenerator::MakeParticles(int amount, CS_ArcheTypes arche,
	glm::vec2 init_pos,
	float starting_degree, float ending_degree,
	float min_velocity, float max_velocity,
	float life) 
{
	std::vector<CS_Object*> vec = CS_ObjectManager::CreateParticles(amount, arche);


	for (int i = 0; i < amount; i++) {
		int random = rand();
		int random2 = rand();

		Particle particle;
		particle.life = life;
		particle.pObject = vec[i];
		vec[i]->transform.SetTranslation(init_pos);
		
		glm::vec2 vel = (random % static_cast<int>(max_velocity - min_velocity) + min_velocity) * glm::normalize(glm::vec2(random * (random % 2 == 0? 1 : -1), random2 * (random2 % 2 == 0? 1 : -1)));

		float rand_radian = (random % static_cast<int>(ending_degree - starting_degree)) * PI / 180.f;
		
		vec[i]->physics.setVelocity(vel);
		vec[i]->transform.SetRotation(rand_radian);
		vec[i]->transform.SetScale({ 100.0f, 100.0f });
		vec[i]->physics.setDamping(1.0f);
		Particles.push_back(particle);
	}
}

void ParticleGenerator::MakeParticles(int amount, CS_ArcheTypes arche,
	glm::vec2 init_pos,
	glm::vec2 vector,
	float speed,
	float life)
{
	glm::vec2 normalvec = glm::normalize(vector);
	float rot = 0;
	if (normalvec.x != 0)
		rot = glm::atan(normalvec.y / normalvec.x);
	std::vector<CS_Object*> vec = CS_ObjectManager::CreateParticles(amount, arche);
	for (int i = 0; i < amount; i++) {
		Particle particle;
		particle.life = life;
		particle.pObject = vec[i];
		vec[i]->transform.SetTranslation(init_pos);
		glm::vec2 vel = speed * normalvec;
		vec[i]->physics.setVelocity(vel);
		vec[i]->transform.SetRotation(rot);
		vec[i]->transform.SetScale({ 100.0f, 100.0f });
		vec[i]->physics.setDamping(1.0f);
		Particles.push_back(particle);
	}
}

void ParticleGenerator::MakeChargingParticles(int amount, CS_ArcheTypes arche,
	glm::vec2 object,
	glm::vec2 offset,
	float life) {
	std::vector<CS_Object*> vec = CS_ObjectManager::CreateParticles(amount, arche);
	for (int i = 0; i < amount; i++) {
		int random = rand();
		int random2 = rand();

		Particle particle;
		particle.life = life;
		particle.pObject = vec[i];
		vec[i]->transform.SetTranslation(object	+ glm::vec2(random % static_cast<int>(offset.y - offset.x) + offset.x, random2 % static_cast<int>(offset.y - offset.x) + offset.x));
		glm::vec2 vel = glm::normalize(vec[i]->transform.GetTranslation() - object) / life;
		vec[i]->physics.setVelocity(vel);
		float rot = (random % 180) * PI / 180.f;
		vec[i]->transform.SetRotation(rot);
		vec[i]->transform.SetScale({ 10.0f, 10.0f });
		vec[i]->physics.setDamping(1.0f);
		Particles.push_back(particle);
	}
}

void ParticleGenerator::MakeBackGroundParticle(int amount, CS_ArcheTypes arche) {
	std::vector<CS_Object*> vec = CS_ObjectManager::CreateParticles(amount, arche);
	float min_ground_x = -1.5f * CS_Graphics::GetDisplayAreaSize().x;
	float min_ground_y = -1.5f * CS_Graphics::GetDisplayAreaSize().x;

	float backgroundwidth = 3.0f * CS_Graphics::GetDisplayAreaSize().x;
	float backgroundheight = 3.0f * CS_Graphics::GetDisplayAreaSize().x;

	for (int i = 0; i < amount; i++) {
		int random = rand();
		int random2 = rand();
		Particle particle;
		particle.life = 1;
		particle.pObject = vec[i];
		particle.alpha_multiplier = 0.95f;
		particle.scale_multiplier = 1.f;
		float rot = (random % 180) * PI / 180.f;
		vec[i]->transform.SetTranslation(glm::vec2(random % static_cast<int>(backgroundwidth) + min_ground_x,
			random2 % static_cast<int>(backgroundheight) + min_ground_y));
		vec[i]->transform.SetRotation(rot);

		int random3 = random % 15,
			random4 = random2 % 15;
		vec[i]->physics.setVelocity(glm::vec2(random3 - vec[i]->transform.GetTranslation_x(),
			random4 - vec[i]->transform.GetTranslation_y()));
		vec[i]->gfx.SetColor(CS_Color{255,255,255});
		vec[i]->transform.SetScale({ random % 8 + 5, random % 8 + 5 });
		vec[i]->physics.setDamping(1.0f);
		vec[i]->transform.SetDepth(0.51f);
		Particles.push_back(particle);
	}
}

void ParticleGenerator::Update(float dt)
{
	for (int i = 0; i < Particles.size(); i++) {
		if (Particles[i].life < 0) {
			delete Particles[i].pObject;
			Particles[i].life = Particles[Particles.size() - 1].life;
			Particles[i].pObject = Particles[Particles.size() - 1].pObject;
			Particles.pop_back();
			i--;
		}
		else {
			Particles[i].life -= dt;
			Particles[i].pObject->transform.SetScale(Particles[i].pObject->transform.GetScale() * Particles[i].scale_multiplier);
			Particles[i].pObject->gfx.SetColor(CS_Color{ Particles[i].pObject->gfx.GetColor().Red,
				Particles[i].pObject->gfx.GetColor().Blue,
				Particles[i].pObject->gfx.GetColor().Green,
				static_cast<unsigned char>(Particles[i].pObject->gfx.GetColor().Alpha * Particles[i].alpha_multiplier) });
			Particles[i].pObject->Update(dt);
		}
	}
}