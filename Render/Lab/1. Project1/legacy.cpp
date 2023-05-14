/*#include <iostream>
#include <glm/glm.hpp>

#include "cow.h"
#include "easyppm.h"

float edgeFunction(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& p)
{
	return (p.x - v0.x) * (v1.y - v0.y) - (p.y - v0.y) * (v1.x - v0.x);
}

//screnn to raster with (-1, 1)NDC
glm::vec2 screen2NDC(const glm::vec2& screen)
{
	glm::vec2 result;
	result.x = (screen.x + 1) / 2;
	result.y = (1 - screen.y) / 2;
	return result;
}

glm::vec3 NDC2raster(const glm::vec2& NDC, int width, int height, float z)
{
	glm::vec3 result;
	result.x = (int)(NDC.x * width);
	result.y = (int)(NDC.y * height);
	result.z = -z; // no cam point

	if (result.x == width)
		result.x = width - 1;
	if (result.y == height)
		result.y = height - 1;
	return result;
}

glm::vec3 screen2NDC(const glm::vec2 screen, int width, int height, float z = 1)
{
	return NDC2raster(screen2NDC(screen), width, height, z);
}

int main()
{
	int width = 100, height = 100;

	//screen space
	glm::vec2 screen_p[3] = { glm::vec2(-0.3, -0.6), glm::vec2(0, 0.7), glm::vec2(0.3, -0.6) };
	float z[3] = { -10, -2, -10 };

	//screen to raster
	glm::vec3 raster_p[3];
	for (int i = 0; i < 3; i++)
		raster_p[i] = screen2NDC(screen_p[i], width, height, z[i]);

	//testing point on raster space
	glm::vec2 p1(0, 0);
	glm::vec3 raster_p1 = screen2NDC(p1, width, height);

	//test1
	bool inside = true;
	float triangle[3];
	for (int i = 0; i < 3; i++)
	{
		triangle[i] = edgeFunction(raster_p[(i + 2) % 3], raster_p[(i + 1) % 3], raster_p1);
		if (triangle[i] < 0) {
			inside = false;
			break;
		}
	}
	if (inside) {
		float triangle_v0v1v2 = edgeFunction(raster_p[0], raster_p[2], raster_p[1]);
		float z = 0;
		for (int i = 0; i < 3; i++)
		{
			z += triangle[i] / triangle_v0v1v2 * (1 / raster_p[i].z);
		}
		raster_p1.z = 1 / z;
	}
	std::cout << "p1.z = " << raster_p1.z << std::endl;

	return 0;
} */