#include "Mover.h"

void Mover::setSize(int siz)
{
	this->size = siz;
}

void Mover::setPos(cyclone::Vector3 pos)
{
	position = pos;
}

void Mover::setVel(cyclone::Vector3 vel)
{
	velocity = vel;
}

cyclone::Vector3 Mover::getPos() const
{
	return position;
}

cyclone::Vector3 Mover::getVel() const
{
	return velocity;
}

void Mover::update(float duration) 
{
	if (inverse_mass < 0.f) 
		return;
	cyclone::Vector3 force = cyclone::Vector3(0, 0, 0);
	cyclone::Vector3 acc = acceleration;
	acc += force * inverse_mass;
	velocity += acc * duration;
	velocity *= pow(damping, duration);
	position += velocity * duration;

	if (checkEdge()) {
		position.y = size;
		velocity.y *= -1;
	}
}

void Mover::stop() 
{
	velocity = cyclone::Vector3(0, 0, 0);
}

void Mover::draw(bool is_shadow) const
{
	if (is_shadow)
		glColor3f(0.1f, 0.1f, 0.1f);
	else
		glColor3f(1.0f, 0.f, 0.f);

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(size, 30, 30);
}

bool Mover::checkEdge() const
{
	//y=0에 벽이 있다고 가정
	if (position.y < size)
		return true;
	return false;
}