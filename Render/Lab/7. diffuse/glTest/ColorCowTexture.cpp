#include "ColorCowTexture.h"
#include "cow.h"
#include <vector>

void ColorCowTexture::setup()
{
	//calc normal vectors
	std::vector<glm::vec3> normals;
	normals.resize(1732);

	for (unsigned int i = 0; i < 1732; i++)
	{
		const glm::vec3& v0 = vertices[nvertices[i * 3]];
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
		const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];

		glm::vec3 n = glm::normalize(glm::cross((v1 - v0), (v2 - v0)));

		normals[nvertices[i * 3]] = n;
		normals[nvertices[i * 3 + 1]] = n;
		normals[nvertices[i * 3 + 2]] = n;
	}

	//VAO
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	//VBO - position
	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(0);

	//VBO - normals
	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		0
	);
	glEnableVertexAttribArray(1);

	//ibo
	glGenBuffers(1, &ibo_cow_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cow_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(nvertices), nvertices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

ColorCowTexture::ColorCowTexture()
{
	setup();
}

void ColorCowTexture::draw()
{
	glBindVertexArray(vaoHandle);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(uint32_t), GL_UNSIGNED_INT, 0);
}
