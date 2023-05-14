#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Fire.h"

Fire::Fire() : type(FIRE_TYPE::FIRE_CHILD), size(1), age(1), rule(nullptr),
color(Vector3(255, 0, 0))
{
	particle = new Particle();
	particle->setAcceleration(Vector3::GRAVITY);
	particle->setMass(1.f);
	particle->setDamping(0.99f);
}

Fire::~Fire()
{
	delete particle;
}

bool Fire::update(float duration)
{
	if (age <= 0 || checkGround()) {
		return true;
	}

	particle->integrate(duration);
	age -= duration;
	putHistory();

	return false;
}

void Fire::draw(bool isShadow)
{
	Vector3 pos;
	particle->getPosition(&pos);

	if (isShadow)
		glColor3f(0.1f, 0.1f, 0.1f);
	else
		glColor3f(color.x, color.y, color.z);

	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(size, 30, 30);

	glPopMatrix();
}

void Fire::drawHistory()
{
	glLineWidth(2.f);
	glColor3f(color.x, color.y, color.z);
	glPushMatrix();

	glBegin(GL_LINE_STRIP);
	for (unsigned int i = 0; i < history.size(); i += 2)
	{
		Vector3& pos = history[i];
		glVertex3f(pos.x, pos.y, pos.z);
	}
	glEnd();
	glPopMatrix();
	glLineWidth(1.f);
}

void Fire::putHistory()
{
	Vector3 pos;
	particle->getPosition(&pos);

	if (history.size() >= 15)
		history.pop_front();

	history.push_back(pos);
}

void Fire::clearHistory()
{
	history.clear();
}

bool Fire::checkGround() const
{
	cyclone::Vector3 position;
	particle->getPosition(&position);

	if (position.y < size)
		return true;
	return false;
}

void Fire::setRule(FireworksRule* r)
{
	rule = r;
}

void Fire::setParameter(float size, float age, Vector3 position,
	Vector3 velocity, Vector3 color, real damping)
{
	Fire::size = size;
	Fire::age = age;
	particle->setPosition(position);
	particle->setVelocity(velocity);
	Fire::color = color;
	particle->setDamping(damping);
}