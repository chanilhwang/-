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

	double getDistance(Vector3 a, Vector3 b)
	{
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
	}

	unsigned ParticleCollision::addContact(ParticleContact* contact, unsigned limit) const
	{
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];

		double distance = getDistance(particle[0]->getPosition(), particle[1]->getPosition());

		if (distance < size * 2) {
			contact->contactNormal = particle[0]->getPosition() - particle[1]->getPosition();
			contact->penetration = 2 * size - distance;
			contact->restitution = 1.0;
			return 1;
		}
		return 0;
	}
}