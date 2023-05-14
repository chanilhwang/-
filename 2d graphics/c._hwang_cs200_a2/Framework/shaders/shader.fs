#version 330 core

/*  Uniform variable for user-defined texture */
uniform sampler2D tex;

/*  Varying variable to receive texture uv coordinate from the previous shader */
in vec2 texCoord;

/*  Fragment shader always need to have an output variable to indicate the color of the fragment */
out vec4 color;

void main(void)
{
    /*  Color the fragment using the texture image */
    color = texture(tex, texCoord);
}