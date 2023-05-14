#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

#include "cow.h"
#include "easyppm.h"

//to compute lrbt
void computScreenCoord(const float& fovy, const int& width, const int& height, const float& near, float& t, float& b, float& l, float& r)
{
	float ratio = (float)width / height;
	glm::vec2 canvasSize(0);
	canvasSize.y = 2 * glm::tan(glm::radians(fovy * 0.5)) * near;

	t = canvasSize.y / 2;
	b = -t;

	canvasSize.x = canvasSize.y * ratio;

	r = canvasSize.x / 2;
	l = -r;
}

//to convert world to cam
void convertToCam(
	const glm::vec3& vertexWorld,
	const glm::mat4& worldToCamera,
	glm::vec3& vectexCam
)
{
	vectexCam = glm::vec3(worldToCamera * glm::vec4(vertexWorld, 1));
}

//to convert cam to raster
void convertToRaster(
	const glm::vec3& vertexCam,
	const float& l,
	const float& r,
	const float& t,
	const float& b,
	const float& near,
	const uint32_t& imageWidth,
	const uint32_t& imageHeight,
	glm::vec3& vertexRaster
)
{
	glm::vec3 result = vertexCam;
	
	//cam to screen
	result.x = near * result.x / -result.z;
	result.y = near * result.y / -result.z;

	//screen to NDC
	result.x = 2 * result.x / (r - l) - (r + l) / (r - l);
	result.y = 2 * result.y / (t - b) - (t + b) / (t - b);

	//NDC to raster
	result.x = (result.x + 1) / 2 * imageWidth;
	result.y = (1 - result.y) / 2 * imageHeight;
	result.z = -result.z;

	vertexRaster = result;
}

float min3(const float& a, const float& b, const float& c)
{
	return std::min(a, std::min(b, c));
}

float max3(const float& a, const float& b, const float& c)
{
	return std::max(a, std::max(b, c));
}

float edgeFunction(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

int main()
{
	//initialization
	const uint32_t imageWidth = 640, imageHeight = 480;

	const glm::mat4 worldToCamera = glm::lookAt(glm::vec3(-20, 10, -20), glm::vec3(0, 5, 0), glm::vec3(0, 1, 0));
	
	const uint32_t ntris = 3156;

	const float nearClippingPlane = 1;
	const float farClippingPlane = 1000;

	float fovy = 60;
	float l, r, b, t;

	//compute lrbt
	computScreenCoord(fovy, imageWidth, imageHeight, nearClippingPlane, t, b, l, r);

	//create buffer
	struct rgb {
		unsigned char r, g, b;
	};
	std::vector<rgb> frameBuffer;
	frameBuffer.resize(imageWidth * imageHeight);

	for (uint32_t i = 0; i < imageWidth * imageHeight; i++)
	{
		frameBuffer[i].r = 0;
		frameBuffer[i].g = 0;
		frameBuffer[i].b = 0;
	}

	float* depthBuffer = new float[imageWidth * imageHeight];

	for (uint32_t i = 0; i < imageWidth * imageHeight; i++)
		depthBuffer[i] = farClippingPlane;

	//loop
	for (uint32_t i = 0; i < ntris; i++)
	{
		//world vertices
		const glm::vec3& v0 = vertices[nvertices[i * 3]];
		const glm::vec3& v1 = vertices[nvertices[i * 3 + 1]];
		const glm::vec3& v2 = vertices[nvertices[i * 3 + 2]];

		//camera vertices
		glm::vec3 v0Cam, v1Cam, v2Cam;

		//raster vertices
		glm::vec3 v0Raster, v1Raster, v2Raster;

		//convert world to cam
		convertToCam(v0, worldToCamera, v0Cam);
		convertToCam(v1, worldToCamera, v1Cam);
		convertToCam(v2, worldToCamera, v2Cam);

		//convert cam to raster
		convertToRaster(v0Cam, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v0Raster);
		convertToRaster(v1Cam, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v1Raster);
		convertToRaster(v2Cam, l, r, t, b, nearClippingPlane, imageWidth, imageHeight, v2Raster);

		//bounding box
		float xmin = min3(v0Raster.x, v1Raster.x, v2Raster.x);
		float xmax = max3(v0Raster.x, v1Raster.x, v2Raster.x);
		float ymin = min3(v0Raster.y, v1Raster.y, v2Raster.y);
		float ymax = max3(v0Raster.y, v1Raster.y, v2Raster.y);

		if (xmin > imageWidth - 1 || xmax < 0 || ymin > imageHeight - 1 || ymax < 0) continue;

		uint32_t x_min = std::max(int32_t(0), (int32_t)(std::floor(xmin)));
		uint32_t x_max = std::min(int32_t(imageWidth - 1), (int32_t)(std::floor(xmax)));
		uint32_t y_min = std::max(int32_t(0), (int32_t)(std::floor(ymin)));
		uint32_t y_max = std::min(int32_t(imageHeight - 1), (int32_t)(std::floor(ymax)));

		//compute area of triangle * 2
		float area = edgeFunction(v0Raster, v1Raster, v2Raster);

		for (uint32_t y = y_min; y <= y_max; y++)
		{
			for (uint32_t x = x_min; x <= x_max; x++)
			{
				//center of pixel
				glm::vec3 pixelSample(x + 0.5, y + 0.5, 0);
				
				//compute area of w
				float w0 = edgeFunction(v1Raster, v2Raster, pixelSample);
				float w1 = edgeFunction(v2Raster, v0Raster, pixelSample);
				float w2 = edgeFunction(v0Raster, v1Raster, pixelSample);

				if (w0 >= 0 && w1 >= 0 && w2 >= 0)
				{
					//r
					float r0 = w0 / area;
					float r1 = w1 / area;
					float r2 = w2 / area;

					//z
					pixelSample.z = 1 / (r0 / v0Raster.z + r1 / v1Raster.z + r2 / v2Raster.z);
					if (pixelSample.z < depthBuffer[y * imageWidth + x]) 
					{
						//update depth buffer
						depthBuffer[y * imageWidth + x] = pixelSample.z;

						//shading
						float px = (v0Cam.x / -v0Cam.z) * r0 + (v1Cam.x / -v1Cam.z) * r1 + (v2Cam.x / -v2Cam.z) * r2;
						float py = (v0Cam.y / -v0Cam.z) * r0 + (v1Cam.y / -v1Cam.z) * r1 + (v2Cam.y / -v2Cam.z) * r2;

						glm::vec3 pt(px * pixelSample.z, py * pixelSample.z, -pixelSample.z);

						glm::vec3 n = glm::cross((v1Cam - v0Cam), (v2Cam - v0Cam));
						n = glm::normalize(n);

						glm::vec3 L = -pt;
						L = glm::normalize(L);

						float nDotL = std::max(0.f, glm::dot(n, L));

						glm::vec3 diffuse = glm::vec3(1, 1, 1) * glm::vec3(1, 1, 0) * nDotL;

						frameBuffer[y * imageWidth + x].r = diffuse.r * 255;
						frameBuffer[y * imageWidth + x].g = diffuse.g * 255;
						frameBuffer[y * imageWidth + x].b = diffuse.b * 255;
					}
				}
			}
		}
	}

	PPM ppm = easyppm_create(imageWidth, imageHeight, IMAGETYPE_PPM);
	ppm.image = (unsigned char*) frameBuffer.data();

	easyppm_write(&ppm, "test.ppm");

	return 0;
}