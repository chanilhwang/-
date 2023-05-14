/*
Clip.cpp
-Name : Hwang Chan IL
-Assignment Number : 7
-Course Name : CS250
-term : Spring 2019
*/
#include "Clip.h"

void push_intersect(const Hcoord& P, const Hcoord& Q,
	float dot_half_P, float dot_half_Q,
	std::vector<Hcoord>& vert_vec)
{
	float t = dot_half_P / (dot_half_P - dot_half_Q);
	Hcoord new_point = P + t * (Q - P);
	vert_vec.push_back(new_point);
}

bool Clip::operator()(std::vector<Hcoord>& vertices)
{
	bool result = false;

	int half_space_size = static_cast<int>(half_spaces.size());
	for (int j = 0; j < half_space_size; j++)
	{
		temp_vertices.clear();
		const HalfSpace& half_space = half_spaces[j];
		int vertices_size = static_cast<int>(vertices.size());
		for (int i = 0; i < vertices_size; i++)
		{
			const Hcoord& P = vertices[i % vertices_size];
			const Hcoord& Q = vertices[(i + 1) % vertices_size];

			float dot_half_P = dot(half_space, P);
			float dot_half_Q = dot(half_space, Q);

			if (dot_half_P > 0 && dot_half_Q <= 0)
			{
				push_intersect(P, Q, dot_half_P, dot_half_Q, temp_vertices);
				result = true;
			}
			else if (dot_half_P <= 0 && dot_half_Q <= 0)
			{
				temp_vertices.push_back(P);
				result = true;
			}
			else if (dot_half_P <= 0 && dot_half_Q > 0)
			{
				temp_vertices.push_back(P);
				push_intersect(P, Q, dot_half_P, dot_half_Q, temp_vertices);
				result = true;
			}
		}
		vertices = temp_vertices;
	}

	return result;
}
