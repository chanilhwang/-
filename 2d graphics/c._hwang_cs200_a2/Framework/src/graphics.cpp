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
\date   22/5/2015
*/
/******************************************************************************/                    

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <cmath>

#include "graphics.hpp"
#include "textfile.h"


/******************************************************************************/
/*!
\fn     Graphics::Graphics(int width, int height)
\brief
        Graphics class constructor.
\param  width
        Given width for viewport.
\param  height
        Given height for viewport.
*/
/******************************************************************************/
Graphics::Graphics(int width, int height) :
    width(width), height(height),
    numElems(width * height * 4),
    offscreenFB(new unsigned char[numElems]),
    fbUpdated(true),
    posBuffer{{ -1, -1,
                 1, -1,
                -1,  1,
                 1,  1}},
    uvBuffer{{  0, 1,
                1, 1,
                0, 0,
                1, 0}},
    vsFilename("shaders/shader.vs"), fsFilename("shaders/shader.fs")
{
    memset(offscreenFB, 255, numElems);
    drawnPix.clear();
}


/******************************************************************************/
/*!
\fn     Graphics::~Graphics()
\brief
        Destructor to clean up the graphics-related stuffs.
*/
/******************************************************************************/
Graphics::~Graphics()
{
    drawnPix.clear();
    delete [] offscreenFB;

    /*  Unbind the current VAO */
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &uvVBO);

    glDeleteTextures(1, &texObj);

    glDeleteProgram(renderProg);
}


/******************************************************************************/
/*!
\fn     void InitGLExtensions()
\brief
        Initialize the OpenGL extensions supported on the current platform.
*/
/******************************************************************************/
void Graphics::InitGLExtensions() const
{
    /* GL3W Initialization */
    if (gl3wInit())
    {
        std::cerr << "Failed to initialize OpenGL" << "\n";
        std::exit(-1);
    }

    if (!gl3wIsSupported(3, 3))
    {
        std::cerr << "OpenGL 3.3 not supported" << "\n";
        std::exit(-1);
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "GLSL version  : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}


/******************************************************************************/
/*!
\fn     void ValidateShader(GLuint shader, const char *file) const
\brief
        Check whether shader files can be compiled successfully.
\param  shader
        ID of the shader
\param  file
        Shader file name
*/
/******************************************************************************/
void Graphics::ValidateShader(GLuint shader, const char *file) const
{
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];

    GLsizei length = 0;
    GLint result;

    glGetShaderInfoLog(shader, 512, &length, buffer);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result != GL_TRUE && length > 0)
    {
        std::cerr << "Shader " << file << " compilation error: " << buffer << "\n";
        exit(1);
    }
    else
        std::cout << "Shader " << file << " compilation successful.\n";
}


/******************************************************************************/
/*!
\fn     void ValidateProgram(GLuint program) const
\brief
        Check whether shader program can be linked successfully.
\param  program
        ID of the shader program
*/
/******************************************************************************/
void Graphics::ValidateProgram(GLuint program) const
{
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;
    GLint status;

    /*  Ask OpenGL to give us the log associated with the program */
    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE && length > 0)
    {
        std::cout << "Program " << program << " link error: " << buffer << "\n";
        exit(1);
    }
    else
        std::cout << "Program " << program << " link successful.\n";

    /*  Ask OpenGL to validate the program */
    glValidateProgram(program);

    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int log_length = 0;
        glGetShaderiv(program, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length> 0) {
            char *info_log = (char*)malloc(log_length);
            int bytes = 0;
            glGetShaderInfoLog(program, log_length, &bytes, info_log);
            std::cout << info_log << "\n";
            free(info_log);
        }

        exit(1);
    }
    else
    {
        std::cout << "Program " << program << " validation successful.\n";
    }
}


/******************************************************************************/
/*!
\fn     GLuint CompileShaders()
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
GLuint Graphics::CompileShaders()
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint program;

    char *vsSource = textFileRead(vsFilename);
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsSource, NULL);
    glCompileShader(vertexShader);
    ValidateShader(vertexShader, vsFilename);           /*  Prints any errors */

    char * fsSource = textFileRead(fsFilename);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsSource, NULL);
    glCompileShader(fragmentShader);
    ValidateShader(vertexShader, fsFilename);           /*  Prints any errors */

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    ValidateProgram(program);                           /*  Print any errors */

    free(vsSource);
    free(fsSource);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}


/******************************************************************************/
/*!
\fn     void SendVertexAttribute(   GLuint VBO, const float vertexData[], 
                                    int size, int location) const
\brief
        Send the vertex attribute in vertexData array, who occupies size bytes,
        to the GPU memory indicated by VBO. This attribute will be matched to
        the variable in the shaders that has the given location.
\param  VBO
        The ID to indicate where to send vertex attributes to in the GPU memory.
\param  vertexData
        The actual vertex attribute data.
\param  size
        Size of the vertex data in bytes.
\param  location
        Location of the corresponding attribute variable in the shaders to match.
*/
/******************************************************************************/
void Graphics::SendVertexAttribute(GLuint VBO, const float vertexData[], int size, int location) const
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    /*  Allocate buffer for the quad vertex coordinates */
    glBufferData(GL_ARRAY_BUFFER, size, vertexData, GL_STATIC_DRAW);

    /*  Feed the quad vertex buffer into shaders */
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
        location,           // location of vertex pos in shader
        2,                  // dimension of vertex attribute
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );
}


/******************************************************************************/
/*!
\fn     void SetUpTextureObjs()
\brief
        Read texture image from the predefined file and send it to the GPU.
        The the active texture ID will be sent to the shaders for coloring the
        object.
*/
/******************************************************************************/
void Graphics::SetUpTextureObjs()
{
    glGenTextures(1, &texObj);

    /*  Activate texture memory GL_TEXTURE0 for binding texture object */
    glActiveTexture(GL_TEXTURE0);

    /*  Bind corresponding texture ID */
    glBindTexture(GL_TEXTURE_2D, texObj);

    /*  Handle image format's padding */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

    /*  Set up texture behaviors */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /*  Obtain the location of the variable in the shaders with the given name */
    texLoc = glGetUniformLocation(renderProg, "tex");

    /*  Tell shaders to use the texture object at TEXTURE0 */
    glUniform1i(texLoc, 0);
}


/******************************************************************************/
/*!
\fn     void Graphics::ClearFB(const Color &c)
\brief
        Clearing up the previously drawn pixels in the offscreen buffer.
\param  c
        Reference to the color used for clearing the buffer.
*/
/******************************************************************************/
void Graphics::ClearFB(const Color &c)
{
    while (!drawnPix.empty())
    {
        std::pair<int, int> X = drawnPix.back();
        drawnPix.pop_back();
        SetPix(X.first, X.second, c, false);
    }
}


/******************************************************************************/
/*!
\fn     void Graphics::SetPix(int x, int y, const Color &c, bool drawing)
\brief
        Color a pixel in the offscreen buffer.
\param  x
        x-coordinate of the pixel to be drawn.
\param  y
        y-coordinate of the pixel to be drawn.
\param  c
        Reference to the color used for drawing the pixel.
\param  drawing
        Indicating whether we're drawing or clearing the pixel. Default = true.
*/
/******************************************************************************/
void Graphics::SetPix(int x, int y, const Color &c, bool drawing)
{
    if (x >= 0 && y >= 0 && x < width && y < height)
    {
        int base = (y * width + x) * 4;

        /*  If we're drawing on a white pixel, then add it to the array for clearing later */
        if (drawing 
         && offscreenFB[base + 0] == 255
         && offscreenFB[base + 1] == 255
         && offscreenFB[base + 2] == 255)
            drawnPix.push_back(std::make_pair(x, y));

        offscreenFB[base + 0] = c.GetR();
        offscreenFB[base + 1] = c.GetG();
        offscreenFB[base + 2] = c.GetB();
        offscreenFB[base + 3] = c.GetA();
    }

    fbUpdated = true;
}


/******************************************************************************/
/*!
\fn     void SetUp()
\brief
        Set up the render program, VAO, VBO and graphics data for rendering.
*/
/******************************************************************************/
void Graphics::SetUp()
{
    /*  Create vertex array ID to store the states of all attributes */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /*  Create vertex buffer ID to store the state of the rectangle vertex positions */
    glGenBuffers(1, &posVBO);
    /*  Send vertex position data to GPU, match it with shader attribute variable at location 0 */
    SendVertexAttribute(posVBO, posBuffer.data(), sizeof(posBuffer), 0);

    /*  Create vertex buffer ID to store the state of the rectangle vertex uv */
    glGenBuffers(1, &uvVBO);
    /*  Send vertex uv datqa to GPU, match it with shader attribute variable at location 1 */
    SendVertexAttribute(uvVBO, uvBuffer.data(), sizeof(uvBuffer), 1);


    /*  Link the shaders into rendering program and compile it */
    renderProg = CompileShaders();
    glUseProgram(renderProg);


    /*  When setting up texture, we need to call glUniform to send the active
        texture ID directly to the shaders, so we need to call glUseProgram 
        before that to enable the shaders. */
    SetUpTextureObjs();
    

    /*  Bind framebuffer to 0 to render to the screen (by default already 0) */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*  Filling the triangles */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    /*  Culling back-facing triangles for efficiency, but no back faces now */
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    /*  Enabling anti-aliasing */
    glEnable(GL_MULTISAMPLE);

    glViewport(0, 0, width, height);
}


/******************************************************************************/
/*!
\fn     void Resize(int w, int h)
\brief
        Resize function for updating camera projection upon window resize.
\param  int w
        The new width of the window
\param  int h
        The new height of the window
*/
/******************************************************************************/
void Graphics::Resize(int w, int h)
{
    /*  Update viewport */
    glViewport(0, 0, w, h);

    /*  Update offscreen fb */
    delete [] offscreenFB;
    numElems = w * h * 4;
    offscreenFB = new unsigned char[numElems];
    memset(offscreenFB, 255, numElems);

    drawnPix.clear();   /*  The whole buffer is already white, so just reset drawPix */

    width = w;
    height = h;

    /*  Adjust view frustum aspect if needed, but let's not do anything now */
    //...
}


/******************************************************************************/
/*!
\fn     void Render() const
\brief
        Render function for drawing out the geometry based on vertex data.
*/
/******************************************************************************/
void Graphics::Render()
{
    if (fbUpdated)
    {
        /*  Copy offscreen FB to the texture memory */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, offscreenFB);

        fbUpdated = false;
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}