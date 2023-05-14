#include "Mover.h" 



void Mover::update(float duration)
{
	particle->integrate(duration);
}

void Mover::stop() 
{
	particle->setVelocity(cyclone::Vector3(0, 0, 0));
}

void Mover::draw(bool is_shadow) const
{
	cyclone::Vector3 position;
	particle->getPosition(&position);

	if (is_shadow)
		glColor3f(0.01f, 0.01f, 0.01f);
	else
		glColor3f(1.0f, 0.f, 0.f);

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(size, 30, 30);
}

void Mover::setConnection(Mover* a)
{
	if (spring)
		delete spring;
	spring = new cyclone::MySpring(a->particle, 20.f, 3);
}

MoverConnection::MoverConnection()
{
	Mover* mover_1 = new Mover();
	Mover* mover_2 = new Mover();

	mover_1->particle->setPosition(0, 4, 0);
	mover_2->particle->setPosition(0, 7, 0);

	m_movers.push_back(mover_1);
	m_movers.push_back(mover_2);

	m_forces = new cyclone::ParticleForceRegistry();
	m_gravity = new cyclone::ParticleGravity(cyclone::Vector3(0.f, -9.8f, 0.f));

	unsigned int size = m_movers.size();
	for (unsigned int i = 0; i < size; i++)
	{
		m_forces->add(m_movers[i]->particle, m_gravity);
		m_movers[i]->setConnection(m_movers[(i + 1) % size]);
		m_forces->add(m_movers[i]->particle, m_movers[i]->spring);
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
		cyclone::Vector3 p = m_movers[i]->particle->getPosition();
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
}