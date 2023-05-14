
#include <iostream>
#include "Mover.h"
#include "gl/glut.h"
#include "timing.h"


Mover::Mover()
{
	m_particle = new cyclone::Particle();

	m_particle->setPosition(5, 5, 5);
	m_particle->setVelocity(0, 0, 0);
	m_particle->setMass(5.0f);
	m_particle->setDamping(0.7f);

	m_bouyancy = new cyclone::MyBouyancy(3, 1, 10, 2);

	m_forces = new cyclone::ParticleForceRegistry();

	m_forces->add(m_particle, m_bouyancy);
}

Mover::~Mover()
{
}

float size = 3.0f;
void Mover::update(float duration)
{
	m_forces->updateForces(duration);
	m_particle->integrate(duration);
	
	checkEdges();
}

void Mover::checkEdges()
{
	cyclone::Vector3  pos;
	m_particle->getPosition(&pos);
	cyclone::Vector3  vel;
	m_particle->getVelocity(&vel);
	
	if (pos.x + size > 100) {
		m_particle->setPosition(cyclone::Vector3(100 - size, pos.y, pos.z));
		m_particle->setVelocity(cyclone::Vector3(vel.x * -1.0, vel.y, vel.z));
	}
	else if (pos.x - size < -100) {
		m_particle->setPosition(cyclone::Vector3(-100 + size, pos.y, pos.z));
		m_particle->setVelocity(cyclone::Vector3(vel.x * -1.0, vel.y, vel.z));
	}

	if (pos.z + size > 100) {
		m_particle->setPosition(cyclone::Vector3(pos.x, pos.y, 100 - size));
		m_particle->setVelocity(cyclone::Vector3(vel.x, vel.y, vel.z * -1.0));
	}
	else if (pos.z - size < -100) {
		m_particle->setPosition(cyclone::Vector3(pos.x, pos.y, -100 + size));
		m_particle->setVelocity(cyclone::Vector3(vel.x, vel.y, vel.z * -1.0));
	}

	if (pos.y - size < 0) {
		m_particle->setPosition(cyclone::Vector3(pos.x, size, pos.z));
		m_particle->setVelocity(cyclone::Vector3(vel.x, -1.0 * vel.y, vel.z));
	}
}

void Mover::draw(int shadow)
{
	cyclone::Vector3 pos;
	m_particle->getPosition(&pos);

	if (shadow)
		glColor4f(0.1,0.1,0.1,0.5f);
	else {
		glLoadName(1);
		glColor3f(1.0f, 0.0f, 0.0f);
	}

	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(size, 30, 30);

	glPopMatrix();
}