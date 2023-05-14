#version 450

in vec3 fColor;

out vec4 fragColors;

void main()
{
	fragColors = vec4(fColor, 1.0);
}