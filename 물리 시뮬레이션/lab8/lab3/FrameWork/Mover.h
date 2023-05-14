#pragma once

#include "particle.h"
#include "fgen.h"
#include "pfgen.h"
#include "MyBouyancy.h"

class Mover
{
public:
	Mover();
	~Mover();

	cyclone::MyBouyancy* m_bouyancy;
	
	cyclone::ParticleForceRegistry* m_forces;
	cyclone::Particle* m_particle;

	void update(float duration);
	void draw(int shadow);
	void checkEdges();
};

