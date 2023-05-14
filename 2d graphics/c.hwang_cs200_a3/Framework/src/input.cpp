/******************************************************************************/
/*!
\file   input.cpp
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

#include "input.hpp"

/*  Indicate whether the user currently selects test mode or normal draw mode */
bool testMode = false;

/*  For creating the vertices where the user clicks */
Vertex2D vert[3];
int vertID = 0;
const Color vertColor[3] = { RED, GREEN, BLUE };

/*  For storing the range of pixels that the selected vertices cover */
int minX = width + 1, maxX = -1, minY = height + 1, maxY = -1;


/******************************************************************************/
/*!
\fn     Keyboard
\brief
        Events when a key is pressed.
\param  unsigned char key
        Pressed key.
\param  int x
        Current x-position of the mouse cursor.
\param  int y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 9:     // TAB
            testMode = !testMode;
            if (testMode)
                SwitchToTestBuffer();
            else
                SwitchToDrawBuffer();
            break;

        case 'c': case 'C':
            if (!testMode)
                ClearDrawBuffer();
            break;
    }
}


/******************************************************************************/
/*!
\fn     SpecialKeyboard
\brief
        Events when a special key is pressed. This include CTRL+ ..., ALT+ ...
\param  unsigned char key
        Pressed key.
\param  int x
        Current x-position of the mouse cursor.
\param  int y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void SpecialKeyboard(int key, int x, int y)
{
    /* ALT+F4: Quit */
    if (key == GLUT_KEY_F4 && glutGetModifiers() == GLUT_ACTIVE_ALT)
    {
        CleanUp();
        exit(0);
    }
    else
    /*  For now we're not handling any other special keys */
    if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT)
    {
    }
}


/******************************************************************************/
/*!
\fn     MouseClick
\brief
        Events when a mouse button is clicked.
\param  int button
        Clicked button.
\param  int state
        State of the button (down/up only. holding/dragging is not handled here).
\param  int x
        Current x-position of the mouse cursor.
\param  int y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void MouseClick(int button, int state, int x, int y)
{
    /*  Only handle if the current mode is drawing */
    if (!testMode && button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN && x < width && y < height)
        {
            /*  Uncomment this if you want to track the vertex positions */
            //std::cout <<    "vert " << vertID << 
            //                " : x = " << x << " ; y = " << y << std::endl;


            /*  Compute min/max x/y for updating the affected area */
            if (minX > x)   minX = x;
            if (maxX < x)   maxX = x;

            if (minY > y)   minY = y;
            if (maxY < y)   maxY = y;

            /*  Update triangle's vertex */
            vert[vertID] = Vertex2D(static_cast<float>(x), 
                                    static_cast<float>(y),
                                    vertColor[vertID]);
            ++vertID;

            /*  Draw & reset if already selecting 3 vertices */
            if (vertID == 3)
            {
                Triangle tri(vert[0], vert[1], vert[2]);
                if (tri.IsCounterClockwise())
                {
                    tri.Draw(fbDrawTex);
                    UpdateDrawBuffer(minX, minY, maxX, maxY);
                }

                /*  Reset to first vertex for new triangle */
                vertID = 0;
                minX = width + 1;
                minY = height + 1;
                maxX = -1;
                maxY = -1;
            }
        }
}


/******************************************************************************/
/*!
\fn     MouseMotion
\brief
        Events when a mouse button is held and dragged.
\param  int x
        Current x-position of the mouse cursor.
\param  int y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void MouseMotion(int x, int y)
{
    /*  This is just a placeholder. Nothing is implemented yet. */
}


/******************************************************************************/
/*!
\fn     MousePassiveMotion
\brief
        Events when a mouse is dragged but not clicked.
\param  int x
        Current x-position of the mouse cursor.
\param  int y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void MousePassiveMotion(int x, int y)
{
    /*  This is just a placeholder. Nothing is implemented yet. */
}


/******************************************************************************/
/*!
\fn     MouseWheel
\brief
        Events when a mouse middle button is scrolled.
\param  int direction
        Whether it's scrolling up or down.
\param  int x
        Current x-position of the mouse cursor.
\param  int y
        Current y-position of the mouse cursor (from top).
*/
/******************************************************************************/
void MouseWheel(int wheel, int direction, int x, int y)
{
    /*  This is just a placeholder. Nothing is implemented yet. */
    switch (direction)
    {
        case 1:	/* SCROLL DOWN */
            break;
        case -1: /* SCROLL DOWN */
            break;
    }
}
