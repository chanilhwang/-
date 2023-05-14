#include "MySpring.h"

namespace cyclone {
	MySpring::MySpring(Particle* p, double springConstant, double restLength)
	{
		other = p;
		this->springConstant = springConstant;
		this->restLength = restLength;
	}

	void MySpring::updateForce(Particle* p, double duration)
	{
		Vector3 force = other->getPosition() - p->getPosition();
		double mag = force.magnitude();
		force.normalise();
		force = force * -springConstant * abs(mag - restLength);
		other->addForce(force);
	}
}