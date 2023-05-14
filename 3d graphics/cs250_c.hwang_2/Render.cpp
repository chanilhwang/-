/*
Render.cpp
-Name : Hwang Chan IL
-Assignment Number : 2
-Course Name : CS250
-term : Spring 2019
*/
#include "Render.h"
#include <stdexcept>

namespace {
	GLuint VBO;
}

Render::Render(void)
{
	//compile and link shaders
	const char* vertex_Shader_Text = 
		"#version 130\n\
		attribute vec4 position;\
		void main() {\
			gl_Position = position;\
		}";
	const char* fragment_Shader_Text =
		"#version 130\n\
		uniform vec3 color;\
		out vec4 frag_color;\
		void main(void) {\
			frag_color = vec4(color, 1);\
		}";
	int success;

	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_Shader_Text, 0);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glDeleteShader(vertexShader);
		throw std::runtime_error("ERROR : Vertex Shader Compilation");
	}

	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_Shader_Text, 0);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		throw std::runtime_error("ERROR : Fragment Shader Compilation");
	}

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		glUseProgram(0);
		throw std::runtime_error("ERROR : Program Linking");
	}

	aposition = glGetAttribLocation(program, "position");
	ucolor = glGetUniformLocation(program, "color");

	glEnable(GL_DEPTH_TEST);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGenBuffers(1, &VBO);
}

Render::~Render(void)
{
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(program);
	glUseProgram(0);
}

void Render::ClearBuffers(const Vector & c)
{
	glClearColor(c.x, c.y, c.z, c.w);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::SetColor(const Vector & c)
{
	glUseProgram(program);
	glUniform3f(ucolor, c.x, c.y, c.z);
	glUseProgram(0);
}

void Render::DrawLine(const Hcoord & P, const Hcoord & Q)
{
	Hcoord vertices[2] = { P, Q };

	glUseProgram(program);
	glEnableVertexAttribArray(aposition);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(aposition, 4, GL_FLOAT, false, sizeof(Hcoord), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_LINES, 0, 2);
	glUseProgram(0);
}

void Render::FillTriangle(const Hcoord & P, const Hcoord & Q, const Hcoord & R)
{
	Hcoord vertices[3] = { P, Q, R };

	glUseProgram(program);
	glEnableVertexAttribArray(aposition);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(aposition, 4, GL_FLOAT, GL_FALSE, sizeof(Hcoord), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);
}