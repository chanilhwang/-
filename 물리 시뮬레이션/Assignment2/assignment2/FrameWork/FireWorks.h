#pragma once

#include <vector>
#include <deque>
#include <map>
#include "particle.h"
#include "Fire.h"

class Fireworks
{
public:
	Fireworks();
	~Fireworks();

	FireworksRule rules[3];
	std::vector <Fire*> fireworks;
	std::vector <Fire*> erased_fire;

	void update(float duration);
	Fire* create();
	void create(Fire* parent);
	void draw(bool isShadow);
};