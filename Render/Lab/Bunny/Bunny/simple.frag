#version 450

in vec4 Position;
in vec3 Normal;
out vec4 fragColor;

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

uniform mat4 view;

void main()
{
	vec3 L = vec3(normalize(view * Light.Position - Position));
	vec3 V = vec3(normalize(-Position));
	vec3 R = normalize(reflect(-L, Normal));

	vec3 ambient = Light.La * Material.Ka;
	vec3 diffuse = Light.Ld * Material.Kd * max(dot(L, Normal), 0);
	vec3 spec = Light.Ls * Material.Ks * pow(max(dot(R, V), 0.0), Material.Shiness);

	fragColor = vec4((ambient + diffuse + spec), 1.0);
}