#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "core.h"
#include "particle.h"

class Mover
{
private:
	int size;
	cyclone::Particle* particle;

public:
	Mover() {
		size = 2;
		particle = new cyclone::Particle;
		particle->setInverseMass(1.f / 1.f);
		particle->setDamping(0.999f);
		particle->setPosition(cyclone::Vector3(2.f, 20.f, 0.f));
		particle->setVelocity(cyclone::Vector3(0.f, 0.f, 0.f));
		particle->setAcceleration(cyclone::Vector3::GRAVITY);
	}
	~Mover() {
		delete particle;
	}

	void update(float duration);
	void stop();
	void draw(bool is_shadow) const;
	bool checkEdge();
};