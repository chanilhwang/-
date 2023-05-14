/*
Interpolate.cpp
-Name : Hwang Chan IL
-Assignment Number : 8
-Course Name : CS250
-term : Spring 2019
*/
#include "Interpolate.h"

float TexturedCoord::color_scale;
Texture* TexturedCoord::texture;

template<typename T1>
struct AABB {
	T1 min_x,
		max_x,
		min_y,
		max_y;
};

struct Barycentric_Interpolants {
	float lambda,
		mu,
		nu;
};

float FindMinValue(float P, float Q, float R) {
	float result = P;
	if (result > Q)
		result = Q;
	if (result > R)
		result = R;
	return result;
}

float FindMaxValue(float P, float Q, float R) {
	float result = P;
	if (result < Q)
		result = Q;
	if (result < R)
		result = R;
	return result;
}

void UpdateInterpolants(Barycentric_Interpolants& interpolants, const Hcoord& PI,
	const Hcoord& PQ, const Hcoord& PR,	float det) {
	interpolants.mu = (PR.y * PI.x - PR.x * PI.y) / det;
	interpolants.nu = (PQ.x * PI.y - PQ.y * PI.x) / det;
	interpolants.lambda = 1 - interpolants.mu - interpolants.nu;
}

void UpdateWarpedInterpolants(Barycentric_Interpolants& warped_interpolants, const Barycentric_Interpolants& interpolants,
	float Pw, float Qw, float Rw) {
	float divided_lambda = interpolants.lambda / Pw,
		divided_mu = interpolants.mu / Qw,
		divided_nu = interpolants.nu / Rw;

	float d = divided_lambda + divided_mu + divided_nu;

	if (d == 0) {
		warped_interpolants.lambda = 0;
		warped_interpolants.mu = 0;
		warped_interpolants.nu = 0;
	}

	warped_interpolants.lambda = divided_lambda / d;
	warped_interpolants.mu = divided_mu / d;
	warped_interpolants.nu = divided_nu / d;
}

void FillTriangle(Raster & r, const TexturedCoord & P, const TexturedCoord & Q, const TexturedCoord & R)
{
	Hcoord Pdev = (1 / P.w) * P,
		Qdev = (1 / Q.w) * Q,
		Rdev = (1 / R.w) * R;

	AABB<float> aabb;
	aabb.min_x = FindMinValue(Pdev.x, Qdev.x, Rdev.x);
	aabb.max_x = FindMaxValue(Pdev.x, Qdev.x, Rdev.x);
	aabb.min_y = FindMinValue(Pdev.y, Qdev.y, Rdev.y);
	aabb.max_y = FindMaxValue(Pdev.y, Qdev.y, Rdev.y);

	AABB<int> casted_aabb;
	casted_aabb.min_x = static_cast<int>(ceil(aabb.min_x));
	casted_aabb.max_x = static_cast<int>(floor(aabb.max_x));
	casted_aabb.min_y = static_cast<int>(ceil(aabb.min_y));
	casted_aabb.max_y = static_cast<int>(floor(aabb.max_y));
	
	Barycentric_Interpolants interpolants;
	Barycentric_Interpolants warped_interpolants;
	float u, v, z;

	Hcoord PQ = Qdev - Pdev;
	Hcoord PR = Rdev - Pdev;
	float det = PQ.x * PR.y - PR.x * PQ.y;
	if (det == 0)
		return;

	bool IsXIncreased = true;
	r.GotoPoint(casted_aabb.min_x, casted_aabb.min_y);
	for (int j = casted_aabb.min_y; j <= casted_aabb.max_y; j++) {
		if (IsXIncreased) {
			for (int i = casted_aabb.min_x; i <= casted_aabb.max_x; i++) {
				UpdateInterpolants(interpolants, Hcoord(static_cast<float>(i - Pdev.x), static_cast<float>(j - Pdev.y), 0, 0), PQ, PR, det);
				if (0 <= interpolants.lambda && interpolants.lambda <= 1 &&
					0 <= interpolants.mu && interpolants.mu <= 1 &&
					0 <= interpolants.nu && interpolants.nu <= 1) {
					z = interpolants.lambda * Pdev.z + interpolants.mu * Qdev.z + interpolants.nu * Rdev.z;
					if (z < r.GetZ()) {
						UpdateWarpedInterpolants(warped_interpolants, interpolants, P.w, Q.w, R.w);
						u = warped_interpolants.lambda * P.u + warped_interpolants.mu * Q.u + warped_interpolants.nu * R.u;
						v = warped_interpolants.lambda * P.v + warped_interpolants.mu * Q.v + warped_interpolants.nu * R.v;
						Vector color = TexturedCoord::color_scale * TexturedCoord::texture->uvToRGB(u, v);

						r.SetColor(static_cast<Raster::byte>(ceil(color.x)),
							static_cast<Raster::byte>(ceil(color.y)),
							static_cast<Raster::byte>(ceil(color.z)));
						r.WritePixel();
						r.WriteZ(z);
					}
				}

				r.IncrementX();
			}
		}
		else {
			for (int i = casted_aabb.max_x; i >= casted_aabb.min_x; i--) {
				r.DecrementX();
				UpdateInterpolants(interpolants, Hcoord(static_cast<float>(i - Pdev.x), static_cast<float>(j - Pdev.y), 0, 0), PQ, PR, det);
				if (0 <= interpolants.lambda && interpolants.lambda <= 1 &&
					0 <= interpolants.mu && interpolants.mu <= 1 &&
					0 <= interpolants.nu && interpolants.nu <= 1) {
					z = interpolants.lambda * Pdev.z + interpolants.mu * Qdev.z + interpolants.nu * Rdev.z;
					if (z < r.GetZ()) {
						UpdateWarpedInterpolants(warped_interpolants, interpolants, P.w, Q.w, R.w);
						u = warped_interpolants.lambda * P.u + warped_interpolants.mu * Q.u + warped_interpolants.nu * R.u;
						v = warped_interpolants.lambda * P.v + warped_interpolants.mu * Q.v + warped_interpolants.nu * R.v;
						Vector color = TexturedCoord::color_scale * P.texture->uvToRGB(u, v);
						
						r.SetColor(static_cast<Raster::byte>(ceil(color.x)),
							static_cast<Raster::byte>(ceil(color.y)),
							static_cast<Raster::byte>(ceil(color.z)));
						r.WritePixel();
						r.WriteZ(z);
					}
				}
			}
		}
		IsXIncreased = !IsXIncreased;
		r.IncrementY();
	}
}

TexturedCoord FindIntersection(const TexturedCoord& P, const TexturedCoord& Q,
	float dot_P, float dot_Q) {
	float s = dot_P / (dot_P - dot_Q);
	Hcoord I = P + s * (Q - P);
	float u = P.u + s * (Q.u - P.u),
		v = P.v + s * (Q.v - P.v);

	if (Hcoord::near(I.x, 0))
		I.x = 0;
	if (Hcoord::near(I.y, 0))
		I.y = 0;
	if (Hcoord::near(I.z, 0))
		I.z = 0;
	if (Hcoord::near(I.w, 0))
		I.w = 0;
	if (Hcoord::near(u, 0))
		u = 0;
	if (Hcoord::near(v, 0))
		v = 0;

	return TexturedCoord(I, u, v);
}

bool TextureClip::operator()(std::vector<TexturedCoord>& vertices)
{
	bool result = false;

	int half_space_size = static_cast<int>(half_spaces.size());
	if (half_space_size == 0)
		result = true;
	for (int j = 0; j < half_space_size; j++) {
		temp_vertices.clear();
		int vertices_size = static_cast<int>(vertices.size());
		const HalfSpace& half_space = half_spaces[j];
		for (int i = 0; i < vertices_size; i++) {
			const TexturedCoord& P = vertices[i],
				Q = vertices[(i + 1) % vertices_size];

			float dot_P = dot(half_space, P);
			float dot_Q = dot(half_space, Q);

			if (dot_Q <= 0) {
				if (dot_P > 0) {
					temp_vertices.push_back(FindIntersection(P, Q, dot_P, dot_Q));
				}
				else {
					temp_vertices.push_back(P);
				}
				result = true;
			}
			else {
				if (dot_P <= 0) {
					temp_vertices.push_back(P);
					temp_vertices.push_back(FindIntersection(P, Q, dot_P, dot_Q));
					result = true;
				}
			}
		}
		vertices = temp_vertices;
	}
	return result;
}
