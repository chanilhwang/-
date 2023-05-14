#version 450

//quantifier
layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 lightIntensity;

uniform vec4 lightLocation;
uniform vec3 Kd;
uniform vec3 Id;

uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform mat4 mvp;
uniform mat4 model;

void main()
{
	vec3 N = normalize(normalMatrix * vertexNormal);
	vec4 P;
	vec3 L = vec3(normalize(lightLocation - vertexPosition));

	lightIntensity = Kd * Id * max(0, dot(L, N));

	gl_Position = mvp * vertexPosition; // in-built 내장 변수 gl_Position에 좌표값 입력
}