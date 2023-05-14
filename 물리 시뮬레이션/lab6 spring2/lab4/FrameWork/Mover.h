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
	cyclone::MySpring* m_spring;

	std::vector<Mover*> m_movers;
	void update(float duration);
	void draw(int shadow);
};

class AnchorConnection
{
public:
	AnchorConnection();
	~AnchorConnection();

	cyclone::ParticleGravity* m_gravity;
	cyclone::MyAnchoredSpring* m_spring;
	cyclone::ParticleForceRegistry* m_forces;

	std::vector<Mover*> m_movers;
	void update(float duration);
	void draw(int shadow);
};