#version 330 core

struct DircetionalLight
{
	vec3 Direction;
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	//vec3 Specular;
};	

in vec2 TexCoord;
in vec3 Normal;
out vec4 Color;

uniform sampler2D u_Texture;
uniform vec4 u_BaseColor;
uniform DircetionalLight u_DirLight;
uniform bool u_AffectsLighting;

void main()
{
	// This is slow. Will swap shaders instead later.
	if (!u_AffectsLighting)
	{
		Color = texture(u_Texture, TexCoord) * u_BaseColor;
		return;
	}

	vec4 ambientColor = vec4(u_DirLight.Color, 1.0f) * u_DirLight.AmbientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(u_DirLight.Direction)), 0.0f);
	vec4 diffuseColour = vec4(u_DirLight.Color, 1.0f) * u_DirLight.DiffuseIntensity * diffuseFactor;

	Color = texture(u_Texture, TexCoord)* (ambientColor + diffuseColour);
}