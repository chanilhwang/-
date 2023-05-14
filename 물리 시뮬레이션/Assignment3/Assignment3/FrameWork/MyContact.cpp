#include "MyContact.h"


using namespace cyclone;

MyGroundContact::MyGroundContact()
{
}


MyGroundContact::~MyGroundContact()
{
}


void MyGroundContact::init(cyclone::Particle * p, double size)
{

	if (p)
		particles.push_back(p);

	this->size = size;
}

unsigned MyGroundContact::addContact(cyclone::ParticleContact *contact, unsigned limit) const
{
	unsigned count = 0;

	for (int i = 0; i < particles.size(); i++)
	{
		cyclone::Particle * p = particles[i];
		cyclone::real y = p->getPosition().y;
		if (y < size)
		{
			contact->contactNormal = cyclone::Vector3::UP;
			contact->particle[0] = p;
			contact->particle[1] = NULL;
			contact->penetration = size - y;
			contact->restitution = 1.0f;
		
			contact++;
			count++;
		}

		if (count >= limit) return count;
	}
	return count;
}

cyclone::ParticleCollision::ParticleCollision(double size)
{
	this->size = size;
}

cyclone::ParticleCollision::~ParticleCollision()
{
}

unsigned cyclone::ParticleCollision::addContact(ParticleContact* contact, unsigned limit) const
{
	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];

	Vector3 aTob = particle[0]->getPosition() - particle[1]->getPosition();
	double distance = aTob.magnitude();
	double collision_condition = distance - size * 2;
	if (collision_condition < 0) {
		aTob.normalise();
		contact->contactNormal = aTob;
		contact->penetration = -collision_condition;
		contact->restitution = 1.0;
		return 1;
	}
	return 0;
}
