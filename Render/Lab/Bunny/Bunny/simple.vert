#version 450

//quantifier
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec4 Position;
out vec3 Normal;

struct LightInfo {
	vec4 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};
uniform LightInfo Light;

struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shiness;
};
uniform MaterialInfo Material;

uniform mat3 normalMatrix;
uniform mat4 mvp;
uniform mat4 modelView;

void main()
{
	Normal = normalize(normalMatrix * vertexNormal);
	Position = modelView * vec4(vertexPosition, 1.0);

	gl_Position = mvp * vec4(vertexPosition, 1.0); // in-built 내장 변수 gl_Position에 좌표값 입력
}