/******************************************************************************/
/*!
\file   graphics.hpp
\par    Purpose: Declaring the graphics-related variables & functions
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


#ifndef __GRAPHICS_HPP_
#define __GRAPHICS_HPP_

#include "gl/gl3w.h"
#include "gl/freeglut.h"
#include "glm/gtc/type_ptr.hpp"

#include "color.hpp"


#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "freeglut.lib")


/*  The graphics-related variables are defined and initialized in graphics.cpp */
extern int width;
extern int height;

/*  Render program ID */
extern GLuint renderProg;

/*  Locations of the variables in the shader */
extern GLint modelMatLoc;
extern GLint viewprojMatLoc;
extern GLint fbTexLoc;

/*  Matrices for modelview transformations and camera projection */
extern glm::mat4 modelMat, viewMat, projMat, viewprojMat;

/*  Vertex array ID */
extern GLuint VAO;

/*  Vertex buffer ID */
extern GLuint VBOQuadCoord, VBOTexCoord;

/*  Framebuffer ID */
extern GLuint fbObj;

/*  Texture ID that will be mapped to framebuffer */
extern GLuint fbTexObj;

/*  Actual texture buffers for drawing and testing */
extern unsigned char *fbDrawTex, *fbTestTex;


/*  Public functions */
void SetUp();
void CleanUp();
void Resize(int w, int h);
void Render();

void SetPixel(unsigned char *frameBuffer, int x, int y, Color c);
void UpdateDrawBuffer(int minX, int minY, int maxX, int maxY);
void ClearDrawBuffer();
void SwitchToTestBuffer();
void SwitchToDrawBuffer();

void DrawTestTriangles();

#endif