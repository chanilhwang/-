/**
 * @file CS_Mesh.hpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Mesh management.
 * @version 0.1
 * @date 2018-12-14
 * 
 * @copyright Copyright (c) 2018
 * 
 */
#pragma once
#include <vector>
#include <glm/vec2.hpp>
#include "CS_Color.hpp"
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

enum class CS_DrawType
{
	Lines,
	LineStrip,
	Triangles,
	TriangleStrip,
	TriangleFan
};
class CS_Mesh
{
public:
	int GetPointCount() const;
	glm::vec2 GetPoint(int index) const;
	void SetPoint(int index, glm::vec2 Point);
	glm::vec2 GetTextureCoordinate(int index) const;
	void SetTextureCoordinate(int index, glm::vec2 textureCoordinate);
	CS_Color GetColor(int index) const;
	
	void ComputeMatrix(glm::vec2 x_y, float Width, float Height);
	void Update();
	int GetDrawType() const;
	void SetDrawType(CS_DrawType type);

	void AddColor(CS_Color color);
	void SetColor(int index, CS_Color color);
	void AddPoint(glm::vec2 point);

	void ClearColors();
	bool HasMoreThanOneColor() const;

	void ClearPoints();
	void Clear();

	glm::vec2 GetMin() const;
	glm::vec2 GetMax() const;

private:
	void UpdateMinMax(const glm::vec3& point);
	glm::vec2 min{-.5f,-.5f};
	glm::vec2 max{.5f,.5f}; 
	std::vector<glm::vec3> Points{{-.5f,.5f,1.f},{.5f,.5f,1.f },
								  {-.5f,-.5f,1.f },{.5f,-.5f,1.f }};
	std::vector<glm::vec3> TextureCoordinates{{0.f,0.f,1.f },{1.f,0.f,1.f },
											  {0.f,1.f,1.f },{1.f,1.f,1.f }};
	glm::mat3 point_to_tex_coord{ 1,0,0,0,-1,0,0.5f,0.5f, 1.f };
	CS_DrawType Type = CS_DrawType::TriangleStrip;
	std::vector<CS_Color> Colors{{255,255,255}};
};

namespace CreateMesh
{
    CS_Mesh CreateCircle(float radius = 0.5f, CS_Color color = { 255, 255, 255 }, int pointCount = 30);
    //CS_Mesh CreateWireCircle(float radius = 0.5f, CS_Color color = { 255, 255, 255 }, int pointCount = 30);
    //CS_Mesh CreateBox(float dimension = 1, CS_Color color = { 255, 255, 255 });
    //CS_Mesh CreateWireBox(float dimension = 1, CS_Color color = { 255, 255, 255 });
    //CS_Mesh CreateLine(glm::vec2 a = { 0, 0 }, glm::vec2 b = { 1, 0 }, CS_Color color = { 255, 255, 255 });
}