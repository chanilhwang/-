#pragma once

#include "fgen.h"

namespace cyclone {
	class MyBouyancy : public ParticleForceGenerator
	{
		double maxDepth;
		double volume;
		double waterHeight;
		double liquidDensity;

	public:
		MyBouyancy(double maxDepth, double volume, double waterHeight,
			double liquidDensity);
		virtual void updateForce(Particle* p, double duration);
	};
}