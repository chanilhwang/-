#pragma once

#include "core.h"
#include "particle.h"
#include "pfgen.h"


#include "MySpring.h"

class Mover
{
public:
	Mover() {}
	Mover(cyclone::Vector3 &p);
	~Mover();

	cyclone::Quaternion m_orientation;
	cyclone::Vector3 m_angular_velocity;
	cyclone::Vector3 m_angular_acceleration;
	double m_angular_damping;


	cyclone::Matrix4 transformMatrix;

	cyclone::Particle * m_particle;

	void update(float duration);
	void draw(int shadow);
	void setOrientation(cyclone::Quaternion q);
	static cyclone::Vector3 lerp(const Mover* my, const Mover* other, float u)
	{	
		return my->m_particle->getPosition() * (1 - u) + other->m_particle->getPosition() * u;
	}

	void getGLTransform(float matrix[16]);
};


class MoverConnection
{
public:
	MoverConnection();
	~MoverConnection();

	std::vector<Mover *> m_movers;
	void update(float duration);
	void draw(int shadow);

};
