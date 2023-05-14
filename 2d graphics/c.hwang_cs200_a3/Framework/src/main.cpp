/******************************************************************************/
/*!
\file   main.cpp
\par    Purpose: Main file of the assignment
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Sang Le
\par    email: sang.le@digipen.edu
\par    DigiPen login: sang.le
\par    Course: CS200
\par    Assignment 4
\date   26/06/2015
*/
/******************************************************************************/

#include <iostream>
#include <iomanip>

#include "input.hpp"
#include "graphics.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif


/******************************************************************************/
/*!
\fn     DisplayUserInputs
\brief
        Display the assignment requirements and user inputs.
*/
/******************************************************************************/
void DisplayUserInputs()
{
    std::cout <<    std::endl;
    std::cout <<    "CS200 Assignment Triangle Drawing: \n"
                    "Triangle drawing using top-left tie-breaking rule \n"
                    "and bilinear color interpolation technique.\n";

    std::cout <<    std::endl;
    std::cout <<    "- Left-click 3 vertices in counter-clockwise order to draw"
                    " a triangle.\n";
    std::cout <<    "- Press TAB to toggle between drawing and testing modes.\n";
    std::cout <<    "- Press C in drawing mode to clear the triangles.\n";

    std::cout <<    "\n\n";
    std::cout <<    "(C) 2015 DigiPen Institute of Technology Singapore\n\n";
    std::cout <<    "Please report any bugs to sang.le@digipen.edu\n";
}


/******************************************************************************/
/*!
\fn     CreateGLContext
\brief
        Create the context for OpenGL commands to work.
\param  int argc
        Use argc from main function
\param  char **argv
        Use argv from main function
*/
/******************************************************************************/
void CreateGLContext(int argc, char **argv)
{
    /* GLUT Window Creation & Initialization */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(width, height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Triangle Drawing");

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
}


/******************************************************************************/
/*!
\fn     InitGLExtensions
\brief
        Initialize the OpenGL extensions supported on the current platform.
*/
/******************************************************************************/
void InitGLExtensions()
{
    /* GL3W Initialization */
    if (gl3wInit())
    {
        std::cerr << "Failed to initialize OpenGL" << std::endl;
        std::exit(-1);
    }

    if (!gl3wIsSupported(3, 3))
    {
        std::cerr << "OpenGL 3.3 not supported" << std::endl;
        std::exit(-1);
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}


/******************************************************************************/
/*!
\fn     MainLoop
\brief
        The main loop of OpenGL rendering and user interactions.
*/
/******************************************************************************/
void MainLoop()
{
    /* GLUT callback loop */
    glutDisplayFunc(Render);
    glutIdleFunc(Render);
    glutReshapeFunc(Resize);
    glutKeyboardFunc(Keyboard);
    glutSpecialFunc(SpecialKeyboard);
    glutMouseFunc(MouseClick);
    glutMotionFunc(MouseMotion);
    glutMouseWheelFunc(MouseWheel);
    glutPassiveMotionFunc(MousePassiveMotion);
    glutCloseFunc(CleanUp);

    glutMainLoop();
}


/******************************************************************************/
/*!
\fn     main
\brief
        Entry point of the program.
\param  int argc
        Number of arguments when the program is executed.
\param  char **argv
        The array of arguments when the program is executed.
*/
/******************************************************************************/
void main(int argc, char **argv)
{
    /*  This function can be used in Windows to disable the console.
        Currently, we still keep our console for debugging purpose. */
    //FreeConsole();

    CreateGLContext(argc, argv);
    InitGLExtensions();

    DisplayUserInputs();

    /*  Set up all the attributes before drawing */
    SetUp();

    /*  The main loop for user drawing and interactions */
    MainLoop();
}
