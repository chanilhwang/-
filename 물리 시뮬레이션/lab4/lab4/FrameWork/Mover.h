#pragma once

#include "particle.h"
#include "fgen.h"
#include "pfgen.h"

class Mover
{
public:
	Mover();
	~Mover();

	cyclone::ParticleGravity* m_gravity;
	cyclone::ParticleDrag* m_drag;
	
	cyclone::ParticleForceRegistry* m_forces;
	cyclone::Particle* m_particle;

	void update(float duration);
	void draw(int shadow);
	void checkEdges();
};