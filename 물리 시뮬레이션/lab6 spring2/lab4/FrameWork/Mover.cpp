
#include <iostream>
#include "Mover.h"
#include "gl/glut.h"
#include "timing.h"


Mover::Mover()
{
	m_particle = new cyclone::Particle();

	m_particle->setPosition(5, 20, 0);
	m_particle->setVelocity(0, 0, 0);
	m_particle->setMass(10.0f);
	m_particle->setDamping(0.9f);	
}

Mover::~Mover()
{
	delete m_particle;
}

float size = 2.0f;
void Mover::update(float duration)
{
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
		glColor3f(1.0f, 0.0f, 0.0f);
	}

	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(size, 30, 30);

	glPopMatrix();
}

void Mover::setConnection(Mover* a)
{
	/*
	if (m_spring)
		delete m_spring;
	m_spring = new cyclone::MySpring(a->m_particle, 20.f, 3);
	*/
}

MoverConnection::MoverConnection()
{
	Mover* mover = new Mover();
	mover->m_particle->setPosition(0, 4, 0);
	mover->m_particle->setMass(1.0);
	mover->m_particle->setDamping(0.7);

	m_movers.push_back(mover);
	
	m_forces = new cyclone::ParticleForceRegistry();
	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0.f, -9.8f, 0.f));
	
	unsigned int size = m_movers.size();
	for (unsigned int i = 0; i < size; i++)
	{
		m_forces->add(m_movers[i]->m_particle, m_gravity);
		m_movers[i]->setConnection(m_movers[(i + 1) % size]);
		//m_forces->add(m_movers[i]->m_particle, m_movers[i]->m_spring);
	}
	
}

MoverConnection::~MoverConnection()
{
	delete m_forces;
	delete m_gravity;
	for (unsigned int i = 0; i < m_movers.size(); i++)
	{
		delete m_movers[i];
	}
	m_movers.clear();
}

void MoverConnection::update(float duration)
{
	int size = m_movers.size();
	for (int i = 0; i < size; i++)
	{
		m_movers[i]->update(duration);
	}
	m_forces->updateForces(duration);
}

void MoverConnection::draw(int shadow)
{
	unsigned int size = m_movers.size();
	for (unsigned int i = 0; i < size; i++)
	{
		if (!shadow)
			glLoadName(i + 1);
		m_movers[i]->draw(shadow);
	}
	
	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < size; i++)
	{
		cyclone::Vector3 p = m_movers[i]->m_particle->getPosition();
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
}

AnchorConnection::AnchorConnection()
{
	Mover* mover = new Mover();
	cyclone::Particle* p = mover->m_particle;
	p->setDamping(0.7);
	p->setPosition(cyclone::Vector3(0, 4, 0));
	p->setMass(1.0);

	m_movers.push_back(mover);

	m_forces = new cyclone::ParticleForceRegistry();
	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0.f, -9.8f, 0.f));

	cyclone::Vector3* anchor = new cyclone::Vector3(5, 15, 5);
	m_spring = new cyclone::MyAnchoredSpring(anchor, 5, 3);

	m_forces->add(p, m_gravity);
	m_forces->add(p, m_spring);
}

AnchorConnection::~AnchorConnection()
{
	delete m_gravity;
	delete m_spring;
	delete m_forces;
	for (unsigned int i = 0; i < m_movers.size(); i++)
	{
		delete m_movers[i];
	}
}

void AnchorConnection::update(float duration)
{
	for (unsigned int i = 0; i < m_movers.size(); i++)
	{
		m_movers[i]->update(duration);
	}
	
	m_forces->updateForces(duration);
}

void AnchorConnection::draw(int shadow)
{
	for (unsigned int i = 0; i < m_movers.size(); i++)
	{
		if (!shadow)
			glLoadName(i + 1);
		m_movers[i]->draw(shadow);
	}
	if (shadow)
		return;
	glColor3f(0, 0, 0);
	glLineWidth(3.0f);
	glPushMatrix();
	glBegin(GL_LINES);

	const cyclone::Vector3* anchor = m_spring->getAnchor();
	for (unsigned int i = 0; i < m_movers.size(); i++)
	{
		cyclone::Vector3 p = m_movers[i]->m_particle->getPosition();

		glVertex3f(anchor->x, anchor->y, anchor->z);
		glVertex3f(p.x, p.y, p.z);
	}

	glColor3f(1, 1, 0);
	glVertex3f(anchor->x, anchor->y, anchor->z);
	glVertex3f(anchor->x, 0, anchor->z);

	glEnd();
	glPopMatrix();
}
