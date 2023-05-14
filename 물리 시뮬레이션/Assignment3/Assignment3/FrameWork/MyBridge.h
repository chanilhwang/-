#pragma once

#include <vector>

#include "Mover.h"
#include "plinks.h"
#include "MyContact.h"
#include "pworld.h"

class MyBridge
{
private:
	std::vector<Mover*> m_movers;

	cyclone::ParticleGravity* m_gravity;

	std::vector<cyclone::ParticleCable*> m_cables;
	std::vector<cyclone::ParticleRod*> m_rods;
	std::vector<cyclone::ParticleCableConstraint*> m_supports;
	std::vector<cyclone::ParticleCollision*> m_collisions;

public:
	MyBridge(cyclone::ParticleWorld* world);

	const std::vector<Mover*>& getMovers() const;
	void draw(int shadow);
};