#version 330 core

/*  Inputs of fragment shader */
uniform sampler2D fbTex;
in vec2 UV;

/*  Output of fragment shader, which is just pixel color */
out vec4 fragColor;

void main(void)
{
    /*  Pixel color is sampled from the pixel in the framebuffer texture */
    fragColor = texture2D(fbTex, UV);
}