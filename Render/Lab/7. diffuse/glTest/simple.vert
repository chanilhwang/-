#version 450

//quantifier
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 lightIntensity;

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
uniform mat4 view;

void main()
{
	vec3 N = normalize(normalMatrix * vertexNormal);
	vec4 P = modelView * vec4(vertexPosition, 1.0);
	vec3 L = vec3(normalize(view * Light.Position - P));
	vec3 V = vec3(normalize(-P));
	vec3 R = normalize(reflect(-L, N));

	vec3 ambient = Light.La * Material.Ka;
	vec3 diffuse = Light.Ld * Material.Kd * max(dot(L, N), 0);
	vec3 spec = Light.Ls * Material.Ks * pow(max(dot(R, V), 0.0), Material.Shiness);

	lightIntensity = ambient + diffuse + spec;

	gl_Position = mvp * vec4(vertexPosition, 1.0); // in-built 내장 변수 gl_Position에 좌표값 입력
}