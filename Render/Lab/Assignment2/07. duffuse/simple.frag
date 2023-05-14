#version 450

in vec3 lightIntensity;

layout(location = 0) out vec4 fragColors;

void main()
{
	fragColors = vec4(lightIntensity, 1.0);
}