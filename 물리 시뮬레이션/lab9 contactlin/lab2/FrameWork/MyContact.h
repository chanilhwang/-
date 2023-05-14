#pragma once

#include <vector>
#include "particle.h"
#include "plinks.h"

namespace cyclone {
	class MyGroundContact : public ParticleContactGenerator
	{
		std::vector<Particle*> particles;
		double size;
	public:
		MyGroundContact();
		~MyGroundContact();
		void init(Particle* p, double size);
		virtual unsigned addContact(ParticleContact* contact,
			unsigned limit) const;
	};

	class ParticleCollision : public ParticleLink
	{
	public:
		double size;
		
		virtual unsigned addContact(ParticleContact* contact,
			unsigned limit) const;
	};
}