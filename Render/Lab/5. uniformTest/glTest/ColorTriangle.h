#pragma once

#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"
#include <GLFW/glfw3.h>

class ColorTriangle
{
private:
	GLuint vaoHandle; // VAO handler
	GLuint vboPosition, vboColor; // VBO handler

	void setup();

public:
	ColorTriangle();
	void draw();
};