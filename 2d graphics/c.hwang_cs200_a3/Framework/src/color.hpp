/******************************************************************************/
/*!
\file   color.hpp
\par    Purpose: Declare class, attributes and functions for 2d point.
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\par    Course: CS200
\par    Assignment 4
\date   28/10/2014
*/
/******************************************************************************/

#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <iostream>


enum ColorComponent{R = 0, G, B, A};

/******************************************************************************/
/*!
\class  Color
\brief
        Simple color class that contains 4 components.
*/
/******************************************************************************/
class Color
{
    /*  Overloaded input & output operators */
    friend std::ostream& operator<<(std::ostream&, const Color&);
    friend std::istream& operator>>(std::istream&, Color&);

public:
    /*  Default constructor */
    Color(	const unsigned char r = 0,
            const unsigned char g = 0,
            const unsigned char b = 0,
            const unsigned char a = 255)
    {
        mColorRGBA[R] = r;
        mColorRGBA[G] = g;
        mColorRGBA[B] = b;
        mColorRGBA[A] = a;
    }

    /*  No need for a Color copy constructor.
        No need for a Color overloaded assignment operator.
        No need for a Color destructor. */

    /*  Auxiliary functions */
    unsigned char GetR() const				{ return mColorRGBA[R]; }
    unsigned char GetG() const				{ return mColorRGBA[G]; }
    unsigned char GetB() const				{ return mColorRGBA[B]; }
    unsigned char GetA() const				{ return mColorRGBA[A]; }

    void SetR(const unsigned char r)		{ mColorRGBA[R] = r; }
    void SetG(const unsigned char g)		{ mColorRGBA[G] = g; }
    void SetB(const unsigned char b)		{ mColorRGBA[B] = b; }
    void SetA(const unsigned char a)		{ mColorRGBA[A] = a; }
    void SetRGBA(	const unsigned char r,
                    const unsigned char g,
                    const unsigned char b,
                    const unsigned char a)	{ SetR(r); SetG(g); SetB(b); SetA(a); }

private:
    unsigned char mColorRGBA[4];
};


/*  Define some common colors for convenience */
const Color RED     = Color(255 , 0     , 0     , 255);
const Color GREEN   = Color(0   , 255   , 0     , 255);
const Color BLUE    = Color(0   , 0     , 255   , 255);
const Color YELLOW  = Color(255 , 255   , 0     , 255);
const Color BLACK   = Color(0   , 0     , 0     , 255);

#endif