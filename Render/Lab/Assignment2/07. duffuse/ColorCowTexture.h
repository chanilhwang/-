#pragma once

#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"
#include <GLFW/glfw3.h>
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

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