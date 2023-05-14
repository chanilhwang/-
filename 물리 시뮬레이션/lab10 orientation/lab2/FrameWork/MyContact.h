#pragma once


#include <vector>
#include "particle.h"
#include "plinks.h"


namespace cyclone {

	class MyGroundContact : public cyclone::ParticleContactGenerator
	{
		std::vector<cyclone::Particle *> particles;
		double size;
	public:
		MyGroundContact();
		~MyGroundContact();
		void init(cyclone::Particle * p, double size);

		virtual unsigned addContact(cyclone::ParticleContact *contact, unsigned limit) const;
	};


	class ParticleCollision : public ParticleLink
	{
	public:
		/**
		* Holds the length of the rod.
		*/

	public:
		/**
		* Fills the given contact structure with the contact needed
		* to keep the rod from extending or compressing.
		*/
		virtual unsigned addContact(ParticleContact *contact,
			unsigned limit) const;
	};
}