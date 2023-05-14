
#include <iostream>
#include "Mover.h"
#include "gl/glut.h"
#include "timing.h"

#include "pfgen.h"



Mover::Mover()
{
	m_particle = new cyclone::Particle();

	m_particle->setPosition(0, 0, 0);
	m_particle->setVelocity(0, 0, 0);
	m_particle->setMass(10.0f);
	m_particle->setDamping(0.9);
	m_particle->setAcceleration(0, 0, 0);
}

Mover::Mover(cyclone::Vector3 position)
{
	m_particle = new cyclone::Particle();

	m_particle->setPosition(position);
	m_particle->setVelocity(0, 0, 0);
	m_particle->setMass(10.0f);
	m_particle->setDamping(0.9);
	m_particle->setAcceleration(0, 0, 0);
}

Mover::~Mover()
{
}

float size = 0.5;

void Mover::draw(int shadow)
{
	
	cyclone::Vector3 position;
	m_particle->getPosition(&position);

	if (shadow)
		glColor4f(0.1f, 0.1f, 0.1f,0.7f);
	else {
		glLoadName(1);
		glColor3f(1.0f, 0.0f, 0.0f);
	}

	glPushMatrix();

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(size, 30, 30);

	glPopMatrix();

}