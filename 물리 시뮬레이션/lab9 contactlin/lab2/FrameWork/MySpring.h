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

	class MyAnchoredSpring : public ParticleForceGenerator
	{
	protected:
		Vector3* anchor;
		double springConstant;
		double restLength;
	public:
		MyAnchoredSpring();
		MyAnchoredSpring(Vector3* anchor, double springConstant, double restLength);
		~MyAnchoredSpring();
		const Vector3* getAnchor() const;
		void init(Vector3* anchor, double springConstant, double restLength);
		virtual void updateForce(Particle* p, double duration);
	};
}