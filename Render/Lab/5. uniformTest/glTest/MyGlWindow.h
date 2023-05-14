#pragma once

#include "ColorTriangle.h"

#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class MyGlWindow
{
public:
	MyGlWindow(int w, int h);
	void draw();
	void setSize(int w, int h);

	ColorTriangle* m_triangle;

private:
	int m_width, m_height;

	void setupBuffer();
};