#include "MyContact.h"

namespace cyclone {
	MyGroundContact::MyGroundContact() : size(0) {}
	MyGroundContact::~MyGroundContact() {}
	void MyGroundContact::init(Particle* p, double size)
	{
		if (p)
			particles.push_back(p);
		this->size = size;
	}
	unsigned MyGroundContact::addContact(ParticleContact* contact,
		unsigned limit) const
	{
		unsigned count = 0;
		for (unsigned int i = 0; i < particles.size(); i++) {
			Particle* p = particles[i];
			double y = p->getPosition().y;
			//collide ground
			double penetration = size - y;
			if (penetration > 0) {
				contact->contactNormal = Vector3(0, 1, 0);
				contact->particle[0] = p;
				contact->particle[1] = nullptr;
				contact->penetration = penetration;
				contact->restitution = 1.0;
				count++;
				contact++;
			}
			if (count >= limit)
				return count;
		}
		return count;
	}
}