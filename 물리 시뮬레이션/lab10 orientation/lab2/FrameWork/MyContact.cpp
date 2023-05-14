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
			contact->penetration = size-y;
			contact->restitution = 0.2f;
			contact++;
			count++;
		}

		if (count >= limit) return count;
	}
	return count;
}





///////////////////////////////////////////////////////////////////////////////////////////////



unsigned ParticleCollision::addContact(ParticleContact *contact,
	unsigned limit) const
{
	// Otherwise return the contact
	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];


	// Calculate the normal
	Vector3 normal = particle[0]->getPosition() - particle[1]->getPosition();
	normal.normalise();
	contact->contactNormal = normal;

	
	float d = (particle[0]->getPosition() - particle[1]->getPosition()).magnitude();
	if (d < 2.0f) {
		contact->penetration = 2.0 - d;
		contact->restitution = 1.0;
	}
	else return 0;
	return 1;
}