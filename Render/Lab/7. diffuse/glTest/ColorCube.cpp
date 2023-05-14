#include "ColorCube.h"

void ColorCube::setup()
{

	const float cube_vertices[] = {
		//front
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		//back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f
	};

	const float cube_colors[] = {
		//front
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 0.f,
		1.f, 1.f, 1.f,
		//back
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f,
		1.f, 1.f, 1.f
	};

	const short cube_elements[] = {
		0,1,2,	2,3,0,	1,5,6,
		6,2,1,	7,6,5,	5,4,7,
		4,0,3,	3,7,4,	4,5,1,
		1,0,4,	3,2,6,	6,7,3
	};



	//VAO 생성
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle); // 여기에 작업할거다 activate 의미

	//VBO 생성 - 포지션
	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition); // vbo 이용
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // attrib 번호
		3, // # of data for each vertex
		GL_FLOAT, // data type
		GL_FALSE, // normalized
		0,
		0
	);
	glEnableVertexAttribArray(0);

	//VBO 생성 - 컬러
	glGenBuffers(1, &vboColor);
	glBindBuffer(GL_ARRAY_BUFFER, vboColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	//VBO 큐브
	glGenBuffers(1, &ibo_cube_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);

	glBindVertexArray(0); // VAO를 close
}

ColorCube::ColorCube()
{
	setup();
}

void ColorCube::draw()
{
	glBindVertexArray(vaoHandle);

	//cube
	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}