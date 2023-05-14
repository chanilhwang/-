#pragma once

#include "core.h"

using namespace cyclone;

enum class FIRE_TYPE {
	FIRE_INIT,
	FIRE_CHILD
};

class FireworksRule
{
public:
	FireworksRule();
	~FireworksRule();

	//spawn parameters
	FIRE_TYPE type;
	float size;
	real minAge;
	real maxAge;
	Vector3 minVelocity;
	Vector3 maxVelocity;
	real damping;
	unsigned payloadCount;
	FireworksRule* nextRule;
	bool isColorRandom;

	void setParameters(FIRE_TYPE type, float size, real minAge,
		real maxAge, const Vector3& minVelocity, const Vector3& maxVelocity, 
		real damping, int count, FireworksRule* nextRule, bool isColorRandom);
};