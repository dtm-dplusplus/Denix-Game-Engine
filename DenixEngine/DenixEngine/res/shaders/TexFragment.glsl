#version 330 core

struct DircetionalLight
{
	//vec3 Direction;
	vec3 Albedo;
	float AmbientIntensity;
	//vec3 Specular;
};	

in vec2 TexCoord;

out vec4 colour;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform DircetionalLight u_DirLight;

void main()
{
	vec4 ambientColor = vec4(u_DirLight.Albedo, 1.0f) * u_DirLight.AmbientIntensity;
	colour = texture(u_Texture, TexCoord) * ambientColor;
}