#pragma once

#include "core.h"
#include "particle.h"
#include <pfgen.h>


namespace cyclone {
	class Myspring : public ParticleForceGenerator
	{
		cyclone::Particle *other;
		double springConstant;
		double restLength;

	public:
		Myspring(cyclone::Particle * p, double springConstant, double restLenght);
		virtual void updateForce(cyclone::Particle * p, double duration);

	};

	class MyAnchoredSpring : public ParticleForceGenerator
	{
		cyclone::Vector3 *anchor;
		double springConstant;
		double restLength;

	public:
		MyAnchoredSpring();
		MyAnchoredSpring(cyclone::Vector3 *anchor, double springConstant, double restLenght);
		void init(cyclone::Vector3 *anchor, double springConstant, double restLenght);
		virtual void updateForce(cyclone::Particle * p, double duration);

	};

}


