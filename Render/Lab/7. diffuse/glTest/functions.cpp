#include "functions.h"

glm::mat4 lookAt(const glm::vec3& camPos, const glm::vec3& look, const glm::vec3& up)
{
	glm::mat4 result;
	glm::vec3 xAxis, yAxis, zAxis;

	zAxis = glm::normalize(camPos - look);
	xAxis = glm::normalize(glm::cross(up, zAxis));
	yAxis = glm::normalize(glm::cross(zAxis, xAxis));

	result[0] = glm::vec4(xAxis.x, yAxis.x, zAxis.x, -glm::dot(xAxis, look));
	result[1] = glm::vec4(xAxis.y, yAxis.y, zAxis.y, -glm::dot(yAxis, look));
	result[2] = glm::vec4(xAxis.z, yAxis.z, zAxis.z, -glm::dot(zAxis, look));
	result[3] = glm::vec4(0, 0, 0, 1);

	return result;
}

glm::mat4 perspective(const float& fovY, const float& aspect, const float& near, const float& far)
{
	glm::mat4 result;
	float tanFovYHalf = glm::tan(glm::radians(fovY) / 2.f);
	result[0] = glm::vec4(1.f / (aspect * tanFovYHalf), 0, 0, 0);
	result[1] = glm::vec4(0, 1.f / tanFovYHalf, 0, 0);
	result[2] = glm::vec4(0, 0, -(far + near) / (far - near), -1.f);
	result[3] = glm::vec4(0, 0, -2.f * (far * near) / (far - near), 0);

	return result;
}