#pragma once

#include <deque>
#include "particle.h"
#include "FIreworksRule.h"

class Fire
{
public:
	float size;
	float age;
	Particle* particle;
	Vector3 color;
	FireworksRule* rule;
	std::deque <cyclone::Vector3> history;
	FIRE_TYPE type;

	Fire();
	~Fire();

	bool update(float duration);
	void draw(bool isShadow);
	void drawHistory();
	void putHistory();
	void clearHistory();

	bool checkGround() const;

	void setRule(FireworksRule* r);
	void setParameter(float size, float age, Vector3 position,
		Vector3 velocity, Vector3 color, real damping);
};