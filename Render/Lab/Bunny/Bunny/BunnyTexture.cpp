#include "BunnyTexture.h"
#include "bunny.h"

void BunnyTexture::setup()
{
	//VAO
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//VBO - position
	glGenBuffers(1, &vboModelVertex);
	glBindBuffer(GL_ARRAY_BUFFER, vboModelVertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(modelVertices), modelVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelVertex),
		0
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ModelVertex),
		(GLvoid*)(3 * sizeof(GL_FLOAT))
	);
	glEnableVertexAttribArray(1);

	//ibo
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

BunnyTexture::BunnyTexture()
{
	setup();
}

void BunnyTexture::draw()
{
	glBindVertexArray(vaoHandle);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, 0);
}
