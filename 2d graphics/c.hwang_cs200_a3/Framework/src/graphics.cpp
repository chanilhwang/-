/******************************************************************************/
/*!
\file   graphics.cpp
\par    Purpose: Implement the graphics-related functions
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

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "graphics.hpp"
#include "triangle.hpp"

#include "textfile.h"


/******************************************************************************/
/*  Initialization for the window and graphics attributes                     */
/******************************************************************************/
/*  Viewport width & height */
int width = 800;
int height = 600;

/*  Background color of viewport */
const GLfloat bgColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

/*  Vertex coordinates of the quad that we'll use to cover the viewport */
const GLfloat quadCoordBuff[] =
{
    -1.0f, -1.0f,
    -1.0f, 1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f
};

/*  Texture coordinates that we'll use to color the quad using a texture */
const GLfloat texCoordBuff[] =
{
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
};

/*  Shader filenames */
const char vsFileName[] = "shaders/shader.vs";
const char fsFileName[] = "shaders/shader.fs";


/******************************************************************************/
/*  Declaration for other graphics-related variables                          */
/******************************************************************************/
/*  ID of the set of shaders that we'll use */
GLuint renderProg;

/*  Locations of the variables in the shader */
GLint modelMatLoc;
GLint viewprojMatLoc;
GLint fbTexLoc;
GLint quadCoordLoc, texCoordLoc;

/*  Matrices for modelview transformations and camera projection */
glm::mat4 modelMat, viewMat, projMat, viewprojMat;

/*  Vertex array ID to store the states of all attributes */
GLuint VAO;

/*  Vertex buffer IDs to store the states of the buffer data */
GLuint VBOQuadCoord, VBOTexCoord;

/*  Texture ID to store the state of the current texture
    used to fill the framebuffer quad */
GLuint fbTexObj;

/*  The textures used to fill framebuffer quad.
    fbDrawTex stores the pixels that the user can color to draw triangles.
    fbTestTex stores the pixels that contain two testing triangles.
    User cannot draw on fbTestTex. */
unsigned char *fbDrawTex, *fbTestTex;


/******************************************************************************/
/*!
\fn     CompileShaders
\brief
        Read the shader files, compile and link them into a program for render.
\param  const char vsFilename[]
        Vertex shader filename.
\param  const char fsFilename[]
        Fragment shader filename.
\return
        The rendering program ID.
*/
/******************************************************************************/
GLuint CompileShaders(const char vsFilename[], const char fsFilename[])
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    char *vsSource = textFileRead(vsFilename);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSource, NULL);
    glCompileShader(vertexShader);

    char * fsSource = textFileRead(fsFilename);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSource, NULL);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    free(vsSource);
    free(fsSource);

    return program;
}


/******************************************************************************/
/*!
\fn     CompileShadersFromString
\brief
        Instead of read the shader programs from files, this function reads them 
        from strings, then also compile and link them into a program for render.
\param  const char * vsSource[]
        Vertex shader source code.
\param  const char * fsSource[]
        Fragment shader source code.
\return
        The rendering program ID.
*/
/******************************************************************************/
GLuint CompileShadersFromString(const char * vsSource[], const char * fsSource[])
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, vsSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, fsSource, NULL);
    glCompileShader(fragmentShader);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


/******************************************************************************/
/*!
\fn     SetUpMatrices
\brief
        Set up the matrices for model & view transformations, and camera
        projection. Then sending these matrices to the (vertex) shader to
        transform the vertices from model frame to world frame, then view frame,
        then project them onto view plane in NDC.
*/
/******************************************************************************/
void SetUpMatrices()
{
    /*  Model matrix will be used for transforming verticess to their positions
        in the world. Each vertex has a different model matrix.
        View matrix will be used for converting from world frame to camera view 
        frame.
        Projection matrix will be used for projecting the vertices onto the 2D 
        view plane.

        View & projection matrices are normally the same for the whole scene, 
        since we only have one camera and one view plane. Hence we can concat 
        them right here to save the cost.
        Then later on, model matrix & view*projection matrix will be sent to 
        vertex shader to compute model*view*projection matrix for each vertex.

        In CS200, model & view & projection matrices will be set to identities,
        since we will assume our world is the same as the viewport, and not use
        any transformations or projection. */
    modelMat = glm::mat4(1.0);
    viewMat = glm::mat4(1.0);
    projMat = glm::mat4(1.0);
    viewprojMat = projMat * viewMat;    // R to L

    /*  Obtain the locations of the variables in the shaders with the given names */
    modelMatLoc = glGetUniformLocation(renderProg, "modelMat");
    viewprojMatLoc = glGetUniformLocation(renderProg, "viewprojMat");

    /*  Feed matrix data into shaders */
    glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(viewprojMatLoc, 1, GL_FALSE, glm::value_ptr(viewprojMat));
}


/******************************************************************************/
/*!
\fn     SetUpQuad
\brief
        Set up a quad to cover the whole viewport framebuffer. We need to use 
        this "trick" to practice rasterization, because rasterization is fixed 
        and non-programmable in OpenGL.
        We will use a texture (2D array of pixels) to fill this quad. On this
        texture, the user can color pixels, and the modified regions will later 
        be updated and sent to the shaders for rendering the framebuffer again.

        The quad we use is formed by 2 triangles with coordinates already in
        NDC. Hence, the model-view-projection matrices can be all identities.
        For the triangle vertex coordinates, see the top part of this file.
*/
/******************************************************************************/
void SetUpQuad()
{
    /*  Create vertex buffer ID to store the state of the quad vertices */
    glGenBuffers(1, &VBOQuadCoord);
    glBindBuffer(GL_ARRAY_BUFFER, VBOQuadCoord);

    /*  Allocate buffer for the quad vertex coordinates */
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadCoordBuff), quadCoordBuff, GL_STATIC_DRAW);

    /*  Obtain the location of the variable in the shaders with the given names */
    quadCoordLoc = glGetAttribLocation(renderProg, "quadCoord");

    /*  Feed the quad vertex buffer into shaders */
    glEnableVertexAttribArray(quadCoordLoc);
    glVertexAttribPointer(
        quadCoordLoc,       // location of quadCoord variable in shader
        2,                  // dimension of vertex coord
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );
}


/******************************************************************************/
/*!
\fn     SetUpTextures
\brief
        Set up two textures and the corresponding texel coordinates for mapping
        to the framebuffer quad.
        One texture is for the user to color the pixels on.
        One is for storing two default testing triangles.
        These two textures will be interchangably used for filling the 
        framebuffer quad that covers the whole viewport.
*/
/******************************************************************************/
void SetUpTextures()
{
    /*  Initialze the texture buffer for testing */
    fbTestTex = new unsigned char[width * height * 4];
    std::fill_n(fbTestTex, width * height * 4, 255);

    /*  Draw 2 testing triangles to check if 
        top-left tie-breaking rule is used correctly */
    DrawTestTriangles();    


    /*  Initialze the texture buffer for user drawing */
    fbDrawTex = new unsigned char[width * height * 4];
    std::fill_n(fbDrawTex, width * height * 4, 255);


    /*  Activate TEXTURE0 */
    glActiveTexture(GL_TEXTURE0);

    /*  Generate texture ID to store the texture for filling the quad */
    glGenTextures(1, &fbTexObj);
    glBindTexture(GL_TEXTURE_2D, fbTexObj);

    /*  Copy fbDrawTex to the current texture */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, fbDrawTex);

    /*  Set up the parameter.
        In this method, GL_TEXTURE_MIN_FILTER must be set to GL_NEAREST
        because only one level of texture is initialized. */
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    /*  Obtain the location of the variable in the shaders with the given
        name */
    fbTexLoc = glGetUniformLocation(renderProg, "fbTex");

    /*  Feed TEXTURE0 to shaders */
    glUniform1i(fbTexLoc, 0);


    /*  Generate texture buffer ID for storing the texture coordinates to be 
        mapped with the quad vertex coordinates. */
    glGenBuffers(1, &VBOTexCoord);
    glBindBuffer(GL_ARRAY_BUFFER, VBOTexCoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoordBuff), texCoordBuff, GL_STATIC_DRAW);

    /*  Obtain the location of the variable in the shaders with the given name */
    texCoordLoc = glGetAttribLocation(renderProg, "texCoord");

    /*  Feed the texture coordinate buffer into shaders */
    glEnableVertexAttribArray(texCoordLoc);
    glVertexAttribPointer(
        texCoordLoc,        // location of texCoord variable in shader
        2,                  // dimension of texture coord
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );
}


/******************************************************************************/
/*!
\fn     SetUp
\brief
        Set up the render program, VAO, VBO and graphics data for rendering.
*/
/******************************************************************************/
void SetUp()
{
    /*  Link the shaders into rendering program and compile it */
    renderProg = CompileShaders(vsFileName, fsFileName);
    glUseProgram(renderProg);


    /*  Create vertex array ID to store the states of all attributes */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /*  Create matrices for model-to-world transforms, view transform and
        projection, then feed them into shaders
    */
    SetUpMatrices();

    /*  Since the rasterization is fixed in OpenGL and we cannot modify it,
        we will practice it by using a trick.
        We will render a quadrangle to fill the viewport and color it using 
        a texture that contains the pixels obtained from our rasterization
        algorithm.
    */
    SetUpQuad();
    SetUpTextures();


    /*  Unbind the current VAO. We'll bind it again when we need to render */
    glBindVertexArray(0);

    /*  Bind framebuffer to 0 to render to the screen (by default already 0) */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


/******************************************************************************/
/*!
\fn     CleanUp
\brief
        Clean up the graphics-related stuffs before shutting down.
*/
/******************************************************************************/
void CleanUp()
{
    delete [] fbTestTex;
    delete [] fbDrawTex;
    glDisableVertexAttribArray(quadCoordLoc);
    glDisableVertexAttribArray(texCoordLoc);
    glDeleteBuffers(1, &VBOQuadCoord);
    glDeleteBuffers(1, &VBOTexCoord);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(renderProg);
    exit(0);
}


/******************************************************************************/
/*!
\fn     Resize
\brief
        Resize function for updating camera projection upon window resize.
\param  int w
        The new width of the window
\param  int h
        The new height of the window
        
*/
/******************************************************************************/
void Resize(int w, int h)
{
    /*  Resize viewport if needed, but let's not do anything now */
    //width = w;
    //height = h;

    /*  Here we flip along y because the quad texture (representing device frame)
        has y pointing down, while OpenGL has y pointing up */
    glViewport(0, h - height, width, height);
    
    /*  This works in normal cases */
    //glViewport(0, 0, width, height);

    /*  Adjust view frustum aspect if needed, but let's not do anything now */
    //...
}


/******************************************************************************/
/*!
\fn     Render
\brief
        Render function for drawing out the geometry based on vertex data.
*/
/******************************************************************************/
void Render()
{
    /*  Set background color to bgColor */
    glClearBufferfv(GL_COLOR, 0, bgColor);

    /*  Bind this vertex array ID so OpenGL knows which data to look up */
    glBindVertexArray(VAO);

    /*  Drawing call */
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /*  Unbind the VAO above. In this specific program, it's not necessary
        to keep binding and unbinding this VAO, since it's the only one.
        But in general, it's a good practice to bind the required VAO when
        rendering and unbind it to minimize memory usage. */
    glBindVertexArray(0);

    /*  Bring the back buffer to foreground and vice versa. Without this
        the framebuffer won't get automatically refreshed. */
    glutSwapBuffers();
}


/******************************************************************************/
/*!
\fn     SetPixel
\brief
        Color a pixel.
\param  unsigned char *tex
        The texture that we want to draw to.
\param  int x
        x-coordinate of the considered pixel.
\param  int y
        y-coordinate of the considered pixel.
\param  Color c
        Color that we want to set for the pixel.
*/
/******************************************************************************/
void SetPixel(unsigned char *tex, int x, int y, Color c)
{
    int base = (y * width + x) * 4;
    tex[base + 0] = c.GetR();
    tex[base + 1] = c.GetG();
    tex[base + 2] = c.GetB();
    tex[base + 3] = 255;
}


/******************************************************************************/
/*!
\fn     UpdateDrawBuffer
\brief
        Update a region from texture fbDrawTex to the framebuffer.
\param  int minX
        Minimal x of the updated region.
\param  int minY
        Minimal y of the updated region.
\param  int maxX
        Maximal x of the updated region.
\param  int maxY
        Minimal y of the updated region.
*/
/******************************************************************************/
void UpdateDrawBuffer(int minX, int minY, int maxX, int maxY)
{
    /*  Use these parameters to define the location of the updated region in fbDrawTex */
    glPixelStorei(GL_UNPACK_SKIP_ROWS, minY);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, minX);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

    /*  Copy the updated region to the currently bound texture */
    glTexSubImage2D(GL_TEXTURE_2D, 0, minX, minY, maxX - minX + 1, maxY - minY + 1, GL_RGBA, GL_UNSIGNED_BYTE, fbDrawTex);
}


/******************************************************************************/
/*!
\fn     ClearDrawBuffer
\brief
        Clear all the pixels in the draw buffer texture.
*/
/******************************************************************************/
void ClearDrawBuffer()
{
    std::fill_n(fbDrawTex, width * height * 4, 255);

    /*  Re-copy the cleared texture to the bound texture */
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, fbDrawTex);
}


/******************************************************************************/
/*!
\fn     SwitchToTestBuffer
\brief
        Switch (from draw buffer) to test buffer.
*/
/******************************************************************************/
void SwitchToTestBuffer()
{
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, fbTestTex);
}


/******************************************************************************/
/*!
\fn     SwitchToDrawBuffer
\brief
        Switch (from test buffer) to draw buffer.
*/
/******************************************************************************/
void SwitchToDrawBuffer()
{
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, fbDrawTex);
}


/******************************************************************************/
/*!
\fn     DrawTestTriangles
\brief
        Draw the two triangles for testing.
*/
/******************************************************************************/
void DrawTestTriangles()
{
    const int minX = 100, maxX = width - minX;
    const int minY = 100, maxY = height - minY;
    
    /*  Draw a black (empty) rectangle to bound the test triangles */
    for (int i = minY; i <= maxY; ++i)
    {
        SetPixel(fbTestTex, minX, i, BLACK);
        SetPixel(fbTestTex, maxX, i, BLACK);
    }
    for (int j = minX; j <= maxX; ++j)
    {
        SetPixel(fbTestTex, j, minY, BLACK);
        SetPixel(fbTestTex, j, maxY, BLACK);
    }


    const float fminX = static_cast<float>(minX), fmaxX = static_cast<float>(maxX), 
                fminY = static_cast<float>(minY), fmaxY = static_cast<float>(maxY);
    /*  Testing triangles */
    Triangle leftTri(   Vertex2D(fminX, fminY, RED),
                        Vertex2D(fminX, fmaxY, RED),
                        Vertex2D(fmaxX, fminY, RED));

    Triangle rightTri(  Vertex2D(fmaxX, fmaxY, YELLOW),
                        Vertex2D(fmaxX, fminY, YELLOW),
                        Vertex2D(fminX, fmaxY, YELLOW));

    leftTri.Draw(fbTestTex);
    rightTri.Draw(fbTestTex);
}