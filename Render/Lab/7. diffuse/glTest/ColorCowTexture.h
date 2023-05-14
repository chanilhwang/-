#pragma once

#include "GL/gl3w.h"

class ColorCowTexture
{
private:
	GLuint vaoHandle;
	GLuint ibo_cow_elements;
	GLuint vboPosition, vboNormal;

	void setup();

public:
	ColorCowTexture();
	void draw();

};