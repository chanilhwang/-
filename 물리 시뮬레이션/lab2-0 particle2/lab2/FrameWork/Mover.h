#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "core.h"
#include "particle.h"
#include "fgen.h"
#include "pfgen.h"

class Mover
{
private:
	int size;
	cyclone::ParticleGravity* m_gravity;
	cyclone::ParticleForceRegistry* m_forces;

public:
	cyclone::Particle* particle;

	Mover() {
		size = 2;
		particle = new cyclone::Particle;
		particle->setMass(5.0);
		particle->setDamping(0.9f);
		particle->setPosition(cyclone::Vector3(2.f, 20.f, 0.f));
		particle->setVelocity(cyclone::Vector3(0.f, 0.f, 0.f));
		particle->setAcceleration(cyclone::Vector3(0, 0, 0));

		m_gravity = new cyclone::ParticleGravity(cyclone::Vector3::GRAVITY * 5);
		m_forces = new cyclone::ParticleForceRegistry();

		m_forces->add(particle, m_gravity);
	}
	~Mover() {
		delete particle;
	}

	void update(float duration);
	void stop();
	void draw(bool is_shadow) const;
	bool checkEdge();
};