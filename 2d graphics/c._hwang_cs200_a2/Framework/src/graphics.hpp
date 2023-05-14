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
\date   22/5/2015
*/
/******************************************************************************/


#ifndef __GRAPHICS_HPP_
#define __GRAPHICS_HPP_

#include <array>
#include <vector>
#include <utility>

#include "gl/gl3w.h"

#include "color.hpp"

#pragma comment (lib, "opengl32.lib")


/****************************************************************************/
/*!
\class  Graphics

\brief  Graphics class for rendering-related stuffs.
*/
/****************************************************************************/
class Graphics
{
public:
    Graphics(int width, int height);
    ~Graphics();

    /*  Public functions */
    void            InitGLExtensions() const;

    void            SetUp();
    void            Render();
    void            Resize(int w, int h);
    void            ClearFB(const Color &c);
    void            SetPix(int x, int y, const Color &c, bool drawing = true);


private:
    // Prevent defaut construction
    Graphics();
    // Prevent copying
    Graphics(const Graphics& rhs);
    Graphics& operator=(const Graphics& rhs);

    int             width, height;              /*  Viewport width & height */
    int             numElems;                   /*  Used for allocate offscreenFB */
    unsigned char   *offscreenFB;
    bool            fbUpdated;                  /*  Used to trigger fb copy only when necessary */

    std::vector<std::pair<int, int>>     
                    drawnPix;                   /*  Used to clear the buffer quickly */

    std::array<float, 8> posBuffer;             /*  Full-window rectangle's vertex position */
    std::array<float, 8> uvBuffer;              /*  Vertex uv for applying offline framebuffer on */

    char            *vsFilename, *fsFilename;   /*  Shader filenames */

    GLuint          renderProg;                 /*  ID of the set of shaders that we'll use */
    GLuint          texObj;                     /*  Texture object (e.g. ID) to indicate the state of texture sent to the GPU */
    GLint           texLoc;                     /*  Location of texture variable in the shaders */
    GLuint          VAO;                        /*  Vertex array object, which is the ID to indicate the state of the vertices in the GPU */
    GLuint          posVBO;                     /*  Vertex buffer object (e.g. ID) to indicate the state of the vertex position data */
    GLuint          uvVBO;                      /*  Vertex buffer object (e.g. ID) to indicate the state of the vertex UV data */

    void            ValidateShader(GLuint shader, const char *file) const;
    void            ValidateProgram(GLuint program) const;
    GLuint          CompileShaders();

    void            SetUpTextureObjs();
    void            SendVertexAttribute(GLuint VBO, const float vertexData[], int size, int location) const;
};  // class Graphics

#endif // __GRAPHICS_HPP_