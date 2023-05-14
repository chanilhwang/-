/*
Affine.cpp
-Name : Hwang Chan IL
-Assignment Number : 1
-Course Name : CS250
-term : Spring 2019
*/

#include "Affine.h"

Hcoord::Hcoord(float X, float Y, float Z, float W)
	: x(X), y(Y), z(Z), w(W)
{ }

Point::Point(float X, float Y, float Z)
	: Hcoord(X,Y,Z,1)
{ }

Vector::Vector(float X, float Y, float Z)
	: Hcoord(X,Y,Z,0)
{ }

Affine::Affine()
{
	row[3][3] = 1;
}

Affine::Affine(const Vector& Lx, const Vector& Ly, const Vector& Lz, const Point& D)
{
	for (int i = 0; i < 4; i++)
	{
		row[i].x = Lx[i];
		row[i].y = Ly[i];
		row[i].z = Lz[i];
		row[i].w = D[i];
	}
}

Hcoord operator+(const Hcoord& u, const Hcoord& v)
{
	Hcoord result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = u[i] + v[i];
	}

	return result;
}

Hcoord operator-(const Hcoord& u, const Hcoord& v)
{
	Hcoord result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = u[i] - v[i];
	}

	return result;
}

Hcoord operator-(const Hcoord& v)
{
	Hcoord result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = -v[i];
	}

	return result;
}

Hcoord operator*(float r, const Hcoord& v)
{
	Hcoord result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = r * v[i];
	}

	return result;
}

Hcoord operator*(const Matrix& A, const Hcoord& v)
{
	Hcoord result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i] += A[i][j] * v[j];
		}
	}

	return result;
}

Matrix operator*(const Matrix& A, const Matrix& B)
{
	Matrix result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] += A[i][k] * B[k][j];
			}
		}
	}

	return result;
}

Matrix operator*(float r, const Matrix& A)
{
	Matrix result = A;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] *= r;
		}
	}

	return result;
}

Matrix operator+(const Matrix& A, const Matrix& B)
{
	Matrix result = A;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] += B[i][j];
		}
	}

	return result;
}

Affine operator*(float r, const Affine& A)
{
	Matrix result = A;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] *= r;
		}
	}

	return result;
}

Affine operator+(const Affine& A, const Affine& B)
{
	Matrix result = A;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] += B[i][j];
		}
	}

	return result;
}

Affine makeOuterProductMatrix(const Vector& A)
{
	Affine result;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = A[i] * A[j];
		}
	}

	return result;
}

Affine makeCrossProductMatrix(const Vector& v)
{
	Affine result;

	result[0][1] = -v[2];
	result[0][2] = v[1];
	result[1][0] = v[2];
	result[1][2] = -v[0];
	result[2][0] = -v[1];
	result[2][1] = v[0];

	return result;
}

float dot(const Vector& u, const Vector& v)
{
	float result = 0;

	for (int i = 0; i < 3; i++)
	{
		result += u[i] * v[i];
	}

	return result;
}

float abs(const Vector& v)
{
	return sqrt(dot(v, v));
}

Vector normalize(const Vector& v)
{
	float length = abs(v);
	Vector result = v;

	for (int i = 0; i < 3; i++)
	{
		result[i] /= length;
	}

	return result;
}

Vector cross(const Vector& u, const Vector& v)
{
	Vector result;

	for (int i = 0; i < 3; i++)
	{
		result[i] = u[(i + 1) % 3] * v[(i + 2) % 3] - u[(i + 2) % 3] * v[(i + 1) % 3];
	}

	return result;
}

Affine rotate(float t, const Vector& v)
{
	Affine result;
	Affine identityMatrix = scale(1);
	Affine outerProductMatrix = makeOuterProductMatrix(v);
	Affine crossProductMatrix = makeCrossProductMatrix(v);

	result = cos(t) * identityMatrix + ((1 - cos(t)) / (dot(v, v))) * outerProductMatrix + (sin(t) / abs(v)) * crossProductMatrix;



	return result;
}

Affine translate(const Vector& v)
{
	Affine result = scale(1);

	for (int i = 0; i < 3; i++)
	{
		result[i][3] = v[i];
	}

	return result;
}

Affine scale(float r)
{
	Affine result;

	for (int i = 0; i < 3; i++)
	{
		result[i][i] = r;
	}

	return result;
}

Affine scale(float rx, float ry, float rz)
{
	Affine result;

	result[0][0] = rx;
	result[1][1] = ry;
	result[2][2] = rz;

	return result;
}

Affine transpose(const Affine& A)
{
	Affine result;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result[i][j] = A[j][i];
		}
	}

	return result;
}

Affine inverse(const Affine& A)
{
	Affine inverseTranslationAffine = scale(1);

	for (int i = 0; i < 3; i++)
	{
		inverseTranslationAffine[i][3] = -A[i][3];
	}

	Affine determinantMatrix;

	determinantMatrix[0][0] = A[1][1] * A[2][2] - A[1][2] * A[2][1];
	determinantMatrix[0][1] = A[1][0] * A[2][2] - A[1][2] * A[2][0];
	determinantMatrix[0][2] = A[1][0] * A[2][1] - A[1][1] * A[2][0];

	determinantMatrix[1][0] = A[0][1] * A[2][2] - A[0][2] * A[2][1];
	determinantMatrix[1][1] = A[0][0] * A[2][2] - A[0][2] * A[2][0];
	determinantMatrix[1][2] = A[0][0] * A[2][1] - A[0][1] * A[2][0];

	determinantMatrix[2][0] = A[0][1] * A[1][2] - A[0][2] * A[1][1];
	determinantMatrix[2][1] = A[0][0] * A[1][2] - A[0][2] * A[1][0];
	determinantMatrix[2][2] = A[0][0] * A[1][1] - A[0][1] * A[1][0];

	Affine cofactorMatrix;
	
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cofactorMatrix[i][j] = (((i + j) % 2 == 0) ? 1 : -1) * determinantMatrix[i][j];
		}
	}

	float determinant = A[0][0] * cofactorMatrix[0][0] + A[0][1] * cofactorMatrix[0][1] + A[0][2] * cofactorMatrix[0][2];

	return (1 / determinant) * transpose(cofactorMatrix) * inverseTranslationAffine;
}