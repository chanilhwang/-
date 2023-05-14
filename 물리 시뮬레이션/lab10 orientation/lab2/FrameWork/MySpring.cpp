
#include "MySpring.h"
////////////////////////////////////////////////////////////////
using namespace cyclone;


Myspring::Myspring(cyclone::Particle * p, double springConstant, double restLenght)
{
	other = p;
	this->springConstant = springConstant;
	this->restLength = restLenght;

}

void Myspring::updateForce(cyclone::Particle * p, double duration)
{

	cyclone::Vector3 force;

	cyclone::Vector3 pos = p->getPosition();
	cyclone::Vector3 pos2 = other->getPosition();

	cyclone::Vector3 d = pos - pos2;
	double l = d.magnitude();

	d.normalise();
	force = d * -springConstant * (l - restLength);
	/*
	p->getPosition(&force);
	force -= other->getPosition();

	float magnitude = force.magnitude();
	magnitude = real_abs(magnitude - restLength);
	magnitude *= springConstant;

	force.normalise();
	force *= -magnitude;
	*/
	p->addForce(force);
}


//////////////////////////////////////////////

MyAnchoredSpring::MyAnchoredSpring()
{


}
MyAnchoredSpring::MyAnchoredSpring(cyclone::Vector3 * anchor, double springConstant, double restLenght)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLenght;

}

void MyAnchoredSpring::init(cyclone::Vector3 *anchor, double springConstant, double restLenght)
{
	this->anchor = anchor;
	this->springConstant = springConstant;
	this->restLength = restLenght;

}
void MyAnchoredSpring::updateForce(cyclone::Particle * p, double duration)
{

	cyclone::Vector3 force;

	cyclone::Vector3 pos = p->getPosition();
	
	cyclone::Vector3 d = pos - *anchor;
	double l = d.magnitude();

	d.normalise();
	force = d * -springConstant * (l - restLength);
	
	p->addForce(force);
}
