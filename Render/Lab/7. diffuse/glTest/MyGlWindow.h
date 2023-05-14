#pragma once

#include "ColorCowTexture.h"
#include "Loader.h"
#include "Viewer.h"

class MyGlWindow
{
public:
	MyGlWindow(int w, int h);
	void draw();
	void setSize(int w, int h);
	void setAspect(float a);

	ColorCowTexture* m_cow;
	Viewer* m_viewer;

private:
	int m_width, m_height;
	float m_aspect;
	ShaderProgram* shaderProgram;

	void Initilize();
};