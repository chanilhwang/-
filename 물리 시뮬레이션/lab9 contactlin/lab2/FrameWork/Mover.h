#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "core.h"
#include "particle.h"
#include "fgen.h"
#include "pfgen.h"
#include "MySpring.h"

class Mover
{
private:
	int size;

public:
	cyclone::Particle* particle;
	cyclone::MySpring* spring;

	Mover() : spring(nullptr) {
		size = 2;
		particle = new cyclone::Particle;
		particle->setMass(5.0);
		particle->setDamping(0.7f);
		particle->setPosition(cyclone::Vector3(2.f, 20.f, 0.f));
		particle->setVelocity(cyclone::Vector3(0.f, 0.f, 0.f));
		particle->setAcceleration(cyclone::Vector3(0, 0, 0));
	}
	~Mover() {
		delete particle;
	}

	void update(float duration);
	void stop();
	void setConnection(Mover* a);
	void draw(bool is_shadow) const;
};

class MoverConnection
{
public:
	MoverConnection();
	~MoverConnection();

	cyclone::ParticleGravity* m_gravity;
	cyclone::ParticleForceRegistry* m_forces;
	cyclone::MySpring* m_spring;

	std::vector<Mover*> m_movers;
	void update(float duration);
	void draw(int shadow);
};