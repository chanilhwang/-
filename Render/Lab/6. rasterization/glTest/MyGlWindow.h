#pragma once

#include "ColorTriangle.h"
#include "ColorCube.h"
#include "Viewer.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGlWindow
{
public:
	MyGlWindow(int w, int h);
	void draw();
	void setSize(int w, int h);
	void setAspect(float a);

	ColorTriangle* m_triangle;
	ColorCube* m_cube;
	Viewer* m_viewer;

private:
	int m_width, m_height;
	float m_aspect;

	void Initilize();
};