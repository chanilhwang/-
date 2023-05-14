#pragma once

#include "GL/gl3w.h"

class BunnyTexture
{
private:
	GLuint vaoHandle;
	GLuint ibo_elements;
	GLuint vboModelVertex;

	void setup();

public:
	BunnyTexture();
	void draw();

};