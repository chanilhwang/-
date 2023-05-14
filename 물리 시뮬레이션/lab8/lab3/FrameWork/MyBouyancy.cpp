#include "Mybouyancy.h"

namespace cyclone {
	MyBouyancy::MyBouyancy(double maxDepth, double volume, double waterHeight, double liquidDensity)
	{
		this->maxDepth = maxDepth;
		this->volume = volume;
		this->waterHeight = waterHeight;
		this->liquidDensity = liquidDensity;
	}

	void MyBouyancy::updateForce(Particle* p, double duration)
	{
		Vector3 force;
		Vector3 pos = p->getPosition();

		double h = pos.y;

		if (h > waterHeight + maxDepth)
			force = Vector3(0, -9.8, 0);
		else if (h <= waterHeight - maxDepth)
			force = Vector3(0, volume * liquidDensity, 0);
		else
			force = Vector3(0, volume * liquidDensity * (h - waterHeight - maxDepth) / (2 * maxDepth), 0);

		p->addForce(force);
	}
}