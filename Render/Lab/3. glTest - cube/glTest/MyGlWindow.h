#pragma once
#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"
#include <GLFW/glfw3.h>

class MyGlWindow
{
public:
	MyGlWindow(int w, int h);
	void draw();
	void setSize(int w, int h);

private:
	int m_width, m_height;

	void setupBuffer();
};