#pragma once


#include "particle.h"
#include "fgen.h"

class Mover
{
public:
	Mover();
	Mover(cyclone::Vector3 position);
	~Mover();

	cyclone::Particle * m_particle;

	void draw(int shadow);
};