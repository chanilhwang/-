#pragma once

#include "particle.h"
#include "fgen.h"
#include "pfgen.h"
#include "MySpring.h"

#include <vector>

class Mover
{
public:
	Mover();
	~Mover();
	
	cyclone::MySpring* m_spring;
	cyclone::Particle* m_particle;

	void update(float duration);
	void draw(int shadow);
	void setConnection(Mover* a);
	void checkEdges();
};

class MoverConnection
{
public:
	MoverConnection();
	~MoverConnection();

	cyclone::ParticleGravity* m_gravity;
	cyclone::ParticleForceRegistry* m_forces;

	std::vector<Mover*> m_movers;
	void update(float duration);
	void draw(int shadow);
};