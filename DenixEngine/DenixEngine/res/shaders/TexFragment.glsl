#version 330 core

struct DircetionalLight
{
	vec3 Direction;
	vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	//vec3 Specular;
};	

struct Material
{
	float SpecularIntensity;
	float SpecularPower;
};

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosition;

out vec4 Color;

uniform sampler2D u_Texture;
uniform vec4 u_BaseColor;
uniform DircetionalLight u_DirLight;
uniform bool u_AffectsLighting;
uniform Material u_Material;
uniform vec3 u_CameraPosition;

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
	vec4 diffuseColor = vec4(u_DirLight.Color, 1.0f) * u_DirLight.DiffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0.0f);

	if (diffuseFactor > 0.0f)
	{
		vec3 directionToEye = normalize(u_CameraPosition - FragPosition);
		vec3 reflectedVertex = normalize(reflect(u_DirLight.Direction, normalize(Normal)));
		float specularFactor = dot(directionToEye, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, u_Material.SpecularPower);
			specularColor = vec4(u_DirLight.Color * u_Material.SpecularIntensity * specularFactor, 0.0f);
		}
	}

	Color = texture(u_Texture, TexCoord)* (ambientColor + diffuseColor + specularColor);
}