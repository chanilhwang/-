#include "Mover.h" 



void Mover::update(float duration)
{
	particle->integrate(duration);
	m_forces->updateForces(duration);
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

bool Mover::checkEdge()
{
	bool result = false;

	cyclone::Vector3 position, velocity;
	particle->getPosition(&position);
	particle->getVelocity(&velocity);

	//y = 0 
	if (position.y < size) {
		result = true;
		position.y = size;
		velocity.y *= -1;
	}
	//x = 0, 
	if (position.x < size) {
		result = true;
		position.x = size;
		velocity.x *= -1;
	}
	//x = 100
	else if (position.x > 100 - size) {
		result = true;
		position.x = 100 - size;
		velocity.x *= -1;
	}
	if (result) {
		particle->setPosition(position);
		particle->setVelocity(velocity);
		return result;
	}
	return result;
}