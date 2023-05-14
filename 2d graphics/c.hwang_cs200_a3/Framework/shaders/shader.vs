#version 330 core

/*  Inputs of vertex shader from user application */
in vec2 quadCoord;          //  Coordinate of quad - should cover the whole NDC frame now
in vec2 texCoord;           //  Coordinate of texture to be mapped to quad

uniform mat4 modelMat;      //  Model-to-world matrix
uniform mat4 viewprojMat;   //  View-projection matrix

/*  Output of vertex shader to be sent to the next shader */
out vec2 UV;                //  Coordinate of texture to be mapped to quad

void main()
{
    /*  Append z=0.0 and w=1.0 to make 3D vertex in homogeneous space */
    vec4 quadCoord4 = vec4(quadCoord, 0.0, 1.0);

    /*  This transformation is actually rendundant and not efficient in this case,
        since we only draw a quad that covers the whole viewport.
        However, we still use it here for illustration purpose */
    gl_Position = viewprojMat * modelMat * quadCoord4;

    /*  No change to texture coordinate */
    UV = texCoord;
}

