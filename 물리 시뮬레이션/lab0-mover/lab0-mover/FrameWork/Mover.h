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
	cyclone::Vector3 position;

public:
	Mover();
	Mover(cyclone::Vector3 init_pos);

	void update(float duration);
	void stop();
	void draw(bool is_shadow);
};