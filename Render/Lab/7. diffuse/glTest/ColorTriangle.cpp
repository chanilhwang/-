#include "ColorTriangle.h"

ColorTriangle::ColorTriangle()
{
	setup();
}

void ColorTriangle::setup()
{
	const float vertexPosition[] = {
	-0.2f, -0.2f, 0.f,
	0.2f, -0.2f, 0.f,
	0.0f, 0.2f, 0.f
	};

	const float vertexColor[] = {
		1.f, 0.f, 0.f,
		0.f, 1.f, 0.f,
		0.f, 0.f, 1.f
	};

	//VAO 생성
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle); // 여기에 작업할거다 activate 의미

	//VBO 생성 - 포지션
	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition); // vbo 이용
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPosition), vertexPosition, GL_STATIC_DRAW);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColor), vertexColor, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // VAO를 close
}

void ColorTriangle::draw()
{
	glBindVertexArray(vaoHandle);

	//normal draw
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
