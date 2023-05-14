#pragma once

#include "particle.h"
#include "fgen.h"

namespace cyclone {

	class MySpring : public ParticleForceGenerator
	{
		Particle* other;
		double springConstant;
		double restLength;
	public:
		MySpring(Particle* p, double springConstant, double restLength);
		virtual void updateForce(Particle* p, double duration);
	};
}