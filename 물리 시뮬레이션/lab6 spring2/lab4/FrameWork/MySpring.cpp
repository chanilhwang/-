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
		Vector3 force = p->getPosition() - other->getPosition();
		double mag = force.magnitude();
		force.normalise();
		force = force * -springConstant * abs(mag - restLength);
		p->addForce(force);
	}

	MyAnchoredSpring::MyAnchoredSpring() {}

	MyAnchoredSpring::MyAnchoredSpring(Vector3* anchor, double springConstant,
		double restLength)
	{
		this->anchor = anchor;
		this->springConstant = springConstant;
		this->restLength = restLength;
	}

	MyAnchoredSpring::~MyAnchoredSpring()
	{
		if (anchor)
			delete anchor;
	}

	const Vector3* MyAnchoredSpring::getAnchor() const
	{
		return anchor;
	}

	void MyAnchoredSpring::init(Vector3* anchor, double springConstant,
		double restLength)
	{
		if (anchor)
			delete anchor;
		this->anchor = anchor;
		this->springConstant = springConstant;
		this->restLength = restLength;
	}

	void MyAnchoredSpring::updateForce(Particle* p, double duration)
	{
		Vector3 force = p->getPosition() - *anchor;
		double mag = force.magnitude();
		force.normalise();
		force = force * -springConstant * abs(mag - restLength);
		p->addForce(force);
	}
}