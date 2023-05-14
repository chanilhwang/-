#include "Mover.h"

Mover::Mover() : size(1), position(cyclone::Vector3(0.f, 0.f, 0.f)) {}

Mover::Mover(cyclone::Vector3 init_pos) : size(1), position(init_pos) {}

void Mover::update(float duration) {}

void Mover::stop() {}

void Mover::draw(bool is_shadow)
{
	if (is_shadow)
		glColor3f(0.1f, 0.1f, 0.1f);
	else
		glColor3f(1.0f, 0.f, 0.f);

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(size, 30, 30);
}