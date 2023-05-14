/* Start Header ---------------------------------------------------------------
Copyright (C) 2018 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ellipse2d.cpp
Purpose: drawing ellipse
Language: c++
Platform: visual studio 17, window 64bit
Project: c.hwang_cs200_a2
Author: Hwang Chan IL, c.hwang, 5415279
Creation date: 10/18/2018
- End Header ----------------------------------------------------------------*/

// Includes
#include "ellipse2d.hpp"


/******************************************************************************/
/*!
\fn     void Ellipse2D::DrawMidpoint(Graphics &gl) const
\brief
        Renders filled ellipse object to input window using integer-only, 
        second-order differences based Midpoint ellipse algorithm.

        Use the following data members of class Ellipse
        mCenterX, mCenterY: position of ellipse center
        mMajorRadius: Semi-major axis length
        mMinorRadius: Semi-minor axis length

        Here's how you set a pixel: gl.SetPix(x, y, color);

\param  gl
        Reference to graphics instance, for access to offscreen buffer.
*/
/******************************************************************************/
void Ellipse2D::DrawMidpoint(Graphics &gl) const
{
	int d2;

	int a = mMajorRadius;	
	int b = mMinorRadius;

	int x = 0;
	int y = b;

	int d1 = (b*b) - (a*a)*b - (a*a) / 4;
	int dE = (b*b)*3;
	int dSE = (b*b)*3 + (a*a)*(-2*y + 2);

	gl.SetPix(mCenterX, b + mCenterY, mColor);
	gl.SetPix(mCenterX, b + mCenterY, mColor);

	while (a * a * (y - 0.5f) > b * b * (x + 1))
	{
		if (d1 < 0)
		{
			d1 +=dE;
			dE += 2 * (b*b);
			dSE += 2 * (b*b);
		}
		else
		{
			d1 += dSE;
			dE += 2 * (b*b);
			dSE += 2 * (b*b) + 2 * (a * a);
			y--;
		}
		x++;
		for (int i = 0; i <= x; i++)
		{
			gl.SetPix(x + mCenterX - i, -y + mCenterY, mColor);
			gl.SetPix(-x + mCenterX + i, y + mCenterY, mColor);
			gl.SetPix(x + mCenterX - i, y + mCenterY, mColor);
			gl.SetPix(-x + mCenterX + i, -y + mCenterY, mColor);
		}
	}

	d2 = b * b*(x*x+x+1/4) + a * a*(y - 1)*(y - 1) - (a* a)*(b*b);
	dE = (a*a) * (-2 * y + 3);
	dSE = (b*b) * (2*x + 2) + (a*a) * (-2 * y + 3);
	while (y > 0)
	{
		if (d2 < 0)
		{
			d2 += dSE;
			dE += 2 * (a*a);
			dSE += 2 * (b*b) + 2 * (a*a);
			x++;
		}
		else
		{
			d2 += dE;
			dE += 2 * (a*a);
			dSE += 2 * (a*a);
		}
		y--;
		for (int i = 0; i <= x; i++)
		{
			gl.SetPix(x + mCenterX -i, -y + mCenterY, mColor);
			gl.SetPix(-x + mCenterX +i, y + mCenterY, mColor);
			gl.SetPix(x + mCenterX-i, y + mCenterY, mColor);
			gl.SetPix(-x + mCenterX+i, -y + mCenterY, mColor);
		}
	}

	gl.SetPix(a + mCenterX, mCenterY, mColor);
	gl.SetPix(-a + mCenterX, mCenterY, mColor);
}

