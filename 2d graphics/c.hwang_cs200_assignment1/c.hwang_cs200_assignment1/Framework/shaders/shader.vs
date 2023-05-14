#version 330 core

/*  layout location is used to match the attribute variables with the variables in user application */
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

/*  Varying variable that we be sent to the next shader */
out vec2 texCoord;

void main(void) 
{
    /*  NDC-frame vertex coordinate */
    gl_Position = vec4(pos, -1.0, 1.0);

    /*  Just use user-defined vertex UV */
    texCoord = uv;
}