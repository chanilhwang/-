/******************************************************************************/
/*!
\file   input.hpp
\par    Purpose: Handle events upon user inputs
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

#ifndef UI_HPP_
#define UI_HPP_

#include "graphics.hpp"
#include "triangle.hpp"

/*  Public functions */
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void MouseClick(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void MousePassiveMotion(int x, int y);
void MouseWheel(int wheel, int direction, int x, int y);

#endif

