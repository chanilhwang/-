#pragma once
#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"

class ColorCube
{
private:
	GLuint vaoHandle; // VAO handler
	GLuint ibo_cube_elements;
	GLuint vboPosition, vboColor; // VBO handler

	void setup();
public:
	ColorCube();
	void draw();
};