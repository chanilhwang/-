/*
RasterUtilities.cpp
-Name : Hwang Chan IL
-Assignment Number : 5
-Course Name : CS250
-term : Spring 2019
*/
#include "RasterUtilities.h"

void ClearBuffers(Raster & r, float z)
{
	r.GotoPoint(0, 0);
	for (int i = 0; i < r.Height(); i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < r.Width(); j++) {
				r.WriteZ(z);
				r.WritePixel();
				r.IncrementX();
			}
		}
		else {
			for (int j = 0; j < r.Width(); j++) {
				r.DecrementX();
				r.WriteZ(z);
				r.WritePixel();
			}
		}
		r.IncrementY();
	}
}

float min(float x1, float x2, float x3) {
	float result = x1;
	if (result > x2)
		result = x2;
	if (result > x3)
		result = x3;
	return result;
}

float max(float x1, float x2, float x3) {
	float result = x1;
	if (result < x2)
		result = x2;
	if (result < x3)
		result = x3;
	return result;
}

float FindDeterminant(Point p, Vector n) {
	return p.x * n.x + p.y * n.y;
}

void FillTriangle(Raster & r, const Hcoord & P, const Hcoord & Q, const Hcoord & R)
{
	Hcoord Pndc = (1 / P.w) * P,
		Qndc = (1 / Q.w) * Q,
		Rndc = (1 / R.w) * R;

	Vector PQ = Qndc - Pndc,
		QR = Rndc - Qndc,
		RP = Pndc - Rndc;

	Vector normalPQ = Vector(-PQ.y, PQ.x),
		normalQR = Vector(-QR.y, QR.x),
		normalRP = Vector(-RP.y, RP.x);

	if (dot(normalPQ, Rndc - Pndc) > 0) {
		normalPQ = -normalPQ;
	}
	if (dot(normalQR, Pndc - Qndc) > 0) {
		normalQR = -normalQR;
	}
	if (dot(normalRP, Qndc - Rndc) > 0) {
		normalRP = -normalRP;
	}
	float d_PQ = normalPQ.x * Pndc.x + normalPQ.y * Pndc.y;
	float d_QR = normalQR.x * Qndc.x + normalQR.y * Qndc.y;
	float d_RP = normalRP.x * Rndc.x + normalRP.y * Rndc.y;

	Vector normal = cross(PQ, RP);
	float d = normal.x * Pndc.x + normal.y * Pndc.y + normal.z * Pndc.z;
	float z_incrementation_by_x = -normal.x / normal.z;
	float z_incrementation_by_y = -normal.y / normal.z;
	
	float xmin = ceil(min(Pndc.x, Qndc.x, Rndc.x));
	float xmax = floor(max(Pndc.x, Qndc.x, Rndc.x));
	float ymin = ceil(min(Pndc.y, Qndc.y, Rndc.y));
	float ymax = floor(max(Pndc.y, Qndc.y, Rndc.y));
	int imin = static_cast<int>(xmin);
	int imax = static_cast<int>(xmax);
	int jmin = static_cast<int>(ymin);
	int jmax = static_cast<int>(ymax);

	Point first_point = Point(xmin, ymin);
	float determine0 = normalPQ.x * first_point.x + normalPQ.y * first_point.y - d_PQ,
		determine1 = normalQR.x * first_point.x + normalQR.y * first_point.y - d_QR,
		determine2 = normalRP.x * first_point.x + normalRP.y * first_point.y - d_RP;
	float z = (d - normal.x * first_point.x - normal.y * first_point.y) / normal.z;

	r.GotoPoint(imin, jmin);
	for (int j = 0; j <= jmax - jmin; j++) {		
		if (j % 2 == 0) {
			for (int i = imin; i <= imax; i++) {
				if (determine0 <= 0 &&
					determine1 <= 0 &&
					determine2 <= 0) {
					if (z < r.GetZ()) {
						r.WritePixel();
						r.WriteZ(z);
					}
				}

				determine0 += normalPQ.x;
				determine1 += normalQR.x;
				determine2 += normalRP.x;
				z += z_incrementation_by_x;
				r.IncrementX();
			}
		}
		else {
			for (int i = imin; i <= imax; i++) {
				determine0 -= normalPQ.x;
				determine1 -= normalQR.x;
				determine2 -= normalRP.x;
				z -= z_incrementation_by_x;
				r.DecrementX();

				if (determine0 <= 0 &&
					determine1 <= 0 &&
					determine2 <= 0) {
					if (z < r.GetZ()) {
						r.WritePixel();
						r.WriteZ(z);
					}
				}
			}
		}

		determine0 += normalPQ.y;
		determine1 += normalQR.y;
		determine2 += normalRP.y;
		z += z_incrementation_by_y;
		r.IncrementY();
	}
}
