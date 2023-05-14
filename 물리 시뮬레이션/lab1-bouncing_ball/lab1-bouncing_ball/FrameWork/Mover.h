#pragma once

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "core.h"

class Mover
{
private:
	int size;
	float inverse_mass;
	float damping;
	cyclone::Vector3 position;
	cyclone::Vector3 velocity;
	cyclone::Vector3 acceleration;

public:
	Mover() {
		size = 2;
		inverse_mass = 1.f / 1.f;
		damping = 0.999f;
		position = cyclone::Vector3(0.f, 8.f, 0.f);
		velocity = cyclone::Vector3(5.f, 0.f, 0.f);
		acceleration = cyclone::Vector3(0.f, -9.8f, 0.f);
	}

	void setSize(int siz);
	void setPos(cyclone::Vector3 pos);
	void setVel(cyclone::Vector3 vel);
	cyclone::Vector3 getPos() const;
	cyclone::Vector3 getVel() const;

	void update(float duration);
	void stop();
	void draw(bool is_shadow) const;
	bool checkEdge() const;
};