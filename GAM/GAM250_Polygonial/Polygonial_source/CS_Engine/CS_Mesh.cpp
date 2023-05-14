/**
 * @file CS_Mesh.cpp
 * @author Junwoo Seo
 * @email seojunwoo72\@gmail.com
 * @brief Mesh management.
 * @version 0.1
 * @date 2018-12-14
 *
 * @copyright Copyright (c) 2018
 *
 */
#include "CS_Mesh.hpp"
#include <GL/glew.h>
int CS_Mesh::GetPointCount() const
{
	return int(Points.size());
}

glm::vec2 CS_Mesh::GetPoint(int index) const
{
	auto&point = Points[index];
	return {point.x,point.y};
}

void CS_Mesh::SetPoint(int index, glm::vec2 Point)
{
	Points[index] = { Point.x,Point.y,1.f };
}

glm::vec2 CS_Mesh::GetTextureCoordinate(int index) const
{
	auto&tex = TextureCoordinates[index];
	return {tex.x,tex.y};
}

void CS_Mesh::SetTextureCoordinate(int index, glm::vec2 textureCoordinate)
{
	TextureCoordinates[index] = { textureCoordinate.x,textureCoordinate.y,1.f };
}

CS_Color CS_Mesh::GetColor(int index) const
{
	return Colors.at(index);
}

void CS_Mesh::ComputeMatrix(glm::vec2 x_y, float Width, float Height)
{
	point_to_tex_coord[0].x = Width;
	point_to_tex_coord[1].y = -Height;
	point_to_tex_coord[2].x = x_y.x + Width / 2.f;
	point_to_tex_coord[2].y = x_y.y + Height / 2.f;
}

void CS_Mesh::Update()
{
	int size = int(Points.size());
	if(size!=0)
	{
		min = Points[0];
		max = Points[0];
		TextureCoordinates.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			TextureCoordinates[i] = point_to_tex_coord * Points[i];
			UpdateMinMax(Points[i]);
		}
	}
}


int CS_Mesh::GetDrawType() const
{
    switch (Type)
    {
    case CS_DrawType::Triangles:
        return GL_TRIANGLES;
    case CS_DrawType::TriangleFan:
        return GL_TRIANGLE_FAN;
    case CS_DrawType::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    }
    //just in case
    return GL_TRIANGLE_STRIP;
}

void CS_Mesh::SetDrawType(CS_DrawType type)
{
	Type = type;
}

void CS_Mesh::AddColor(CS_Color color)
{
	Colors.push_back(color);
}

void CS_Mesh::SetColor(int index, CS_Color color)
{
	Colors.at(index) = color;
}

void CS_Mesh::AddPoint(glm::vec2 point)
{
	Points.push_back({ point.x,point.y,1.f });
        TextureCoordinates.push_back(point_to_tex_coord*glm::vec3{ point.x,point.y,1.f });
}

void CS_Mesh::ClearColors()
{
	Colors.clear();
}

bool CS_Mesh::HasMoreThanOneColor() const
{
	return Colors.size()>1;
}

void CS_Mesh::ClearPoints()
{
	Points.clear();
        TextureCoordinates.clear();
}

void CS_Mesh::Clear()
{
	Points.clear();
	TextureCoordinates.clear();
	Colors.clear();
}

glm::vec2 CS_Mesh::GetMin() const
{
	return min;
}

glm::vec2 CS_Mesh::GetMax() const
{
	return max;
}

void CS_Mesh::UpdateMinMax(const glm::vec3& point)
{
	if (min.x>point.x)
	{
		min.x = point.x;
	}
	if (max.x<point.x)
	{
		max.x = point.x;
	}
	if (min.y>point.y)
	{
		min.y = point.y;
	}
	if (max.y<point.y)
	{
		max.y = point.y;
	}
}

CS_Mesh CreateMesh::CreateCircle(float radius, CS_Color color, int pointCount)
{
    CS_Mesh mesh;
    mesh.Clear();
    mesh.SetDrawType(CS_DrawType::TriangleFan);
    float pi = (float)std::acos(-1);
    float angle = (2 * pi) / pointCount;
    float cos = (float)std::cos(angle);
    float sin = (float)std::sin(angle);

    float temp_X = radius * cos;
    float temp_Y = radius * sin;
    glm::vec2 temp = { temp_X,temp_Y };
    glm::vec2 first = { temp_X,temp_Y };
    mesh.AddPoint({ 0,0 });
    mesh.AddColor(color);
    mesh.AddPoint(temp);
    for (int i = 0; i<pointCount; ++i)
    {
        temp_X = temp.x*cos - temp.y*sin;
        temp_Y = temp.x*sin + temp.y*cos;
        temp = { temp_X,temp_Y };
        mesh.AddPoint(temp);
    }
    mesh.AddPoint(first);
    return mesh;
}
